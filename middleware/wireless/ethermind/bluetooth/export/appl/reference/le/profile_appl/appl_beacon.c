/*
 * appl_beacon.c
 *
 *  Created on: Feb 9, 2022
 *      Author: nxf81885
 */
 /*
  *  Copyright (C) NXP
  *  All rights reserved.
  */


#include "appl_hci_le.h"
#include "appl_utils.h"
#include "BT_hci_api.h"
#include "appl_beacon.h"
#define DEBUG_BEACON 0

Pre_defined_Eddystone_header Eddystone_header ;
void print_buffer(char *ptr,char size)
{
	APPL_TRC("\n");
	for(int c=0;c <size;c++)
	{
		APPL_TRC("I %d_%X  ",c,*(ptr+c));
	}
	APPL_TRC("\n");
}

/* REQUIRED FOR REFERENCE
   advertising_data[0]  =0x02;  //Length
   advertising_data[1]  =0x01;  //Flags data type value -General Discoverable Mode
   advertising_data[2]  =0x02;  //Flag Data
   advertising_data[3]  =0x03;  //Length
   advertising_data[4]  =0x03;  //Complete list of  16-bit Service UUID data Type value
   advertising_data[5]  =0xaa;  //16 bit Eddystone UUID
   advertising_data[6]  =0xfe;
   advertising_data[7]  =(UCHAR) (read_val+7U); Enter Manually // Length of URL data with hhtp + .com
   advertising_data[8]  =0x16;  //Service Data data type Value
   advertising_data[9]  =0xaa;  //16 bit Eddystone UUID
   advertising_data[10] =0xfe;*/


void get_eddystone_format_header(UCHAR *adv_data)
{

	Eddystone_header.length1                = 0x02 ;
	Eddystone_header.Flags_data_type        = BT_AD_FLAGS_LE_LIMITED_DISC_MODE;     //Flags data type value -General Discoverable Mode Check SIG Document,CSS_v10 0x01
	Eddystone_header.Flag_Data              = 0x02;     //Flag Data
	Eddystone_header.Length2                = 0x03;     //Length
	Eddystone_header.Service_UUID           = HCI_EIR_DATA_TYPE_COMPLETE_16_BIT_UUIDS; //Complete list of  16-bit Service UUID data Type value/0x03;
	Eddystone_header.Eddystone_UUID1[0]     = 0xaa;     //16 bit Eddystone UUID
	Eddystone_header.Eddystone_UUID1[1]     = 0xfe;
	Eddystone_header.Length_of_service_data = 0;        //filled from user
	Eddystone_header.Servcie_Data_data_type = HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS;     //Servcie Data data type Value/0x16
	Eddystone_header.Eddystone_UUID2[0]     = 0xaa;     //16 bit Eddystone UUID
	Eddystone_header.Eddystone_UUID2[1]     = 0xfe;
	memcpy(adv_data,&Eddystone_header,sizeof(Eddystone_header));
}

void  eddystone_config(void)
{

	API_RESULT   retval;
	UCHAR        advertising_channel_map;
	UCHAR        advertising_filter_policy;
	UCHAR        advertising_type;
	UCHAR        appl_direct_addr[BT_BD_ADDR_SIZE];
	UCHAR        direct_addr_type;
	UCHAR        own_addr_type;
	UINT16       advertising_interval_max;
	UINT16       advertising_interval_min;

	UCHAR        advertising_data[31U];
	UCHAR        advertising_data_len;
	unsigned int read_val;
	char         scan_url[18];
	int len;


	appl_hci_le_read_transmit_power();
    BT_sleep(1U);  //TODO: we will have to a. reduce sleep b.move this to LE event handler(which is tedious task)
	retval = BT_hci_le_set_advertising_enable (HCI_ADVERTISING_DISABLE);//Disable Advertisement
    if(API_SUCCESS != retval)
	{
		APPL_TRC("FAILED !!! Error code = %04X\n", retval);
		return;
	}
	else
	{
		APPL_TRC("Advertisement Disabled ...\n");
	}

	/*Configure Advertisement parameter*/
	direct_addr_type         = 0U;                  // appl_direct_addr not applicable for Broadcasting
	advertising_interval_min = (UINT16) 512;        //20ms Min       / 0X0200
	advertising_interval_max = (UINT16) 16384;      //10.24 Sec Max / 0X4000
	advertising_type         = HCI_ADV_NONCONN_IND; //Non connect able undirected advertising: 0x03 (UCHAR) 0x03;
	own_addr_type            = BD_ADDR_TYPE_PUBLIC; //Public Device Address: \ref BT_BD_PUBLIC_ADDRESS_TYPE (0x00)
	direct_addr_type         = BD_ADDR_TYPE_PUBLIC; //Public Device or Public Identity Address: (UCHAR) 0x00;
	advertising_channel_map  = (UCHAR) 7;          // All Channels used for Adv  three bit, 111 -all enable,001 ony one chahel for Adv.
	advertising_filter_policy= (UCHAR) 0;          // Filter Policy not applicable

	retval = BT_hci_le_set_advertising_parameters
	(
		advertising_interval_min,
		advertising_interval_max,
		advertising_type,
		own_addr_type,
		direct_addr_type,
		appl_direct_addr,
		advertising_channel_map,
		advertising_filter_policy
	);

	if(API_SUCCESS != retval)
	{
		APPL_TRC("FAILED !!! Error code = %04X\n", retval);
	    return;
	}
	else
	{
		APPL_TRC("API Advertisement parameter set returned success...\n");
	}

	BT_mem_set(advertising_data, 0, 31U); // Clear Advertisement data
	get_eddystone_format_header(&advertising_data[0]);

	advertising_data[11] = 0x10; // Type URL
	get_le_max_tx_power(&advertising_data[12]);
	APPL_TRC("\nPOWER->%X\n",advertising_data[12]);   // Power at Adv data 12

    APPL_TRC("\nPRESS OPTION-> \n0 - http://www. \n1 - https://www. \n2 - http:// \n3 - https://\n");
	scanf("%d", &read_val);
	advertising_data[13] = (UCHAR) (read_val);

	APPL_TRC("Enter URL {NOTE:-allowed char 17} ->\n");
	scanf("%17s", scan_url);                  // Fail safe:-Character restriction at 17 not more then this is allowed
	strcpy((char *)&advertising_data[14], scan_url);
	len=strlen((char *)&advertising_data[14]);
	printf("Length of String -> %d\n",len);     // Fail safe:- No space in URL

	/*Length variable with Change in string to be filled*/
	advertising_data[7]  = len+6u;                  // 6 byte are part of length calculation including advertising_data[8],9,10,11,12,13,len[14 to Enetered string]
	advertising_data_len = advertising_data[7]+	8U; // 8 byte are fix till we enter the Size. i.e.. advertising_data[0] to 7

#if DEBUG_BEACON
	APPL_TRC("Advertising_data_len  =%d", advertising_data_len );
	APPL_TRC("Advertisement packet: \n");
	print_buffer(advertising_data,31);
#endif

	retval = BT_hci_le_set_advertising_data(advertising_data_len,advertising_data);
	if(API_SUCCESS != retval)
	{
		APPL_TRC("FAILED !!! Error code = %04X\n", retval);
		return;
	}
	else
	{
		APPL_TRC("API returned success...\n");
	}
	retval = BT_hci_le_set_advertising_enable ( HCI_ADVERTISING_ENABLE);

	if(API_SUCCESS != retval)
	{
		APPL_TRC("FAILED !!! Error code = %04X\n", retval);
		return;
	}
	else
	{
		APPL_TRC("\n........Becaon Enabled......\n");
	}
}
