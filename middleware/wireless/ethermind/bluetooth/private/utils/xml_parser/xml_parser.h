
/**
 *  \file xml_parser.h
 *
 *  This file contains APIs of the XML parser
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_XML_PARSER_
#define _H_XML_PARSER_

/* ------------------------------------------ Header File Inclusion */
#include "xml_common_defines.h"

/* ------------------------------------------ Function Declarations */
/*
 * main parse function which takes a string to parse and
 * returns the data in a structure
 */
#ifdef __cplusplus
extern "C" {
#endif
API_RESULT    xml_parser( XML_PARSER_OBJECT *xml_obj );
void          xml_free_parse_struct( XML_DOC_COMPONENT *pTemp );
#ifdef __cplusplus
};
#endif

#endif /* _H_XML_PARSER_ */

