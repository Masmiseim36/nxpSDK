# Microsoft Developer Studio Project File - Name="Kinetis_SDK" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Kinetis_SDK - Win32 Debug
!MESSAGE Dies ist kein g�ltiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und f�hren Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "Kinetis_SDK.mak".
!MESSAGE 
!MESSAGE Sie k�nnen beim Ausf�hren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "Kinetis_SDK.mak" CFG="Kinetis_SDK - Win32 Debug"
!MESSAGE 
!MESSAGE F�r die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "Kinetis_SDK - Win32 Release" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE "Kinetis_SDK - Win32 Debug" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Kinetis_SDK - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Kinetis_SDK - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Kinetis_SDK - Win32 Release"
# Name "Kinetis_SDK - Win32 Debug"
# Begin Group "devices"

# PROP Default_Filter ""
# Begin Group "MK02"

# PROP Default_Filter ""
# Begin Group "MK02_drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_vref.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MK02F12810\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\MK02F12810.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\MK02F12810_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\RTE_Device.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\system_MK02F12810.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK02F12810\system_MK02F12810.h
# End Source File
# End Group
# Begin Group "MK22"

# PROP Default_Filter ""
# Begin Group "MK22 CMSIS Driver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK22F51212\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\cmsis_drivers\fsl_uart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\cmsis_drivers\fsl_uart_cmsis.h
# End Source File
# End Group
# Begin Group "MK22 Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_flexbus.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_flexbus.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_adapter.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_controller.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_controller.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_flexnvm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_flexnvm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftfx_utilities.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_rnga.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_rnga.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_vref.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Group "Utils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\fsl_assert.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\fsl_debug_console.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\fsl_debug_console.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\fsl_debug_console_conf.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\io\fsl_io.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\io\fsl_io.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\log\fsl_log.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\log\fsl_log.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\fsl_notifier.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\fsl_notifier.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\fsl_sbrk.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\fsl_shell.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\fsl_shell.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\str\fsl_str.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\str\fsl_str.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\io\swo\fsl_swo.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\io\swo\fsl_swo.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\usb_device_cdc_acm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\usb_device_cdc_acm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\usb_device_ch9.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\usb_device_ch9.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\usb_device_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\usb_device_descriptor.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\usb_device_descriptor.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\virtual_com.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\utilities\virtual_com.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MK22F51212\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\MK22F51212.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\MK22F51212_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\system_MK22F51212.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK22F51212\system_MK22F51212.h
# End Source File
# End Group
# Begin Group "MK28"

# PROP Default_Filter ""
# Begin Group "MK28 drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_cmt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_cmt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexbus.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexbus.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_camera.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_camera.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_camera_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_camera_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_mculcd.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_mculcd.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_mculcd_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_mculcd_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_lmem_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_lmem_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_qspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_qspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_qspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_qspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sdramc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sdramc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sysmpu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_sysmpu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_tpm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_tpm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_vref.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Group "MK28 CMSIS drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK28F15\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MK28F15\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\MK28F15.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\MK28F15_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\system_MK28F15.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK28F15\system_MK28F15.h
# End Source File
# End Group
# Begin Group "MK64F12"

# PROP Default_Filter ""
# Begin Group "MK64 drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_cmt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_cmt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_flexbus.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_flexbus.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_rnga.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_rnga.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_sdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_sdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_sysmpu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_sysmpu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_vref.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Group "MK64 CMSIS drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK64F12\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\cmsis_drivers\fsl_enet_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\cmsis_drivers\fsl_enet_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\cmsis_drivers\fsl_enet_phy_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\cmsis_drivers\fsl_enet_phy_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\cmsis_drivers\fsl_uart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\cmsis_drivers\fsl_uart_cmsis.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MK64F12\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\MK64F12.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\MK64F12_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\system_MK64F12.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK64F12\system_MK64F12.h
# End Source File
# End Group
# Begin Group "MK65"

# PROP Default_Filter ""
# Begin Group "MK65_drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_cmt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_cmt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dspi_ucosii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dspi_ucosii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dspi_ucosiii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_dspi_ucosiii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_flexbus.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_flexbus.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_i2c_ucosii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_i2c_ucosii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_i2c_ucosiii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_i2c_ucosiii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lmem_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lmem_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lpuart_ucosii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lpuart_ucosii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lpuart_ucosiii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_lpuart_ucosiii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_mpu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_mpu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_rnga.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_rnga.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_sdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_sdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_sdramc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_sdramc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_tpm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_tpm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_tsi_v4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_tsi_v4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\cmsis_drivers\fsl_uart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\cmsis_drivers\fsl_uart_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_uart_ucosii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_uart_ucosii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_uart_ucosiii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_uart_ucosiii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_vref.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MK65F18\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\MK65F18.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\MK65F18_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\system_MK65F18.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK65F18\system_MK65F18.h
# End Source File
# End Group
# Begin Group "MK66"

# PROP Default_Filter ""
# Begin Group "MK66_drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_cmt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_cmt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_flexbus.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_flexbus.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_lmem_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_lmem_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_mpu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_mpu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_rnga.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_rnga.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sdramc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sdramc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sysmpu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_sysmpu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_tpm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_tpm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_tsi_v4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_tsi_v4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_vref.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MK66F18\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\MK66F18.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\MK66F18_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\system_MK66F18.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK66F18\system_MK66F18.h
# End Source File
# End Group
# Begin Group "MK80"

# PROP Default_Filter ""
# Begin Group "MK80_drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_cmt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_cmt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dspi_ucosii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dspi_ucosii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dspi_ucosiii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_dspi_ucosiii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexbus.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexbus.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_camera.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_camera.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_camera_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_camera_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_i2c_ucosii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_i2c_ucosii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_i2c_ucosiii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_i2c_ucosiii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lmem_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lmem_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lpuart_ucosii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lpuart_ucosii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lpuart_ucosiii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_lpuart_ucosiii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_mpu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_mpu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_qspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_qspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_qspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_qspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_sdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_sdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_sdramc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_sdramc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_emvsim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_emvsim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_phy_emvsim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_phy_emvsim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_phy_tda8035.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_phy_tda8035.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_ucosii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_ucosii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_ucosiii.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smartcard_ucosiii.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_tpm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_tpm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_tsi_v4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_tsi_v4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_vref.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MK80F25615\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\MK80F25615.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\MK80F25615_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\system_MK80F25615.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK80F25615\system_MK80F25615.h
# End Source File
# End Group
# Begin Group "MK82"

# PROP Default_Filter ""
# Begin Group "MK82 Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_cmt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_cmt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexbus.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexbus.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_camera.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_camera.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_camera_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_camera_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_lmem_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_lmem_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_ltc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_ltc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_ltc_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_ltc_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_mpu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_mpu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_qspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_qspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_qspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_qspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_sdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_sdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_sdramc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_sdramc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smartcard.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smartcard_emvsim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smartcard_emvsim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smartcard_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smartcard_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smartcard_phy_emvsim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smartcard_phy_emvsim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smartcard_phy_ncn8025.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smartcard_phy_ncn8025.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_tpm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_tpm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_tsi_v4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_tsi_v4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_vref.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MK82F25615\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\MK82F25615.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\MK82F25615_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\gcc\startup_MK82F25615.S
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\system_MK82F25615.c
# End Source File
# Begin Source File

SOURCE=.\devices\MK82F25615\system_MK82F25615.h
# End Source File
# End Group
# Begin Group "MKL25Z4"

# PROP Default_Filter ""
# Begin Group "MKL25 CMSIS Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKL25Z4\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\cmsis_drivers\fsl_lpsci_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\cmsis_drivers\fsl_lpsci_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\cmsis_drivers\fsl_spi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\cmsis_drivers\fsl_spi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\cmsis_drivers\fsl_uart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\cmsis_drivers\fsl_uart_cmsis.h
# End Source File
# End Group
# Begin Group "MKL25 Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_cop.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_cop.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_dma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_dma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_i2c_dma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_i2c_dma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_lpsci.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_lpsci.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_lpsci_dma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_lpsci_dma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_spi_dma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_spi_dma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_tpm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_tpm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_tsi_v4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_tsi_v4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_uart_dma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\drivers\fsl_uart_dma.h
# End Source File
# End Group
# Begin Group "MKL25 Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKL25Z4\utilities\fsl_debug_console.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\utilities\fsl_debug_console.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\utilities\fsl_notifier.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\utilities\fsl_notifier.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\utilities\fsl_sbrk.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\utilities\fsl_shell.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\utilities\fsl_shell.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MKL25Z4\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\MKL25Z4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\MKL25Z4_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\gcc\startup_MKL25Z4.S
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\system_MKL25Z4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL25Z4\system_MKL25Z4.h
# End Source File
# End Group
# Begin Group "MKL82Z7"

# PROP Default_Filter ""
# Begin Group "MKL82 CMSIS drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKL82Z7\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Group "MKL82 Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_camera.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_camera.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_camera_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_camera_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_adapter.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_controller.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_controller.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_flexnvm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_flexnvm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ftfx_utilities.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_intmux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_intmux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ltc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ltc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ltc_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_ltc_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_qspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_qspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_qspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_qspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_smartcard.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_smartcard_emvsim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_smartcard_emvsim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_smartcard_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_smartcard_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_smartcard_phy.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_smartcard_phy_emvsim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_smartcard_phy_tda8035.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_sysmpu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_sysmpu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_tpm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_tpm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_tsi_v4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_tsi_v4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_vref.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Group "MKL82 Project Template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKL82Z7\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\project_template\pin_mux.h
# End Source File
# End Group
# Begin Group "MKL82 utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKL82Z7\utilities\fsl_assert.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\utilities\fsl_notifier.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\utilities\fsl_notifier.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\utilities\fsl_sbrk.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\utilities\fsl_shell.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\utilities\fsl_shell.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\utilities\usb_device_config.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MKL82Z7\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\MKL82Z7.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\MKL82Z7_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\gcc\startup_MKL82Z7.S
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\system_MKL82Z7.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKL82Z7\system_MKL82Z7.h
# End Source File
# End Group
# Begin Group "MKV11"

# PROP Default_Filter ""
# Begin Group "MKV11 CMSIS Driver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKV11Z7\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\cmsis_drivers\fsl_uart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\cmsis_drivers\fsl_uart_cmsis.h
# End Source File
# End Group
# Begin Group "MKV11 Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_flexcan_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_flexcan_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_mmdvsq.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_mmdvsq.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MKV11Z7\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\MKV11Z7.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\MKV11Z7_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\system_MKV11Z7.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV11Z7\system_MKV11Z7.h
# End Source File
# End Group
# Begin Group "MKV31F51212"

# PROP Default_Filter ""
# Begin Group "KMV31 CMSIS Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKV31F51212\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\cmsis_drivers\fsl_uart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\cmsis_drivers\fsl_uart_cmsis.h
# End Source File
# End Group
# Begin Group "MKV31 Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_flexbus.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_flexbus.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_adapter.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_controller.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_controller.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_flexnvm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_flexnvm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftfx_utilities.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_rnga.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_rnga.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_vref.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Group "MKV31 template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKV31F51212\template\RTE_Device.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MKV31F51212\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\MKV31F51212.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\MKV31F51212_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\system_MKV31F51212.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV31F51212\system_MKV31F51212.h
# End Source File
# End Group
# Begin Group "MKV46F16"

# PROP Default_Filter ""
# Begin Group "MKV46 CMSIS Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKV46F16\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\cmsis_drivers\fsl_uart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\cmsis_drivers\fsl_uart_cmsis.h
# End Source File
# End Group
# Begin Group "MKV46 Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_aoi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_aoi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_cadc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_cadc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_enc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_enc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_flexcan_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_flexcan_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_adapter.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_controller.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_controller.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_flexnvm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_flexnvm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftfx_utilities.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_wdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_xbara.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_xbara.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_xbarb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\drivers\fsl_xbarb.h
# End Source File
# End Group
# Begin Group "MKV46 Project templates"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKV46F16\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\project_template\pin_mux.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MKV46F16\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\MKV46F16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\MKV46F16_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\gcc\startup_MKV46F16.S
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\system_MKV46F16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV46F16\system_MKV46F16.h
# End Source File
# End Group
# Begin Group "MKV58F24"

# PROP Default_Filter ""
# Begin Group "MKV58F24 CMSIS Driver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKV58F24\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\cmsis_drivers\fsl_uart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\cmsis_drivers\fsl_uart_cmsis.h
# End Source File
# End Group
# Begin Group "MKV58F24 Driver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_aoi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_aoi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_enc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_enc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_flexbus.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_flexbus.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_flexcan_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_flexcan_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_ftm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_ftm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_hsadc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_hsadc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_pdb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_pdb.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_sysmpu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_sysmpu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_wdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_xbara.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_xbara.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_xbarb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\drivers\fsl_xbarb.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MKV58F24\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\MKV58F24.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\MKV58F24_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\system_MKV58F24.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKV58F24\system_MKV58F24.h
# End Source File
# End Group
# Begin Group "MKW31Z4"

# PROP Default_Filter ""
# Begin Group "MKW31Z4 CMSIS Driver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_cmt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_cmt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_cop.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_cop.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dcdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dcdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_ltc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_ltc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_ltc_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_ltc_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_tpm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_tpm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_tsi_v4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_tsi_v4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\drivers\fsl_vref.h
# End Source File
# End Group
# Begin Group "MKW31Z4 Driver"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\devices\MKW31Z4\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\MKW31Z4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\MKW31Z4_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\RTE_Device.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\system_MKW31Z4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW31Z4\system_MKW31Z4.h
# End Source File
# End Group
# Begin Group "MKW41Z4"

# PROP Default_Filter ""
# Begin Group "MKW41Z4 CMSIS Driver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKW41Z4\cmsis_drivers\fsl_dspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\cmsis_drivers\fsl_dspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\cmsis_drivers\fsl_i2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\cmsis_drivers\fsl_i2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Group "MKW41Z4 Driver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_adc16.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_adc16.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_cmt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_cmt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_cop.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_cop.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dac.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dac.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dcdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dcdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_dspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_flash.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_i2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_i2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_ltc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_ltc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_ltc_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_ltc_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_pmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_pmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_rcm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_rcm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_rtc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_rtc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_sim.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_sim.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_smc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_smc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_tpm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_tpm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_tsi_v4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_tsi_v4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_vref.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\drivers\fsl_vref.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MKW41Z4\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\MKW41Z4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\MKW41Z4_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\system_MKW41Z4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MKW41Z4\system_MKW41Z4.h
# End Source File
# End Group
# Begin Group "MIMXRT1015"

# PROP Default_Filter ""
# Begin Group "1015_CMSISdrivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1015\cmsis_drivers\fsl_lpi2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\cmsis_drivers\fsl_lpi2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\cmsis_drivers\fsl_lpspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\cmsis_drivers\fsl_lpspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Group "1015_Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_adc_etc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_adc_etc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_aipstz.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_aipstz.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_aoi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_aoi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_bee.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_bee.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_dcdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_dcdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_dcp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_dcp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_enc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_enc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexram.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexram.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_flexspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_kpp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_kpp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpi2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpi2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpi2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpi2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpi2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpi2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_qtmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_qtmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_rtwdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_rtwdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_snvs_lp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_snvs_lp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_spdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_spdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_spdif_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_spdif_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_tempmon.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_tempmon.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_wdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_xbara.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_xbara.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_xbarb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\drivers\fsl_xbarb.h
# End Source File
# End Group
# Begin Group "1015_ProjectTemplate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1015\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\project_template\pin_mux.h
# End Source File
# End Group
# Begin Group "1015_Template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1015\template\RTE_Device.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "1015_Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\fsl_assert.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\debug_console\fsl_debug_console.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\debug_console\fsl_debug_console.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\debug_console\fsl_debug_console_conf.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\fsl_notifier.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\fsl_notifier.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\fsl_sbrk.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\fsl_shell.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\fsl_shell.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\str\fsl_str.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\str\fsl_str.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\utilities\usb_device_config.h
# End Source File
# End Group
# Begin Group "1015_XiP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1015\xip\fsl_flexspi_nor_boot.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\xip\fsl_flexspi_nor_boot.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MIMXRT1015\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\MIMXRT1015.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\MIMXRT1015_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\gcc\startup_MIMXRT1015.S
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\system_MIMXRT1015.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1015\system_MIMXRT1015.h
# End Source File
# End Group
# Begin Group "MIMXRT1021"

# PROP Default_Filter ""
# Begin Group "1021 drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_adc_etc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_adc_etc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_aipstz.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_aipstz.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_aoi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_aoi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_bee.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_bee.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_dcdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_dcdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_dcp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_dcp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_enc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_enc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexram.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexram.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_flexspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_kpp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_kpp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpi2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpi2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpi2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpi2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpi2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpi2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_qtmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_qtmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_rtwdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_rtwdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_semc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_semc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_snvs_lp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_snvs_lp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_spdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_spdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_spdif_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_spdif_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_tempmon.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_tempmon.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_usdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_usdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_wdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_xbara.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_xbara.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_xbarb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\drivers\fsl_xbarb.h
# End Source File
# End Group
# Begin Group "1021 CMSIS drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1021\cmsis_drivers\fsl_lpi2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\cmsis_drivers\fsl_lpi2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\cmsis_drivers\fsl_lpspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\cmsis_drivers\fsl_lpspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Group "1021 project_template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1021\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\project_template\pin_mux.h
# End Source File
# End Group
# Begin Group "1021 XiP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1021\xip\evkmimxrt1020_flexspi_nor_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\xip\evkmimxrt1020_flexspi_nor_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\xip\evkmimxrt1020_sdram_ini_dcd.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\xip\evkmimxrt1020_sdram_ini_dcd.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\xip\fsl_flexspi_nor_boot.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\xip\fsl_flexspi_nor_boot.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MIMXRT1021\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\MIMXRT1021.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\MIMXRT1021_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\system_MIMXRT1021.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1021\system_MIMXRT1021.h
# End Source File
# End Group
# Begin Group "MIMXRT1051"

# PROP Default_Filter ""
# Begin Group "1051 cmsis_drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1051\cmsis_drivers\fsl_lpi2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\cmsis_drivers\fsl_lpi2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\cmsis_drivers\fsl_lpspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\cmsis_drivers\fsl_lpspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Group "1051 drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_adc_etc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_adc_etc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_aipstz.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_aipstz.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_aoi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_aoi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_bee.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_bee.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_dcdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_dcdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_dcp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_dcp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_enc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_enc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexram.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexram.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_flexspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_kpp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_kpp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpi2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpi2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpi2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpi2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpi2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpi2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_qtmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_qtmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_rtwdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_rtwdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_semc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_semc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_snvs_lp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_snvs_lp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_spdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_spdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_spdif_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_spdif_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_tsc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_tsc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_usdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_usdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_wdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_xbara.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_xbara.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_xbarb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\drivers\fsl_xbarb.h
# End Source File
# End Group
# Begin Group "1051 project_template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1051\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\project_template\pin_mux.h
# End Source File
# End Group
# Begin Group "1051 XiP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1051\xip\fsl_flexspi_nor_boot.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\xip\fsl_flexspi_nor_boot.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\xip\fsl_flexspi_nor_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\xip\fsl_flexspi_nor_flash.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MIMXRT1051\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\MIMXRT1051.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\MIMXRT1051_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\system_MIMXRT1051.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1051\system_MIMXRT1051.h
# End Source File
# End Group
# Begin Group "MIMXRT1052"

# PROP Default_Filter ""
# Begin Group "1052 drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_adc_etc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_adc_etc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_aipstz.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_aipstz.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_aoi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_aoi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_bee.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_bee.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_csi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_csi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_dcdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_dcdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_dcp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_dcp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_elcdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_elcdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_enc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_enc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexram.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexram.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_flexspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_kpp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_kpp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpi2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpi2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpi2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpi2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpi2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpi2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_pxp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_pxp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_qtmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_qtmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_rtwdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_rtwdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_semc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_semc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_snvs_lp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_snvs_lp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_spdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_spdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_spdif_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_spdif_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_tsc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_tsc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_usdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_usdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_wdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_xbara.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_xbara.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_xbarb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\drivers\fsl_xbarb.h
# End Source File
# End Group
# Begin Group "1052 CMSIS drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1052\cmsis_drivers\fsl_lpi2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\cmsis_drivers\fsl_lpi2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\cmsis_drivers\fsl_lpspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\cmsis_drivers\fsl_lpspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Group "1052 project_template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1052\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\project_template\pin_mux.h
# End Source File
# End Group
# Begin Group "1052 XiP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1052\xip\evkbimxrt1050_flexspi_nor_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\xip\evkbimxrt1050_flexspi_nor_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\xip\evkbimxrt1050_sdram_ini_dcd.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\xip\evkbimxrt1050_sdram_ini_dcd.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\xip\fsl_flexspi_nor_boot.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\xip\fsl_flexspi_nor_boot.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\xip\fsl_flexspi_nor_flash.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\xip\fsl_flexspi_nor_flash.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MIMXRT1052\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\MIMXRT1052.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\MIMXRT1052_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\system_MIMXRT1052.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1052\system_MIMXRT1052.h
# End Source File
# End Group
# Begin Group "MIMXRT1061"

# PROP Default_Filter ""
# Begin Group "1061 cmsis_drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1061\cmsis_drivers\fsl_lpi2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\cmsis_drivers\fsl_lpi2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\cmsis_drivers\fsl_lpspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\cmsis_drivers\fsl_lpspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Group "1061 drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_adc_etc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_adc_etc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_aipstz.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_aipstz.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_aoi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_aoi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_bee.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_bee.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_dcdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_dcdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_dcp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_dcp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_enc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_enc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexram.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexram.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_flexspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_kpp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_kpp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpi2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpi2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpi2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpi2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_qtmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_qtmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_rtwdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_rtwdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_semc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_semc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_snvs_lp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_snvs_lp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_spdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_spdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_spdif_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_spdif_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_tempmon.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_tempmon.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_tsc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_tsc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_usdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_usdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_wdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_xbara.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_xbara.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_xbarb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\drivers\fsl_xbarb.h
# End Source File
# End Group
# Begin Group "1061 project_template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1061\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\project_template\pin_mux.h
# End Source File
# End Group
# Begin Group "1061 XiP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1061\xip\fsl_flexspi_nor_boot.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\xip\fsl_flexspi_nor_boot.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MIMXRT1061\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\MIMXRT1061.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\MIMXRT1061_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\system_MIMXRT1061.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1061\system_MIMXRT1061.h
# End Source File
# End Group
# Begin Group "MIMXRT1062"

# PROP Default_Filter ""
# Begin Group "1062 cmsis_drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1062\cmsis_drivers\fsl_lpi2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\cmsis_drivers\fsl_lpi2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\cmsis_drivers\fsl_lpspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\cmsis_drivers\fsl_lpspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Group "1062 drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_adc_etc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_adc_etc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_aipstz.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_aipstz.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_aoi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_aoi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_bee.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_bee.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_csi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_csi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_dcdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_dcdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_dcp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_dcp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_elcdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_elcdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_enc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_enc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexram.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexram.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_flexspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_kpp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_kpp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpi2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpi2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpi2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpi2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpi2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpi2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_pxp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_pxp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_qtmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_qtmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_rtwdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_rtwdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_semc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_semc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_snvs_lp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_snvs_lp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_spdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_spdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_spdif_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_spdif_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_tempmon.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_tempmon.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_tsc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_tsc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_usdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_usdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_wdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_xbara.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_xbara.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_xbarb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\drivers\fsl_xbarb.h
# End Source File
# End Group
# Begin Group "1062 project_template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1062\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\project_template\pin_mux.h
# End Source File
# End Group
# Begin Group "1062 XiP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1062\xip\evkmimxrt1060_flexspi_nor_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\xip\evkmimxrt1060_flexspi_nor_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\xip\evkmimxrt1060_sdram_ini_dcd.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\xip\evkmimxrt1060_sdram_ini_dcd.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\xip\fsl_flexspi_nor_boot.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\xip\fsl_flexspi_nor_boot.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MIMXRT1062\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\MIMXRT1062.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\MIMXRT1062_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\system_MIMXRT1062.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1062\system_MIMXRT1062.h
# End Source File
# End Group
# Begin Group "MIMXRT1064"

# PROP Default_Filter ""
# Begin Group "1064 cmsis_drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1064\cmsis_drivers\fsl_lpi2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\cmsis_drivers\fsl_lpi2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\cmsis_drivers\fsl_lpspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\cmsis_drivers\fsl_lpspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Group "1064 drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_adc_etc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_adc_etc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_aipstz.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_aipstz.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_aoi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_aoi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_bee.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_bee.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_cmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_cmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_csi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_csi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_dcdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_dcdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_dcp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_dcp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_elcdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_elcdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_enc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_enc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexram.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexram.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_flexspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_kpp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_kpp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpi2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpi2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpi2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpi2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpi2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpi2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_pit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_pit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_pxp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_pxp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_qtmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_qtmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_rtwdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_rtwdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_semc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_semc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_snvs_lp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_snvs_lp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_spdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_spdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_spdif_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_spdif_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_tempmon.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_tempmon.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_tsc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_tsc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_usdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_usdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_wdog.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_xbara.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_xbara.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_xbarb.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\drivers\fsl_xbarb.h
# End Source File
# End Group
# Begin Group "1064 project_template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1064\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\project_template\pin_mux.h
# End Source File
# End Group
# Begin Group "1064 XiP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MIMXRT1064\xip\evkmimxrt1064_flexspi_nor_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\xip\evkmimxrt1064_flexspi_nor_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\xip\evkmimxrt1064_sdram_ini_dcd.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\xip\evkmimxrt1064_sdram_ini_dcd.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\xip\fsl_flexspi_nor_boot.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\xip\fsl_flexspi_nor_boot.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MIMXRT1064\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\MIMXRT1064.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\MIMXRT1064_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\system_MIMXRT1064.c
# End Source File
# Begin Source File

SOURCE=.\devices\MIMXRT1064\system_MIMXRT1064.h
# End Source File
# End Group
# Begin Group "MIMX6"

# PROP Default_Filter ""
# Begin Group "G0"

# PROP Default_Filter ""
# Begin Group "G0_Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_ecspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_ecspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_ecspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_ecspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_epit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_epit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_mmdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_mmdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_pxp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_pxp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_qspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_qspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_sai_sdma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_sai_sdma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_sdma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_sdma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_tsc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_tsc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_usdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_usdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Group "G0_Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX6G0\utilities\fsl_debug_console.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\utilities\fsl_debug_console.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\utilities\fsl_debug_console_conf.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\utilities\fsl_io.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\utilities\fsl_io.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\utilities\fsl_log.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\utilities\fsl_log.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\utilities\fsl_str.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\utilities\fsl_str.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MCIMX6G0\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\MCIMX6G0.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\MCIMX6G0_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\gcc\MCIMX6G0xxx05_flash.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\gcc\MCIMX6G0xxx05_ram.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\gcc\startup_MCIMX6G0.S
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\system_MCIMX6G0.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G0\system_MCIMX6G0.h
# End Source File
# End Group
# Begin Group "G1"

# PROP Default_Filter ""
# Begin Group "G1_Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_ecspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_ecspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_ecspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_ecspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_epit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_epit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_mmdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_mmdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_pxp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_pxp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_qspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_qspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_sai_sdma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_sai_sdma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_sdma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_sdma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_tsc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_tsc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_usdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_usdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Group "G1_Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX6G1\utilities\fsl_debug_console.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\utilities\fsl_debug_console.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\utilities\fsl_debug_console_conf.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\utilities\fsl_io.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\utilities\fsl_io.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\utilities\fsl_log.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\utilities\fsl_log.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\utilities\fsl_str.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\utilities\fsl_str.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MCIMX6G1\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\MCIMX6G1.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\MCIMX6G1_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\gcc\MCIMX6G1xxx05_flash.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\gcc\MCIMX6G1xxx05_ram.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\gcc\MCIMX6G1xxx07_flash.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\gcc\MCIMX6G1xxx07_ram.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\gcc\startup_MCIMX6G1.S
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\system_MCIMX6G1.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G1\system_MCIMX6G1.h
# End Source File
# End Group
# Begin Group "G2"

# PROP Default_Filter ""
# Begin Group "G2_Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_adc_5hc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_adc_5hc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_csi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_csi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_ecspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_ecspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_ecspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_ecspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_elcdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_elcdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_epit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_epit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_mmdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_mmdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_pxp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_pxp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_qspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_qspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_sai_sdma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_sai_sdma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_sdma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_sdma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_tsc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_tsc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_usdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_usdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Group "G2_Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX6G2\utilities\fsl_debug_console.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\utilities\fsl_debug_console.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\utilities\fsl_debug_console_conf.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\utilities\fsl_io.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\utilities\fsl_io.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\utilities\fsl_log.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\utilities\fsl_log.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\utilities\fsl_str.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\utilities\fsl_str.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MCIMX6G2\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\MCIMX6G2.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\MCIMX6G2_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\gcc\MCIMX6G2xxx05_flash.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\gcc\MCIMX6G2xxx05_ram.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\gcc\MCIMX6G2xxx07_flash.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\gcc\MCIMX6G2xxx07_ram.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\gcc\startup_MCIMX6G2.S
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\system_MCIMX6G2.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G2\system_MCIMX6G2.h
# End Source File
# End Group
# Begin Group "G3"

# PROP Default_Filter ""
# Begin Group "G3_Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_adc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_adc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_adc_5hc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_adc_5hc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_csi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_csi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_ecspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_ecspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_ecspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_ecspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_ecspi_sdma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_ecspi_sdma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_elcdif.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_elcdif.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_enet.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_enet.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_epit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_epit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_flexcan.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_flexcan.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_gpc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_gpc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_gpt.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_gpt.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_i2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_i2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_i2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_i2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_mmdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_mmdc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_pmu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_pmu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_pwm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_pwm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_pxp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_pxp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_qspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_qspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_sai_sdma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_sai_sdma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_sdma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_sdma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_src.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_src.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_tsc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_tsc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_uart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_uart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_uart_sdma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_uart_sdma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_usdhc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_usdhc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_wdog.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\drivers\fsl_wdog.h
# End Source File
# End Group
# Begin Group "G3_Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX6G3\utilities\fsl_debug_console.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\utilities\fsl_debug_console.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\utilities\fsl_debug_console_conf.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\utilities\fsl_io.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\utilities\fsl_io.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\utilities\fsl_log.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\utilities\fsl_log.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\utilities\fsl_sbrk.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\utilities\fsl_str.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\utilities\fsl_str.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MCIMX6G3\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\MCIMX6G3.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\MCIMX6G3_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\gcc\MCIMX6G3xxx05_flash.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\gcc\MCIMX6G3xxx05_ram.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\gcc\startup_MCIMX6G3.S
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\system_MCIMX6G3.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX6G3\system_MCIMX6G3.h
# End Source File
# End Group
# End Group
# Begin Group "MIMX7"

# PROP Default_Filter ""
# Begin Group "X7_CMSIS_Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX7U5\cmsis_drivers\fsl_lpi2c_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\cmsis_drivers\fsl_lpi2c_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\cmsis_drivers\fsl_lpspi_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\cmsis_drivers\fsl_lpspi_cmsis.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\cmsis_drivers\fsl_lpuart_cmsis.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\cmsis_drivers\fsl_lpuart_cmsis.h
# End Source File
# End Group
# Begin Group "X7_Drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_acmp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_acmp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_cache.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_cache.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_clock.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_clock.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_common.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_common.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_crc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_crc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_dac12.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_dac12.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_dmamux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_dmamux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_ewm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_ewm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_i2c_master.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_i2c_master.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_i2s.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_i2s.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_i2s_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_i2s_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_spi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_spi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_spi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_spi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_uart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_uart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_uart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_flexio_uart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_gpio.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_gpio.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_llwu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_llwu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpadc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpadc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpi2c.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpi2c.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpi2c_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpi2c_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpi2c_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpi2c_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpit.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpit.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpspi_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpspi_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lptmr.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lptmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpuart.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpuart.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpuart_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpuart_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpuart_freertos.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_lpuart_freertos.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_ltc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_ltc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_ltc_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_ltc_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_msmc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_msmc.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_mu.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_mu.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_pmc0.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_port.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_qspi.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_qspi.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_qspi_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_qspi_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_sai.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_sai.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_sai_edma.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_sai_edma.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_sema42.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_sema42.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_snvs_hp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_snvs_hp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_snvs_lp.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_snvs_lp.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_tpm.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_tpm.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_trgmux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_trgmux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_trng.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_trng.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_tstmr.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_wdog32.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_wdog32.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_xrdc.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\drivers\fsl_xrdc.h
# End Source File
# End Group
# Begin Group "X7_Template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX7U5\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\project_template\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\template\RTE_Device.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "X7_Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\fsl_assert.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\fsl_debug_console.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\fsl_debug_console.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\fsl_debug_console_conf.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\fsl_notifier.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\fsl_notifier.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\fsl_sbrk.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\fsl_shell.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\fsl_shell.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\str\fsl_str.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\utilities\str\fsl_str.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\devices\MCIMX7U5\fsl_device_registers.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\MCIMX7U5_cm4.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\MCIMX7U5_cm4_features.h
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\gcc\MCIMX7U5xxxxx_cm4_flash.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\gcc\MCIMX7U5xxxxx_cm4_ram.ld
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\gcc\startup_MCIMX7U5_cm4.S
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\system_MCIMX7U5_cm4.c
# End Source File
# Begin Source File

SOURCE=.\devices\MCIMX7U5\system_MCIMX7U5_cm4.h
# End Source File
# End Group
# End Group
# Begin Group "boards"

# PROP Default_Filter ""
# Begin Group "FRDM-K22F"

# PROP Default_Filter ""
# Begin Group "K22 Demo Apps"

# PROP Default_Filter ""
# Begin Group "ecompass Nr. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk22f\demo_apps\ecompass\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\demo_apps\ecompass\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\demo_apps\ecompass\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\demo_apps\ecompass\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\demo_apps\ecompass\ecompass.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\demo_apps\ecompass\fsl_fxos.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\demo_apps\ecompass\fsl_fxos.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\demo_apps\ecompass\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\demo_apps\ecompass\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "wifi_qca_examples"

# PROP Default_Filter ""
# Begin Group "qca_demo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\main.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\wlan_qca400x.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\wlan_qcom.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_demo\wlan_qcom.h
# End Source File
# End Group
# Begin Group "qca_fwupdate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_fwupdate\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_fwupdate\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_fwupdate\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_fwupdate\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_fwupdate\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_fwupdate\fw_serial.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_fwupdate\main.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_fwupdate\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_fwupdate\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_fwupdate\wlan_qca400x.c
# End Source File
# End Group
# Begin Group "qca_throughput"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_throughput\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_throughput\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_throughput\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_throughput\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_throughput\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_throughput\main.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_throughput\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_throughput\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_throughput\wlan_qca400x.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk22f\wifi_qca_examples\qca_throughput\wlan_qcom.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "FRDM-K28F"

# PROP Default_Filter ""
# Begin Group "K28_driver_examples"

# PROP Default_Filter ""
# Begin Group "QSPI_EDMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\edma_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\edma_transfer\qspi_edma_transfer.c
# End Source File
# End Group
# Begin Group "QSPI_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\polling_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\polling_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\polling_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\polling_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\polling_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\polling_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk28f\driver_examples\qspi\polling_transfer\qspi_polling_transfer.c
# End Source File
# End Group
# End Group
# End Group
# Begin Group "FRDM-K64F"

# PROP Default_Filter ""
# Begin Group "K64 demo Apps"

# PROP Default_Filter ""
# Begin Group "adc16_low_power"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\adc16_low_power\adc16_low_power.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\adc16_low_power\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\adc16_low_power\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\adc16_low_power\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\adc16_low_power\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\adc16_low_power\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\adc16_low_power\pin_mux.h
# End Source File
# End Group
# Begin Group "bubble"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\bubble\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\bubble\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\bubble\bubble.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\bubble\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\bubble\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\bubble\fsl_fxos.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\bubble\fsl_fxos.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\bubble\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\bubble\pin_mux.h
# End Source File
# End Group
# Begin Group "dac_adc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\dac_adc\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\dac_adc\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\dac_adc\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\dac_adc\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\dac_adc\dac_adc.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\dac_adc\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\dac_adc\pin_mux.h
# End Source File
# End Group
# Begin Group "ecompass"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\ecompass\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\ecompass\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\ecompass\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\ecompass\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\ecompass\ecompass.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\ecompass\fsl_fxos.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\ecompass\fsl_fxos.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\ecompass\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\ecompass\pin_mux.h
# End Source File
# End Group
# Begin Group "wifi_qca"

# PROP Default_Filter ""
# Begin Group "QCA Demo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\main.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\wlan_qca400x.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\wlan_qcom.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_demo\wlan_qcom.h
# End Source File
# End Group
# Begin Group "QCA Throughput"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_throughput\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_throughput\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_throughput\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_throughput\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_throughput\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_throughput\main.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_throughput\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_throughput\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_throughput\wlan_qca400x.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\demo_apps\wifi_qca\qca_throughput\wlan_qcom.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "driver_examples"

# PROP Default_Filter ""
# Begin Group "adc16"

# PROP Default_Filter ""
# Begin Group "adc_irq"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\interrupt\adc16_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "adc_polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\polling\adc16_polling.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\adc16\polling\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "sdcard"

# PROP Default_Filter ""
# Begin Group "sdcard_irq"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\interrupt\event.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\interrupt\event.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\interrupt\fsl_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\interrupt\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\interrupt\sdcard_interrupt.c
# End Source File
# End Group
# Begin Group "sdcard_polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\polling\event.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\polling\event.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\polling\fsl_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\polling\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\polling\sdcard_polling.c
# End Source File
# End Group
# Begin Group "sdcard_freertos"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\event.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\event.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\fsl_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard\freertos\sdcard_freertos.c
# End Source File
# End Group
# Begin Group "fatfs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\event.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\event.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\ffconf.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\fsl_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\sdcard_fatfs\sdcard_fatfs.c
# End Source File
# End Group
# End Group
# Begin Group "ftm"

# PROP Default_Filter ""
# Begin Group "combine_pwm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\combine_pwm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\combine_pwm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\combine_pwm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\combine_pwm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\combine_pwm\ftm_combine_pwm.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\combine_pwm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\combine_pwm\pin_mux.h
# End Source File
# End Group
# Begin Group "dual_edge_capture"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\dual_edge_capture\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\dual_edge_capture\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\dual_edge_capture\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\dual_edge_capture\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\dual_edge_capture\ftm_dual_edge_capture.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\dual_edge_capture\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\ftm\dual_edge_capture\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "i2C"

# PROP Default_Filter ""
# Begin Group "edma_b2b_transfer_master"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\master\i2c_edma_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "edma_b2b_transfer_slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\slave\i2c_edma_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_b2b_transfer\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "edma_transfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_transfer\i2c_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\edma_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "interrupt Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\interrupt\i2c_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\i2c\interrupt\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "flexcan Nr. 1"

# PROP Default_Filter ""
# Begin Group "loopback"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\flexcan\loopback\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\flexcan\loopback\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\flexcan\loopback\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\flexcan\loopback\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\flexcan\loopback\flexcan_loopback.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\flexcan\loopback\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\driver_examples\flexcan\loopback\pin_mux.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "K64_cmsis_driver_examples"

# PROP Default_Filter ""
# Begin Group "I2C edma_b2b_transfer_master"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\master\cmsis_i2c_edma_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\master\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\master\RTE_Device.h
# End Source File
# End Group
# Begin Group "I2C edma_b2b_transfer_slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\slave\cmsis_i2c_edma_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\slave\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_b2b_transfer\slave\RTE_Device.h
# End Source File
# End Group
# Begin Group "I2C edma_transfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_transfer\cmsis_i2c_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmk64f\cmsis_driver_examples\i2c\edma_transfer\RTE_Device.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "FRDM-KW41Z"

# PROP Default_Filter ""
# Begin Group "Wireless Examples"

# PROP Default_Filter ""
# Begin Group "bluetooth"

# PROP Default_Filter ""
# Begin Group "alert_notification_server"

# PROP Default_Filter ""
# Begin Group "BM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\bm\app_preinclude.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\bm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\bm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\bm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\bm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\bm\gpio_pins.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\bm\gpio_pins.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\bm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\bm\pin_mux.h
# End Source File
# End Group
# Begin Group "freeRTOS Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\freertos\app_preinclude.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\freertos\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\freertos\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\freertos\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\freertos\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\freertos\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\freertos\gpio_pins.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\freertos\gpio_pins.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\freertos\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\freertos\pin_mux.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\alert_notification_server.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\alert_notification_server.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\app_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\gatt_db.h
# End Source File
# Begin Source File

SOURCE=.\boards\frdmkw41z\wireless_examples\bluetooth\alert_notification_server\gatt_uuid128.h
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "EVK-iMXRT1015"

# PROP Default_Filter ""
# Begin Group "1015_bootloader_examples"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\bootloader_examples\flashloader\usb_device_ch9.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\bootloader_examples\flashloader\usb_device_ch9.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\bootloader_examples\flashloader\usb_device_class.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\bootloader_examples\flashloader\usb_device_class.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\bootloader_examples\flashloader\usb_device_hid.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\bootloader_examples\flashloader\usb_device_hid.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\bootloader_examples\flashloader\usb_device_msc.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\bootloader_examples\flashloader\usb_device_msc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\bootloader_examples\flashloader\usb_device_msc_ufi.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\bootloader_examples\flashloader\usb_device_msc_ufi.h
# End Source File
# End Group
# Begin Group "1015_CMSIS_DriverExamples"

# PROP Default_Filter ""
# End Group
# Begin Group "1015_DemoApps"

# PROP Default_Filter ""
# Begin Group "1015_bubble"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble\bubble.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble\pin_mux.h
# End Source File
# End Group
# Begin Group "1015_bubble_peripheral"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble_peripheral\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble_peripheral\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble_peripheral\bubble.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble_peripheral\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble_peripheral\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble_peripheral\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble_peripheral\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble_peripheral\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\bubble_peripheral\pin_mux.h
# End Source File
# End Group
# Begin Group "1015_ecompass"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\ecompass\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\ecompass\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\ecompass\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\ecompass\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\ecompass\ecompass.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\ecompass\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\demo_apps\ecompass\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "1015_DriverExample"

# PROP Default_Filter ""
# Begin Group "1015_ADC_Interrupt"

# PROP Default_Filter ""
# End Group
# Begin Group "1015_ADC_Polling"

# PROP Default_Filter ""
# End Group
# End Group
# Begin Group "1015_ProjectTemplate Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\project_template\pin_mux.h
# End Source File
# End Group
# Begin Group "1015_RtosExample"

# PROP Default_Filter ""
# End Group
# Begin Group "1015_USBexample"

# PROP Default_Filter ""
# End Group
# Begin Group "1015_WiFiqcaExample"

# PROP Default_Filter ""
# End Group
# Begin Group "1015_XiP Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\xip\evkmimxrt1015_flexspi_nor_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\xip\evkmimxrt1015_flexspi_nor_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\xip\evkmimxrt1015_sdram_ini_dcd.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1015\xip\evkmimxrt1015_sdram_ini_dcd.h
# End Source File
# End Group
# End Group
# Begin Group "EVK-iMXRT1020"

# PROP Default_Filter ""
# Begin Group "1020_aws_examples"

# PROP Default_Filter ""
# End Group
# Begin Group "1020_bootloader_examples"

# PROP Default_Filter ""
# End Group
# Begin Group "1020_cmsis_driver_examples"

# PROP Default_Filter ""
# End Group
# Begin Group "1020_demo_apps"

# PROP Default_Filter ""
# End Group
# Begin Group "1020_driver example"

# PROP Default_Filter ""
# Begin Group "1020_ADC_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\interrupt\adc_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_ADC_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\polling\adc_polling.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc\polling\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_adc_etc_hardware_trigger_"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\adc_etc_hardware_trigger_conv.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_adc_etc_software_trigger_"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_software_trigger_conv\adc_etc_software_trigger_conv.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_software_trigger_conv\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_software_trigger_conv\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_software_trigger_conv\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_software_trigger_conv\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_software_trigger_conv\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\adc_etc\adc_etc_software_trigger_conv\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_bee"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\bee\bee.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\bee\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\bee\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\bee\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\bee\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\bee\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\bee\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_cache"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cache\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cache\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cache\cache.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cache\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cache\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cache\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cache\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_CMP_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\interrupt\cmp_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_CMP_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\polling\cmp_polling.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\cmp\polling\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_DCP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\dcp\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\dcp\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\dcp\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\dcp\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\dcp\dcp.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\dcp\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\dcp\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_EDMA_Memory2Memoy"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\memory_to_memory\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\memory_to_memory\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\memory_to_memory\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\memory_to_memory\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\memory_to_memory\edma_memory_to_memory.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\memory_to_memory\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\memory_to_memory\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_EDMA_ScatterGather"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\scatter_gather\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\scatter_gather\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\scatter_gather\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\scatter_gather\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\scatter_gather\edma_scatter_gather.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\scatter_gather\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\edma\scatter_gather\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_ENC_basic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\basic\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\basic\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\basic\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\basic\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\basic\enc_basic.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\basic\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\basic\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_ENC_IndexInterrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\index_interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\index_interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\index_interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\index_interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\index_interrupt\enc_index_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\index_interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enc\index_interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_ENET_PTP1588"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_ptp1588_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_ptp1588_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_ptp1588_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_ptp1588_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_ptp1588_transfer\enet_txrx_ptp1588_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_ptp1588_transfer\fsl_phy.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_ptp1588_transfer\fsl_phy.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_ptp1588_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_ptp1588_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_ENET_Transfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_transfer\enet_txrx_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_transfer\fsl_phy.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_transfer\fsl_phy.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\enet\txrx_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_EWM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\ewm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\ewm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\ewm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\ewm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\ewm\ewm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\ewm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\ewm\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexCAN_Interrupttranser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\interrupt_transfer\flexcan_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\interrupt_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexCAN_Loopback"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback\flexcan_loopback.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexCAN_LoopbackTransfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback_transfer\flexcan_loopback_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexcan\loopback_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_I2C_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\flexio_i2c_interrupt_lpi2c_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_I2C_ReadAccelerate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\read_accel_value_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\read_accel_value_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\read_accel_value_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\read_accel_value_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\read_accel_value_transfer\flexio_i2c_read_accel_value_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\read_accel_value_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2c\read_accel_value_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_I2S_EDMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\flexio_i2s_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\edma_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_I2S_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\flexio_i2s_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\i2s\interrupt_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_PWM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\pwm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\pwm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\pwm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\pwm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\pwm\flexio_pwm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\pwm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\pwm\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_SPI_EDMA_Master"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\master\flexio_spi_edma_lpspi_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_SPI_EDMA_Slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\slave\flexio_spi_edma_lpspi_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\edma_lpspi_transfer\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_SPI_Interrupt_Mast"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\master\flexio_spi_int_lpspi_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_SPI_Interrupt_Slav"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\slave\flexio_spi_int_lpspi_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\spi\int_lpspi_transfer\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_UART_EDMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\edma_transfer\flexio_uart_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\edma_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_UART_Interrupt_Rea"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\int_rb_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\int_rb_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\int_rb_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\int_rb_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\int_rb_transfer\flexio_uart_int_rb_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\int_rb_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\int_rb_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_UART_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\interrupt_transfer\flexio_uart_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\interrupt_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexIO_UART_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\polling_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\polling_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\polling_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\polling_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\polling_transfer\flexio_uart_polling_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\polling_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexio\uart\polling_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexRAM_Access"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_access\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_access\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_access\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_access\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_access\flexram.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_access\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_access\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexRAM_Allocate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_allocate\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_allocate\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_allocate\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_allocate\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_allocate\flexram.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_allocate\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexram\ram_allocate\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_FlexSPI_NOR_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\app.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\flexspi_nor_flash_ops.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\flexspi_nor_polling_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_GPIO_InputInterrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\input_interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\input_interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\input_interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\input_interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\input_interrupt\gpio_input_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\input_interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\input_interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_GPIO_LED_Output"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\led_output\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\led_output\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\led_output\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\led_output\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\led_output\gpio_led_output.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\led_output\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpio\led_output\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_GPT_Timer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpt\timer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpt\timer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpt\timer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpt\timer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpt\timer\gpt_timer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpt\timer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\gpt\timer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_KPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\kpp\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\kpp\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\kpp\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\kpp\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\kpp\kpp.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\kpp\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\kpp\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPI2C_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt\lpi2c_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPI2C_Interrupt_B2B_Maste"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\master\lpi2c_interrupt_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPI2C_Interrupt_B2B_Slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\slave\lpi2c_interrupt_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\interrupt_b2b_transfer\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPI2C_Polling_B2B_Master"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\master\lpi2c_polling_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPI2C_Polling_B2B_Slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\slave\lpi2c_polling_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpi2c\polling_b2b_transfer\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPSPI_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt\lpspi_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPSPI_Interrupt_B2B_Maste"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\master\lpspi_interrupt_b2b_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\master\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPSPI_Interrupt_B2B_Slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\slave\lpspi_interrupt_b2b_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpspi\interrupt_b2b\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPUART_EDMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\edma_transfer\lpuart_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\edma_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPUART_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt\lpuart_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPUART_Interrupt_RB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_rb_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_rb_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_rb_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_rb_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_rb_transfer\lpuart_interrupt_rb_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_rb_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_rb_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPUART_InterruptTransfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer\lpuart_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPUART_Interrupttransfer7"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer_seven_bits\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer_seven_bits\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer_seven_bits\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer_seven_bits\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer_seven_bits\interrupt_transfer_seven_bits.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer_seven_bits\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\interrupt_transfer_seven_bits\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPUART_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling\lpuart_polling.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling\pin_mux.h
# End Source File
# End Group
# Begin Group "1020_LPUART_Polling7"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling_seven_bits\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling_seven_bits\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling_seven_bits\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling_seven_bits\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling_seven_bits\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling_seven_bits\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\lpuart\polling_seven_bits\polling_seven_bits.c
# End Source File
# End Group
# Begin Group "1020_PIT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pit\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pit\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pit\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pit\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pit\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pit\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pit\pit.c
# End Source File
# End Group
# Begin Group "1020_PWM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pwm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pwm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pwm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pwm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pwm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pwm\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\pwm\pwm.c
# End Source File
# End Group
# Begin Group "1020_QTMR_InCapture_OutCompare"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm\qtmr_inputcapture_outputpwm.c
# End Source File
# End Group
# Begin Group "1020_QTMR_InCaputre_OutCom_DMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm_dma\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm_dma\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm_dma\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm_dma\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm_dma\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm_dma\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\inputcapture_outputpwm_dma\qtmr_inputcapture_outputpwm_dma.c
# End Source File
# End Group
# Begin Group "1020_QTMR_Timer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\timer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\timer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\timer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\timer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\timer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\timer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\qtmr\timer\qtmr_timer.c
# End Source File
# End Group
# Begin Group "1020_RTWDOG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\rtwdog\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\rtwdog\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\rtwdog\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\rtwdog\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\rtwdog\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\rtwdog\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\rtwdog\rtwdog.c
# End Source File
# End Group
# Begin Group "1020_SAI_EDMA_transfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\music.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\edma_transfer\sai_edma_transfer.c
# End Source File
# End Group
# Begin Group "1020_SAI_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\music.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt\sai_interrupt.c
# End Source File
# End Group
# Begin Group "1020_SAI_Interrupttransfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\music.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sai\interrupt_transfer\sai_interrupt_transfer.c
# End Source File
# End Group
# Begin Group "1020_SDCARD_FreeRTOS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\fsl_sdmmc_event.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\fsl_sdmmc_event.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\fsl_sdmmc_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\freertos\sdcard_freertos.c
# End Source File
# End Group
# Begin Group "1020_SDCARD_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\interrupt\fsl_sdmmc_event.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\interrupt\fsl_sdmmc_event.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\interrupt\fsl_sdmmc_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\interrupt\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\interrupt\sdcard_interrupt.c
# End Source File
# End Group
# Begin Group "1020_SDCARD_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\polling\fsl_sdmmc_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\polling\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\sdcard\polling\sdcard_polling.c
# End Source File
# End Group
# Begin Group "1020_SEMC_SDRAM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\semc\sdram\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\semc\sdram\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\semc\sdram\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\semc\sdram\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\semc\sdram\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\semc\sdram\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\semc\sdram\semc_sdram.c
# End Source File
# End Group
# End Group
# End Group
# Begin Group "EVK-iMXRT1050B"

# PROP Default_Filter ""
# Begin Group "1050 driver_examples"

# PROP Default_Filter ""
# Begin Group "1050_ADC_Interupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\interrupt\adc_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_ADC_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\polling\adc_polling.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc\polling\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_ADC_ETC_HWtrigger"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\adc_etc_hardware_trigger_conv.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_hardware_trigger_conv\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_ADC_ETC_SWtrigger"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_software_trigger_conv\adc_etc_software_trigger_conv.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_software_trigger_conv\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_software_trigger_conv\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_software_trigger_conv\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_software_trigger_conv\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_software_trigger_conv\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\adc_etc\adc_etc_software_trigger_conv\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_BEE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\bee\bee.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\bee\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\bee\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\bee\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\bee\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\bee\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\bee\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_Cache"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cache\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cache\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cache\cache.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cache\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cache\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cache\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cache\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_CMP_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\interrupt\cmp_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_CMP_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\polling\cmp_polling.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\cmp\polling\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_CSI_CCIR656"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\csi_ccir656.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_camera.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_camera_device.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_camera_receiver.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_csi_camera_adapter.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_csi_camera_adapter.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_mt9m114.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_mt9m114.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_ov7725.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_ov7725.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_sccb.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_sccb.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_video_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_video_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_video_i2c.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\fsl_video_i2c.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\ccir656\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_CSI_FragmentGray"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\fragment_gray\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\fragment_gray\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\fragment_gray\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\fragment_gray\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\fragment_gray\csi_fragment_gray.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\fragment_gray\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\fragment_gray\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_CSI_RGB565"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\csi_rgb565.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_camera.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_camera_device.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_camera_receiver.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_csi_camera_adapter.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_csi_camera_adapter.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_mt9m114.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_mt9m114.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_ov7725.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_ov7725.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_sccb.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_sccb.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_video_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_video_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_video_i2c.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\fsl_video_i2c.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\csi\rgb565\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_DCP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\dcp\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\dcp\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\dcp\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\dcp\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\dcp\dcp.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\dcp\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\dcp\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_EDMA_Mem2Mem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\memory_to_memory\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\memory_to_memory\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\memory_to_memory\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\memory_to_memory\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\memory_to_memory\edma_memory_to_memory.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\memory_to_memory\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\memory_to_memory\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_EDMA_Scatter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\scatter_gather\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\scatter_gather\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\scatter_gather\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\scatter_gather\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\scatter_gather\edma_scatter_gather.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\scatter_gather\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\edma\scatter_gather\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_ELCDIF_LUT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\lut\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\lut\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\lut\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\lut\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\lut\elcdif_lut.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\lut\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\lut\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_ELCDIF_RGB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\rgb\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\rgb\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\rgb\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\rgb\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\rgb\elcdif_rgb.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\rgb\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\elcdif\rgb\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_ENC_Basic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\basic\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\basic\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\basic\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\basic\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\basic\enc_basic.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\basic\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\basic\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_ENC_IndexInterrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\index_interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\index_interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\index_interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\index_interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\index_interrupt\enc_index_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\index_interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enc\index_interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_ENET_1588"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_ptp1588_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_ptp1588_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_ptp1588_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_ptp1588_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_ptp1588_transfer\enet_txrx_ptp1588_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_ptp1588_transfer\fsl_phy.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_ptp1588_transfer\fsl_phy.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_ptp1588_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_ptp1588_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_ENET_TxRx_transfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_transfer\enet_txrx_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_transfer\fsl_phy.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_transfer\fsl_phy.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\enet\txrx_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_EWM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\ewm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\ewm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\ewm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\ewm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\ewm\ewm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\ewm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\ewm\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_CAN_InterruptTransfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\interrupt_transfer\flexcan_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\interrupt_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_CAN_Loopback"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback\flexcan_loopback.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_CAN_LoopbackTransfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback_transfer\flexcan_loopback_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexcan\loopback_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_I2C_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\flexio_i2c_interrupt_lpi2c_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_I2C_ReadAccel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\read_accel_value_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\read_accel_value_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\read_accel_value_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\read_accel_value_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\read_accel_value_transfer\flexio_i2c_read_accel_value_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\read_accel_value_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2c\read_accel_value_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_I2S_EDMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\flexio_i2s_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\edma_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_I2S_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\flexio_i2s_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\i2s\interrupt_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_PWM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\pwm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\pwm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\pwm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\pwm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\pwm\flexio_pwm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\pwm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\pwm\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_SPI_EDMA_Master"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\master\flexio_spi_edma_lpspi_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_SPI_EDMA_Slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\slave\flexio_spi_edma_lpspi_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\edma_lpspi_transfer\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_SPI_Interrupt_Mast"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\master\flexio_spi_int_lpspi_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_SPI_Interrupt_Slav"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\slave\flexio_spi_int_lpspi_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\spi\int_lpspi_transfer\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_UART_EDMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\edma_transfer\flexio_uart_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\edma_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_UART_Interrupt_RB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\int_rb_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\int_rb_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\int_rb_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\int_rb_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\int_rb_transfer\flexio_uart_int_rb_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\int_rb_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\int_rb_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_UART_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\interrupt_transfer\flexio_uart_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\interrupt_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexIO_UART_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\polling_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\polling_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\polling_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\polling_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\polling_transfer\flexio_uart_polling_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\polling_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexio\uart\polling_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexRAM_Access"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_access\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_access\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_access\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_access\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_access\flexram.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_access\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_access\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexRAM_Allocate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_allocate\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_allocate\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_allocate\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_allocate\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_allocate\flexram.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_allocate\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexram\ram_allocate\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexSPI_Hyperflash_Pollin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\app.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\flexspi_hyper_flash_ops.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\flexspi_hyper_flash_polling_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_FlexSPI_NOR_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\nor\polling_transfer\app.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\nor\polling_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\nor\polling_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\nor\polling_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\nor\polling_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\nor\polling_transfer\flexspi_nor_flash_ops.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\nor\polling_transfer\flexspi_nor_polling_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\nor\polling_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\nor\polling_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_GPIO_InputInterrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\input_interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\input_interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\input_interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\input_interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\input_interrupt\gpio_input_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\input_interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\input_interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_GPIO_LED_Output"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\led_output\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\led_output\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\led_output\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\led_output\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\led_output\gpio_led_output.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\led_output\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpio\led_output\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_GPT_Timer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpt\timer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpt\timer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpt\timer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpt\timer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpt\timer\gpt_timer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpt\timer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\gpt\timer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_KPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\kpp\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\kpp\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\kpp\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\kpp\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\kpp\kpp.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\kpp\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\kpp\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPI2C_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt\lpi2c_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPI2C_Interrupt_B2B_Maste"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\master\lpi2c_interrupt_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPI2C_Interrupt_B2B_Slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\slave\lpi2c_interrupt_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\interrupt_b2b_transfer\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPI2C_Polling_B2B_Master"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\master\lpi2c_polling_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPI2C_Polling_B2B_Slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\slave\lpi2c_polling_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpi2c\polling_b2b_transfer\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPSPI_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt\lpspi_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPSPI_Interrupt_B2B_Maste"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\master\lpspi_interrupt_b2b_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\master\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPSPI_Interrupt_B2B_Slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\slave\lpspi_interrupt_b2b_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpspi\interrupt_b2b\slave\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPUART_EDMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\edma_transfer\lpuart_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\edma_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPUART_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt\lpuart_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPUART_Interrupt_RB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_rb_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_rb_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_rb_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_rb_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_rb_transfer\lpuart_interrupt_rb_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_rb_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_rb_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPUART_InterruptTransfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer\lpuart_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPUART_Interrupttransfer7"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer_seven_bits\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer_seven_bits\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer_seven_bits\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer_seven_bits\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer_seven_bits\interrupt_transfer_seven_bits.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer_seven_bits\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\interrupt_transfer_seven_bits\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPUART_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling\lpuart_polling.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling\pin_mux.h
# End Source File
# End Group
# Begin Group "1050_LPUART_Polling7"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling_seven_bits\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling_seven_bits\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling_seven_bits\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling_seven_bits\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling_seven_bits\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling_seven_bits\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\lpuart\polling_seven_bits\polling_seven_bits.c
# End Source File
# End Group
# Begin Group "1050_PIT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pit\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pit\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pit\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pit\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pit\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pit\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pit\pit.c
# End Source File
# End Group
# Begin Group "1050_PWM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pwm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pwm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pwm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pwm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pwm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pwm\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pwm\pwm.c
# End Source File
# End Group
# Begin Group "1050_PxP_Blend"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\blend\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\blend\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\blend\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\blend\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\blend\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\blend\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\blend\pxp_blend.c
# End Source File
# End Group
# Begin Group "1050_PxP_Flip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\flip\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\flip\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\flip\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\flip\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\flip\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\flip\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\flip\pxp_flip.c
# End Source File
# End Group
# Begin Group "1050_PxP_LCDIF_Handshake"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\lcdif_handshake\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\lcdif_handshake\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\lcdif_handshake\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\lcdif_handshake\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\lcdif_handshake\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\lcdif_handshake\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\lcdif_handshake\pxp_lcdif_handshake.c
# End Source File
# End Group
# Begin Group "1050_PxP_Queue"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\queue\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\queue\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\queue\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\queue\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\queue\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\queue\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\queue\pxp_queue.c
# End Source File
# End Group
# Begin Group "1050_PxP_Rotate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\rotate\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\rotate\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\rotate\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\rotate\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\rotate\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\rotate\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\rotate\pxp_rotate.c
# End Source File
# End Group
# Begin Group "1050_PxP_Scale"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\scale\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\scale\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\scale\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\scale\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\scale\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\scale\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\pxp\scale\pxp_scale.c
# End Source File
# End Group
# Begin Group "1050_QTMR_InCapture_OutCompare"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm\qtmr_inputcapture_outputpwm.c
# End Source File
# End Group
# Begin Group "1050_QTMR_InCaputre_OutCom_DMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm_dma\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm_dma\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm_dma\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm_dma\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm_dma\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm_dma\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\inputcapture_outputpwm_dma\qtmr_inputcapture_outputpwm_dma.c
# End Source File
# End Group
# Begin Group "1050_QTMR_Timer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\timer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\timer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\timer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\timer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\timer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\timer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\qtmr\timer\qtmr_timer.c
# End Source File
# End Group
# Begin Group "1050_RTWDOG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\rtwdog\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\rtwdog\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\rtwdog\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\rtwdog\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\rtwdog\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\rtwdog\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\rtwdog\rtwdog.c
# End Source File
# End Group
# Begin Group "1050_SAI_EDMA_record"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_record_playback\sai_edma_record_playback.c
# End Source File
# End Group
# Begin Group "1050_SAI_EDMA_transfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\music.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\edma_transfer\sai_edma_transfer.c
# End Source File
# End Group
# Begin Group "1050_SAI_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\music.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt\sai_interrupt.c
# End Source File
# End Group
# Begin Group "1050_SAI_InterruptRecord"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_record_playback\sai_interrupt_record_playback.c
# End Source File
# End Group
# Begin Group "1050_SAI_Interrupttransfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\fsl_codec_common.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\fsl_codec_common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\fsl_wm8960.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\fsl_wm8960.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\music.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sai\interrupt_transfer\sai_interrupt_transfer.c
# End Source File
# End Group
# Begin Group "1050_SDCARD_FreeRTOS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\fsl_sdmmc_event.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\fsl_sdmmc_event.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\fsl_sdmmc_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\freertos\sdcard_freertos.c
# End Source File
# End Group
# Begin Group "1050_SDCARD_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\interrupt\fsl_sdmmc_event.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\interrupt\fsl_sdmmc_event.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\interrupt\fsl_sdmmc_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\interrupt\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\interrupt\sdcard_interrupt.c
# End Source File
# End Group
# Begin Group "1050_SDCARD_Polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\polling\fsl_sdmmc_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\polling\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\sdcard\polling\sdcard_polling.c
# End Source File
# End Group
# Begin Group "1050_SEMC_SDRAM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\semc\sdram\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\semc\sdram\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\semc\sdram\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\semc\sdram\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\semc\sdram\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\semc\sdram\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\semc\sdram\semc_sdram.c
# End Source File
# End Group
# Begin Group "1050_SNVS_HP_HAC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_hac\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_hac\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_hac\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_hac\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_hac\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_hac\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_hac\snvs_hp_hac.c
# End Source File
# End Group
# Begin Group "1050_SNVS_HP_RTC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_rtc\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_rtc\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_rtc\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_rtc\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_rtc\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_rtc\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_hp_rtc\snvs_hp_rtc.c
# End Source File
# End Group
# Begin Group "1050_SNVS_LP_MC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_mc\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_mc\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_mc\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_mc\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_mc\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_mc\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_mc\snvs_lp_mc.c
# End Source File
# End Group
# Begin Group "1050_SNVS_LP_SRTC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_srtc\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_srtc\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_srtc\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_srtc\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_srtc\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_srtc\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_lp_srtc\snvs_lp_srtc.c
# End Source File
# End Group
# Begin Group "1050_SNVS_SW_ZMK"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_sw_zmk\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_sw_zmk\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_sw_zmk\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_sw_zmk\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_sw_zmk\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_sw_zmk\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\snvs\snvs_sw_zmk\snvs_sw_zmk.c
# End Source File
# End Group
# Begin Group "1050_SRC_Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\src\src_reset_source\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\src\src_reset_source\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\src\src_reset_source\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\src\src_reset_source\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\src\src_reset_source\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\src\src_reset_source\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\src\src_reset_source\src_reset_source.c
# End Source File
# End Group
# Begin Group "1050_TempMon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\tempmon\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\tempmon\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\tempmon\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\tempmon\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\tempmon\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\tempmon\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\tempmon\tempmon.c
# End Source File
# End Group
# Begin Group "1050_TRNG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\trng\random\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\trng\random\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\trng\random\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\trng\random\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\trng\random\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\trng\random\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\trng\random\trng_random.c
# End Source File
# End Group
# Begin Group "1050_WDOG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\wdog\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\wdog\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\wdog\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\wdog\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\wdog\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\wdog\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\wdog\wdog.c
# End Source File
# End Group
# Begin Group "1050_XBAR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\xbara\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\xbara\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\xbara\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\xbara\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\xbara\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\xbara\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\xbara\xbara.c
# End Source File
# End Group
# End Group
# Begin Group "lwIP_DHCP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\lwip_examples\lwip_dhcp\bm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\lwip_examples\lwip_dhcp\bm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\lwip_examples\lwip_dhcp\bm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\lwip_examples\lwip_dhcp\bm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\lwip_examples\lwip_dhcp\bm\fsl_phy.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\lwip_examples\lwip_dhcp\bm\fsl_phy.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\lwip_examples\lwip_dhcp\bm\lwip_dhcp_bm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\lwip_examples\lwip_dhcp\bm\lwipopts.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\lwip_examples\lwip_dhcp\bm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\lwip_examples\lwip_dhcp\bm\pin_mux.h
# End Source File
# End Group
# Begin Group "demo_apps"

# PROP Default_Filter ""
# Begin Group "bubble Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\bubble\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\bubble\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\bubble\bubble.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\bubble\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\bubble\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\bubble\fsl_fxos.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\bubble\fsl_fxos.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\bubble\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\bubble\pin_mux.h
# End Source File
# End Group
# Begin Group "ecompass Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\ecompass\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\ecompass\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\ecompass\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\ecompass\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\ecompass\ecompass.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\ecompass\fsl_fxos.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\ecompass\fsl_fxos.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\ecompass\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\ecompass\pin_mux.h
# End Source File
# End Group
# Begin Group "flexio_pwm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\flexio_pwm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\flexio_pwm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\flexio_pwm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\flexio_pwm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\flexio_pwm\flexio_pwm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\flexio_pwm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\flexio_pwm\pin_mux.h
# End Source File
# End Group
# Begin Group "hello_world"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world\hello_world.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world\pin_mux.h
# End Source File
# End Group
# Begin Group "hello_world_xip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world_xip\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world_xip\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world_xip\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world_xip\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world_xip\hello_world.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world_xip\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\hello_world_xip\pin_mux.h
# End Source File
# End Group
# Begin Group "lwip_ping"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\lwip\lwip_ping\bm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\lwip\lwip_ping\bm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\lwip\lwip_ping\bm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\lwip\lwip_ping\bm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\lwip\lwip_ping\bm\fsl_phy.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\lwip\lwip_ping\bm\fsl_phy.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\lwip\lwip_ping\bm\lwip_ping_bm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\lwip\lwip_ping\bm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\demo_apps\lwip\lwip_ping\bm\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "cmsis_driver_examples"

# PROP Default_Filter ""
# Begin Group "LPUART Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\cmsis_usart_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\Driver_Common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\Driver_USART.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\RTE_Device.h
# End Source File
# End Group
# Begin Group "LPUART DMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\cmsis_usart_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\Driver_Common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\Driver_USART.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\RTE_Device.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "EVK-iMXRT1060"

# PROP Default_Filter ""
# Begin Group "1060_aws_examples"

# PROP Default_Filter ""
# End Group
# Begin Group "1060_bootloader_examples"

# PROP Default_Filter ""
# End Group
# Begin Group "1060_cmsis_driver_examples"

# PROP Default_Filter ""
# End Group
# Begin Group "1060_demo_apps"

# PROP Default_Filter ""
# End Group
# Begin Group "1060_driver_examples"

# PROP Default_Filter ""
# Begin Group "1060_PWM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\pwm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\pwm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\pwm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\pwm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\pwm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\pwm\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\pwm\pwm.c
# End Source File
# End Group
# Begin Group "1060_XBAR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\xbara\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\xbara\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\xbara\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\xbara\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\xbara\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\xbara\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\xbara\xbara.c
# End Source File
# End Group
# Begin Group "1060_flexio"

# PROP Default_Filter ""
# Begin Group "FlexIO_i2C"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\flexio_i2c_interrupt_lpi2c_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\i2c\interrupt_lpi2c_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "FlexIO_I2S"

# PROP Default_Filter ""
# End Group
# Begin Group "FlexIO_PWM Nr. 1"

# PROP Default_Filter ""
# End Group
# Begin Group "FlexIO_SPI"

# PROP Default_Filter ""
# Begin Group "FlexIO_SPI_EDMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\edma_lpspi_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\edma_lpspi_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\edma_lpspi_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\edma_lpspi_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\edma_lpspi_transfer\master\flexio_spi_edma_lpspi_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\edma_lpspi_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\edma_lpspi_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "FlexIO_SPI_INT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\int_lpspi_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\int_lpspi_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\int_lpspi_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\int_lpspi_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\int_lpspi_transfer\master\flexio_spi_int_lpspi_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\int_lpspi_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1060\driver_examples\flexio\spi\int_lpspi_transfer\master\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "FlexIO_UART"

# PROP Default_Filter ""
# End Group
# End Group
# Begin Group "1060_flexio3"

# PROP Default_Filter ""
# End Group
# End Group
# End Group
# Begin Group "EVK-iMXRT1064"

# PROP Default_Filter ""
# Begin Group "1064_driver_examples"

# PROP Default_Filter ""
# End Group
# Begin Group "1064_cmsis_driver_examples"

# PROP Default_Filter ""
# Begin Group "1064_I2C_EDMA_master"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\master\cmsis_lpi2c_edma_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\master\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\master\RTE_Device.h
# End Source File
# End Group
# Begin Group "1064_I2C_EDMA_slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\slave\cmsis_lpi2c_edma_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\slave\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\edma_b2b_transfer\slave\RTE_Device.h
# End Source File
# End Group
# Begin Group "1064_I2C_Int_master"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\master\cmsis_lpi2c_int_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\master\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\master\RTE_Device.h
# End Source File
# End Group
# Begin Group "1064_I2C_Int_slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\slave\cmsis_lpi2c_int_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\slave\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpi2c\int_b2b_transfer\slave\RTE_Device.h
# End Source File
# End Group
# Begin Group "1064_LPSPI_EDMA_master"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\master\cmsis_lpspi_edma_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\master\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\master\RTE_Device.h
# End Source File
# End Group
# Begin Group "1064_LPSPI_EDMA_slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\slave\cmsis_lpspi_edma_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\slave\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\edma_b2b_transfer\slave\RTE_Device.h
# End Source File
# End Group
# Begin Group "1064_LPSPI_INT_master"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\master\cmsis_lpspi_int_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\master\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\master\RTE_Device.h
# End Source File
# End Group
# Begin Group "1064_LPSPI_INT_slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\slave\cmsis_lpspi_int_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\slave\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1064\cmsis_driver_examples\lpspi\int_b2b_transfer\slave\RTE_Device.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "EVK-MCIMX6UL"

# PROP Default_Filter ""
# Begin Group "6UL_DemoApps"

# PROP Default_Filter ""
# End Group
# Begin Group "6UL_DriverExamples"

# PROP Default_Filter ""
# Begin Group "UART_Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\driver_examples\uart\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\driver_examples\uart\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\driver_examples\uart\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\driver_examples\uart\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\driver_examples\uart\interrupt\clock_config_ocram.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\driver_examples\uart\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\driver_examples\uart\interrupt\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\driver_examples\uart\interrupt\uart_interrupt.c
# End Source File
# End Group
# End Group
# Begin Group "6UL_ProjectTemplate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\project_template\pin_mux.h
# End Source File
# End Group
# Begin Group "6UL_rtos_examples"

# PROP Default_Filter ""
# Begin Group "freertos_debug_console"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\rtos_examples\freertos_debug_console\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\rtos_examples\freertos_debug_console\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\rtos_examples\freertos_debug_console\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\rtos_examples\freertos_debug_console\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\rtos_examples\freertos_debug_console\clock_config_ocram.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\rtos_examples\freertos_debug_console\freertos_debug_console.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\rtos_examples\freertos_debug_console\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\rtos_examples\freertos_debug_console\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx6ul\rtos_examples\freertos_debug_console\pin_mux.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "EVK-MCIMX7ULP"

# PROP Default_Filter ""
# Begin Group "7UL_CMSIS_DriverExamples"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_DemoApps"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_DriverExamples"

# PROP Default_Filter ""
# Begin Group "7UL_Acmp"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_Crc"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_DAC12"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_EDMA"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_EWM"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_FlexIO"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_GPIO"

# PROP Default_Filter ""
# Begin Group "input_interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\input_interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\input_interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\input_interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\input_interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\input_interrupt\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\input_interrupt\gpio_input_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\input_interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\input_interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "led_output"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\led_output\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\led_output\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\led_output\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\led_output\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\led_output\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\led_output\gpio_led_output.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\led_output\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\gpio\led_output\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "7UL_LpADC"

# PROP Default_Filter ""
# Begin Group "Interrupt Nr. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_interrupt\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_interrupt\lpadc_single_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "Polling Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_polling\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_polling\lpadc_single_polling.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpadc\single_polling\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "7UL_lpi2c"

# PROP Default_Filter ""
# Begin Group "edma_b2b_transfer"

# PROP Default_Filter ""
# Begin Group "maszer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\master\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\master\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\master\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\master\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\master\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\master\lpi2c_edma_b2b_transfer_master.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\master\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\master\pin_mux.h
# End Source File
# End Group
# Begin Group "slave"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\slave\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\slave\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\slave\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\slave\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\slave\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\slave\lpi2c_edma_b2b_transfer_slave.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\slave\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\edma_b2b_transfer\slave\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "interrupt Nr. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\interrupt\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\interrupt\lpi2c_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpi2c\interrupt\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "7UL_lpit"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_lpspi"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_lptmr"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_lpuart"

# PROP Default_Filter ""
# Begin Group "7UL_edma_transfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\edma_transfer\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\edma_transfer\lpuart_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\edma_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "7UL_interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\interrupt\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\interrupt\lpuart_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\driver_examples\lpuart\interrupt\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "7UL_ltc"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_pf1550"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_qspi"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_sai"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_sema42"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_snvs"

# PROP Default_Filter ""
# End Group
# End Group
# Begin Group "7UL_MbedTlsExamples"

# PROP Default_Filter ""
# End Group
# Begin Group "7UL_MulticoreExamples"

# PROP Default_Filter ""
# Begin Group "erpc_common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_common\erpc_error_handler.cpp
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_common\erpc_error_handler.h
# End Source File
# End Group
# Begin Group "erpc_matrix_multiply_rpmsg_rto"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\app_srtm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\app_srtm.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\erpc_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\main_core1.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\erpc_matrix_multiply_rpmsg_rtos\rpmsg_config.h
# End Source File
# End Group
# Begin Group "rpmsg_lite_pingpong_rtos"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\app_srtm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\app_srtm.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\main_remote.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_pingpong_rtos\rpmsg_config.h
# End Source File
# End Group
# Begin Group "rpmsg_lite_str_echo_rtos"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\app_srtm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\app_srtm.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\fsl_iomuxc.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\main_remote.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\multicore_examples\rpmsg_lite_str_echo_rtos\rpmsg_config.h
# End Source File
# End Group
# End Group
# Begin Group "7UL_ProjectTemplates"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\project_template\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\project_template\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\project_template\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\project_template\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\project_template\peripherals.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\project_template\peripherals.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\project_template\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmcimx7ulp\project_template\pin_mux.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Middleware"

# PROP Default_Filter ""
# Begin Group "SDmmc"

# PROP Default_Filter ""
# Begin Group "port_FreeRTOS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\port\sdhc\freertos\event.c
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\port\sdhc\freertos\fsl_host.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\port\sdhc\polling\event.c
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\inc\fsl_card.h
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\port\sdhc\polling\fsl_host.c
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\inc\fsl_host.h
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\src\fsl_mmc.c
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\src\fsl_sd.c
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\src\fsl_sdio.c
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\src\fsl_sdmmc.c
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\inc\fsl_sdmmc.h
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\src\fsl_sdspi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\inc\fsl_sdspi.h
# End Source File
# Begin Source File

SOURCE=.\middleware\sdmmc_2.1.2\inc\fsl_specification.h
# End Source File
# End Group
# Begin Group "lwip"

# PROP Default_Filter ""
# Begin Group "port Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lwip\port\arch\cc.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\port\ethernetif.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\port\ethernetif.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\port\lwipopts.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\port\arch\perf.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\port\sys_arch.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\port\arch\sys_arch.h
# End Source File
# End Group
# Begin Group "API Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lwip\src\api\api_lib.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\api\api_msg.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\api\err.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\api\netbuf.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\api\netdb.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\api\netifapi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\api\sockets.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\api\tcpip.c
# End Source File
# End Group
# Begin Group "core Nr. 1"

# PROP Default_Filter ""
# Begin Group "ipv6"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv6\dhcp6.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv6\ethip6.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv6\icmp6.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv6\inet6.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv6\ip6.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv6\ip6_addr.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv6\ip6_frag.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv6\mld6.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv6\nd6.c
# End Source File
# End Group
# Begin Group "ipv4"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv4\autoip.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv4\dhcp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv4\etharp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv4\icmp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv4\igmp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv4\ip4.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv4\ip4_addr.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ipv4\ip4_frag.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\middleware\lwip\src\core\def.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\dns.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\inet_chksum.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\init.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\ip.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\mem.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\memp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\netif.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\pbuf.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\raw.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\stats.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\sys.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\tcp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\tcp_in.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\tcp_out.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\timeouts.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip\src\core\udp.c
# End Source File
# End Group
# End Group
# Begin Group "littlefs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\littlefs\lfs.c
# End Source File
# Begin Source File

SOURCE=.\middleware\littlefs\lfs.h
# End Source File
# Begin Source File

SOURCE=.\middleware\littlefs\lfs_util.c
# End Source File
# Begin Source File

SOURCE=.\middleware\littlefs\lfs_util.h
# End Source File
# End Group
# Begin Group "fatFS Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\fatfs\source\diskio.c
# End Source File
# Begin Source File

SOURCE=.\middleware\fatfs\source\diskio.h
# End Source File
# Begin Source File

SOURCE=.\middleware\fatfs\source\ff.c
# End Source File
# Begin Source File

SOURCE=.\middleware\fatfs\source\ff.h
# End Source File
# Begin Source File

SOURCE=.\middleware\fatfs\source\ffconf_template.h
# End Source File
# Begin Source File

SOURCE=.\middleware\fatfs\source\ffsystem.c
# End Source File
# Begin Source File

SOURCE=.\middleware\fatfs\source\ffunicode.c
# End Source File
# Begin Source File

SOURCE=.\middleware\fatfs\source\integer.h
# End Source File
# End Group
# Begin Group "ntag_i2c_plus"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\inc\global_types.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_I2C\inc\HAL_I2C_driver.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_I2C\inc\HAL_I2C_kinetis_fsl.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_ISR\inc\HAL_ISR_callback_list.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_ISR\inc\HAL_ISR_driver.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_TMR\inc\HAL_timer_driver.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_I2C\i2c_kinetis_fsl.c
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_ISR\isr_common.c
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_ISR\isr_driver_intern.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_ISR\isr_kinetis.c
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_ISR\isr_kinetis_intern.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\inc\nfc_device.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_NTAG\ntag_bridge.c
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_NTAG\inc\ntag_bridge.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_NTAG\inc\ntag_defines.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_NTAG\inc\ntag_device_list.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_NTAG\ntag_driver.c
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_NTAG\inc\ntag_driver.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_NTAG\ntag_driver_intern.h
# End Source File
# Begin Source File

SOURCE=.\middleware\ntag_i2c_plus\HAL_TMR\timer_kinetis.c
# End Source File
# End Group
# Begin Group "issdk"

# PROP Default_Filter ""
# Begin Group "Board_K22F"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\board.c"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\board.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\clock_config.c"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\clock_config.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\frdm_k22f.c"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\frdm_k22f.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\frdm-stbc-agm01\frdm_stbc_agm01_shield.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\frdm-stbc-agmp03\frdm_stbc_agmp03_shield.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\frdm-stbc-sa9500\frdm_stbc_sa9500_shield.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\frdm-stbc-agm01\issdk_hal.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\frdm-stbc-agmp03\issdk_hal.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\frdm-stbc-sa9500\issdk_hal.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\pin_mux.c"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\pin_mux.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k22f\RTE_Device.h"
# End Source File
# End Group
# Begin Group "sensors"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\issdk\sensors\diff_p.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxas21002.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxas21002_drv.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxas21002_drv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxlc95000.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxlc95000_drv.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxlc95000_drv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxls8471q.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxls8471q_drv.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxls8471q_drv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxls8962.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxls8962_drv.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxls8962_drv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxos8700.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxos8700_drv.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxos8700_drv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\fxpq3115.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\host_io_uart.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\host_io_uart.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mag3110.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mag3110_drv.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mag3110_drv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mma845x.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mma8491q.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mma865x.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mma865x_drv.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mma865x_drv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mma9553.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mma9553_drv.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mma9553_drv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mpl3115.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mpl3115_drv.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\mpl3115_drv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\register_io_i2c.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\register_io_i2c.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\register_io_spi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\register_io_spi.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\sensor_drv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\sensor_io_i2c.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\sensor_io_i2c.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\sensor_io_spi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\sensors\sensor_io_spi.h
# End Source File
# End Group
# Begin Group "frdm-k64f Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k64f\board.c"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k64f\board.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k64f\clock_config.c"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k64f\clock_config.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k64f\frdm_k64f.c"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k64f\frdm_k64f.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k64f\pin_mux.c"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k64f\pin_mux.h"
# End Source File
# Begin Source File

SOURCE=".\middleware\issdk\boardkit\frdm-k64f\RTE_Device.h"
# End Source File
# End Group
# Begin Group "example"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\data_logger_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\demo_11_axis_mult2b.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\diff_p_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\dp5004_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\dp5004\dp5004_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\fxas21002_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxas21002\fxas21002_fifo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxas21002\fxas21002_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxas21002\fxas21002_poll.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\fxas21002_poll_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxas21002\fxas21002_poll_spi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxlc95000\fxlc95000_accel_i2c.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxlc95000\fxlc95000_accel_spi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\fxlc95000_flash_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\fxls8471_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\fxls8471_demo_kl25z.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxls8471q\fxls8471q_spi_fifo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxls8471q\fxls8471q_spi_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxls8471q\fxls8471q_spi_poll.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\fxls8962_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxls8962\fxls8962_fifo_spi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxls8962\fxls8962_freefall.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxls8962\fxls8962_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxls8962\fxls8962_motion_detection.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxls8962\fxls8962_normal.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxls8962\fxls8962_normal_spi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxls8962\fxls8962_orientation_detection.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\fxos8700_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxos8700\fxos8700_fifo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxos8700\fxos8700_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxos8700\fxos8700_poll.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\fxos8700_poll_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxos8700\fxos8700_poll_spi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxpq3115\fxpq3115_altitude.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\fxpq3115_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxpq3115\fxpq3115_fifo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxpq3115\fxpq3115_fifo_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxpq3115\fxpq3115_normal.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxpq3115\fxpq3115_normal_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\fxpq3115\fxpq3115_oneshot.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\host_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\mag3110_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mag3110\mag3110_normal.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mag3110\mag3110_normal_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mag3110\mag3110_oneshot.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\mag3110_poll_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\mma845x_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma845x\mma845x_fifo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma845x\mma845x_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma845x\mma845x_poll.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\mma8491q_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma8491q\mma8491q_example.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\mma865x_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma865x\mma865x_fifo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma865x\mma865x_freefall.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma865x\mma865x_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma865x\mma865x_poll.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\mma9553_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma9553\mma9553_pedometer_i2c.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma9553\mma9553_pedometer_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mma9553\mma9553_pedometer_spi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mpl3115\mpl3115_altitude.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\mpl3115_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mpl3115\mpl3115_fifo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mpl3115\mpl3115_fifo_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mpl3115\mpl3115_normal.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mpl3115\mpl3115_normal_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\mpl3115\mpl3115_oneshot.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\mpl3115_oneShot_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\mpl3115_poll_demo.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\orientaion_application_baremetal_agm01.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\orientaion_application_baremetal_agm04.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\driverexamples\demos\orientaion_application_baremetal_agmp03.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\approximations.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\approximations.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\calibration_storage.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\calibration_storage.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\control.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\control.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\debug.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\debug.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\DecodeCommandBytes.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_FXAS21002.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_FXLS8962.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_FXOS8700.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_KSDK_NVM.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_KSDK_NVM.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_MAG3110.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_MMA8652.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_MPL3115.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_pit.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_pit.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\driver_systick.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\drivers.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\fusion.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\fusion.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\hal_frdm_fxs_mult2_b.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\pedometer\include\libinclude\KeynetikPedometer.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\magnetic.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\magnetic.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\matrix.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\matrix.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\orientation.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\orientation.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\output_stream.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\pedometer\source\pedometer.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\pedometer\include\pedometer.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\precisionAccelerometer.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\precisionAccelerometer.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\sensor_fusion.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\sensor_fusion.h
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\status.c
# End Source File
# Begin Source File

SOURCE=.\middleware\issdk\algorithms\sensorfusion\sources\status.h
# End Source File
# End Group
# Begin Group "wireless"

# PROP Default_Filter ""
# Begin Group "bluetooth_1.2.5"

# PROP Default_Filter ""
# Begin Group "application"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\app_coap\app_coap.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\app_coap\app_coap.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\app_echo_udp\app_echo_udp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\app_echo_udp\app_echo_udp.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\app_sockets\app_sockets.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\app_sockets\app_sockets.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ApplMain.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\hybrid\ApplMain.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ApplMain.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\hybrid\ApplMain.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ble_conn_manager.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ble_conn_manager.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ble_controller_task.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ble_controller_task_config.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ble_host_task_config.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ble_host_tasks.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ble_init.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\hybrid\ble_init.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ble_init.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\hybrid\ble_init.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ble_service_discovery.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ble_service_discovery.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gap.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gap.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gap_types.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gap_types.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gatt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gatt.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gatt_db_app.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gatt_db_app.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gatt_db_app_types.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gatt_db_app_types.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gatt_types.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_gatt_types.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_l2cap.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_l2cap.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_l2cap_types.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_l2cap_types.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_types.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\fsci\fsci_ble_types.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\macros\gatt_alloc_x.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\gatt_database.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\gatt_database_dynamic.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\gatt_database_dynamic.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\gatt_db_handles.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\macros\gatt_db_macros.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\macros\gatt_db_x_macros.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\macros\gatt_decl_x.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\macros\gatt_enum_x.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\macros\gatt_init_x.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\macros\gatt_size_x.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\macros\gatt_uuid_decl_x.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\gatt_db\macros\gatt_uuid_def_x.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\host\host_ble.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\host\host_ble.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\host\host_gap.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\host\host_gatt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\host\host_gatt_db.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\fsci_host\host\host_l2cap.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\ieee11073.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\nwk_ip_globals.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\nwk_ip_init.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\nwk_ip_init.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\nwk_ip_manager.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\nwk_ip_manager.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\shell_ip\shell_nwk_ip.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\application\common\nwk_ip\shell_ip\shell_nwk_ip.h
# End Source File
# End Group
# Begin Group "controller"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\controller\interface\controller_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\controller\lib\lib_ble_kw41z_controller.a
# End Source File
# End Group
# Begin Group "hci_transport"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\hci_transport\interface\hci_transport.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\hci_transport\source\hcit_serial_interface.c
# End Source File
# End Group
# Begin Group "host"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\att_callbacks.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\att_errors.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\att_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\att_params.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\att_types.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\config\ble_config.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\ble_constants.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\ble_general.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\config\ble_globals.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\ble_host_tasks.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\ble_sig_defines.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\ble_utils.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\fsci_ble_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\gap_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\gap_types.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\gatt_client_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\gatt_database.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\gatt_db_app_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\gatt_db_dynamic.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\gatt_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\gatt_server_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\gatt_types.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\hci_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\hci_types.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\l2ca_cb_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\l2ca_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\l2ca_types.h
# End Source File
# Begin Source File

SOURCE=".\middleware\wireless\bluetooth_1.2.5\host\lib\lib_ble_4-2_host_central_cm0p.a"
# End Source File
# Begin Source File

SOURCE=".\middleware\wireless\bluetooth_1.2.5\host\lib\lib_ble_4-2_host_cm0p.a"
# End Source File
# Begin Source File

SOURCE=".\middleware\wireless\bluetooth_1.2.5\host\lib\lib_ble_4-2_host_fsci_cm0p.a"
# End Source File
# Begin Source File

SOURCE=".\middleware\wireless\bluetooth_1.2.5\host\lib\lib_ble_4-2_host_peripheral_cm0p.a"
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\host\interface\vendor_debug_commands.h
# End Source File
# End Group
# Begin Group "profiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\alert_notification\alert_notification_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\alert_notification\alert_notification_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\battery\battery_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\battery\battery_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\blood_pressure\blood_pressure_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\blood_pressure\blood_pressure_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\time\current_time_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\time\current_time_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\cycling_power\cycling_power_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\cycling_power\cycling_power_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\cycling_speed_cadence\cycling_speed_cadence_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\cycling_speed_cadence\cycling_speed_cadence_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\device_info\device_info_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\device_info\device_info_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\glucose\glucose_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\glucose\glucose_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\health_thermometer\health_thermometer_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\health_thermometer\health_thermometer_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\heart_rate\heart_rate_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\heart_rate\heart_rate_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\hid\hid_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\hid\hid_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\proximity\immediate_alert_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\proximity\immediate_alert_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\ipsp\ips_profile.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\ipsp\ipsp_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\proximity\link_loss_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\proximity\link_loss_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\time\next_dst_change_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\time\next_dst_change_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\otap\otap_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\otap\otap_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\pulse_oximeter\pulse_oximeter_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\pulse_oximeter\pulse_oximeter_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\time\reference_time_update_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\time\reference_time_update_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\running_speed_cadence\running_speed_cadence_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\running_speed_cadence\running_speed_cadence_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\temperature\temperature_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\temperature\temperature_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\proximity\tx_power_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\proximity\tx_power_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\wireless_uart\wireless_uart_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\wireless_uart\wireless_uart_service.c
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\wireless_power_transfer\wpt_interface.h
# End Source File
# Begin Source File

SOURCE=.\middleware\wireless\bluetooth_1.2.5\profiles\wireless_power_transfer\wpt_service.c
# End Source File
# End Group
# End Group
# End Group
# Begin Group "dhara"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\dhara\ecc\bch.c
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\ecc\bch.h
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\dhara\bytes.h
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\dhara\error.c
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\dhara\error.h
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\ecc\hamming.c
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\ecc\hamming.h
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\dhara\journal.c
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\dhara\journal.h
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\dhara\map.c
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\dhara\map.h
# End Source File
# Begin Source File

SOURCE=.\middleware\dhara\dhara\nand.h
# End Source File
# End Group
# Begin Group "aml"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\aml\common_aml.h
# End Source File
# Begin Source File

SOURCE=.\middleware\aml\tmr_aml\ftm_aml.c
# End Source File
# Begin Source File

SOURCE=.\middleware\aml\tmr_aml\ftm_aml.h
# End Source File
# Begin Source File

SOURCE=.\middleware\aml\gpio_aml.h
# End Source File
# Begin Source File

SOURCE=.\middleware\aml\spi_aml\spi_aml.c
# End Source File
# Begin Source File

SOURCE=.\middleware\aml\spi_aml\spi_aml.h
# End Source File
# Begin Source File

SOURCE=.\middleware\aml\tmr_aml\tmr_aml.h
# End Source File
# Begin Source File

SOURCE=.\middleware\aml\tmr_aml\tpm_aml.c
# End Source File
# Begin Source File

SOURCE=.\middleware\aml\tmr_aml\tpm_aml.h
# End Source File
# Begin Source File

SOURCE=.\middleware\aml\wait_aml\wait_aml.c
# End Source File
# Begin Source File

SOURCE=.\middleware\aml\wait_aml\wait_aml.h
# End Source File
# End Group
# Begin Group "dma manager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\dma_manager\fsl_dma_manager.c
# End Source File
# Begin Source File

SOURCE=.\middleware\dma_manager\fsl_dma_manager.h
# End Source File
# End Group
# Begin Group "emv"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\emv\inc\emvl1_adapter_sdk.h
# End Source File
# Begin Source File

SOURCE=.\middleware\emv\src\emvl1_core.c
# End Source File
# Begin Source File

SOURCE=.\middleware\emv\inc\emvl1_core.h
# End Source File
# End Group
# Begin Group "libjpeg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\cderror.h
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\cdjpeg.h
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jaricom.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcapimin.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcapistd.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcarith.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jccoefct.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jccolor.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jchuff.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcinit.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcmainct.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcmarker.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcmaster.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcomapi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcparam.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcprepct.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jcsample.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jctrans.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdapimin.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdapistd.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdarith.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdatadst.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdcolor.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\jdct.h
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdhuff.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdinput.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdmainct.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdmarker.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdmaster.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdmerge.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdpostct.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdsample.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jdtrans.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jerror.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\jerror.h
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jfdctint.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jidctflt.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jidctfst.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jidctint.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jquant1.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jquant2.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\src\jutils.c
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\jversion.h
# End Source File
# Begin Source File

SOURCE=.\middleware\libjpeg\inc\transupp.h
# End Source File
# End Group
# Begin Group "lvhb"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lvhb\lvhb.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lvhb\lvhb.h
# End Source File
# End Group
# Begin Group "mbedtls"

# PROP Default_Filter ""
# End Group
# End Group
# Begin Group "CMSIS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CMSIS\Include\arm_common_tables.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\arm_const_structs.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\arm_math.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\cmsis_armcc.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\cmsis_armcc_V6.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\cmsis_armclang.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\cmsis_compiler.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\cmsis_gcc.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\cmsis_version.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_armv8mbl.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_armv8mml.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_cm0.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_cm0plus.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_cm23.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_cm3.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_cm33.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_cm4.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_cm7.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_cmFunc.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_cmInstr.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_cmSimd.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_sc000.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\core_sc300.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\mpu_armv7.h
# End Source File
# Begin Source File

SOURCE=.\CMSIS\Include\tz_context.h
# End Source File
# End Group
# Begin Group "RTOS Nr. 1"

# PROP Default_Filter ""
# Begin Group "freertos_9.0.0"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\croutine.c
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\croutine.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\deprecated_definitions.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\event_groups.c
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\event_groups.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\FreeRTOS.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\freertos_tasks_c_additions.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\list.c
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\list.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\mpu_prototypes.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\mpu_wrappers.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\portable.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\projdefs.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\queue.c
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\queue.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\semphr.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\StackMacros.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\task.h
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\tasks.c
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\timers.c
# End Source File
# Begin Source File

SOURCE=.\rtos\freertos_9.0.0\Source\include\timers.h
# End Source File
# End Group
# Begin Group "amazon-freertos"

# PROP Default_Filter ""
# Begin Group "freeRTOS"

# PROP Default_Filter ""
# Begin Group "Port GCC CM4F"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS\portable\GCC\ARM_CM4F\fsl_tickless_generic.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS\portable\GCC\ARM_CM4F\fsl_tickless_systick.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS\portable\GCC\ARM_CM4F\port.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS\portable\GCC\ARM_CM4F\portmacro.h"
# End Source File
# End Group
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS\event_groups.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS\list.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS\queue.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS\stream_buffer.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS\tasks.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS\timers.c"
# End Source File
# End Group
# Begin Group "freeRTOSTCP"

# PROP Default_Filter ""
# Begin Group "protocoll"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\FTP\FreeRTOS_FTP_commands.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\include\FreeRTOS_FTP_commands.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\FTP\FreeRTOS_FTP_server.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\HTTP\FreeRTOS_HTTP_commands.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\include\FreeRTOS_HTTP_commands.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\HTTP\FreeRTOS_HTTP_server.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\include\FreeRTOS_server_private.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\Common\FreeRTOS_TCP_server.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\include\FreeRTOS_TCP_server.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\include\NTPClient.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\NTP\NTPDemo.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\protocols\include\NTPDemo.h"
# End Source File
# End Group
# Begin Group "portable"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\portable\BufferManagement\BufferAllocation_1.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\portable\BufferManagement\BufferAllocation_2.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\portable\NetworkInterface\board_family\NetworkInterface.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\portable\NetworkInterface\ksz8851snl\NetworkInterface.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\portable\Compiler\CompilerName\pack_struct_end.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\portable\Compiler\GCC\pack_struct_end.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\portable\Compiler\CompilerName\pack_struct_start.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\portable\Compiler\GCC\pack_struct_start.h"
# End Source File
# End Group
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\FreeRTOS_ARP.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_ARP.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\FreeRTOS_DHCP.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_DHCP.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\FreeRTOS_DNS.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_DNS.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_errno_TCP.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\FreeRTOS_IP.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_IP.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_IP_Private.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\FreeRTOS_Sockets.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_Sockets.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\FreeRTOS_Stream_Buffer.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_Stream_Buffer.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\FreeRTOS_TCP_IP.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_TCP_IP.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\FreeRTOS_TCP_WIN.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_TCP_WIN.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\source\FreeRTOS_UDP_IP.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOS_UDP_IP.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\FreeRTOSIPConfigDefaults.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\IPTraceMacroDefaults.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\NetworkBufferManagement.h"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\FreeRTOS-Plus-TCP\include\NetworkInterface.h"
# End Source File
# End Group
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\bufferpool\aws_bufferpool_static_thread_safe.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\crypto\aws_crypto.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\greengrass\aws_greengrass_discovery.c"
# End Source File
# Begin Source File

SOURCE=".\rtos\amazon-freertos\lib\greengrass\aws_helper_secure_connect.c"
# End Source File
# End Group
# End Group
# End Target
# End Project
