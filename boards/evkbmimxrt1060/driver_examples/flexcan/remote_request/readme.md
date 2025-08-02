# flexcan_remote_request

## Overview
The flexcan_remote_request example shows how to use remote request feature by SDK driver.

In this example, 2 boards are connected through CAN bus.

Board A send a remote frame to request message from board B.  
After successful request, board A will receive response message with same mailbox as transmit mailbox.

Board B receive remote frame and send response message automatically, but do not save remote frame.  
After successful response, board B will update response message and wait for next remote request.


## Supported Boards
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/flexcan/remote_request/example_board_readme.md)
