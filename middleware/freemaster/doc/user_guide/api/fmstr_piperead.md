# FMSTR_PipeRead

##### Prototype

```c
FMSTR_PIPE_SIZE FMSTR_PipeRead(FMSTR_HPIPE pipeHandle, FMSTR_ADDR pipeData, 
	FMSTR_PIPE_SIZE pipeDataLen, FMSTR_PIPE_SIZE readGranularity);
```

- Declaration: *freemaster.h*
- Implementation: *freemaster_pipes.c*

##### Arguments

 - *pipeHandle* [in] - pipe handle returned from the FMSTR_PipeOpen function call
 - *pipeData* [in] - address of the data buffer to be filled with the received data
 - *pipeDataLen* [in] - length of the data to be read
 - *readGranularity* [in] - size of the minimum unit of data which is to be read

##### Description

This function copies the data received from the pipe from its receive buffer to the user buffer for further processing. The function returns the number of bytes that were successfully copied to the buffer. This number may be smaller than the number of the requested bytes if there is not enough data bytes available in the receive buffer.

The readGranularity argument can be used to copy the data in larger chunks in the same way as described in the FMSTR_PipeWrite function.
