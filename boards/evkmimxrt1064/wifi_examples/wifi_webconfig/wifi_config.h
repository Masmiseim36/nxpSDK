/* Wi-Fi boards configuration */
#if defined(WIFI_BOARD_PAN9026_SDIO)
#define SD8977
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#define SD_CLOCK_MAX (25000000U)
#elif defined(WIFI_BOARD_AW_AM281SM)
#define SD8977
#elif defined(WIFI_BOARD_AW_NM191MA)
#define SD8801
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#else
#error "Please define macro related to wifi board"
#endif

#define CONFIG_MAX_AP_ENTRIES 10
#define CONFIG_UAP_AMPDU_TX   1
#define CONFIG_UAP_AMPDU_RX   1

#if defined(SD8977)
#define CONFIG_5GHz_SUPPORT 1
#endif

/* Logs */
#define CONFIG_ENABLE_ERROR_LOGS   1
#define CONFIG_ENABLE_WARNING_LOGS 1

/* WLCMGR debug */
#undef CONFIG_WLCMGR_DEBUG

/*
 * Wifi extra debug options
 */
#undef CONFIG_WIFI_EXTRA_DEBUG
#undef CONFIG_WIFI_EVENTS_DEBUG
#undef CONFIG_WIFI_CMD_RESP_DEBUG
#undef CONFIG_WIFI_SCAN_DEBUG
#undef CONFIG_WIFI_IO_INFO_DUMP
#undef CONFIG_WIFI_IO_DEBUG
#undef CONFIG_WIFI_IO_DUMP
#undef CONFIG_WIFI_MEM_DEBUG
#undef CONFIG_WIFI_AMPDU_DEBUG
#undef CONFIG_WIFI_TIMER_DEBUG
#undef CONFIG_WIFI_SDIO_DEBUG
