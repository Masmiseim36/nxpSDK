
/**
 *  \file xml_common_defines.h
 *
 *  This file contains defintions common to the xml parser and encoder
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_XML_COMMON_DEFINES_
#define _H_XML_COMMON_DEFINES_

/* ------------------------------------------ Header File Inclusion */
#include "BT_common.h"
#include "BT_fops.h"

/* ------------------------------------------ Data Structures/Data Types */
typedef struct _ATTRIB_VAL
{
    CHAR    attrib[ 25 ];

    CHAR    value[ 50 ];

} ATTRIB_VAL;


typedef struct xml_doc_component
{
    CHAR                        element_name[ 50 ];

    INT32                       no_of_attribs;

    ATTRIB_VAL                  list [ XML_MAX_NO_OF_ATTRIBUTES ];

    CHAR                        content [ XML_MAX_TOKEN_SIZE ];

    INT32                       no_of_sub_elems;

    struct  xml_doc_component*  parent_elem;

    struct  xml_doc_component*  sub_elems [ XML_MAX_SUB_ELEMENTS ];

} XML_DOC_COMPONENT, *PXML_DOC_COMPONENT;


typedef struct xml_object
{
    CHAR                 *xml_doc_bfr;

    CHAR                 file [256U];

    BOOLEAN              use_buffer;

    PXML_DOC_COMPONENT   pStart;

    UCHAR                command [ XML_MAX_COMMANDS ][ XML_MAX_TOKEN_SIZE ];

    UCHAR                no_of_commands;

} XML_PARSER_OBJECT, XML_ENCODER_OBJECT;

#endif /* _H_XML_COMMON_DEFINES_ */

