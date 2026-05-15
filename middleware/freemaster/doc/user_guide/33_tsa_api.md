# TSA Tables

When the TSA is enabled in the FreeMASTER driver configuration file (by setting the FMSTR_USE_TSA macro to a non-zero value), it defines the so-called TSA tables in the application. This section describes the macros that must to be used to define the TSA tables.

There can be any number of TSA tables spread across the application source files. There must be always exactly one TSA Table List defined, which informs the FreeMASTER driver about the active TSA tables.

When there is at least one TSA table and one TSA Table List defined in the application, the TSA information automatically appears in the FreeMASTER symbols list. The symbols can then be used to create FreeMASTER variables for visualization or control.


```{include} ./tsa/tsa_table_definition.md
:heading-offset: 2
```

```{include} ./tsa/tsa_table_list.md
:heading-offset: 2
```

```{include} ./tsa/tsa_active_content.md
:heading-offset: 2
```

# TSA API

```{include} ./api/fmstr_setuptsabuff.md
:heading-offset: 2
```

```{include} ./api/fmstr_tsaaddvar.md
:heading-offset: 2
```
