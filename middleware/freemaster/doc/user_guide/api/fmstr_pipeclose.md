# FMSTR_PipeClose

##### Prototype

```c
void FMSTR_PipeClose(FMSTR_HPIPE pipeHandle);
```

- Declaration: *freemaster.h*
- Implementation: *freemaster_pipes.c*

##### Arguments

 - *pipeHandle* [in] - pipe handle returned from the FMSTR_PipeOpen function call

##### Description

This function de-initializes the pipe object. No data can be received or sent on the pipe after this call.

