# API data types

This section describes the data types used in the FreeMASTER driver. The information provided here can be useful when modifying or porting the FreeMASTER  Communication Driver to new NXP platforms.

**Note:** The licensing conditions prohibit use of FreeMASTER and the FreeMASTER Communication Driver with non-NXP MPU or MCU products.

## Public common types

The table below describes the public data types used in the FreeMASTER driver API calls. The data types are declared in the *freemaster.h* header file.

```{list-table}
:header-rows: 1
:widths: 10 60
 * - Type name
   - Description

 * - *FMSTR_ADDR*
   - Data type used to hold the memory address. On most platforms, this is normally a C-pointer, but it may also be a pure integer type.  
     For example, this type is defined as long integer on the 56F8xxx platform where the 24-bit addresses must be supported, but the C-pointer may be only 16 bits wide in some compiler configurations.

 * - *FMSTR_SIZE*
   - Data type used to hold the memory block size.  
     It is required that this type is unsigned and at least 16 bits wide integer.

 * - *FMSTR_BOOL* 
   - Data type used as a general boolean type.  
     This type is used only in zero/non-zero conditions in the driver code.

 * - *FMSTR_APPCMD_CODE*
   - Data type used to hold the Application Command code.  
     Generally, this is an unsigned 8-bit value.

 * - *FMSTR_APPCMD_DATA*
   - Data type used to create the Application Command data buffer.  
     Generally, this is an unsigned 8-bit value.

 * - *FMSTR_APPCMD_RESULT*
   - Data type used to hold the Application Command result code.  
     Generally, this is an unsigned 8-bit value.

 * - *FMSTR_PAPPCMDFUNC*
   - Pointer to the Application Command handler function.  
     See [FMSTR_RegisterAppCmdCall](fmstr-registerappcmdcall) for more details.
```

## Public TSA types

The table describes the TSA-specific public data types. These types are declared in the *freemaster_tsa.h* header file, which is included in the user application indirectly by the *freemaster.h* file.

```{list-table}
:header-rows: 0
:widths: 10 60
 * - *FMSTR_TSA_TINDEX*
   - Data type used to hold a descriptor index in the TSA table or a table index in the list of TSA tables.  
     By default, this is defined as FMSTR_SIZE.

 * - *FMSTR_TSA_TSIZE*
   - Data type used to hold a memory block size, as used in the TSA descriptors.  
     By default, this is defined as FMSTR_SIZE.
```

## Public Pipes types

The table describes the data types used by the FreeMASTER Pipes API:

```{list-table}
:header-rows: 0
:widths: 10 60
 * - *FMSTR_HPIPE*
   - Pipe handle that identifies the open-pipe object.  
     Generally, this is a pointer to a void type.

 * - *FMSTR_PIPE_PORT*
   - Integer type required to hold at least 7 bits of data.  
     Generally, this is an unsigned 8-bit or 16-bit type.

 * - *FMSTR_PIPE_SIZE*
   - Integer type required to hold at least 16 bits of data.  
     This is used to store the data buffer sizes.

 * - *FMSTR_PPIPEFUNC*
   - Pointer to the pipe handler function.  
     See [FMSTR_PipeOpen](fmstr-pipeopen) for more details.
```

## Internal types

The table describes the data types used internally by the FreeMASTER driver. The data types are declared in the platform-specific header file and they are not available in the application code.

```{list-table}
:header-rows: 0
:widths: 10 60
 * - *FMSTR_U8*
   - The smallest memory entity.  
     On the vast majority of platforms, this is an unsigned 8-bit integer.  
     On the 56F8xx DSP platform, this is defined as an unsigned 16-bit integer.

 * - *FMSTR_U16*
   - Unsigned 16-bit integer.

 * - *FMSTR_U32*
   - Unsigned 32-bit integer.

 * - *FMSTR_S8*
   - Signed 8-bit integer.

 * - *FMSTR_S16*
   - Signed 16-bit integer.

 * - *FMSTR_S32*
   - Signed 32-bit integer.

 * - *FMSTR_FLOAT*
   - 4-byte standard IEEE floating-point type.

 * - *FMSTR_FLAGS*
   - Data type forming a union with a structure of flag bit-fields.

 * - *FMSTR_SIZE8*
   - Data type holding a general size value, at least 8 bits wide.

 * - *FMSTR_INDEX*
   - General for-loop index. Must be signed, at least 16 bits wide.

 * - *FMSTR_BCHR*
   - A single character in the communication buffer.  
     Typically, this is an 8-bit unsigned integer, except for the DSP platforms where it is a 16-bit integer.

 * - *FMSTR_BPTR*
   - A pointer to the communication buffer (an array of FMSTR_BCHR).
```
