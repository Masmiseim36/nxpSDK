# Serial recovery

Serial recovery offers a backup way to restore a corrupted image on a device or an alternative way to update an image for the single application slot upgrade mode.
The option is enabled with `CONFIG_BOOT_SERIAL_RECOVERY` on some boards by default. 

In OTA examples, the bootloader automatically falls into serial recovery if there is no valid image to boot. Alternatively, the mode can be entered with a specific button pressed during reset.

For more information see [MCUboot's serial recovery documentation](https://docs.mcuboot.com/serial_recovery.html)

**Encrypted image support**

Serial recovery is not fully supported in the current MCUboot version when using Encrypted XIP mode. However, MCUboot encrypted images can still be used as OTA containers. To enable this functionality, switch the crypto backend to either the mbedTLS legacy API (PSA is not supported) or TinyCrypt, and add the following configuration to your `mcuboot_config.h` file.

~~~
CONFIG_BOOT_ENCRYPT_EC256

//crypto backend
CONFIG_BOOT_USE_MBEDTLS
or
CONFIG_BOOT_USE_TINYCRYPT
~~~

## MCU Manager usage

There are several implementations of the MCUmgr. The feature was tested with [mynewt-mcumgr](https://github.com/apache/mynewt-mcumgr/tree/master) (cli) and [AuTerm](https://github.com/thedjnK/AuTerm) (gui).

### Command examples

Setting a connection profile:
~~~
mcumgr conn add serial_1 type="serial" connstring="COM4,baud=115200"

Connection profile serial_1 successfully added
~~~

Echo command:
~~~
mcumgr -c serial echo Test

Test
~~~

Image upload:
~~~
mcumgr -c serial image upload .\signed__ota_mcuboot_basic.bin

31.82 KiB / 31.82 KiB [=======================================================================] 100.00% 2.79 KiB/s 11s
Done
~~~

Image list:
~~~
mcumgr -c serial_1 image list

Images:
 image=0 slot=0
    version: 1.0.3.1000
    bootable: false
    flags:
    hash: Unavailable
Split status: N/A (0)
~~~

Reset:
~~~
mcumgr -c serial_1 reset

Done
~~~