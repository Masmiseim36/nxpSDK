################################################################################
Secure Boot - Advanced
################################################################################

################################################################################
Secure Boot - Requirements
################################################################################

Hardware:
    - LPC55S16-EVK
    - USB cable

Software and Tools:
    - Windows 10 platform, to execute batch scripts.
    - Keil µVision® IDE V5.29.0.0 or higher. http://www2.keil.com/mdk5
    - OpenSSL v1.1.1 or higher. https://slproweb.com/products/Win32OpenSSL.html
    - srec_cat.exe v1.64 or higher. Already installed in <SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools.
    - elftosb.exe v5.1.19 or higher. Already installed in <SDK>\in middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools.
    - blhost.exe v2.6.2 or higher. Already installed in <SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools.
    - ymltopfr.exe v1.0.0 or higher. Already installed in <SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools.
 
The template certificates, keys, PFR and other configuration files are in the "<SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\template" folder.

Please follow the step by step guide how to provision, to prepare and to upload secure firmware.

################################################################################
0. Connect device to the PC
################################################################################

    0.1 Connect Micro USB to the J1 connector on the device.
    0.2 Get port number:
    0.2.1 Open the Device Manager in the Windows 10.
    0.2.2 Find the "LPC-LinkII UCom Port (COMx)" port number in the "Ports(COM & LPT)" folder. Remember the used COM port number, you will need it for the step 2 and 3.
    0.3 Switch board to the In System Programming (ISP) mode:
    0.3.1 Press and hold the RESET button (Switch SW2) on the LPC55S1x development board.
    0.3.2 Press and hold the ISP button (Switch SW4) on the LPC55S1x development board.
    0.3.3 Release the RESET button (Switch SW2) but still hold the ISP button (Switch SW4).
    0.3.4 Release the ISP button (Switch SW4).
    0.3.5 The bootloader entered to the ISP mode.

################################################################################
1. Create Secure Binary
################################################################################

    1.1 Build Secure and Non-Secure Applications:
    
        1.1.1 Go to "../tfm_secure_boot_s/mdk".
        1.1.2 Open the "tfm_secure_boot.uvmpw" project file in the Keil µVision® IDE.
        1.1.3 Click on the "Batch Build" button in the "Build Toolbar". It starts building of tfm_secure_boot_s and tfm_secure_boot_ns projects.
        1.1.4 Wait until all builds processes are finished.
    
        NOTE:
             The output "tfm_secure_boot_s.hex" file is in the "../tfm_secure_boot_s/mdk/release" folder.
             The output "tfm_secure_boot_ns.hex" file is in the "../tfm_secure_boot_ns/mdk/release" folder.
			 
    1.2 Generate "Customer Filed Programmable Area" (CFPA) binary:
        1.2.1 Create CFPA configuration file:
            The .yml file content, input for the ymltopfr.exe tool:
                FFR:
                #CFPA
                    Header: "0x0"                    # Not used.
                    Version: "0x04"                  # CFPA version (previous version + 3. Response status = 132 (0x84) kStatus_FLASH_OutOfDateCfpaPage error will appear, if value of Version is smaller or equal to value already stored in the device.)
                    Secure_FW_Version: "0x01"        # Secure firmware version (Monotonic counter)
                    NS_FW_Version: "0x01"            # Non Secure firmware version (Monotonic counter)
                    IMAGE_KEY_REVOKE: "0x00"         # Image key revocation ID (Monotonic counter)
                    DBG_KEY_REVOKE: "0x0"            # Not used.
                    ROTKH_REVOKE: "0x55"             # 0x55 = Enable All RoTK keys
                    VENDOR_USAGE: "0x0"              # Vendor specific
                    DCFG_CC_SOCU_NS_PIN: "0x0"       # Not set. Defined by CMPA.
                    DCFG_CC_SOCU_NS_DFLT: "0x0"      # Not set. Defined by CMPA.
                    ENABLE_FA_MODE: "0x0"            # FA mode not set.
                    ZEROED1: "0x0"                   # Not used.
                    SHA256_1: "0x0"                  # Not used. Calculated by ROM.
                
                ###### OTHER PARAMETERS ######
                CMPA_INCLUDED: 0 # CMPA 0 is not included (only CFPA), 1 included
            
            The cfpa.yml example is in "<SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\template".
            
        1.2.2 Generate CFPA binary by means of "ymltopfr":
                ymltopfr.exe "<path to the *.yml file>" 
                
    1.3 Build CFPA Application:
    
        1.3.1 Go to "../tfm_cfpa/mdk".
        1.3.2 Open the "tfm_cfpa.uvmpw" project file in the Keil µVision® IDE.
        1.3.3 Click on the "Batch Build" button in the "Build Toolbar". It starts building of tfm_cfpa project.
        1.3.4 Wait until all builds processes are finished.
    
        NOTE:
             The output "tfm_cfpa.hex" file is in the "../tfm_cfpa/mdk/release" folder.
             This step is mandatory for very first load of Firmware (FW). It is optional for next FW updates.
    
    1.4 Prepare binaries:
       
        Use the "srec_cat.exe" application to merge the secure and non-secure applications, to align it to the PRINCE region end and finally to convert it to output binary file:
            "srec_cat <path to the Secure .hex file> -Intel -offset - -minimum-addr <path to the Secure .hex file> -Intel <path to the Non-Secure .hex file> -Intel -fill 0xAB -maximum-addr <path to the Non-Secure .hex file> -Intel <value of alignment> -o <path to the output .bin file> -Binary"

        Example of tfm_secure_boot for LPC55S16:
            srec_cat tfm_secure_boot_s.hex -Intel -offset - -minimum-addr tfm_secure_boot_s.hex -Intel tfm_secure_boot_ns.hex -Intel -fill 0xAB -maximum-addr tfm_secure_boot_ns.hex -Intel 0x38000 -o tfm_secure_boot.bin -Binary
        
            NOTE: *.hex files are stored in same folder where this command is executed.
                  TFM examples for LPC55S16 are aligned to 0x38000 (0x20000 belongs to secure code, 0x18000 belongs to non-secure code)

        Use the "srec_cat.exe" application to align tfm_cfpa.hex to the PRINCE region end and finally to convert it to output binary file:
           "srec_cat <path to the CFPA application .hex file> -Intel -fill 0xAB 0x30200 <value of alignment> -o <path to the output .bin file> -Binary"
		   
		 Example of tfm_cfpa for LPC55S16:
		    srec_cat tfm_cfpa.hex -Intel -fill 0xAB 0x20200 0x38000 -o tfm_cfpa.bin -Binary
			
            NOTE: *.hex file is stored in same folder where this command is executed.
                  TFM examples for LPC55S16 are aligned to 0x38000 (alignment is same for tfm_secure_boot and tfm_cfpa projects).
				  The CFPA table is located at address 0x20000.
    
    1.5 Sign the binary:
    
        LPC55Sxx supports booting of RSA2048 or RSA4096 signed images using RSASSA-PKCS1-v1_5 signature verification.

        The inputs to sign binary are following:
            - public key.
            - self-signed certificates.
            - json image configuration file.
            - binary (see section 1.4).
    
        1.5.1 Generate keys and certificates:
        
            1.5.1.1 Generate private keys:
     
                    Up to 4 private keys may be generated (2048 or 4096 bits):
             
                    - 2048 bits: 
                      openssl genrsa -out rotk1_2048.pem 2048
                    - 4096 bits:
                      openssl genrsa -out rotk1_4096.pem 4096
         
            1.5.1.2 Certification Signing Request:

                    After private keys are created (step 1.3.1.1), generate certification signing requests (.csr):
                        - 2048 bits: 
                          openssl req -new -key rotk1_2048.pem -out certificate_rotk1_2048.csr -extensions v3_ca
                        - 4096 bits:
                          openssl req -new -key rotk1_4096.pem -out certificate_rotk1_4096.csr -extensions v3_ca
                   
                    After execution of this command, the tool will ask user to provide an additional information.
                    A user can fill it or leave it empty:
                        Country Name: 
                        State or Province Name:
                        Locality Name: 
                        Organization Name: 
                        Organization Unit Name: 
                        Common Name: 
                        Email address:
                        Password:
                        Company Name:
         
            1.5.1.3 Create Self-Signed Certificates:
                    - 2048 bits:
                      openssl x509 -req -days 365 -in certificate_rotk1_2048.csr -signkey rotk1_2048.pem -sha256 -outform der -out certificate_rotk1_2048.der.crt -extfile x509_v3.ext -set_serial 0x3cc30000abababab
                    - 4096 bits:
                      openssl x509 -req -days 365 -in certificate_rotk1_4096.csr -signkey rotk1_4096.pem -sha256 -outform der -out certificate_rotk1_4096.der.crt -extfile x509_v3.ext -set_serial 0x3cc30000abababab

                    The x509_v3.ext file has to be created to follow x509v3_config -X509 V3 certificate extension configuration format.
                 
                    In this case, the file should contain following information (the *.ext file is stored in 'template' folder): 
                        authorityKeyIdentifier=keyid,issuer
                        basicConstraints=CA:FALSE
                        keyUsage = digitalSignature, nonRepudiation, keyEncipherment, dataEncipherment 
            
        1.5.2 Create image signing configuration file:
        
            The LPC55S16 .json image configuration file content, used by the signing tool:
                {
                    "family": "lpc55s1x",
                    "inputImageFile": "<path to the *.bin file>",
                    "imageLinkAddress": "<set the starting address of binary file - 0x0>",
                    "outputImageExecutionTarget": "<can be set only to "Internal flash (XIP)">",
                    "outputImageAuthenticationType": "<Two options, "CRC" and "Signed". Set always to "Signed">",
                    "enableTrustZone": "<TFM uses Trust Zone. Set always to true>",
                    "trustZonePresetFile": "<leave it clear "">",
                    "enableHwUserModeKeys": "<set it to false>",
                    "imageBuildNumber": "<in default set to "0x1">",
                    "rootCertificate0File": "<path to the certificate 0>",
                    "rootCertificate1File": "<path to the certificate 1>",
                    "rootCertificate2File": "<path to the certificate 2>",
                    "rootCertificate3File": "<path to the certificate 3>",
                    "mainCertChainId": "<select the main certificate file 0,1,2 or 3>",
                    "mainCertPrivateKeyFile": "<select the path to the main private key file according to selected main certificate file>",
                    "masterBootOutputFile": "<path to the output signed *.bin file>"
               } 
           
            The example of this file is in "<SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\template".
       
       1.5.3 Sign binary by means of "elftosb":
       
            elftosb.exe -f lpc55s1x -J "<path to the *.json file>"
			
			Outputs:
			   signed_tfm_secure_boot.bin or signed_tfm_cfpa.bin
        
    1.6. Prepare Secure Binaries:
    
        The elftosb.exe tool is used to create a secure binary (SB) files.
    
        1.6.1 Inputs
            - SBkek.txt : Secure Binary Key Encryption Key used for SB2 firmware update image decrypt (AES-256 symmetric key).
            - commandFile.bd : input command file. More information in (1.6.1.1).
            - main certificate private key .pem : This input must be the same like in "mainCertPrivateKeyFile" field of the .json file, see section (1.5.2).
            - main certificate .crt : This certificate corresponds to the root certificate of the chain defined by "mainCertChainId" in .json file, see section (1.5.2).
            - all certificates .crt: All certificates, see 1.5.1.2.
            
            1.6.1.1 Input command file structure (*.bd):
            
                The .bd file for secure_boot project has the following structure (it is "<SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\template"):
                    options {
                        flags = 0x8; // bd file format: 0x8 encrypted + signed (always 0x8)
                        buildNumber = 0x1; // not used
                        productVersion = "1.00.00"; // not used
                        componentVersion = "1.00.00"; // not used
                        secureBinaryVersion = "2.1";  // must be 2.1
                    }
                    sources {
                         mainImage = extern(0);
                    }
                    
                    /* Bootable section */
                    section (0;hmacEntries=20) {
                    
                        /* Check S and NS versions */
                        version_check sec 0x1;  // the version number must be equal or higher than Secure_FW_Version in CFPA
                        version_check nsec 0x1; // the version number must be equal or higher than NS_FW_Version in CFPA
                                                // when sb file is loaded to the device, the error Response status = 10115 (0x2783) kStatusRomLdrRollbackBlocked will appear in case that version number is smaller 
                    
                        erase 0x0..sizeof(mainImage);   /* 0x39000 reserved = 0x3800 (S+NS) + 0x1000 (SB_TAIL) */
                    
                        load mainImage > 0x0;
                    }
					
                The .bd file for cfpa project has the following structure (it is "<SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\template"):
                    options {
                        flags = 0x8; // bd file format: 0x8 encrypted + signed (always 0x8)
                        buildNumber = 0x1; // not used
                        productVersion = "1.00.00"; // not used
                        componentVersion = "1.00.00"; // not used
                        secureBinaryVersion = "2.1";  // must be 2.1
                    }
                    sources {
                         cfpaApp = extern(0);
                    }
                    
                    /* Bootable section */
                    section (0;hmacEntries=20) {
                    
                    
                        erase 0x0..sizeof(cfpaApp);   /* 0x39000 reserved = 0x3800 (S+NS) + 0x1000 (SB_TAIL) */
                    
                        load cfpaApp > 0x0;
                    }
    
        1.6.2 Generate secure binary
            In the command prompt execute:
                elftosb.exe -f lpc55S1x -k "<path to the SBkek.txt>" -c "<path to the commandFile.bd>" -o "<path to the tfm_secure_boot.sb2>" -s <path to the rotk1.pem> -S <path to the certificate_rotk1.der.crt> -R <path to the certificate_rotk1.der.crt> -R <path to the certificate_rotk2.der.crt> -R <path to the certificate_rotk3.der.crt> -R .<path to the certificate_rotk4.der.crt> -h "<path to the RHKT.bin>" "<path to the tfm_secure_boot_signed.bin>" "<path to the cfpa.bin>"
            Outputs:
                - RTKH.bin : root of trust keys table hash.
                - tfm_secure_boot.sb2 or tfm_cfpa.sb2 : secure binary 

################################################################################
2. Provision Protected Flash Region (PFR) (it is performed only once)
################################################################################

    2.1 Prepare Protected Flash Region (PFR) binary

        2.1.1 Create PFR configuration file:
            The .yml file content, input for the ymltopfr.exe tool:
                FFR:
                #CFPA                                                                    
                    Header: "0x0"                     # Not used.
                    Version: "0x01"                   # CFPA version. (Response status = 132 (0x84) kStatus_FLASH_OutOfDateCfpaPage error will appear, if value of Version is smaller or equal to value already stored in the device)
                    Secure_FW_Version: "0x00"         # Secure firmware version (Monotonic counter)
                    NS_FW_Version: "0x00"             # Non Secure firmware version (Monotonic counter)
                    IMAGE_KEY_REVOKE: "0x00"          # Image key revocation ID (Monotonic counter)
                    DBG_KEY_REVOKE: "0x0"             # Not used.
                    ROTKH_REVOKE: "0x55"              # 0x55 = Enable All RoTK keys
                    VENDOR_USAGE: "0x0"               # Vendor specific
                    DCFG_CC_SOCU_NS_PIN: "0x0"        # Not set. Defined by CMPA.
                    DCFG_CC_SOCU_NS_DFLT: "0x0"       # Not set. Defined by CMPA.
                    ENABLE_FA_MODE: "0x0"             # FA mode not set.
                    ZEROED1: "0x0"                    # Not used.
                    SHA256_1: "0x0"                   # Not used. Calculated by ROM.
                
                #CMPA
                    BOOT_CFG: "0x20"                  # Default ISP mode = b010 (The UART is used to download the image).
                    SPI_FLASH_CFG: "0x0"              # 0 = SPI Recovery mode is disabled.
                    USB_ID: "0x0"                     # USB is not used
                    SDIO_CFG: "0x0"                   # Not Used
                    DCFG_CC_SOCU_PIN: "0x03FF"        # All b1 = Lock DCFG_CC_SOCU_DFLT domains.
                    DCFG_CC_SOCU_DFLT: "0x03FF"       # All b1 = Access to the sub-domain is permanently disabled and can’t be reversed. This setting offers the highest level of restriction.
                    DAP_VENDOR_USAGE_FIXED: "0x0"     # Vendor-specific
                    SECURE_BOOT_CFG: "0x40541241"     # SEC_BOOT_EN = b01 (Boot signed images. (internal flash, RSA signed))
                                                      # BOOT_SEED_INC_EPOCH = b01 (Include security epoch area in BOOT_SEED computation)
                                                      # BOOT_SEED_CUST_CFG = b01 (Include CMPA area in BOOT SEED computation)
                                                      # BOOT_SEED_INC_NXP_CFG = b01 (Include NXP area in BOOT SEED computation).
                                                      # SKIP_BOOT_SEED = b00 (enable BOOT_SEED)
                                                      # DICE_INC_SEC_EPOCH = b00 (DICE computation is not included)
                                                      # BLOCK_ENROLL = b01 (Disable PUF enroll operation)
                                                      # BLOCK_SET_KEY = b00 (Allow PUF Key Code generation)
                                                      # TZM_IMAGE_TYPE = b10 (TZ-M enabled image, boots to secure mode)
                                                      # SKIP_DICE = b01 (Disable DICE)
                                                      # DICE_CUST_CFG = b00 (Not included CFPA page and key store area in DICE computation)
                                                      # DICE_INC_NXP_CFG = b00 (Not included NXP area in DICE computation)
                                                      # RSA4K = b01 (RSA4096 only)
                    PRINCE_BASE_ADDR: "0x0"           # ADDR0_PRG = 0x0 (Programmable portion of the base address of region 0)
                    PRINCE_SR_0: "0x0FFFFFFF"         # SRn_EN = 224KB (Each bit in this field enables a sub-region of crypto region x at offset 8kB*n)
                    PRINCE_SR_1: "0x0"                # Not used
                    PRINCE_SR_2: "0x0"                # Not Used
                    ZEROED4: "0x0"                    # Not Used
                    ROTK: "0x0"                       # RKTH is a 32 byte SHA-256 hash of SHA-256 hashes of up to four root public keys. Added by script from ROTKH_FILES.
                    ZEROED5: "0x0"                    # Not Used
                    HUK: "0x000102030405060708090A0B0C0D0E0F"    # TFM/PSA HUK 
                    ATTESTATION_KEY: "0xA9B454B26D6F90A4EA31193564CBA91FEC6F9A002A7DC0504B92A1937134585F" # TFM/PSA Attestation key
                    ZEROED6: "0x0"                    # Not Used
                    SHA256_2: "0x0"                   # Calculated by script.
                
                ###### OTHER PARAMETERS ######
                ROTKH_FILES:                          #Keys used for RKTH calculation
                    - '../template/rotk1.pem'
                    - '../template/rotk2.pem'
                    - '../template/rotk3.pem'
                    - '../template/rotk4.pem'
                PROTOCOL_VERSION: 1                   # 0 for 2048, 1 for 4096
                CMPA_INCLUDED: 1                      # CMPA 0 is not included (only CFPA), 1 included
                SEAL: 0                               # 0 do not seal CMPA, 1 seal
                
            NOTE:    
                - The "allPFR.yml" example is in "<SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\template".
                - By default, the CFPA is clear. But ROTKH_REVOKE field at CFPA page address 0x3DE18 must be set to accept signed images with created certificates. See LPC55S1x_LPC551x_Protected Flash Region v1.1.xlsx in attachments in UM11295.pdf reference manual.
                - The initial CFPA does not contain inverse values of "vendor usage", "DCFG_CC_SOCU_NS_PIN" and "DCFG_CC_SOCU_NS_DFLT".
                - The CMPA page is configured to boot the signed image.
                
        2.1.2 Generate CFPA and CMPA binaries by means of "ymltopfr":
                ymltopfr.exe "<path to the *.yml file>" 

    2.2 Provision Protected Flash Region (PFR):
    
        2.2.1 Erase all flash        
            blhost -p COMxx -- flash-erase-all 0

        2.2.2 Initialize and Provision Key Store :
            The SBKEK is AES-256 symmetric key (32 bytes), CTR mode.
            The PRINCE encryption/decryption keys are generated in the device using on-chip SRAM PUF and they are delivered to the PRINCE engine through internal hardware bus.            
            The proper Key Store needs to be initialized via the "blhost" application:
                - generate device activation code and store it into key store structure:
                    blhost -p COMxx -- key-provisioning enroll
                - install SBKEK into key store, key type = 3. It must be in binary format (The example of SBkek_PUF.bin is in "<SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\template"):
                    blhost -p COMxx -- key-provisioning set_user_key 3 SBkek_PUF.bin
                - generate random PRINCE region 0, key type = 7:
                    blhost -p COMxx -- key-provisioning set_key 7 16
                - save the key store into PFR page of Flash memory:
                    blhost -p COMxx -- key-provisioning write_key_nonvolatile 0
              
        2.2.3 Load CFPA table into flash memory:
            blhost -p COMxx -- write-memory 0x3DE00 <path to the cfpaInit(.bin)>

        2.2.4 Load CMPA table into flash memory:
            blhost -p COMxx -- write-memory 0x3E400 <path to the CMPA(.bin)>
			
		2.2.5 PRINCE region configuration
		
		    - region selection
			    blhost -p COMxx -- fill-memory 0x20008000 4 0x50000000
			- start address of encrypted area
			    blhost -p COMxx -- fill-memory 0x20008004 4 0x0
			- length of encrypted area
			    blhost -p COMxx -- fill-memory 0x20008008 4 0x38000
			- call configure-memory with prepared structure in RAM
			    blhost -p COMxx -- configure-memory 0 0x20008000
		
			NOTE: For PRINCE encryption and decryption the regions and sub-regions needs to be configured. 
				- Structure of configure-memory command:
				   ********************************************
				   *  Offset  *  Size  *      Description     *
				   ********************************************
				   *  0       *  4     * PRINCE configuration *
				   ********************************************
				   *  4       *  8     * PRINCE Region Info   *
				   ********************************************
				- PRINCE configuration register for configure-memory command:
				   ********************************************
				   *  Bit  *               Symbol             *
				   ********************************************
				   * 1:0   *     0x00 - PRINCE Region 0       *
				   *       *     0x01 - PRINCE Region 1       *
				   *       *     0x10 - PRINCE Region 2       *
				   ********************************************
				   * 25:2  *             Reserved             *
				   ********************************************
				   * 31:8  *  0x50 ('P') - Configure PRINCE   *
				   ********************************************
				- PRINCE region info register for configure-memory command:
				   ********************************************
				   *  Bit  *               Symbol             *
				   ********************************************
				   * 31:0  *     PRINCE Region X Start        *
				   ********************************************
				   * 63:32 *     PRINCE Region X Size         *
				   ********************************************

		2.2.6 Device reset
            blhost -p COMxx -- reset		

################################################################################
3. Update Firmware (Secure Binary)
################################################################################

    Load Secure Binary to the device (without CFPA table):
        blhost -p COMxx -- receive-sb-file "<path to the tfm_secure_boot.sb2>"

    When CFPA table must be updated, a user must follow these instuctions:
        - load cfpa application secure binary file to the device
		    blhost -P COMxx -- receive-sb-file "<path to the tfm_cfpa.sb2>"
        - reset device
            blhost -p COMxx -- reset
        - erase all flash        
            blhost -p COMxx -- flash-erase-all 0
        - load tfm application secure binary file to the device
            blhost -p COMxx -- receive-sb-file "<path to the tfm_secure_boot.sb2>"
            
################################################################################
4. Restore CMPA and CFPA tables (only for soft profile)
################################################################################

   4.1 Read CFPA table from device:
        blhost -p COMxx -- read-memory 0x3DE00 512 <path to the CFPA(.bin)>
   4.2 Delete all values except of version number and ROTKH_REVOKE in downloaded CFPA table
   4.3 Increment version number in downloaded CFPA table
   4.4 Load updated CFPA table
        blhost -p COMxx -- write-memory 0x3DE00 <path to the CFPA(.bin)>
   4.5 Load CMPA table filled by all zeros
        blhost -p COMxx -- write-memory 0x3E400 <path to the CMPA(.bin)>
   4.6 Reset Device
        blhost -p COMxx -- reset   

   NOTE:
        To be able to use device without secure boot, this script must be executed			

* Copyright (c) 2020, NXP. All rights reserved. * 