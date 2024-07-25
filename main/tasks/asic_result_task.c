#include "bm1397.h"
#include "esp_log.h"
#include "global_state.h"
#include "nvs_config.h"
#include "serial.h"
#include "utils.h"
#include "work_queue.h"
#include <string.h>

const char * TAG = "asic_result";

void ASIC_result_task(void * pvParameters)
{
    GlobalState * GLOBAL_STATE = (GlobalState *) pvParameters;
    SERIAL_clear_buffer();

    char * user = nvs_config_get_string(NVS_CONFIG_STRATUM_USER, STRATUM_USER);

    double nonce_diff;
    task_result * asic_result;

    while (1) {

        asic_result = (*GLOBAL_STATE->ASIC_functions.receive_result_fn)(GLOBAL_STATE);

        if (asic_result == NULL) {
            continue;
        }

        // uint8_t job_id = asic_result->job_id;

        // check the nonce difficulty
        nonce_diff = test_nonce_value(GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id], asic_result->nonce,
                                      asic_result->rolled_version);

        // bitdskxiugai if (nonce_diff >= GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id]->pool_diff) {
        if (nonce_diff >= GLOBAL_STATE->stratum_difficulty) {
            STRATUM_V1_submit_share(
                GLOBAL_STATE->sock, user, GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id]->jobid,
                GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id]->extranonce2,
                GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id]->ntime, asic_result->nonce,
                asic_result->rolled_version ^ GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id]->version);
                ESP_LOGI(TAG, "Submit Share %.2f of %ld", nonce_diff,GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id]->pool_diff);
        }else{
                ESP_LOGI(TAG, "Nonce Diff %.2f of %ld", nonce_diff,GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id]->pool_diff);
        }
        SYSTEM_notify_found_nonce(&GLOBAL_STATE->SYSTEM_MODULE,
                                  GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id]->pool_diff, nonce_diff);
        SYSTEM_check_for_best_diff(&GLOBAL_STATE->SYSTEM_MODULE, nonce_diff, GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id]->target);
 

        // if (GLOBAL_STATE->valid_jobs[asic_result->job_id] == 0) {
        //     ESP_LOGI(TAG, "Invalid job nonce found, id=%d", asic_result->job_id);
        // }



        // if (GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id] != NULL) {
        //     free_bm_job(GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id]);
        //     // GLOBAL_STATE->ASIC_TASK_MODULE.active_jobs[asic_result->job_id] = NULL;
        // }
    }
}