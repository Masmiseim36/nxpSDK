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
# Begin Group "MIMXRT1052"

# PROP Default_Filter ""
# Begin Group "MIMXRT drivers"

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
# Begin Group "MIMXRT CMSIS drivers"

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
# End Group
# Begin Group "boards"

# PROP Default_Filter ""
# Begin Group "frdmk64f"

# PROP Default_Filter ""
# Begin Group "demo"

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
# Begin Group "evkmimxrt1050"

# PROP Default_Filter ""
# Begin Group "demo_apps"

# PROP Default_Filter ""
# Begin Group "bubble Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\bubble\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\bubble\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\bubble\bubble.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\bubble\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\bubble\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\bubble\fsl_fxos.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\bubble\fsl_fxos.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\bubble\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\bubble\pin_mux.h
# End Source File
# End Group
# Begin Group "ecompass Nr. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\ecompass\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\ecompass\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\ecompass\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\ecompass\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\ecompass\ecompass.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\ecompass\fsl_fxos.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\ecompass\fsl_fxos.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\ecompass\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\ecompass\pin_mux.h
# End Source File
# End Group
# Begin Group "flexio_pwm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\flexio_pwm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\flexio_pwm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\flexio_pwm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\flexio_pwm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\flexio_pwm\flexio_pwm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\flexio_pwm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\flexio_pwm\pin_mux.h
# End Source File
# End Group
# Begin Group "hello_world"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world\hello_world.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world\pin_mux.h
# End Source File
# End Group
# Begin Group "hello_world_xip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world_xip\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world_xip\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world_xip\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world_xip\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world_xip\hello_world.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world_xip\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\hello_world_xip\pin_mux.h
# End Source File
# End Group
# Begin Group "lwip_ping"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\lwip\lwip_ping\bm\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\lwip\lwip_ping\bm\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\lwip\lwip_ping\bm\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\lwip\lwip_ping\bm\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\lwip\lwip_ping\bm\fsl_phy.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\lwip\lwip_ping\bm\fsl_phy.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\lwip\lwip_ping\bm\lwip_ping_bm.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\lwip\lwip_ping\bm\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\demo_apps\lwip\lwip_ping\bm\pin_mux.h
# End Source File
# End Group
# End Group
# Begin Group "cmsis_driver_examples"

# PROP Default_Filter ""
# Begin Group "LPUART Interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\cmsis_usart_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\Driver_Common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\Driver_USART.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\interrupt_transfer\RTE_Device.h
# End Source File
# End Group
# Begin Group "LPUART DMA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\cmsis_usart_edma_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\Driver_Common.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\Driver_USART.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\cmsis_driver_examples\lpuart\edma_transfer\RTE_Device.h
# End Source File
# End Group
# End Group
# Begin Group "driver examples"

# PROP Default_Filter ""
# Begin Group "GPIO - LED"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\led_output\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\led_output\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\led_output\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\led_output\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\led_output\gpio_led_output.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\led_output\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\led_output\pin_mux.h
# End Source File
# End Group
# Begin Group "ADC"

# PROP Default_Filter ""
# End Group
# Begin Group "BEE"

# PROP Default_Filter ""
# End Group
# Begin Group "GPIO IRQ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\input_interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\input_interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\input_interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\input_interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\input_interrupt\gpio_input_interrupt.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\input_interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\gpio\input_interrupt\pin_mux.h
# End Source File
# End Group
# Begin Group "LPUART - polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\polling\lpuart_polling.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\polling\pin_mux.h
# End Source File
# End Group
# Begin Group "sdcard Nr. 1"

# PROP Default_Filter ""
# Begin Group "polling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\polling\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\polling\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\polling\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\polling\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\polling\fsl_sdmmc_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\polling\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\polling\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\polling\sdcard_polling.c
# End Source File
# End Group
# Begin Group "interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\interrupt\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\interrupt\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\interrupt\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\interrupt\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\interrupt\fsl_sdmmc_event.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\interrupt\fsl_sdmmc_event.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\interrupt\fsl_sdmmc_host.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\interrupt\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\interrupt\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\sdcard\interrupt\sdcard_interrupt.c
# End Source File
# End Group
# End Group
# Begin Group "SEMC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\semc\sdram\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\semc\sdram\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\semc\sdram\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\semc\sdram\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\semc\sdram\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\semc\sdram\pin_mux.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\semc\sdram\semc_sdram.c
# End Source File
# End Group
# Begin Group "LPUART - interrupt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\interrupt_transfer\lpuart_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\lpuart\interrupt_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "flexSPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\flexspi_hyper_flash_polling_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexspi\hyper_flash\polling_transfer\pin_mux.h
# End Source File
# End Group
# Begin Group "flexCAN"

# PROP Default_Filter ""
# Begin Group "CAN Interrupt transfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexcan\interrupt_transfer\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexcan\interrupt_transfer\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexcan\interrupt_transfer\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexcan\interrupt_transfer\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexcan\interrupt_transfer\flexcan_interrupt_transfer.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexcan\interrupt_transfer\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkmimxrt1050\driver_examples\flexcan\interrupt_transfer\pin_mux.h
# End Source File
# End Group
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
# Begin Group "lwip_2.0.0"

# PROP Default_Filter ""
# Begin Group "API"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\api\api_lib.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\api\api_msg.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\api\err.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\api\netbuf.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\api\netdb.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\api\netifapi.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\api\sockets.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\api\tcpip.c
# End Source File
# End Group
# Begin Group "apps"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpd\fs.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpd\fsdata.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpd\fsdata.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpd\httpd.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpd\httpd_structs.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_base64.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_base64.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_config.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_fs.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_fs.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_fs_data.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_port.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_prv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_script.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_script.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_sha1.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_sha1.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_supp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_supp.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_task.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_tls.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_tls.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_utf8.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_utf8.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_ws.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_ws.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_ws_api.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_ws_prv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\lwiperf\lwiperf.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\mdns\mdns.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\netbiosns\netbiosns.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_asn1.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_asn1.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_core.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_core_priv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_mib2.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_mib2_icmp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_mib2_interfaces.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_mib2_ip.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_mib2_snmp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_mib2_system.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_mib2_tcp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_mib2_udp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_msg.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_msg.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_netconn.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_pbuf_stream.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_pbuf_stream.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_raw.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_scalar.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_table.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_threadsync.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmp_traps.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmpv3.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmpv3_dummy.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmpv3_mbedtls.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\snmp\snmpv3_priv.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\sntp\sntp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\apps\tftp\tftp_server.c
# End Source File
# End Group
# Begin Group "core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\ipv4\autoip.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\def.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\ipv4\dhcp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\dns.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\ipv4\etharp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\ipv4\icmp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\ipv4\igmp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\inet_chksum.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\init.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\ip.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\ipv4\ip4.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\ipv4\ip4_addr.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\ipv4\ip4_frag.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\mem.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\memp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\netif.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\pbuf.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\raw.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\stats.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\sys.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\tcp.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\tcp_in.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\tcp_out.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\timeouts.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\core\udp.c
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\api.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\arch.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\autoip.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\debug.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\def.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\dhcp.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\dhcp6.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\dns.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\err.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\errno.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\posix\errno.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\etharp.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\netif\etharp.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\netif\ethernet.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\ethip6.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\icmp.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\icmp6.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\igmp.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\inet.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\inet_chksum.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\init.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\ip.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\ip4.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\ip4_addr.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\ip4_frag.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\ip6.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\ip6_addr.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\ip6_frag.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\ip_addr.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\netif\lowpan6.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\netif\lowpan6_opts.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\mem.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\memp.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\mld6.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\nd6.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\netbuf.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\netdb.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\posix\netdb.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\netif.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\netifapi.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\opt.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\pbuf.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\raw.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\sio.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\netif\slipif.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\snmp.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\sockets.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\stats.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\sys.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\tcp.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\tcpip.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\timeouts.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\include\lwip\udp.h
# End Source File
# End Group
# Begin Group "netif"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\netif\ethernet.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\netif\ethernetif.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\netif\lowpan6.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\src\netif\slipif.c
# End Source File
# End Group
# Begin Group "port"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\port\ethernetif.c
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\port\ethernetif.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\port\lwipopts.h
# End Source File
# Begin Source File

SOURCE=.\middleware\lwip_2.0.0\port\sys_arch.c
# End Source File
# End Group
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
# End Target
# End Project
