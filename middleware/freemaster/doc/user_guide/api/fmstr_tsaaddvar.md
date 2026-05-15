# FMSTR_TsaAddVar

##### Prototype

```c
FMSTR_BOOL FMSTR_TsaAddVar(FMSTR_TSATBL_STRPTR tsaName, FMSTR_TSATBL_STRPTR tsaType, 
	FMSTR_TSATBL_VOIDPTR varAddr, FMSTR_SIZE32 varSize, 
	FMSTR_SIZE flags);
```

- Declaration: *freemaster.h*
- Implementation: *freemaster_tsa.c*

##### Arguments

 - *tsaName* [in] - name of the object
 - *tsaType* [in] - name of the object type
 - *varAddr* [in] - address of the object
 - *varSize* [in] - size of the object
 - *flags* [in] - access flags; a combination of these values:
    - *FMSTR_TSA_INFO_RO_VAR* — read-only memory-mapped object (typically a variable)
    - *FMSTR_TSA_INFO_RW_VAR* — read/write memory-mapped object
    - *FMSTR_TSA_INFO_NON_VAR* — other entry, describing structure types, structure members, enumerations, and other types

##### Description

This function can be called only when the dynamic TSA table is enabled by the FMSTR_USE_TSA_DYNAMIC configuration option and when the FMSTR_SetUpTsaBuff function call is made to assign the dynamic TSA table memory. This function adds an entry into the dynamic TSA table. It can be used to register a read-only or read/write memory object or describe an item of the user-defined type.

See [TSA table definition](tsa-table-definition) for more details about the TSA table entries.
