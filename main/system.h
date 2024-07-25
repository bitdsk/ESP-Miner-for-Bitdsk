#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdbool.h>
#include <stdint.h>

#define HISTORY_LENGTH 100
#define HISTORY_WINDOW_SIZE 5

#define DIFF_STRING_SIZE 10
typedef struct
{
	char ssid[20];
	char wifi_status[20];
    bool startup_done;
	
	
    double duration_start;
    int historical_hashrate_rolling_index;
    double historical_hashrate_time_stamps[HISTORY_LENGTH];
    double historical_hashrate[HISTORY_LENGTH];
    int historical_hashrate_init;
    double current_hashrate;
    int64_t start_time;
    uint32_t shares_accepted;
    uint32_t shares_rejected;
    int screen_page;
    char oled_buf[20];
    uint32_t best_nonce_diff;
    char best_share_string[DIFF_STRING_SIZE];
    bool FOUND_BLOCK;

    
    

    uint32_t lastClockSync;

    double Network_Difficulty; // 全网 难度
} SystemModule;

void SYSTEM_task(void * parameters);


void SYSTEM_notify_accepted_share(SystemModule * module);
void SYSTEM_notify_rejected_share(SystemModule * module);
void SYSTEM_notify_found_nonce(SystemModule * module, double pool_diff, double found_diff);
void SYSTEM_check_for_best_diff(SystemModule * module, double pool_diff, uint32_t nbits);
void SYSTEM_notify_mining_started(SystemModule * module);
void SYSTEM_notify_new_ntime(SystemModule * module, uint32_t ntime);
void SYSTEM_update_net_diff(SystemModule * module, uint32_t nbits);
#endif /* SYSTEM_H_ */
