
/**
 *  \file xml_encoder.h
 *
 *  This file contains APIs of the XML encoder
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_XML_ENCODER_
#define _H_XML_ENCODER_

/* ------------------------------------------ Header File Inclusion */
#include "xml_common_defines.h"

/* ------------------------------------------ Function Declarations */
/*
 * function which encodes an XML document from an XML_ENCODER_OBJ
 * which is a tree structure representation of teh xml doc objs
 */
#ifdef __cplusplus
extern "C" {
#endif
API_RESULT xml_encoder (XML_ENCODER_OBJECT* xml_enc_obj);
#ifdef __cplusplus
};
#endif

#endif /* _H_XML_ENCODER_ */

