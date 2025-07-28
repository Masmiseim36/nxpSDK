# Introduction

SBTSNOOP stands for SERIAL BTSNOOP.
It's a debug module used to log HCI traffic in a BTSNOOP file format in RAM and allows to dump the file content over a serial interface.

BTSNOOP is a standard format supported by various networking softwares like Wireshark. It gives information on every packet logged, like the timestamps, the data, the source, the destination...

The module comes with the C sources to be compiled within your project.
Also, it comes with various PORT files used to port the module over different Host stacks and SDKs.

# How To

## Compile the sources

1. Add the following files to your compilation:
   * sbtsnoop.c
   * sbtsnoop.h
   * sbtsnoop_port.h
   * sbtsnoop_port_<desired SDK/Host>.h
2. Define following macros in your compile options:
   * SERIAL_BTSNOOP -> this macro can be used to enable/disable calls to sbtsnoop APIs
   * SBTSNOOP_PORT_FILE=sbtsnoop_port_<desired SDK/HOST>.h -> this macro is used by sbtsnoop_port.h to include desired port file

## Use SBTSNOOP runtime

SBTSNOOP represents the BTSNOOP log as a file, so the API provides open/close/read/write operations.

Usually, the supported HCI transport layers already implements the HCI logging (using sbtsnoop_write_hci_pkt API under SERIAL_BTSNOOP macro).

The user must add some calls to sbtsnoop_fopen and sbtsnoop_fclose in strategic places that define his test case. For example, if the user wants to debug a connection, he can call sbtsnoop_fopen at connection establishment event and sbtsnoop_fclose at disconnection event. Note: Calling sbtsnoop_fopen twice will call sbtsnoop_fclose in between, so make sure to take that into account when debugging multiple connections.

The API also provides a menu in which the user can chose to perform the different operations in an interactive way, this can be handy in test applications, or shells.

For more details on the API, please check sbtsnoop.h.

The data are printed to the serial port in ASCII format. This should be parsed and converted to a binary file so it can be open by capture tools like Wireshark.
