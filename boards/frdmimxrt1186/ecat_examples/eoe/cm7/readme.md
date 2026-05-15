# ecat_eoe

## Overview
This example demonstrates how to use EtherCAT EoE based on digital_io example.

Note that the demo CAN NOT be built directly. Please see the "Prepare the Demo" section for more details.

For more information on digital_io example, please refer to '...\ecat_examples\digital_io'


## Running the demo
When the example is running, the serial port will output:

    Start the SSC EoE example...

    Hardware init success...

    EoE interface init success...

    ***********************************************************
    HTTP Server example
    ***********************************************************
    IPv4 Address     : 192.168.1.66

    IPv4 Subnet mask : 255.255.255.0

    IPv4 Gateway     : 192.168.1.102

    mDNS hostname    : lwip-eoe
    ***********************************************************

NOTE: To modify the html of http server, please refer to lwip_examples/lwip_httpsrv example.

## TwinCAT Setup

To demonstrate this EoE example, TwinCAT is used as the EtherCAT MainDevice.
- [Generate SSC Code for EoE](../topics/generate_ssc_for_eoe.md)
- [TwinCAT Installation](../topics/download_and_install_the_twincat_software.md)
- [TwinCAT EEPROM Update](../topics/twincat_eeprom_update_eoe.md)
- [TwinCAT Project Setup](../topics/twincat_project_setup_eoe.md)
- [Note](../topics/note_about_the_source_code_in_the_document.md)

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ecat_examples/eoe/example_board_readme.md)
- [FRDM-MIMXRT1186](../../_boards/frdmimxrt1186/ecat_examples/eoe/example_board_readme.md)
- [i.MX943-EVK](../../_boards/imx943evk/ecat_examples/eoe/example_board_readme.md)
