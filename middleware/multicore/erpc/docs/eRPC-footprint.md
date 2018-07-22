This page shows a footprint of the eRPC code used in the eRPC Matrix multiply example. It's role is multiply two matrices and return a result. A transport is used for UART.

IDL:
```C
/*!
 * You can write copyrights rules here. Will be copied into outputs.
 */
@outputDir("erpc_outputs") // output directory
program erpc_matrix_multiply; // specify name of output files

/*! This const defines the matrix size. The value has to be the same as the
    Matrix array dimension. Do not forget to re-generate the erpc code once the
    matrix size is changed in the erpc file */
int const matrix_size = 5;

/*! This is the matrix array type. The dimension has to be the same as the
    matrix size const. Do not forget to re-generate the erpc code once the
    matrix size is changed in the erpc file */
type Matrix = int32[matrix_size][matrix_size];

interface MatrixMultiplyService // cover functions for same topic
{
    erpcMatrixMultiply(in Matrix matrix1, in Matrix matrix2, out Matrix result_matrix) -> void
}
```

Footprint - release target, error checking code in shim code is enabled:
 - client - shim code file: _erpc_matrix_multiply_client.o_

file name | ro code | ro data |rw data
---|---|---|---
basic_codec.o                   |   194   |   200   |     0
client_manager.o                |   260   |    32   |     0
crc16.o                         |    48   |     0   |     0
erpc_client_setup.o             |   164   |    24   |    48
erpc_matrix_multiply_client.o   |   412   |     8   |     0
erpc_port_stdlib.o              |    16   |     0   |     0
erpc_setup_mbf_dynamic.o        |    74   |    32   |     8
erpc_setup_uart_cmsis.o         |    28   |     0   |    16
framed_transport.o              |   136   |     0   |     0
message_buffer.o                |    98   |     0   |     0
uart_cmsis_transport.o          |   152   |    44   |     2
---|---|---|---
**Total:**                          |  **1582**   |   **340**   |    **74**


 - server - shim code file: _erpc_matrix_multiply_server.o_

file name | ro code | ro data |rw data
---|---|---|---
basic_codec.o                  |    194   |   200    |    0
crc16.o                        |     48   |     0    |    0
erpc_matrix_multiply_server.o  |    432   |    28    |    0
erpc_port_stdlib.o             |     16   |     0    |    0
erpc_server_setup.o            |    184   |    24    |   48
erpc_setup_mbf_dynamic.o       |     74   |    32    |    8
erpc_setup_uart_cmsis.o        |     28   |     0    |   16
framed_transport.o             |    136   |     0    |    0
message_buffer.o               |    108   |     0    |    0
server.o                       |    120   |     0    |    0
simple_server.o                |    240   |    36    |    0
uart_cmsis_transport.o         |    156   |    44    |    2
---|---|---|---
**Total:**                         |   **1736**   |   **364**    |   **74**
