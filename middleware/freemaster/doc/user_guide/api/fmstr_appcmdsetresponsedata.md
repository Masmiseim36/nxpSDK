# FMSTR_AppCmdSetResponseData

##### Prototype

```c
void FMSTR_AppCmdSetResponseData(FMSTR_ADDR resultDataAddr, FMSTR_SIZE resultDataLen);
```

- Declaration: *freemaster.h*
- Implementation: *freemaster_appcmd.c*

##### Arguments

 - *resultDataAddr* [in] - pointer to the data buffer that is to be copied to the Application Command data buffer
 - *resultDataLen*  [in] - length of the data to be copied. It must not exceed the FMSTR_APPCMD_BUFF_SIZE value.

##### Description

This function can be used before the Application Command processing finishes, when there are data to be returned back to the PC.

The response data buffer is copied into the Application Command data buffer, from where it is accessed when the host requires it. Do not use FMSTR_GetAppCmdData and the data buffer after FMSTR_AppCmdSetResponseData is called.

**Note:** The current version of FreeMASTER does not support the Application Command response data.
