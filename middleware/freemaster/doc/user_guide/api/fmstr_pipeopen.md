# FMSTR_PipeOpen

##### Prototype

```c
FMSTR_HPIPE FMSTR_PipeOpen(FMSTR_PIPE_PORT pipePort, FMSTR_PPIPEFUNC pipeCallback, 
	FMSTR_ADDR pipeRxBuff, FMSTR_PIPE_SIZE pipeRxSize, 
	FMSTR_ADDR pipeTxBuff, FMSTR_PIPE_SIZE pipeTxSize, 
	FMSTR_U8 type, const FMSTR_CHAR *name);
```

- Declaration: *freemaster.h*
- Implementation: *freemaster_pipes.c*

##### Arguments

 - *pipePort* [in] - port number that identifies the pipe for the client
 - *pipeCallback* [in] - pointer to the callback function that is called whenever a pipe data status changes
 - *pipeRxBuff* [in] - address of the receive memory buffer
 - *pipeRxSize* [in] - size of the receive memory buffer
 - *pipeTxBuff* [in] - address of the transmit memory buffer
 - *pipeTxSize* [in] - size of the transmit memory buffer
 - *type* [in] - a combination of FMSTR_PIPE_MODE_xxx and FMSTR_PIPE_SIZE_xxx constants describing primary pipe data format and usage. This type helps FreeMASTER decide how to access the pipe by default. Optional, use 0 when undetermined.
 - *name* [in] - user name of the pipe port. This name is visible to the FreeMASTER user when creating the graphical pipe interface.

##### Description

This function initializes a new pipe and makes it ready to accept or send the data to the PC Host client. The receive memory buffer is used to store the received data before they are read out by the FMSTR_PipeRead call. When this buffer gets full, the PC Host client denies the data transmission into this pipe until there is enough free space again. The transmit memory buffer is used to store the data transmitted by the application to the PC Host client using the FMSTR_PipeWrite call. The transmit buffer can get full when the PC Host is disconnected or when it is slow in receiving and reading out the pipe data.

The function returns the pipe handle which must be stored and used in the subsequent calls to manage the pipe object.

The callback function (if specified) is called whenever new data are received through the pipe and available for reading. This callback is also called when the data waiting in the transmit buffer are successfully pushed to the PC Host and the transmit buffer free space increases. The prototype of the callback function provided by the user application must be as follows. The *PipeHandler*  name is only a placeholder and must be defined by the application.

```c
void PipeHandler(FMSTR_HPIPE pipeHandle);
```