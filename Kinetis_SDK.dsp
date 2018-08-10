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
# End Group
# Begin Group "boards"

# PROP Default_Filter ""
# Begin Group "frdmk64f"

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
# Begin Group "RTOS"

# PROP Default_Filter ""
# Begin Group "generic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\rtos_examples\freertos_generic\board.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\rtos_examples\freertos_generic\board.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\rtos_examples\freertos_generic\clock_config.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\rtos_examples\freertos_generic\clock_config.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\rtos_examples\freertos_generic\freertos_generic.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\rtos_examples\freertos_generic\FreeRTOSConfig.h
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\rtos_examples\freertos_generic\pin_mux.c
# End Source File
# Begin Source File

SOURCE=.\boards\evkbimxrt1050\rtos_examples\freertos_generic\pin_mux.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "frdmk22f"

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
# End Group
# End Group
# End Target
# End Project
