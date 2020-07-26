#! /usr/bin/env python3
'''
Copyright 2019 NXP.
This software is owned or controlled by NXP and may only be used strictly in accordance with the
license terms that accompany it. By expressly accepting such terms or by downloading, installing,
activating and/or otherwise using the software, you are agreeing that you have read, and that you
agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
applicable license terms, then you may not retain, install, activate or otherwise use the software.
'''

import serial # pyserial module

number_of_mics = 3

def capture_g_afe():
    while(1):
        mic_one_read_data = ser.read(320)
        mic_stream_1_file.write(mic_one_read_data)

        mic_two_read_data = ser.read(320)
        mic_stream_2_file.write(mic_two_read_data)

        if number_of_mics == '3':
            mic_three_read_data = ser.read(320)
            mic_stream_3_file.write(mic_three_read_data)

        amp_read_data = ser.read(320)
        amp_ref_file.write(amp_read_data)

        clean_read_data = ser.read(320)
        clean_processed_audio_file.write(clean_read_data)

        if PRE_DOWNSAMPLED_ENABLE:
            pre_downsampled_data = ser.read(960)
            pre_downsampled_audio_file.write(pre_downsampled_data)
            if not pre_downsampled_data:
                break
        else:
            if not clean_read_data:
                break

MIC_STREAM_SIZE_1=320
MIC_STREAM_SIZE_2=320
MIC_STREAM_SIZE_3=320
AMP_REFERENCE_SIGNAL_SIZE=320
CLEAN_AUDIO_SIZE=320
PRE_DOWNSAMPLED_ENABLE=False
ser = None

test_name = input("Enter Test Name: ")

MIC_FILE_NAME_STREAM_1= test_name + "_mic_stream_1.bin"
MIC_FILE_NAME_STREAM_2= test_name + "_mic_stream_2.bin"
MIC_FILE_NAME_STREAM_3= test_name + "_mic_stream_3.bin"
AMP_REF_FILE_NAME_STREAM= test_name + "_amp_ref_stream.bin"
CLEAN_PROCESSSED_AUDIO_FILE_NAME_STREAM= test_name + "_clean_processed_audio.bin"
AMP_REF_SIGNAL_PRE_DOWNSAMPLED_FILE_NAME_STREAM= test_name + "_ref_signal_pre_downsampled_audio.bin"
BEAM_1_AUDIO_FILE_NAME_STREAM= test_name + "_beam_1.bin"
BEAM_2_AUDIO_FILE_NAME_STREAM= test_name + "_beam_2.bin"

mic_stream_1_file = open(MIC_FILE_NAME_STREAM_1, "wb");
mic_stream_2_file = open(MIC_FILE_NAME_STREAM_2, "wb");
mic_stream_3_file = open(MIC_FILE_NAME_STREAM_3, "wb");
amp_ref_file = open(AMP_REF_FILE_NAME_STREAM, "wb");
clean_processed_audio_file = open(CLEAN_PROCESSSED_AUDIO_FILE_NAME_STREAM, "wb");
pre_downsampled_audio_file = open(AMP_REF_SIGNAL_PRE_DOWNSAMPLED_FILE_NAME_STREAM, "wb");
beam_1_file = open(BEAM_1_AUDIO_FILE_NAME_STREAM, "wb");
beam_2_file = open(BEAM_2_AUDIO_FILE_NAME_STREAM, "wb");

port_num = input("Enter the COM Number to connect: ")
# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port= 'COM'+ port_num,
    baudrate=2048000,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    inter_byte_timeout=2
)

#ser.open()

number_of_mics = input("Enter 2 or 3 for Number of Microphones used: ")
input = input("Press Enter to start >> ")

if input == 'exit':
    ser.close()
    exit()
else:
    ser.write(b'c')

capture_g_afe()

mic_stream_1_file.close()
mic_stream_2_file.close()
amp_ref_file.close()
clean_processed_audio_file.close()
