
/**
 *  \file appl_avrcp_tg_virtual_media_player.c
 *
 *  Source file for AVRCP Controller Source Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_avrcp_tg_virtual_media_player.h"
#include "BT_avrcp_api.h"

#ifdef AVRCP_TG

/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */

APPL_AVRCP_COMPANY_ID_LIST             company_id_list;
APPL_AVRCP_EVENT_ID_LIST               event_id_list;
APPL_AVRCP_PLAYER_APPL_SETTING_INFO    player_appl_info;
APPL_AVRCP_MEDIA_ELEMENT_ATTRIBUTES    media_element_info;
APPL_AVRCP_MEDIA_PLAY_STATUS           cur_song_play_status;
UINT32                                 track_select_status;

UCHAR                                  bow_player_set_rsp_err = 0U;
UCHAR                                  skip_cover_art_handle_get_folder_items_rsp = 0U;

APPL_AVRCP_MEDIA_PLAYER_LIST_INFO      media_player_list_info [APPL_AVRCP_MAX_NUM_PLAYERS];
UINT16                                 num_players;

APPL_AVRCP_VIRTUAL_FOLDER_LIST_INFO    vf_list_info[APPL_AVRCP_MAX_VF_DEPTH][APPL_AVRCP_MAX_FOLDER_ITEMS];
UINT32                                 root_folder_item_count;
UINT32                                 child_folder_item_count;
UCHAR                                  curr_folder_depth;

APPL_AVRCP_MEDIA_LIST_INFO             media_search_list_info [APPL_AVRCP_MAX_SEARCH_LIST_COUNT];
UINT32                                 media_search_list_count;

APPL_AVRCP_MEDIA_LIST_INFO             media_list_info [APPL_AVRCP_MAX_MEDIA_LIST_COUNT];
UINT16                                 media_list_count;

UINT16                                 global_uid_counter;

UCHAR                                  battery_status;
UCHAR                                  system_status;
UCHAR                                  system_volume;

APPL_AVRCP_PLAYER_EVENT                registered_event_list[APPL_AVRCP_MAX_PLAYER_EVENT_COUNT];

UCHAR                                  change_media_elem_attr_flag = 0U;

/* Set the ID's to players */
UINT16                      addressed_player_id;
UINT16                      browsed_player_id;
UINT16                      current_player_id;
UINT16                      player_change_flag;

UINT32  media_attr_ids [APPL_AVRCP_MAX_ATTR_COUNT] =
{
    AVRCP_MEDIA_ATTR_ID_MEDIA_TITLE,
    AVRCP_MEDIA_ATTR_ID_ARTIST_NAME,
    AVRCP_MEDIA_ATTR_ID_ALBUM_NAME,
    AVRCP_MEDIA_ATTR_ID_MEDIA_NUMBER,
    AVRCP_MEDIA_ATTR_ID_TOTAL_MEDIA_NUMBER,
    AVRCP_MEDIA_ATTR_ID_GENRE,
    AVRCP_MEDIA_ATTR_ID_PLAYING_TIME
#ifdef AVRCP_COVER_ART
    ,
    AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART
#endif /* AVRCP_COVER_ART */
};


/* ----------------------------------------- Static Global Variables */
static BT_fops_file_handle fptr;

/* Virtual Media Player Setting update Menu */
static const char avrcp_update_media_player_menu[] = " \n\
--------------- Update Media Player Settings Menu ---------------------- \n\n\
  0.  Exit. \n\
  1.  Refresh. \n\
  2.  Update Company ID List. \n\
  3.  Update Event ID List. \n\
  4.  Update Player Application Setting. \n\
  5.  Update Media Element Attributes.\n\
  6.  Update Current Media Play Status.\n\
  7.  Update Media Player IDs. \n\
  8.  Update Media Player List. \n\
  9.  Update Virtual Filesystem. \n\
 10.  Update Media Element List. \n\
 11.  Update System Params. \n\
 12.  Update Media Player Type. \n\
 13.  Update Track Selection Status. \n\
 14.  Add New Media Element to Virtual Filesystem. \n\n\
 15.  Set Browsing Player response. \n\
 16.  Set No. of Items in child Folder \n\
 17.  Enable/Disable Cover Art Handle in response \n\n\
 20.  Re-Init Media Player. \n\
  \n\
 30.  Change Current Playing Track. \n\
  \n\
Your Option -> \0";

/* Virtual Media Player Setting Display Menu */
static const char avrcp_display_media_player_menu[] = " \n\
--------------- Display Media Player Settings Menu ---------------------- \n\n\
  0.  Exit. \n\
  1.  Refresh. \n\
  2.  Display Company ID List. \n\
  3.  Display Event ID List. \n\
  4.  Display Player Application Setting. \n\
  5.  Display Media Element Attributes.\n\
  6.  Display Current Media Play Status.\n\
  7.  Display Media Player IDs. \n\
  8.  Display Media Player List. \n\
  9.  Display Virtual Filesystem. \n\
 10.  Display Media Element List. \n\
 11.  Display System Params. \n\
 12.  Display Media Player Type. \n\
 13.  Display Track Selection Status. \n\
  \n\
Your Option -> \0";
/* ----------------------------------------- Functions */


API_RESULT  appl_mp_init (void)
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* Read Company ID List */
    retval = appl_mp_read_company_id_list();
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG ("Failed: appl_mp_read_company_id_list.\n");
    }

    /* Read Event ID List */
    retval = appl_mp_read_event_id_list();
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG ("Failed: appl_mp_read_event_id_list.\n");
    }

    /* Read player application attribute info */
    retval = appl_mp_read_player_appl_attr_info ();
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG ("Failed: appl_mp_read_player_appl_attr_info.\n");
    }

    /* Read Media Element attribute info */
    retval = appl_mp_read_media_element_attr_info();
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG ("Failed: appl_mp_read_media_element_attr_info.\n");
    }

    /* Read Current Media Play Status Info */
    retval = appl_mp_read_cur_media_play_status_info();
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG ("Failed: appl_mp_read_cur_media_play_status_info.\n");
    }

    /* Read Current Media Play Status Info */
    retval = appl_mp_read_media_player_list();
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG ("Failed: appl_mp_read_media_player_list.\n");
    }

    if (API_SUCCESS == retval)
    {
        /* Read Virtual Filesystem Items */
        (BT_IGNORE_RETURN_VALUE) appl_mp_read_virtual_filesystem_items
        (
            0x00U /* root folder */
        );

        (BT_IGNORE_RETURN_VALUE) appl_mp_read_virtual_filesystem_items
        (
            0x01U /* child folder */
        );

        (BT_IGNORE_RETURN_VALUE) appl_mp_read_media_element_items ();

        /* Reset Search List */
        appl_mp_reset_search_media_list ();

        /* Set Starting UID Counter */
        global_uid_counter  = APPL_AVRCP_DEFAULT_UID_COUNTER;

        battery_status = AVRCP_BATTERY_STATUS_NORMAL;
        system_status  = AVRCP_SYSTEM_STATUS_POWER_ON;
        system_volume  = 16U;

        /* Track Selected */
        track_select_status = 0U;
    }

    return retval;
}

/* Reading avrcp capability for company id support */
API_RESULT appl_mp_read_company_id_list(void)
{
    API_RESULT retval;
    unsigned int read_val;
    UINT16  i;

    retval = API_SUCCESS;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    read_val = 0U;

    /* Read supported Company ID list */
    /* fptr = fopen((CHAR *)APPL_AVRCP_TG_COMMAND_ID_FILE, "r"); */
    retval = BT_fops_file_open(APPL_AVRCP_TG_COMMAND_ID_FILE, (UCHAR *)"r", &fptr);

    if (NULL == fptr)
    {
        LOG_DEBUG ("Failed to open the file: %s\n", APPL_AVRCP_TG_COMMAND_ID_FILE);
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);
        company_id_list.no_ids = (UCHAR )read_val;

        for (i = 0U; i < company_id_list.no_ids; i++)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);
            company_id_list.company_id_list[i] = read_val;
        }

        /* Close the file */
        if (NULL != fptr)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fptr);
            fptr = NULL;
        }
    }

    return retval;
}

API_RESULT appl_mp_display_company_id_list(void)
{
    API_RESULT      retval;
    UINT16          i;

    retval = API_SUCCESS;

    LOG_DEBUG ("== Company ID List(%d) ==\n", company_id_list.no_ids);
    for (i = 0U; i < company_id_list.no_ids; i++)
    {
        LOG_DEBUG ("  - 0x%06x\n", company_id_list.company_id_list[i]);
    }

    return retval;
}


/* Reading avrcp capability for event id support */
API_RESULT appl_mp_read_event_id_list(void)
{
    API_RESULT      retval;
    unsigned int    read_val;
    UINT16          i;

    retval = API_SUCCESS;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    read_val = 0U;

    /* Read supported Company ID list */
    /* fptr = fopen((CHAR *)APPL_AVRCP_TG_EVENT_ID_FILE, "r"); */
    retval = BT_fops_file_open(APPL_AVRCP_TG_EVENT_ID_FILE, (UCHAR *)"r", &fptr);

    if (NULL == fptr)
    {
        LOG_DEBUG ("Failed to open the file: %s\n", APPL_AVRCP_TG_EVENT_ID_FILE);
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);
        event_id_list.no_ids = (UCHAR )read_val;

        for (i = 0U; i < event_id_list.no_ids; i++)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);

            event_id_list.event_id_list[i] = (UCHAR )read_val;
        }

        /* Close the file */
        if (NULL != fptr)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fptr);
            fptr = NULL;
        }
    }

    return retval;
}

/* Display avrcp capability for event id support */
API_RESULT appl_mp_display_event_id_list(void)
{
    API_RESULT      retval;
    UINT16          i;

    retval = API_SUCCESS;

    LOG_DEBUG ("== Event ID List(%d) ==\n", event_id_list.no_ids);
    for (i = 0U; i < event_id_list.no_ids; i++)
    {
        LOG_DEBUG ("  - 0x%02x\n", event_id_list.event_id_list[i]);
    }

    return retval;
}

API_RESULT  appl_mp_read_player_appl_attr_info(void)
{
    int    read_val;
    API_RESULT      retval;
    UINT16          i, j, size;

    retval = API_SUCCESS;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    read_val = 0;

    /* Read Player Application setting */
    /* fptr = fopen((CHAR *)APPL_AVRCP_TG_PLAYER_APPL_ATTR_INFO_FILE, "r"); */
    retval = BT_fops_file_open(APPL_AVRCP_TG_PLAYER_APPL_ATTR_INFO_FILE, (UCHAR *)"r", &fptr);
    if (NULL == fptr)
    {
        LOG_DEBUG ("Failed to open the file: %s\n", APPL_AVRCP_TG_PLAYER_APPL_ATTR_INFO_FILE);
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);
        player_appl_info.no_attr = (UCHAR )read_val;

        for (i = 0U; i <  player_appl_info.no_attr; i++)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);
            player_appl_info.attr_id_info[i].attr_id = (UCHAR)read_val;

            size = sizeof(player_appl_info.attr_id_info[i].attr_id_text);
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted
            (fptr, "%s", player_appl_info.attr_id_info[i].attr_id_text, &size);

            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);
            player_appl_info.attr_id_info[i].attr_id_val_count = (UCHAR)read_val;

            for (j = 0U; j < player_appl_info.attr_id_info[i].attr_id_val_count; j++)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);
                player_appl_info.attr_id_info[i].attr_id_val[j].val = (UCHAR )read_val;

                size = sizeof(player_appl_info.attr_id_info[i].attr_id_val[j].val_text);
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted
                (fptr, "%s", player_appl_info.attr_id_info[i].attr_id_val[j].val_text, &size);
            }

            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);
            player_appl_info.attr_id_info[i].cur_attr_val.val = (UCHAR)read_val;
            size = sizeof(player_appl_info.attr_id_info[i].cur_attr_val.val_text);
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted
            (fptr, "%s", player_appl_info.attr_id_info[i].cur_attr_val.val_text, &size);
        }

        /* Close the file */
        if (NULL != fptr)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fptr);
            fptr = NULL;
        }
    }

    return retval;
}

API_RESULT  appl_mp_display_player_appl_attr_info(void)
{
    API_RESULT      retval;
    UINT16          i, j;

    retval = API_SUCCESS;

    LOG_DEBUG ("\n==== Player Appl. Attr. ID's (%d)===\n", player_appl_info.no_attr);
    for (i = 0U; i < player_appl_info.no_attr; i++)
    {
        LOG_DEBUG (" %s (0x%02x)\n", player_appl_info.attr_id_info[i].attr_id_text,
                              player_appl_info.attr_id_info[i].attr_id);

        for (j = 0U; j < player_appl_info.attr_id_info[i].attr_id_val_count; j++)
        {
            LOG_DEBUG ("  - %s (0x%02x)\n", player_appl_info.attr_id_info[i].attr_id_val[j].val_text,
                                     player_appl_info.attr_id_info[i].attr_id_val[j].val);

        }

        LOG_DEBUG ("  @ %s (0x%02x)\n", player_appl_info.attr_id_info[i].cur_attr_val.val_text,
                                      player_appl_info.attr_id_info[i].cur_attr_val.val);
    }
    LOG_DEBUG ("\n");

    return retval;
}

API_RESULT  appl_mp_read_media_element_attr_info(void)
{
    int             read_val;
    API_RESULT      retval;
    UINT16          i, str_len;
    UINT16          size;

    if (1U == change_media_elem_attr_flag)
    {
        /* Read Media Element Attribute Information */
        /* fptr = fopen((CHAR *)APPL_AVRCP_TG_MEDIA_ELEMENT_ATTR_INFO_FILE, "r"); */
        retval = BT_fops_file_open(APPL_AVRCP_TG_MEDIA_ELEMENT_ATTR_INFO_FILE_1, (UCHAR *)"r", &fptr);
    }
    else if (2U == change_media_elem_attr_flag)
    {
        /* Read Media Element Attribute Information */
        /* fptr = fopen((CHAR *)APPL_AVRCP_TG_MEDIA_ELEMENT_ATTR_INFO_FILE, "r"); */
        retval = BT_fops_file_open(APPL_AVRCP_TG_MEDIA_ELEMENT_LARGE_ATTR_INFO_FILE, (UCHAR *)"r", &fptr);
    }
    else
    {
        /* Read Media Element Attribute Information */
        /* fptr = fopen((CHAR *)APPL_AVRCP_TG_MEDIA_ELEMENT_ATTR_INFO_FILE, "r"); */
        retval = BT_fops_file_open(APPL_AVRCP_TG_MEDIA_ELEMENT_ATTR_INFO_FILE, (UCHAR *)"r", &fptr);

    }

    if (NULL == fptr)
    {
        LOG_DEBUG ("Failed to open the file: %s\n", APPL_AVRCP_TG_MEDIA_ELEMENT_ATTR_INFO_FILE);
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        retval = API_SUCCESS;

        /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
        read_val = 0;

        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);
        media_element_info.num_attr = (UCHAR )read_val;

        for (i = 0U; i < media_element_info.num_attr; i++)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);

    #ifndef AVRCP_COVER_ART
            if (0x08U == read_val)
            {
                UCHAR   attr_str[64U];

                /**
                 * Just read attribute info and skip updating
                 * the media_element_info
                 */
                size = sizeof(attr_str);
                BT_fops_file_get_formatted
                (fptr, "%s", attr_str, &size);

                /**
                 * Don't consider CoverArt Attribute ID,
                 * so decrementing total no. of attributes by 1
                 */
                media_element_info.num_attr --;
                break;
            }
    #endif /* AVRCP_COVER_ART */

            media_element_info.attr_id_info[i].attr_id = (UINT32)read_val;

            media_element_info.attr_id_info[i].attr_char_set = 0x006AU;

            size = sizeof(media_element_info.attr_id_info[i].attr_str);
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted
            (fptr, "%s", media_element_info.attr_id_info[i].attr_str, &size);

            str_len = (UCHAR)BT_str_len(media_element_info.attr_id_info[i].attr_str);

            media_element_info.attr_id_info[i].attr_len = str_len;
        }

        /* Close the file */
        if (NULL != fptr)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fptr);
            fptr = NULL;
        }
    }

    return retval;
}

API_RESULT  appl_mp_display_media_element_attr_info(void)
{
    API_RESULT retval;
    UCHAR i, num_attr;

    retval = API_SUCCESS;

    num_attr = media_element_info.num_attr;

    LOG_DEBUG ("Media Element Attributes(0x%0x)\n", num_attr);
    for (i = 0U; i < num_attr; i++)
    {
        LOG_DEBUG ("   %d. Attr. ID: 0x%04x\n", (i+1U), media_element_info.attr_id_info[i].attr_id);
       /* LOG_DEBUG ("   - char set: 0x%02x\n", media_element_info.attr_id_info[i].attr_char_set); */
        LOG_DEBUG ("      - Value: %s\n", media_element_info.attr_id_info[i].attr_str);
    }

    return retval;
}


void appl_mp_update_media_player_setting (void)
{
    API_RESULT  retval;
    int choice, menu_choice, read_val;

    BT_LOOP_FOREVER()
    {
        printf ("%s", avrcp_update_media_player_menu); fflush(stdin);
        scanf ("%d", &choice);

        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            (BT_IGNORE_RETURN_VALUE) appl_mp_read_company_id_list ();
            LOG_DEBUG ("==== Updated Company ID List =====\n");
            break;

        case 3:
            (BT_IGNORE_RETURN_VALUE) appl_mp_read_event_id_list ();
            LOG_DEBUG ("==== Updated Event ID List =====\n");
            break;

        case 4:
            (BT_IGNORE_RETURN_VALUE) appl_mp_read_player_appl_attr_info ();
            LOG_DEBUG ("==== Updated Player Application Attribute Info =====\n");
            break;

        case 5:
            (BT_IGNORE_RETURN_VALUE) appl_mp_read_media_element_attr_info ();
            LOG_DEBUG ("==== Updated Media Element Attribute Info =====\n");
            break;

        case 6:
            (BT_IGNORE_RETURN_VALUE) appl_mp_read_cur_media_play_status_info ();
            LOG_DEBUG ("==== Updated Media Play Status =====\n");
            break;

        case 7:
            LOG_DEBUG ("Select Media Player Type\n");
            LOG_DEBUG ("  0 -> Addressed Player\n");
            LOG_DEBUG ("  1 -> Browsed Player\n");
            scanf ("%d", &read_val);
            if (0U == read_val)
            {
                LOG_DEBUG ("Enter Addressed Player ID(in Hex)\n");
                scanf ("%x", &read_val);
                addressed_player_id = (UINT16)read_val;

                LOG_DEBUG ("==== Updated Addressed Player ID =====\n");
            }
            else
            {
                LOG_DEBUG ("Enter Browsed Player ID(in Hex)\n");
                scanf ("%x", &read_val);
                browsed_player_id = (UINT16)read_val;

                LOG_DEBUG ("==== Updated Browsing Player ID =====\n");
            }

            break;

        case 8:
            retval = appl_mp_read_media_player_list ();
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG ("Update Media Player List Success!!!\n");
            }
            else
            {
                LOG_DEBUG ("Update Media Player List Failed!!!\n");
            }
            break;

        case 9:
            LOG_DEBUG ("Select Folder:\n");
            LOG_DEBUG ("  0 -> Root\n");
            LOG_DEBUG ("  1 -> Child\n");
            scanf ("%d", &read_val);

            retval = appl_mp_read_virtual_filesystem_items
                     (
                         (UCHAR )read_val
                     );
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG ("Update Virtual Filesystem List Success!!!\n");
            }
            else
            {
                LOG_DEBUG ("Update Virtual Filesystem List Failed!!!\n");
            }
            break;

        case 10:
            retval = appl_mp_read_media_element_items ();
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG ("Update Media Element Item List Success!!!\n");
            }
            else
            {
                LOG_DEBUG ("Update Media Element Item List Failed!!!\n");
            }
            break;

        case 11:
            LOG_DEBUG ("Select Options:\n");
            LOG_DEBUG ("  0 -> Battery Status\n");
            LOG_DEBUG ("  1 -> System Status\n");
            LOG_DEBUG ("  2 -> Systerm Volume\n");
            scanf ("%d", &read_val);

            if (0x00U == read_val)
            {
                LOG_DEBUG ("Select Options:\n");
                LOG_DEBUG ("   0x0 -> AVRCP_BATTERY_STATUS_NORMAL\n");
                LOG_DEBUG ("   0x1 -> AVRCP_BATTERY_STATUS_WARNING\n");
                LOG_DEBUG ("   0x2 -> AVRCP_BATTERY_STATUS_CRITICAL\n");
                LOG_DEBUG ("   0x3 -> AVRCP_BATTERY_STATUS_EXTERNAL\n");
                LOG_DEBUG ("   0x4 -> AVRCP_BATTERY_STATUS_FULL\n");

                scanf ("%d", &read_val);

                if (read_val < 4U)
                {
                    battery_status = (UCHAR )read_val;
                }

                LOG_DEBUG ("==== Updated Battery Status =====\n");
            }
            else if (0x01U == read_val)
            {

                LOG_DEBUG ("Select Options:\n");
                LOG_DEBUG ("   0x0 -> AVRCP_SYSTEM_STATUS_POWER_ON\n");
                LOG_DEBUG ("   0x1 -> AVRCP_SYSTEM_STATUS_POWER_OFF\n");
                LOG_DEBUG ("   0x2 -> AVRCP_SYSTEM_STATUS_UNPLUGED\n");

                scanf ("%d", &read_val);

                if (read_val < 4U)
                {
                    system_status = (UCHAR )read_val;
                }

                LOG_DEBUG ("==== Updated System Status =====\n");
            }
            else if (0x02U == read_val)
            {
                LOG_DEBUG ("Enter Volume(less <= 127): \n");
                scanf ("%d", &read_val);

                system_volume = (UCHAR )read_val;

                LOG_DEBUG ("==== Updated System Volume =====\n");
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            break;

        case 12:
            LOG_DEBUG ("Select Options:\n");
            LOG_DEBUG ("  0 -> Database Unaware Player\n");
            LOG_DEBUG ("  1 -> Database Aware Player\n");
            scanf ("%d", &read_val);
            if (0x00U == read_val)
            {
                global_uid_counter = 0x00U;
            }
            else
            {
                global_uid_counter = APPL_AVRCP_DEFAULT_UID_COUNTER;
            }

            break;

        case 13:
            LOG_DEBUG ("Select Options:\n");
            LOG_DEBUG ("  0 -> No Track Selected\n");
            LOG_DEBUG ("  1 -> Track Selected\n");
            LOG_DEBUG ("  2 -> Track Selected with Identifier\n");
            scanf ("%d", &read_val);
            if (0U == read_val)
            {
                track_select_status = 0xFFFFFFFFU;
            }
            else if (1U == read_val)
            {
                 track_select_status = 0U;
            }
            else
            {
                track_select_status = (UINT32)read_val;
            }

            break;

        case 14:
            LOG_DEBUG ("Please Update the File new_media_element_file before selectiong the Folder\n");
            LOG_DEBUG ("Select Folder:\n");
            LOG_DEBUG ("  0 -> Root\n");
            LOG_DEBUG ("  1 -> Child\n");
            scanf ("%d", &read_val);

            retval = appl_mp_add_media_item_to_virtual_filesystem
                     (
                         (UCHAR )read_val
                     );
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG ("Added Media Item to Virtual Filesystem\n");
            }
            else
            {
                LOG_DEBUG ("Failed to Add Media Item to Virtual Filesystem\n");
            }

            break;

        case 15:
            LOG_DEBUG ("Set Set Browsing Player response(in Hex)\n");
            LOG_DEBUG ("  0x00 -> Success\n");
            LOG_DEBUG ("  0x12 -> AVRCP_BOW_ERROR_PLAYER_NOT_BROWSABLE\n");
            LOG_DEBUG ("  0x13 -> AVRCP_BOW_ERROR_PLAYER_NOT_ADDRESSED\n");
            scanf ("%x", &read_val);

            bow_player_set_rsp_err = (UCHAR)read_val;

            break;

        case 16:
            LOG_DEBUG ("Set Child Folder Count\n");
            scanf ("%d", &read_val);

            child_folder_item_count = (UCHAR)read_val;

            break;

        case 17:
            LOG_DEBUG ("Select below option\n");
            LOG_DEBUG ("  0x00 -> Send Cover Art Handle in Get Folder Items response\n");
            LOG_DEBUG ("  0x01 -> Do not Send Cover Art Handle in Get Folder Items response\n");
            scanf ("%d", &read_val);

            skip_cover_art_handle_get_folder_items_rsp  = (UCHAR)read_val;
            break;

        case 20:

            bow_player_set_rsp_err = 0U;
            change_media_elem_attr_flag = 0U;
            skip_cover_art_handle_get_folder_items_rsp = 0U;

            retval = appl_mp_init ();
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG ("Media Player Init Success\n");
            }
            else
            {
                LOG_DEBUG ("Media Player Init Failed\n");
            }
            break;

        case 30:
            LOG_DEBUG("Change current playing track response\n");
            LOG_DEBUG("  0x00 -> Reset to original playing track \n");
            LOG_DEBUG("  0x01 -> Change currently playing track \n");
            LOG_DEBUG("  0x02 -> Media Element with large size attribute values \n");

            scanf("%d", &read_val);

            change_media_elem_attr_flag = (UCHAR)read_val;
            break;

        default:
            LOG_DEBUG("Invalid Option: %d\n", choice);
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

void appl_mp_display_media_player_setting (void)
{
    int choice, menu_choice, read_val;

    BT_LOOP_FOREVER()
    {
        LOG_DEBUG ("%s", avrcp_display_media_player_menu); fflush(stdin);
        scanf ("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            (BT_IGNORE_RETURN_VALUE) appl_mp_display_company_id_list ();
            break;

        case 3:
            (BT_IGNORE_RETURN_VALUE) appl_mp_display_event_id_list ();
            break;

        case 4:
            (BT_IGNORE_RETURN_VALUE) appl_mp_display_player_appl_attr_info ();
            break;

        case 5:
            (BT_IGNORE_RETURN_VALUE) appl_mp_display_media_element_attr_info ();
            break;

        case 6:
            (BT_IGNORE_RETURN_VALUE) appl_mp_display_cur_media_play_status_info ();
            break;

        case 7: /* Fall Through */
            LOG_DEBUG ("Player IDs:\n");
            LOG_DEBUG ("  Addressed Player ID: 0x%04x\n", addressed_player_id);
            LOG_DEBUG ("  Browsed Player ID  : 0x%04x\n", browsed_player_id);

        case 8:
            (BT_IGNORE_RETURN_VALUE) appl_mp_display_media_player_list ();
            break;

        case 9:
            LOG_DEBUG ("Select Folder:\n");
            LOG_DEBUG ("  0 -> Root\n");
            LOG_DEBUG ("  1 -> Child\n");
            scanf ("%d", &read_val);

            (BT_IGNORE_RETURN_VALUE) appl_mp_display_virtual_filesystem_items
            (
                (UCHAR )read_val
            );
            break;

        case 10:
            (BT_IGNORE_RETURN_VALUE) appl_mp_display_media_element_list ();
            break;

        case 11:
            LOG_DEBUG ("Select Options:\n");
            LOG_DEBUG ("  0 -> Battery Status\n");
            LOG_DEBUG ("  1 -> System Status\n");
            LOG_DEBUG ("  2 -> System Volume\n");
            scanf ("%d", &read_val);

            if (0x00 == read_val)
            {
                LOG_DEBUG ("Battery Status: ");
                switch(battery_status)
                {
                case AVRCP_BATTERY_STATUS_NORMAL:
                    LOG_DEBUG ("AVRCP_BATTERY_STATUS_NORMAL\n");
                    break;
                case AVRCP_BATTERY_STATUS_WARNING:
                    LOG_DEBUG ("AVRCP_BATTERY_STATUS_NORMAL\n");
                    break;
                case AVRCP_BATTERY_STATUS_CRITICAL:
                    LOG_DEBUG ("AVRCP_BATTERY_STATUS_NORMAL\n");
                    break;

                case AVRCP_BATTERY_STATUS_EXTERNAL:
                    LOG_DEBUG ("AVRCP_BATTERY_STATUS_NORMAL\n");
                    break;

                case AVRCP_BATTERY_STATUS_FULL:
                    LOG_DEBUG ("AVRCP_BATTERY_STATUS_NORMAL\n");
                    break;

                default:
                    LOG_DEBUG ("??\n");
                    break;
                }
            }
            else if (0x01 == read_val)
            {
                LOG_DEBUG ("System Status: ");
                switch (system_status)
                {
                case AVRCP_SYSTEM_STATUS_POWER_ON:
                    LOG_DEBUG ("AVRCP_SYSTEM_STATUS_POWER_ON\n");
                    break;

                case AVRCP_SYSTEM_STATUS_POWER_OFF:
                    LOG_DEBUG ("AVRCP_SYSTEM_STATUS_POWER_ON\n");
                    break;

                case AVRCP_SYSTEM_STATUS_UNPLUGED:
                    LOG_DEBUG ("AVRCP_SYSTEM_STATUS_POWER_ON\n");
                    break;

                default:
                    LOG_DEBUG ("??\n");
                    break;
                }
            }
            else if (0x02 == read_val)
            {
                LOG_DEBUG ("System Volume: 0x%02x\n", system_volume);
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            break;

        case 12:

            if (0x00U == global_uid_counter)
            {
                LOG_DEBUG ("Database Unaware Player\n");
            }
            else
            {
                LOG_DEBUG ("Database Aware Player\n");
                LOG_DEBUG ("UID Counter: 0x%04x\n", global_uid_counter);
            }

            break;

        case 13:
            if (0U == track_select_status)
            {
                LOG_DEBUG ("Yes\n");
            }
            else if (0xFFFFFFFFU == track_select_status)
            {
                LOG_DEBUG ("No\n");
            }
            else
            {
                LOG_DEBUG ("Yes With Identifier\n");
            }
            break;

        default:
            LOG_DEBUG("Invalid Option: %d\n", choice);
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

API_RESULT  appl_mp_read_cur_media_play_status_info (void)
{
    int read_val;
    API_RESULT retval;

    /* Read Media Element Attribute Information */
    /* fptr = fopen((CHAR *)APPL_AVRCP_TG_CUR_MEDIA_PLAY_STATUS_FILE, "r"); */
    retval = BT_fops_file_open(APPL_AVRCP_TG_CUR_MEDIA_PLAY_STATUS_FILE, (UCHAR *)"r", &fptr);

    if (NULL == fptr)
    {
        LOG_DEBUG ("Failed to open the file: %s\n", APPL_AVRCP_TG_CUR_MEDIA_PLAY_STATUS_FILE);
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        retval = API_SUCCESS;

        /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
        read_val = 0;

        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);

        /* Song total length */
        cur_song_play_status.song_total_len = (UINT32)read_val;

        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);

        /* Song current playing position */
        cur_song_play_status.song_cur_pos = (UINT32)read_val;

        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fptr, "%x", &read_val, NULL);

        /* Song play status */
        cur_song_play_status.song_play_status = (UCHAR)read_val;

        /* Close the file */
        if (NULL != fptr)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fptr);
            fptr = NULL;
        }
    }

    return retval;
}

API_RESULT  appl_mp_display_cur_media_play_status_info (void)
{
    API_RESULT retval;

    retval = API_SUCCESS;

    LOG_DEBUG ("Current Media Play Status:\n");
    LOG_DEBUG ("    - Song Total Length(in msec): %d\n", cur_song_play_status.song_total_len);
    LOG_DEBUG ("    - Song Current postion: %d\n", cur_song_play_status.song_cur_pos);
    LOG_DEBUG ("    - Song Play Status: ");

    switch(cur_song_play_status.song_play_status)
    {
    case 0x00U:
        LOG_DEBUG ("0x00(STOPPED)");
        break;

    case 0x01U:
        LOG_DEBUG ("0x01(PLAYING)");
        break;

    case 0x02U:
        LOG_DEBUG ("0x02(PAUSED)");
        break;

    case 0x03U:
        LOG_DEBUG ("0x03(FWD_SEEK)");
        break;

    case 0x04U:
        LOG_DEBUG ("0x04(REV_SEEK)");
        break;

    case 0xFFU:
        LOG_DEBUG ("0xFF(ERROR)");
        break;

    default:
        LOG_DEBUG ("??");
        break;
    }

    LOG_DEBUG ("\n");

    return retval;
}

/*
 * This function reads media player list from a file,
 * It is useful for handling GetMediaPlayerList request from CT.
 */
API_RESULT appl_mp_read_media_player_list (void)
{
    UCHAR i;
    unsigned int read_val;
    BT_fops_file_handle fp;
    API_RESULT retval;
    UINT16    size;

    /* Reset media_player_list_info */
    BT_mem_set
    (
        media_player_list_info,
        0x00,
        (sizeof(APPL_AVRCP_MEDIA_PLAYER_LIST_INFO) * APPL_AVRCP_MAX_NUM_PLAYERS)
    );

    /* Set the ID's to players */
    addressed_player_id = 0x0001U;
    browsed_player_id   = 0x0002U;
    retval = API_SUCCESS;

    current_player_id = addressed_player_id;

    /* fp = fopen ((CHAR *)APPL_AVRCP_TG_MEDIA_PLAYER_LIST_FILE, "r"); */
    retval = BT_fops_file_open(APPL_AVRCP_TG_MEDIA_PLAYER_LIST_FILE, (UCHAR *)"r", &fp);

    if (API_SUCCESS != retval)
    {
        LOG_DEBUG ("Failed to open the file: %s\n", APPL_AVRCP_TG_MEDIA_PLAYER_LIST_FILE);
        /* return retval; */
    }
    else
    {
        /* Num. of items */
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
        num_players = (UINT16 )read_val;

        /* Update the params */
        for (i = 0U; i < num_players; i++)
        {
            /* Item type */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].item_type = (UCHAR )read_val;

            /* Item length */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].item_len = (UINT16 )read_val;

            /* Player ID */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].player_id = (UINT16 )read_val;

            /* Major Player type */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].major_player_type = (UCHAR )read_val;

            /* Player sub type */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].player_sub_type = (UINT32 )read_val;

            /* Play status */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].play_status = (UCHAR )read_val;

            /* Feature Bit mask */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].features_bit_mask[3U] = (UINT32 )read_val;

            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].features_bit_mask[2U] = (UINT32 )read_val;

            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].features_bit_mask[1U] = (UINT32 )read_val;

            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].features_bit_mask[0U] = (UINT32 )read_val;

            /* char set */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].char_set = (UINT16 )read_val;

            /*  name len */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_player_list_info[i].name_len = (UINT16 )read_val;

            /* name */
            size = sizeof(media_player_list_info[i].name);
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted
            (fp, "%s", media_player_list_info[i].name, &size);
        }
    }

    return retval;
}

API_RESULT appl_mp_display_media_player_list(void)
{
    API_RESULT retval;
    UINT16 i, j;

    retval = API_SUCCESS;

    LOG_DEBUG ("No. of player: 0x%x\n", num_players);

    for (i = 0U; i < num_players; i++)
    {
        LOG_DEBUG ("--Player #%d--\n", i);
        LOG_DEBUG ("  Item Type: 0x%x\n", media_player_list_info[i].item_type);
        LOG_DEBUG ("  Item Length: 0x%x\n", media_player_list_info[i].item_len);
        LOG_DEBUG ("  Player Id: 0x%x\n", media_player_list_info[i].player_id);
        LOG_DEBUG ("  Major Player Type: 0x%x\n", media_player_list_info[i].major_player_type);
        LOG_DEBUG ("  Player Sub Type: 0x%x\n", media_player_list_info[i].player_sub_type);
        LOG_DEBUG ("  Play Status : 0x%x\n", media_player_list_info[i].play_status);
        LOG_DEBUG ("  Feature Bit Mask: 0x%x%x%x%x\n", media_player_list_info[i].features_bit_mask[3U],
                                                    media_player_list_info[i].features_bit_mask[2U],
                                                    media_player_list_info[i].features_bit_mask[1U],
                                                    media_player_list_info[i].features_bit_mask[0U]);
        LOG_DEBUG ("  Char Set Id: 0x%x\n", media_player_list_info[i].char_set);
        LOG_DEBUG ("  Display Name Length:0x%x\n", media_player_list_info[i].name_len);
        LOG_DEBUG ("  Display Name:");
        for (j = 0U; j < media_player_list_info[i].name_len; j++)
        {
            LOG_DEBUG ("%c", media_player_list_info[i].name[j]);
        }
        LOG_DEBUG ("\n");
    }

    return retval;
}

API_RESULT appl_mp_reset_virtual_folder_items
           (
               UCHAR    folder_depth
           )
{
    UCHAR    i;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    if ((APPL_AVRCP_MAX_VF_DEPTH - 1U) < folder_depth)
    {
        LOG_DEBUG ("Folder depth > APPL_AVRCP_MAX_VF_DEPTH\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        for (i = 0U; i < APPL_AVRCP_MAX_FOLDER_ITEMS; i++)
        {
            vf_list_info[folder_depth][i].item_type = 0x00U;

            vf_list_info[folder_depth][i].item_length = 0x00U;

            vf_list_info[folder_depth][i].item_uid_msb = 0x00U;

            vf_list_info[folder_depth][i].item_uid_msb = 0x00U;

            BT_mem_set
            (
                &vf_list_info[folder_depth][i].folder_item_info,
                0x00,
                sizeof(APPL_AVRCP_FOLDER_ITEM_TYPE_INFO)
            );

            vf_list_info[folder_depth][i].media_item_info.media_type = 0x00U;
            vf_list_info[folder_depth][i].media_item_info.char_set = 0x00U;
            vf_list_info[folder_depth][i].media_item_info.name_len = 0x00U;

            BT_mem_set
            (
                vf_list_info[folder_depth][i].media_item_info.name,
                0x00,
                sizeof(vf_list_info[folder_depth][i].media_item_info.name)
            );

            vf_list_info[folder_depth][i].media_item_info.num_attr = 0x00U;

            BT_mem_set
            (
                vf_list_info[folder_depth][i].media_item_info.attr_info,
                0x00,
                (sizeof(APPL_AVRCP_MEDIA_ITEM_ATTR_INFO) * APPL_AVRCP_MAX_ATTR_COUNT)
            );
        }
    }

    return retval;
}

/*
 * This function reads folder media items. It is useful for handling
 * GetFolderItems request from CT.
 */
API_RESULT appl_mp_read_virtual_filesystem_items
           (
               UCHAR    folder_depth
           )
{
    unsigned int        read_val;
    UCHAR               i, item_count;
    BT_fops_file_handle fp;
    API_RESULT          retval;

    /* Init */
    retval = API_SUCCESS;

    if ((APPL_AVRCP_MAX_VF_DEPTH - 1U) < folder_depth)
    {
        LOG_DEBUG ("Folder depth > APPL_AVRCP_MAX_VF_DEPTH\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        item_count = 0U;
        fp         = NULL;

        /* Reset Folder items */
        (BT_IGNORE_RETURN_VALUE) appl_mp_reset_virtual_folder_items
        (
            folder_depth
        );

        if (0x00U == folder_depth)
        {
            /* Get the root folder items */
            /* fp = fopen ((CHAR *)APPL_AVRCP_TG_ROOT_FOLDER_ITEMS_LIST_FILE, "r"); */
            retval = BT_fops_file_open(APPL_AVRCP_TG_ROOT_FOLDER_ITEMS_LIST_FILE, (UCHAR *)"r", &fp);

            if (NULL == fp)
            {
                LOG_DEBUG ("Failed to open the file: %s\n", APPL_AVRCP_TG_ROOT_FOLDER_ITEMS_LIST_FILE);
                retval = API_FAILURE; /* return API_FAILURE; */
            }
            else
            {
                /* Get no. of folder items */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
                root_folder_item_count = read_val;

                item_count = (UCHAR )read_val;
            }
        }
        else if (0x01U == folder_depth)
        {
            /* Get the child folder items */
            /* fp = fopen ((CHAR *)APPL_AVRCP_TG_FOLDER_DEPTH_1_ITEM_LIST_FILE, "r"); */
            retval = BT_fops_file_open(APPL_AVRCP_TG_FOLDER_DEPTH_1_ITEM_LIST_FILE, (UCHAR *)"r", &fp);

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to open the file: %s\n", APPL_AVRCP_TG_FOLDER_DEPTH_1_ITEM_LIST_FILE);
                retval = API_FAILURE; /* return API_FAILURE; */
            }
            else
            {
                /* Get no. of folder items */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
                child_folder_item_count = read_val;

                item_count = (UCHAR )read_val;
            }
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (API_SUCCESS == retval)
        {
            for (i = 0U; i < item_count; i++)
            {
                /* Update the media items to current_folder_info */
                appl_mp_update_virtual_folder_media_item
                (
                    &vf_list_info[folder_depth][i],
                    0U,
                    0x00U,
                    fp
                );
            }

            /* Close the file */
            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }
        }
    }

    return retval;
}


API_RESULT appl_mp_add_media_item_to_virtual_filesystem
           (
               UCHAR   folder_depth
           )
{
    UCHAR       curr_count;
    API_RESULT  retval;

    /* Init */
    curr_count = 0x00U;
    retval = API_SUCCESS;

    /* Check Folder depth */
    if (folder_depth >= APPL_AVRCP_MAX_VF_DEPTH)
    {
        LOG_DEBUG ("Invalid Folder Depth to Add an Media Item\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Open file */
        /* fptr = fopen((CHAR *)APPL_AVRCP_TG_NEW_MEDIA_ELEMENT_FILE, "r"); */
        retval = BT_fops_file_open(APPL_AVRCP_TG_NEW_MEDIA_ELEMENT_FILE, (UCHAR *)"r", &fptr);

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("Failed to open the file: %s\n", APPL_AVRCP_TG_NEW_MEDIA_ELEMENT_FILE);
            /* return retval; */
        }
        else
        {
            /* Validate Current Folder Depth */
            if (0x00U == curr_folder_depth)
            {
                curr_count = (UCHAR )root_folder_item_count;
            }
            else if (0x01U == curr_folder_depth)
            {
                curr_count = (UCHAR )child_folder_item_count;
            }
            else
            {
                LOG_DEBUG ("Incorrect Current Folder Depth\n");
                retval = API_FAILURE; /* return API_FAILURE; */
            }
        }

        if (curr_count >= APPL_AVRCP_MAX_FOLDER_ITEMS)
        {
            LOG_DEBUG ("Folder is Full, Can't Add New Item\n");
            retval = API_FAILURE; /* return API_FAILURE; */
        }

        if (API_SUCCESS == retval)
        {
            /* Update the media items to current_folder_info */
            appl_mp_update_virtual_folder_media_item
            (
                &vf_list_info[folder_depth][curr_count],
                0U, /* index */
                0x00U, /* uid offset */
                fptr
            );

            if (0x00U == curr_folder_depth)
            {
                root_folder_item_count ++;
            }
            else if (0x01U == curr_folder_depth)
            {
                child_folder_item_count ++;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            /* Increment UID counter */
            global_uid_counter++;
        }

        /* Close file */
        if (NULL != fptr)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fptr);
            fptr = NULL;
        }
    }

    return retval;
}


void appl_mp_update_virtual_folder_media_item
     (
         APPL_AVRCP_VIRTUAL_FOLDER_LIST_INFO   *vf,
         UCHAR                                  item_index,
         UINT32                                 uid_offset,
         BT_fops_file_handle                    fp
     )
{
    int       read_val;
    UCHAR     j;
    UCHAR     num_attr;
    UINT32    attr_id;
    UINT16    size;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    read_val = 0;

    /* Validate the parameters */
    if((NULL == fp) ||
       (NULL == vf) ||
       (APPL_AVRCP_MAX_FOLDER_ITEMS <= item_index))
    {
        LOG_DEBUG ("Invalid parameters \n ");
        /* return; */
    }
    else
    {
        /* Item type */
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
        vf[item_index].item_type = (UCHAR )read_val;

        /* Item length */
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
        vf[item_index].item_length = (UINT16 )read_val;

        /* Item UID MSB */
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
        vf[item_index].item_uid_msb = read_val;

        /* Item UID LSB */
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
        vf[item_index].item_uid_lsb = (read_val + uid_offset);

        if (APPL_AVRCP_ITEM_TYPE_FOLDER == vf[item_index].item_type)
        {
            /* ==== Update foler item type info === */

            /* Folder type */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            vf[item_index].folder_item_info.folder_type = (UCHAR )read_val;

            /* Is Playable */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            vf[item_index].folder_item_info.is_playable = (UCHAR )read_val;

            /* Char set */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            vf[item_index].folder_item_info.char_set = (UINT16 )read_val;

            /* name length */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            vf[item_index].folder_item_info.name_len = (UINT16 )read_val;

            /*  name */
            size = sizeof(vf[item_index].folder_item_info.name);
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted
            (fp, "%s", vf[item_index].folder_item_info.name, &size);

        }
        else if (APPL_AVRCP_ITEM_TYPE_MEDIA == vf[item_index].item_type)
        {
            /*  Media type */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            vf[item_index].media_item_info.media_type = (UCHAR )read_val;

            /* Char set */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            vf[item_index].media_item_info.char_set = (UINT16 )read_val;

            /* Name length */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            vf[item_index].media_item_info.name_len = (UINT16 )read_val;

            /* Name */
            size = sizeof(vf[item_index].media_item_info.name);
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted
            (fp, "%s", vf[item_index].media_item_info.name, &size);

            /* Number of attributes */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            vf[item_index].media_item_info.num_attr = (UCHAR )read_val;
            num_attr = (UCHAR )read_val;

            /* Update attributes id and values */
            for (j = 0U;  j < num_attr; j++)
            {
                /* Update Attr id */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);

#ifndef AVRCP_COVER_ART
                if (0x08U == read_val)
                {
                    UCHAR att_val[32U];

                    /**
                     * Just read attribute info and skip updating
                     * the vf
                     */

                    /* Update char set  */
                    BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);

                    /* Update value len */
                    BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);

                    /* Update value */
                    size = sizeof(att_val);
                    BT_fops_file_get_formatted
                    (fp, "%s", att_val, &size);

                    /**
                     * Don't consider CoverArt Attribute ID,
                     * so decrementing total no. of attributes by 1
                     */
                    vf[item_index].media_item_info.num_attr --;

                    break;
                }
#endif /* AVRCP_COVER_ART */

                attr_id = (UINT32 )read_val;

                vf[item_index].media_item_info.attr_info[attr_id-1U].attr_id
                    = read_val;

                /* Update char set  */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
                vf[item_index].media_item_info.attr_info[attr_id-1U].char_set
                    = (UINT16 )read_val;

                /* Update value len */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
                vf[item_index].media_item_info.attr_info[attr_id-1U].att_val_len
                    = (UINT16 )read_val;

                /* Update value */
                size = sizeof(vf[item_index].media_item_info.attr_info[attr_id-1U].att_val);
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted
                (fp, "%s", vf[item_index].media_item_info.attr_info[attr_id-1U].att_val, &size);
            }
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }

    return;
}

API_RESULT appl_mp_display_virtual_filesystem_items
           (
               UCHAR    folder_depth
           )
{
    API_RESULT retval;
    UCHAR      i, j, k, item_count;

    retval     = API_SUCCESS;
    item_count = 0U;

    if ((APPL_AVRCP_MAX_VF_DEPTH - 1U) < folder_depth)
    {
        LOG_DEBUG ("Folder depth > APPL_AVRCP_MAX_VF_DEPTH\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        if (0x00U == folder_depth)
        {
            item_count = (UCHAR )root_folder_item_count;

            LOG_DEBUG ("==== Root Folder Items=====\n");
        }
        else if (0x01U == folder_depth)
        {
            item_count = (UCHAR )child_folder_item_count;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        /* LOG_DEBUG folder items */
        for (i = 0U; i < item_count; i++)
        {
            LOG_DEBUG ("---Item #%d ---\n", (i+1U));

            LOG_DEBUG ("ITEM_TYPE: ");
            if (APPL_AVRCP_ITEM_TYPE_FOLDER == vf_list_info[folder_depth][i].item_type)
            {
                LOG_DEBUG ("Folder\n");

                /* Folder UID */
                LOG_DEBUG ("FOLDER UID: ");
                LOG_DEBUG ("0x%x", vf_list_info[folder_depth][i].item_uid_msb);
                LOG_DEBUG ("%x\n", vf_list_info[folder_depth][i].item_uid_lsb);

                /* Folder type */
                LOG_DEBUG ("TYPE: ");
                switch(vf_list_info[folder_depth][i].folder_item_info.folder_type)
                {
                    case AVRCP_VFS_FOLDER_TYPE_MIXED:
                        LOG_DEBUG ("Mixed\n");
                        break;

                    case AVRCP_VFS_FOLDER_TYPE_TITLES:
                        LOG_DEBUG ("Titles\n");
                        break;

                    case AVRCP_VFS_FOLDER_TYPE_ALBUMS:
                        LOG_DEBUG ("Albums\n");
                        break;

                    case AVRCP_VFS_FOLDER_TYPE_ARTISTS:
                        LOG_DEBUG ("Artists\n");
                        break;

                    case AVRCP_VFS_FOLDER_TYPE_GENRES:
                        LOG_DEBUG ("Genres\n");
                        break;

                    case AVRCP_VFS_FOLDER_TYPE_PLAYLISTS:
                        LOG_DEBUG ("Playlists\n");
                        break;

                    case AVRCP_VFS_FOLDER_TYPE_YEARS:
                        LOG_DEBUG ("Years\n");
                        break;

                    default:
                        LOG_DEBUG ("Reserved\n");
                        break;
                }

                LOG_DEBUG ("Is playable: ");
                if (0 != vf_list_info[folder_depth][i].folder_item_info.is_playable)
                {
                    LOG_DEBUG ("Yes\n");
                }
                else
                {
                    LOG_DEBUG ("No\n");
                }

                LOG_DEBUG ("DISPLAY NAME LEN: 0x%x\n", vf_list_info[folder_depth][i].folder_item_info.name_len);
                LOG_DEBUG ("DISPLAY NAME: ");
                for (j = 0U; j < vf_list_info[folder_depth][i].folder_item_info.name_len; j++)
                {
                    LOG_DEBUG ("%c", vf_list_info[folder_depth][i].folder_item_info.name[j]);
                }
                LOG_DEBUG ("\n");
            }
            else
            {
                LOG_DEBUG ("Media Element Item\n");

                /* Media Element UID */
                LOG_DEBUG ("MEDIA ELEMENT UID: ");
                LOG_DEBUG ("0x%x", vf_list_info[folder_depth][i].item_uid_msb);
                LOG_DEBUG ("%x\n", vf_list_info[folder_depth][i].item_uid_lsb);

                 /* Media Element type */
                LOG_DEBUG ("TYPE: ");
                switch(vf_list_info[folder_depth][i].media_item_info.media_type)
                {
                    case AVRCP_MEDIA_ELEMENT_TYPE_AUDIO:
                        LOG_DEBUG ("Audio\n");
                        break;

                    case AVRCP_MEDIA_ELEMENT_TYPE_VIDEO:
                        LOG_DEBUG ("Video\n");
                        break;

                    default:
                        LOG_DEBUG ("Reserved\n");
                        break;
                }

                LOG_DEBUG ("DISPLAY NAME LEN: 0x%x \n", vf_list_info[folder_depth][i].media_item_info.name_len);
                LOG_DEBUG ("DISPLAY NAME: ");
                for (j = 0U; j < vf_list_info[folder_depth][i].media_item_info.name_len; j++)
                {
                    LOG_DEBUG ("%c", vf_list_info[folder_depth][i].media_item_info.name[j]);
                }
                LOG_DEBUG ("\n");

                LOG_DEBUG ("ATTRIBUTE COUNT: %d\n", vf_list_info[folder_depth][i].media_item_info.num_attr);
                for (j = 0U; j < APPL_AVRCP_MAX_ATTR_COUNT; j++)
                {
                    if (0x00U != vf_list_info[folder_depth][i].media_item_info.attr_info[j].attr_id)
                    {
                        LOG_DEBUG ("    ID: 0x%x\n", vf_list_info[folder_depth][i].media_item_info.attr_info[j].attr_id);

                        LOG_DEBUG ("    VALUE: ");
                        for (k = 0U; k < vf_list_info[folder_depth][i].media_item_info.attr_info[j].att_val_len; k++)
                        {
                            LOG_DEBUG ("%c", vf_list_info[folder_depth][i].media_item_info.attr_info[j].att_val[k]);
                        }
                        LOG_DEBUG ("\n");
                    }
                }
            }
        }
    }

    return retval;
}

/*
 * This function read media element items from a file. It is useful for handling
 * GetSearch Items, GetNowPlaying Items request from CT.
 */
API_RESULT appl_mp_read_media_element_items (void)
{
    int                 read_val;
    UCHAR               i, j;
    BT_fops_file_handle fp;
    UCHAR               num_attr;
    UINT32              attr_id;
    API_RESULT          retval;
    UINT16              size;

    /* Reset Media Element List */
    appl_mp_reset_media_element_list ();
    retval = API_SUCCESS;

    /* Read Media Element  List */
    /* fp = fopen ((CHAR *)APPL_AVRCP_TG_MEDIA_ELEMENT_LIST_FILE, "r"); */
    retval = BT_fops_file_open(APPL_AVRCP_TG_MEDIA_ELEMENT_LIST_FILE, (UCHAR *)"r", &fp);

    if (API_SUCCESS != retval)
    {
        LOG_DEBUG ("Failed to open the file: %s\n", APPL_AVRCP_TG_MEDIA_ELEMENT_LIST_FILE);
        retval = API_FAILURE; /* return retval; */
    }
    else
    {
        /* Get number media items */
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
        media_list_count = (UINT16 )read_val;

        for (i = 0U; i < media_list_count; i++)
        {
            /* Item type */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_list_info[i].item_type = (UCHAR )read_val;

            /* Item length */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_list_info[i].item_length = (UINT16 )read_val;

            /* Item UID MSB */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_list_info[i].item_uid_msb = read_val;

            /* Item UID LSB */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_list_info[i].item_uid_lsb = read_val;

            /*  Media type */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_list_info[i].media_item_info.media_type = (UCHAR )read_val;

            /* Char set */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_list_info[i].media_item_info.char_set = (UINT16 )read_val;

            /* Name length */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_list_info[i].media_item_info.name_len = (UINT16 )read_val;

            /* Name */
            size = sizeof(media_list_info[i].media_item_info.name);
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted
            (fp, "%s", media_list_info[i].media_item_info.name, &size);

            /* Number of attributes */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
            media_list_info[i].media_item_info.num_attr = (UCHAR )read_val;
            num_attr = (UCHAR )read_val;

            /* Update attributes id and values */
            for (j = 0U; j < num_attr; j++)
            {
                /* Update Attr id */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
                attr_id = read_val;

#ifndef AVRCP_COVER_ART
                if (0x08U == attr_id)
                {
                    UCHAR attr_val[32U];

                    /**
                     * Just read attribute info and skip updating
                     * the media_list_info
                     */

                    /* Read char set  */
                    BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);

                    /* Read value len */
                    BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);

                    /* Read value */
                    size = sizeof(attr_val);
                    BT_fops_file_get_formatted
                    (fp, "%s", attr_val, &size);

                    /**
                     * Don't consider CoverArt Attribute ID,
                     * so decrementing total no. of attributes by 1
                     */
                    media_list_info[i].media_item_info.num_attr --;
                    break;
                }
#endif /* AVRCP_COVER_ART */

                media_list_info[i].media_item_info.attr_info[attr_id-1U].attr_id
                    = read_val;

                /* Update char set  */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
                media_list_info[i].media_item_info.attr_info[attr_id-1U].char_set
                    = (UINT16 )read_val;

                /* Update value len */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted (fp, "%x", &read_val, NULL);
                media_list_info[i].media_item_info.attr_info[attr_id-1U].att_val_len
                    = (UINT16 )read_val;

                /* Update value */
                size = sizeof(media_list_info[i].media_item_info.attr_info[attr_id-1U].att_val);
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_get_formatted
                (fp, "%s", media_list_info[i].media_item_info.attr_info[attr_id-1U].att_val, &size);
            }
        }

        /* Close the file */
        if (NULL != fp)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
        }
    }

    return retval;
}

void appl_mp_reset_search_media_list (void)
{
    UCHAR   i;

    media_search_list_count = 0x00U;

    for (i = 0U; i < APPL_AVRCP_MAX_SEARCH_LIST_COUNT; i++)
    {
        media_search_list_info[i].item_type    = 0x00U;
        media_search_list_info[i].item_length  = 0x00U;
        media_search_list_info[i].item_uid_msb = 0x00U;
        media_search_list_info[i].item_uid_lsb = 0x00U;

        media_search_list_info[i].media_item_info.media_type = 0x00U;
        media_search_list_info[i].media_item_info.char_set = 0x00U;
        media_search_list_info[i].media_item_info.name_len = 0x00U;

        BT_mem_set
        (
            &media_search_list_info[i].media_item_info.name,
            0x00,
            sizeof(media_search_list_info[i].media_item_info.name)
        );

        media_search_list_info[i].media_item_info.num_attr = 0x00U;

        BT_mem_set
        (
            &media_search_list_info[i].media_item_info.attr_info,
            0x00,
            (sizeof(APPL_AVRCP_MEDIA_ITEM_ATTR_INFO) * APPL_AVRCP_MAX_ATTR_COUNT)
        );
    }

    return;
}

void appl_mp_reset_media_element_list (void)
{
    UCHAR   i;

    for (i = 0U; i < APPL_AVRCP_MAX_MEDIA_LIST_COUNT; i++)
    {
        media_list_info[i].item_type    = 0x00U;
        media_list_info[i].item_length  = 0x00U;
        media_list_info[i].item_uid_msb = 0x00U;
        media_list_info[i].item_uid_lsb = 0x00U;

        media_list_info[i].media_item_info.media_type = 0x00U;
        media_list_info[i].media_item_info.char_set = 0x00U;
        media_list_info[i].media_item_info.name_len = 0x00U;

        BT_mem_set
        (
            &media_list_info[i].media_item_info.name,
            0x00,
            sizeof(media_list_info[i].media_item_info.name)
        );

        media_list_info[i].media_item_info.num_attr = 0x00U;

        BT_mem_set
        (
            &media_list_info[i].media_item_info.attr_info,
            0x00,
            (sizeof(APPL_AVRCP_MEDIA_ITEM_ATTR_INFO) * APPL_AVRCP_MAX_ATTR_COUNT)
        );
    }

    return;
}

API_RESULT appl_mp_display_media_element_list (void)
{
    API_RESULT      retval;
    UCHAR           i, j, k;

    retval = API_SUCCESS;

    for (i = 0U; i < media_list_count; i++)
    {
        LOG_DEBUG ("ITEM_TYPE: ");
        if (APPL_AVRCP_ITEM_TYPE_MEDIA == media_list_info[i].item_type)
        {
            LOG_DEBUG ("Media Element Item\n");
        }

        LOG_DEBUG ("ITEM_LEN: 0x%04x\n", media_list_info[i].item_length);

        /* Media Element UID */
        LOG_DEBUG ("MEDIA ELEMENT UID: ");
        LOG_DEBUG ("0x%04x", media_list_info[i].item_uid_msb);
        LOG_DEBUG ("%04x\n", media_list_info[i].item_uid_lsb);

        /* Media Element type */
        LOG_DEBUG ("TYPE: ");
        switch(media_list_info[i].media_item_info.media_type)
        {
            case AVRCP_MEDIA_ELEMENT_TYPE_AUDIO:
                 LOG_DEBUG ("Audio\n");
                 break;

            case AVRCP_MEDIA_ELEMENT_TYPE_VIDEO:
                 LOG_DEBUG ("Video\n");
                 break;

            default:
                 LOG_DEBUG ("Reserved\n");
                 break;
        }

        LOG_DEBUG ("DISPLAY NAME LEN: 0x%x", media_list_info[i].media_item_info.name_len);
        LOG_DEBUG ("DISPLAY NAME: ");
        for (j = 0U; j < media_list_info[i].media_item_info.name_len; j++)
        {
            LOG_DEBUG ("%c", media_list_info[i].media_item_info.name[j]);
        }
        LOG_DEBUG ("\n");

        LOG_DEBUG ("ATTRIBUTE COUNT: %d\n", media_list_info[i].media_item_info.num_attr);
        for (j = 0U; j < APPL_AVRCP_MAX_ATTR_COUNT; j++)
        {
            if (0x00U != media_list_info[i].media_item_info.attr_info[j].attr_id)
            {
                LOG_DEBUG ("    ID: 0x%x\n", media_list_info[i].media_item_info.attr_info[j].attr_id);

                LOG_DEBUG ("    VALUE: ");
                for (k = 0U; k < media_list_info[i].media_item_info.attr_info[j].att_val_len; k++)
                {
                    LOG_DEBUG ("%c", media_list_info[i].media_item_info.attr_info[j].att_val[k]);
                }
                LOG_DEBUG ("\n");
            }
        }
    }

    return retval;
}


API_RESULT appl_mp_register_player_event
           (
               UCHAR event_id,
               UCHAR avrcp_tl
           )
{
    API_RESULT retval;

    if (event_id <= APPL_AVRCP_MAX_PLAYER_EVENT_COUNT)
    {
        registered_event_list[event_id-1U].reg_status = 0x01U;
        registered_event_list[event_id-1U].avrcp_tl = avrcp_tl;

        retval = API_SUCCESS; /* return API_SUCCESS; */
    }
    else
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }

    return retval;
}

/* Get the avrcp_tl for registered event */
UCHAR appl_mp_get_registered_player_event_tl (UCHAR event_id)
{
    UCHAR       avrcp_tl;

    avrcp_tl = 0xFFU;

    if (APPL_AVRCP_MAX_PLAYER_EVENT_COUNT >= event_id)
    {
        avrcp_tl = registered_event_list[event_id-1U].avrcp_tl;
    }

    return avrcp_tl;
}

#endif /* AVRCP_TG */
