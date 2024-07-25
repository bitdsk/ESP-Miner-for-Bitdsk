#include "DS4432U.h"
#include "EMC2101.h"
#include "INA260.h"
#include "bm1397.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "global_state.h"
#include "math.h"
#include "mining.h"
#include "nvs_config.h"
#include "serial.h"
#include <string.h>

#include "esp_timer.h"
#include <time.h>

#define POLL_RATE 5000
#define MAX_TEMP 90.0
#define THROTTLE_TEMP 80.0
#define THROTTLE_TEMP_RANGE (MAX_TEMP - THROTTLE_TEMP)

#define VOLTAGE_START_THROTTLE 4900
#define VOLTAGE_MIN_THROTTLE 3500
#define VOLTAGE_RANGE (VOLTAGE_START_THROTTLE - VOLTAGE_MIN_THROTTLE)

static const char * TAG = "power_management";

static float _fbound(float value, float lower_bound, float upper_bound)
{
    if (value < lower_bound)
        return lower_bound;
    if (value > upper_bound)
        return upper_bound;

    return value;
}

void POWER_MANAGEMENT_task(void * pvParameters)
{

    GlobalState * GLOBAL_STATE = (GlobalState *) pvParameters;

    PowerManagementModule * power_management = &GLOBAL_STATE->POWER_MANAGEMENT_MODULE;

    SystemModule * system_management = &GLOBAL_STATE->SYSTEM_MODULE;

    power_management->frequency_multiplier = 1;

    int last_frequency_increase = nvs_config_get_u16(NVS_CONFIG_ASIC_STARTFREQ, 200);

    // bool read_power = INA260_installed();

    uint16_t frequency_target = nvs_config_get_u16(NVS_CONFIG_ASIC_FREQ, 300);

    while (1) {

        // if (read_power == true) {
        //     power_management->voltage = INA260_read_voltage();
        //     power_management->power = INA260_read_power() / 1000;
        //     power_management->current = INA260_read_current();
        // }
        // // power_management->fan_speed = EMC2101_get_fan_speed();
        // power_management->fan_speed = 50;

        if (strcmp(GLOBAL_STATE->asic_model, "BM1397") == 0) {

            //     // power_management->chip_temp = EMC2101_get_external_temp();
            // power_management->chip_temp = 50;

            //     // Voltage
            //     // We'll throttle between 4.9v and 3.5v
            // float voltage_multiplier =
            //   _fbound((power_management->voltage - VOLTAGE_MIN_THROTTLE) * (1 / (float) VOLTAGE_RANGE), 0, 1);

            //     // Temperature
            // float temperature_multiplier = 1;
            // float over_temp = -(THROTTLE_TEMP - power_management->chip_temp);
            // if (over_temp > 0) {
            //     temperature_multiplier = (THROTTLE_TEMP_RANGE - over_temp) / THROTTLE_TEMP_RANGE;
            // }

            // float lowest_multiplier = 1;
            // float multipliers[2] = {voltage_multiplier, temperature_multiplier};

            // for (int i = 0; i < 2; i++) {
            //     if (multipliers[i] < lowest_multiplier) {
            //         lowest_multiplier = multipliers[i];
            //     }
            // }

            // power_management->frequency_multiplier = lowest_multiplier;

            // float target_frequency = _fbound(power_management->frequency_multiplier * frequency_target, 0, frequency_target);

            //     if (target_frequency < 50) {
            //         // TODO: Turn the chip off
            //     }

            //     // chip is coming back from a low/no voltage event
            // ESP_LOGI(TAG, "Freq %f", power_management->frequency_value);
            //     if (power_management->frequency_value < 50 && target_frequency > 50) {
            //         // TODO recover gracefully?
            //         esp_restart();
            //     }

            //     if (power_management->frequency_value > target_frequency) {
            //         power_management->frequency_value = target_frequency;
            //         last_frequency_increase = 0;
            //         BM1397_send_hash_frequency(power_management->frequency_value);
            //         ESP_LOGI(TAG, "target %f, Freq %f, Temp %f, Power %f", target_frequency, power_management->frequency_value,
            //                  power_management->chip_temp, power_management->power);
            //     } else {
            //         if (last_frequency_increase > 120 && power_management->frequency_value != frequency_target) {
            //             float add = (target_frequency + power_management->frequency_value) / 2;
            //             power_management->frequency_value += _fbound(add, 2, 20);
            //             BM1397_send_hash_frequency(power_management->frequency_value);
            //             ESP_LOGI(TAG, "target %f, Freq %f, Temp %f, Power %f", target_frequency,
            //             power_management->frequency_value,
            //                      power_management->chip_temp, power_management->power);
            //             last_frequency_increase = 60;
            //         } else {
            //             last_frequency_increase++;
            //         }
            //     }

            if (last_frequency_increase < frequency_target) {
                last_frequency_increase = last_frequency_increase + 25;
                if (last_frequency_increase > frequency_target) {
                    last_frequency_increase = frequency_target;
                }
                BM1397_send_hash_frequency((float) last_frequency_increase);

                ESP_LOGI(TAG, "Set Freq To %d", last_frequency_increase);
            } else if (last_frequency_increase > frequency_target) {
                last_frequency_increase = frequency_target;
                BM1397_send_hash_frequency((float) last_frequency_increase);

                ESP_LOGI(TAG, "Set Freq To %d", last_frequency_increase);
            } else {

                ESP_LOGW(TAG, "Stratum Queue:%d,Asicjob Queue:%d",GLOBAL_STATE->stratum_queue.count,GLOBAL_STATE->ASIC_jobs_queue.count);

                //////
                if(system_management->historical_hashrate_rolling_index>=1){
                    int caches_indes = (system_management->historical_hashrate_rolling_index - 1) % HISTORY_LENGTH;
                    double caches_dou=system_management->historical_hashrate_time_stamps[caches_indes];
                    int nouptime_in_seconds =(int)(esp_timer_get_time() - caches_dou) / 1000000;

                    if (nouptime_in_seconds >= 960)
                    {
                        ESP_LOGE(TAG, "16 Min no have submit share , So restart bitdsk ESP...");
                        esp_restart();
                    }

                }
                ////////end



            }
            //vTaskDelay(3000);
        } else if (strcmp(GLOBAL_STATE->asic_model, "BM1366") == 0) {
            power_management->chip_temp = EMC2101_get_internal_temp() + 5;

            if (power_management->fan_speed < 10) {
                ESP_LOGE(TAG, "Detected fan speed too slow, setting vCore to 0");
                DS4432U_set_vcore(0);
            }
            //vTaskDelay(3000);
        }

        // ESP_LOGI(TAG, "target %f, Freq %f, Volt %f, Power %f", target_frequency, power_management->frequency_value,
        // power_management->voltage, power_management->power);
        vTaskDelay(POLL_RATE / portTICK_PERIOD_MS);
    }

vTaskDelete(NULL);
}
