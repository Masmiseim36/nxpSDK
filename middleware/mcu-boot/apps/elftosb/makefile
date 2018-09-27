#*******************************************************************************
#                               makefile
# Description:
#   gnu make makefile for elftosb executable
 
#*******************************************************************************
#                               Environment

# UNAMES is going to be set to either "Linux", "Darwin" or "CYGWIN_NT-5.1"
UNAMES = $(shell uname -s)

ifeq ("${UNAMES}", "Linux")

SRC_DIR = $(shell pwd)
BUILD_DIR = bld/linux

else 
ifeq ("${UNAMES}", "CYGWIN_NT-5.1")

SRC_DIR = $(shell pwd)
BUILD_DIR = bld/cygwin

else
ifeq ("${UNAMES}", "Darwin")

SRC_DIR = $(shell pwd)
BUILD_DIR = bld/darwin

endif
endif
endif

#*******************************************************************************
#                                 Targets

all clean elftosb:
	@mkdir -p ${BUILD_DIR};
	make -C ${SRC_DIR}/mbedtls $@;
	make -C ${BUILD_DIR} -f ${SRC_DIR}/makefile.rules SRC_DIR=${SRC_DIR} $@;
