# TSA table definition

The TSA table describes the static or global variables together with their address, size, type, and access-protection information. If the TSA-described variables are of a structure type, the TSA table may also describe this type and provide an access to the individual structure members of the variable.

The TSA table definition begins with the FMSTR_TSA_TABLE_BEGIN macro with a *table_id* identifying the table. The *table_id* shall be a valid C-langiage symbol.

```c
FMSTR_TSA_TABLE_BEGIN(table_id)
```


After this opening macro, the TSA descriptors are placed using these macros:

```c
/* Adding variable descriptors */
FMSTR_TSA_RW_VAR(name, type)  /* read/write variable entry */
FMSTR_TSA_RO_VAR(name, type)  /* read-only variable entry */

/* Description of complex data types */
FMSTR_TSA_STRUCT(struct_name) /* structure or union type entry */
FMSTR_TSA_MEMBER(struct_name, member_name, type)  /* structure member entry */

/* Memory blocks */
FMSTR_TSA_RW_MEM(name, type, address, size) /* read/write memory block */
FMSTR_TSA_RO_MEM(name, type, address, size) /* read-only memory block */
```

The table is closed using the FMSTR_TSA_TABLE_END macro:

```c
FMSTR_TSA_TABLE_END()
```

## TSA descriptor parameters

The TSA descriptor macros accept these parameters:

- *name* — variable name. The variable must be defined before the TSA descriptor references it.
- *type* — variable or member type. Only one of the pre-defined type constants may be used (see below).
- *struct_name* — structure type name. The type must be defined (typedef) before the TSA descriptor references it.
- *member_name* — structure member name. 

**Note:** The structure member descriptors (FMSTR_TSA_MEMBER) must immediately follow the parent structure descriptor (FMSTR_TSA_STRUCT) in the table.

**Note:** To write-protect the variables in the FreeMASTER driver (FMSTR_TSA_RO_VAR), enable the TSA-Safety feature in the configuration file.

## TSA variable types

The table lists *type* identifiers which can be used in TSA descriptors:

| Constant                   | Description                                                    |
|----------------------------|----------------------------------------------------------------|
| FMSTR_TSA_UINT*n*          | Unsigned integer type of size *n* bits (n=8,16,32,64)          |
| FMSTR_TSA_SINT*n*          | Signed integer type of size *n* bits (n=8,16,32,64)            |
| FMSTR_TSA_FRAC*n*          | Fractional number of size *n* bits (n=16,32,64).               |
| FMSTR_TSA_FRAC_Q(*m,n*)    | Signed fractional number in general Q form (m+n+1 total bits)  |
| FMSTR_TSA_FRAC_UQ(*m,n*)   | Unsigned fractional number in general UQ form (m+n total bits) |
| FMSTR_TSA_FLOAT            | 4-byte standard IEEE floating-point type                       |
| FMSTR_TSA_DOUBLE           | 8-byte standard IEEE floating-point type                       |
| FMSTR_TSA_POINTER          | Generic pointer type defined (platform-specific 16 or 32 bit)  |
| FMSTR_TSA_USERTYPE(*name*) | Structure or union type declared with FMSTR_TSA_STRUCT record  |
