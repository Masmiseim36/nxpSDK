/*
 * appl_beacon.h
 *
 *  Created on: Feb 18, 2022
 *      Author: nxf81885
 */
 /*
  *  Copyright (C) NXP
  *  All rights reserved.
  */

#ifndef APPL_REFERENCE_LE_PROFILE_APPL_APPL_BEACON_H_
#define APPL_REFERENCE_LE_PROFILE_APPL_APPL_BEACON_H_

typedef struct
	{
        	UCHAR length1;
		UCHAR Flags_data_type;                //Flags data type value -General Discoverable Mode
		UCHAR Flag_Data;                         //Flag Data
		UCHAR Length2;                            //Length
		UCHAR Service_UUID;                   //Complete list of  16-bit Service UUID data Type value
		UCHAR Eddystone_UUID1[2];        //16 bit Eddystone UUID
		UCHAR Length_of_service_data;
		UCHAR Servcie_Data_data_type;
		UCHAR  Eddystone_UUID2[2];
	}Pre_defined_Eddystone_header;

	void print_buffer(char *ptr,char size);
	void eddystone_config(void);
	void get_eddystone_format_header(UCHAR *adv_data);

#endif /* APPL_REFERENCE_LE_PROFILE_APPL_APPL_BEACON_H_ */
