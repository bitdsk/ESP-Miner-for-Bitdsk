#ifndef MAIN_NVS_CONFIG_H
#define MAIN_NVS_CONFIG_H

#include <stdint.h>

#define NVS_CONFIG_HOSTNAME "hostname"
#define NVS_CONFIG_WIFI_SSID "wifissid"
#define NVS_CONFIG_WIFI_PASS "wifipass"
#define NVS_CONFIG_STRATUM_URL "stratumurl"
#define NVS_CONFIG_STRATUM_PORT "stratumport"
#define NVS_CONFIG_STRATUM_USER "stratumuser"
#define NVS_CONFIG_STRATUM_PASS "stratumpass"
#define NVS_CONFIG_ASIC_FREQ "asicfrequency"
#define NVS_CONFIG_ASIC_VOLTAGE "asicvoltage"
#define NVS_CONFIG_ASIC_MODEL "asicmodel"
#define NVS_CONFIG_DEVICE_MODEL "devicemodel"
#define NVS_CONFIG_BOARD_VERSION "boardversion"
#define NVS_CONFIG_FLIP_SCREEN "flipscreen"
#define NVS_CONFIG_INVERT_SCREEN "invertscreen"
#define NVS_CONFIG_ASIC_STARTFREQ "startfreq"


#define NVS_CONFIG_STATIC_IP_OPEN "staticipopen"
#define NVS_CONFIG_STATIC_IP "staticip"
#define NVS_CONFIG_STATIC_NETMASK "staticnetmask"
#define NVS_CONFIG_STATIC_GW "staticopengw"

#define NVS_CONFIG_DNS_MAIN "dnsmain"
#define NVS_CONFIG_DNS_BACKUP "dnsbackup"
#define NVS_CONFIG_DNS_FALLBACK "dnsfallback"

#define NVS_CONFIG_SWARM "swarmconfig"

char * nvs_config_get_string(const char * key, const char * default_value);
void nvs_config_set_string(const char * key, const char * default_value);
uint16_t nvs_config_get_u16(const char * key, const uint16_t default_value);
void nvs_config_set_u16(const char * key, const uint16_t value);

#endif // MAIN_NVS_CONFIG_H
