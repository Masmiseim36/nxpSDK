# Microsoft Developer Studio Project File - Name="Kinetis_SDK" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Kinetis_SDK - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "Kinetis_SDK.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "Kinetis_SDK.mak" CFG="Kinetis_SDK - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
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
# Begin Group "EVK-iMXRT1050B"

# PROP Default_Filter ""
# Begin Group "1050 driver_examples"

# PROP Default_Filter ""
# Begin Group "flexSPI polling"

# PROP Default_Filter ""
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

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\flexspi_hyper_flash_polling_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "CAN_interrupt_transfer"

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
# Begin Group "flexRAM_access"

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
# Begin Group "flexRAM_allocate"

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
# Begin Group "LPSPI_interrupt"

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
# Begin Group "SDcard_freeRTOS Nr. 1"

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
# Begin Group "SDcard_Interrupt"

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
# Begin Group "SDcard_polling Nr. 1"

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
# End Group
# Begin Group "EVK-iMXRT1020"

# PROP Default_Filter ""
# Begin Group "driver example"

# PROP Default_Filter ""
# Begin Group "flexSPI Polling Nr. 1"

# PROP Default_Filter ""
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

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\flexspi_nor_polling_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1020\driver_examples\flexspi\nor\polling_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "Ethernet_Transfer_1020"

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
# Begin Group "Ethernet_1588_1020"

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
# End Group
# Begin Group "flexcan_loopback"

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
# Begin Group "flexcan_interrupt"

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
# Begin Group "flexcan_loopbacktransfer"

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
