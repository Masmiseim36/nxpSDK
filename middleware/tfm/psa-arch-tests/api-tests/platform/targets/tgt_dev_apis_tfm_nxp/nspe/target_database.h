#include "val_target.h"
#include "cmsis_compiler.h"

typedef __PACKED_STRUCT target_database{
    target_cfg_hdr_t target_cfg_hdr;            /* Target Configuration Header */
    soc_peripheral_desc_t   uart;               /* Uart */
    soc_peripheral_desc_t   wd_timer;           /* WD timer */
    memory_desc_t           memory;             /* Memory region */
    cfg_type_t              type_end;           /* End */
} target_database_t;

extern target_database_t target_database;