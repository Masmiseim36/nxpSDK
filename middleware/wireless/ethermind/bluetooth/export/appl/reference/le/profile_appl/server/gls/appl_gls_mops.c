/**
 *  \file appl_gls_mops.c
 *
 *  This file contains the menu handling for GLS Profile.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ------------------------------- Header File Inclusion */
#include "appl_gls.h"
#include "appl_gls_mops.h"

#if (defined ATT && defined GLS)
#ifdef APPL_MENU_OPS

/* --------------------------------------------- External Global Variables */
extern UCHAR gls_msrmt_obs_data
[APPL_GLUCOSE_MEASUREMENT_LENGTH * APPL_GLUCOSE_MEASUREMENT_COUNT];

extern UCHAR gls_msrmt_cntxt_obs_data
[APPL_GLUCOSE_MEASUREMENT_CONTEXT_LENGTH * APPL_GLUCOSE_MEASUREMENT_CONTEXT_COUNT];

extern UCHAR gls_msrmt_seq_id_index[APPL_GLUCOSE_MEASUREMENT_COUNT];
extern UCHAR gls_cntxt_msrmt_seq_id_index[APPL_GLUCOSE_MEASUREMENT_CONTEXT_COUNT];
extern UCHAR  appl_stored_record_count;
extern UCHAR  last_record_update;
extern UINT16 appl_seg_index;

#define APPL_GLS_CNTXT_MSRMT_MEDITN_VAL_INDEX 30U
/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static const char gls_options[] = " \n\
========= GLS : M A I N   M E N U ========= \n\
\n\
    0. Exit \n\
    1. Refresh \n\
\n\
    2. Set Glucose Concentration Units to kg/L \n\
    3. Set Glucose Concentration Units to mol/L \n\
    4. Set Medication Value Units to kilograms \n\
    5. Set Medication Value Units to litres \n\
    6. Enable or disable Glucose Measurement Context in last record \n\
    7. Generate 1 Record \n\
    8. Generate 3 Record \n\
    9. Update User Facing Time \n\
   10. Delete All Records \n\
   11. Generate 4 Record \n\
\n\
    Your Option ?\n";

/* --------------------------------------------- Functions */
void appl_gls_menu_handler(void)
{
    int choice;
    UINT16 index;
    int record_no;
    INT16 total_mins;
    int total_records;
    INT16 prev_rec_timeoffset;
    APPL_GLS_USER_FACING_TIME appl_gls_user_facing_time;
    int i;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", gls_options);
        CONSOLE_IN("%d", &choice);

        switch (choice)
        {
        case 0: /* exit */
            return;

        case 1: /* Refresh */
            break;

        case 2:

            /**
             * Updating the last record Flag field of glucose measurement
             * The units of Glucose concentration is set to kg/L (bit 2 = 0)
             */

            gls_msrmt_obs_data\
                [(gls_msrmt_seq_id_index[appl_stored_record_count - 1U]) - 1U] = 0x0BU;

            /**
             * Updating the last record Glucose Concentration Units of
             * glucose measurement to kg/L.
             * The exponent of the SFLOAT is -5 (since the bluetooth.org base unit is kg/L).
             * Twos Complement 0f -5 is 0xFB. 1st 4bit of exponent is B0.
             */
            gls_msrmt_obs_data\
                [(gls_msrmt_seq_id_index[appl_stored_record_count - 1U]) + 12U] = 0xB0U;

            last_record_update = 1U;

            break;
        case 3:
            /* Updating the last record Flag field of glucose measurement */
            gls_msrmt_obs_data\
                [(gls_msrmt_seq_id_index[appl_stored_record_count - 1U]) - 1U] = 0x0FU;

            /** Updating the last record Glucose Concentration Units of
            * glucose measurement to litres
            * The exponent of the SFLOAT is -5 (since the bluetooth.org base unit is kg/L).
            * Twos Complement 0f -4 is 0xFC. 1st 4bit of exponent is C0.
            */
            gls_msrmt_obs_data\
                [(gls_msrmt_seq_id_index[appl_stored_record_count - 1U]) + 12U] = 0xC0U;

            last_record_update = 1U;

            break;
        case 4:
            /* Updating the last record Flag field of glucose measurement context */

            /* The units of Medication are set to kilograms (bit 5 = 0).*/
            gls_msrmt_cntxt_obs_data\
                [(gls_cntxt_msrmt_seq_id_index\
                [APPL_GLUCOSE_MEASUREMENT_CONTEXT_COUNT - 1U]) - 1U] = 0xDFU;

            /**
             * Update Medication value in kilograms
             * Medication - units of kilograms(if bit4 is set, bit 5 is not set)
             * The exponent of the SFLOAT is -6 (since the bluetooth.org base unit is kg).
             *
             * The intended value to be sent is
             * 96mg = (96 * 10^(-6))Kg
             * In SFLOAT,
             * exponent = -6 = 0xA(2's Complement of -6: with 4bits Value as Exp)
             * Get 2's component for -6
             * 0000 0110 = -6
             * 1111 1001 = 1's component
             * 0000 0001 = Add 1
             *============
             * 1111 1010 = FA (1st 4bits will be 1101 i,e A)
             *============
             * mantissa = 96 = 0x060(in Hexadecimal with 24bits Value of Mantissa)
             * 16bt SFLOAT value will be
             *  => 0000 0000 0000 0000
             *  => 4bits of exponent(A) and 12bits of Mantissa(96)
             *============================
             *  => 1101 0000 0110 0000 => D060
             * Combined Value = 0xD060
             * i.e. 0x60, 0xA0 in Byte Stream (LSB to MSB)
             */
            gls_msrmt_cntxt_obs_data[APPL_GLS_CNTXT_MSRMT_MEDITN_VAL_INDEX] = 0x60U;
            gls_msrmt_cntxt_obs_data[APPL_GLS_CNTXT_MSRMT_MEDITN_VAL_INDEX+1U] = 0xA0U;

            last_record_update = 1U;

            break;
        case 5:
            /* Updating the last record Flag field of glucose measurement context */

            /* The units of Medication are set to litres(bit 5 = 1). */
            gls_msrmt_cntxt_obs_data\
                [(gls_cntxt_msrmt_seq_id_index\
                [APPL_GLUCOSE_MEASUREMENT_CONTEXT_COUNT - 1U]) - 1U] = 0xFFU;

            /**
             * Update Medication value in litres
             * Medication - units of litres(if bit4 is set, bit 5 is set)
             * The exponent of the SFLOAT is -3 (since the bluetooth.org base unit is L).
             *
             * The intended value to be sent is
             * 96mg = (96 * 10^(-3))Kg
             * In SFLOAT,
             * exponent = -3 = 0xD(2's Complement of -3: with 4bits Value as Exp)
             * Get 2's component for -3
             * 0000 0011 = -3
             * 1111 1100 = 1's component
             * 0000 0001 = Add 1
             *============
             * 1111 1101 = FD (1st 4bits will be 1101 i,e D)
             *============
             * mantissa = 96 = 0x060(in Hexadecimal with 24bits Value of Mantissa)
             * 16bt SFLOAT value will be
             *  => 0000 0000 0000 0000
             *  => 4bits of exponent(D) and 12bits od Mantissa(96)
             *============================
             *  => 1101 0000 0110 0000 => D060
             * Combined Value = 0xD060
             * i.e. 0x60, 0xD0 in Byte Stream (LSB to MSB)
             */
            gls_msrmt_cntxt_obs_data[APPL_GLS_CNTXT_MSRMT_MEDITN_VAL_INDEX] = 0x60U;
            gls_msrmt_cntxt_obs_data[APPL_GLS_CNTXT_MSRMT_MEDITN_VAL_INDEX + 1U] = 0xD0U;
            last_record_update = 1U;

            break;
        case 6:
            CONSOLE_OUT("1 (Enable), 0 (Disable)\n");
            CONSOLE_IN("%d", &choice);
            if (choice)
            {
                gls_msrmt_obs_data\
                    [(gls_msrmt_seq_id_index[appl_stored_record_count - 1U]) - 1U] |= 0x10U;
            }
            else
            {
                gls_msrmt_obs_data\
                    [(gls_msrmt_seq_id_index[appl_stored_record_count - 1U]) - 1U] &= 0x0FU;
            }
            break;
        case 7:
            appl_stored_record_count = 1U;

            /* Set the new sequence number to 4 records */
            for (index = 0U; index < APPL_STORED_RECORD_COUNT; index++)
            {
                gls_msrmt_obs_data[gls_msrmt_seq_id_index[index] - 2U] = 0x00U;
                BT_PACK_LE_2_BYTE
                (
                    &gls_msrmt_obs_data[gls_msrmt_seq_id_index[index]],
                    &appl_seg_index
                );

                appl_seg_index++;

                if (index < (APPL_STORED_RECORD_COUNT - 1U))
                {
                    BT_PACK_LE_2_BYTE
                    (
                        &(gls_msrmt_cntxt_obs_data[gls_cntxt_msrmt_seq_id_index[index]]),
                        &appl_seg_index
                    );
                }
            }
            break;
        case 8:
            appl_stored_record_count = 3U;
            appl_activate_gls_records();
            break;
        case 9:
            CONSOLE_OUT("Please enter the hours [DEC]:\n");
            CONSOLE_IN("%hd", &appl_gls_user_facing_time.hours);

            CONSOLE_OUT("Please enter the minutes [DEC]:\n");
            CONSOLE_IN("%hd", &appl_gls_user_facing_time.minutes);

            CONSOLE_OUT("Enter for how many records time offset needs to be change\n");
            CONSOLE_IN("%d", &total_records);

            CONSOLE_OUT("Enter the record number from which time offset needs to"
            " be change with new user facing time[DEC]:\n");
            CONSOLE_IN("%d", &record_no);

            /* Get the time offset of the previous record */
            BT_UNPACK_LE_2_BYTE
            (
                &prev_rec_timeoffset,
                &gls_msrmt_obs_data[gls_msrmt_seq_id_index[record_no - 2U] + 9U]
            );

            total_mins = appl_gls_convert_from_hrs_to_minutes
                         (
                             &appl_gls_user_facing_time,
                             prev_rec_timeoffset
                         );
            /**
             * Total Minutes is equal to
             *  new user facing time + previous records time offset
             */
            CONSOLE_OUT("\n%d hour + %d minutes = %d minutes\n\n", appl_gls_user_facing_time.hours,
                appl_gls_user_facing_time.minutes, total_mins);

            for (i = 0U; i < total_records; i++)
            {
                BT_PACK_LE_2_BYTE
                (
                    &gls_msrmt_obs_data[gls_msrmt_seq_id_index[record_no - 1U] + 9U],
                    (UINT16 *)&total_mins
                );
                record_no = record_no + 1U;
                total_mins += 1U;
            }
            break;
        case 10:
            appl_stored_record_count = 0U;
            break;
        case 11:
            appl_stored_record_count = 4U;
            appl_activate_gls_records();
            break;

        default:
            CONSOLE_OUT("Invalid Choice\n");
            break;
        }
    }
}
#endif /* APPL_MENU_OPS */
#endif /* (defined ATT && defined GLS) */

