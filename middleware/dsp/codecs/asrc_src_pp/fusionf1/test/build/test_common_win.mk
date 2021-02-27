###########################################################################
#### Following variables are configured for default/ recommended settings
#### parent makefile can overrule them if required
###########################################################################
ROOTDIR ?= ../..
STD_FILES?=.
CPU ?= xa
DEBUG ?= 0
QUIET ?=
WARNING_AS_ERROR ?= 1

###########################################################################
### DON'T MODIFY !!!
###########################################################################
# Legacy: Depreciating variable COPT, use COPTION For all future codecs
COPTIONS += $(COPT)
FORCE_QUIET=@
ECHO_START := @echo -e "\033[1m"
ECHO_END := "\033[0m"


# system tools
ifeq (,$(filter $(CPU), gcc x86))
	ISS = xt-run $(XTCORE)
	CONFIGDIR := $(shell $(ISS) --show-config=config)
	include $(CONFIGDIR)/misc/hostenv.mk
else
	RM_R = rm -rf
	MKPATH = mkdir -p
	CP = cp -f
endif

# compiler tools
ifeq (,$(filter $(CPU), gcc x86))
ifeq (,$(XT_CPP_COMPILER))
	CC = xt-xcc $(XTCORE)
else
	CC = $(XT_CPP_COMPILER) $(XTCORE)
endif
	CXX = $(CC) 
	AR = xt-ar $(XTCORE)
	LL = $(CC) $(XTCORE)
	OBJCOPY = xt-objcopy $(XTCORE)
	OBJ_EXTN = xa.o
	DEP_EXTN = xa.d
else
	ifeq ($(CPU), x86)
	    CC = gcc $(PLATFORMOPTS) -Wall
	else
	    CC = gcc $(PLATFORMOPTS)
	endif
	CXX = g++ $(PLATFORMOPTS)
	AR = ar
	LL = gcc $(PLATFORMOPTS)
	OBJCOPY = objcopy $(PLATFORMOPTS)
	ifeq ($(CPU), x86)
		OBJ_EXTN = x86.o
		DEP_EXTN = x86.d
	else
		OBJ_EXTN = gcc.o
		DEP_EXTN = gcc.d
	endif
endif

# compiler flags
CFLAGS += $(EXTRA_CFLAGS)
LDFLAGS += $(EXTRA_LDFLAGS)

COPTGCC = -INLINE:requested -Wsign-compare -Wstrict-prototypes -Wdeclaration-after-statement
ifeq "$(DEBUG)" "1"
	CFLAGS += -g
else
	CFLAGS += -DNDEBUG=1
endif

ifeq (,$(filter $(CPU), gcc x86))
	#ifeq ($(shell $(CXX) --version | grep -c 'version 7'), 0)
	    COPTIONS += -mcoproc
	#else
	#    COPTIONS += -OPT:hifi2_ops
	#endif
	COPTIONS += -Wall $(COPTGCC) \
	            -std=gnu99 -pedantic \
	            -fsigned-char -mno-mul16 -mno-mul32 -mno-div32 -Wall -W

	ifeq ($(WARNING_AS_ERROR),1)
	    COPTIONS += -Werror
	endif
	COPTIONS +=  -mlongcalls
	CFLAGS += -DPROFILE
else
	ifeq ($(CPU), gcc)
	    COPTIONS += $(COPTGCC)
	endif
	COPTIONS += -m32 -malign-double -Wall -mpreferred-stack-boundary=8
	LDFLAGS += -m32
endif


# codec specific file names
ifeq (,$(filter $(CPU), gcc x86))
	APP_PREFIX = xa_
else
	ifeq ($(CPU), gcc)
		APP_PREFIX = xgcc_
	else
		APP_PREFIX = x86_
	endif
endif

BIN = $(APP_PREFIX)$(APP_NAME)_test
LIB ?= $(APP_PREFIX)$(APP_NAME).a

# codec specific paths
SRCDIRS2 = $(addprefix $(ROOTDIR)/,$(SRCDIRS))
INCLUDES = $(addprefix -I$(ROOTDIR)/,$(INCLUDE_DIRS))
VPATH = %.cpp %.c %.cc $(SRCDIRS2)

BINDIR = $(ROOTDIR)\bin
LIBDIR= $(ROOTDIR)\lib
OBJDIR = .\objs\$(APP_NAME)_test




#create object list
OBJS_COBJS = $(addprefix $(OBJDIR)/,$(patsubst %.c,%.$(OBJ_EXTN),$(filter %.c, $(SRCS)))) 
OBJS_CPPOBJS = $(addprefix $(OBJDIR)/,$(patsubst %.cpp,%.$(OBJ_EXTN),$(filter %.cpp, $(SRCS))))
OBJS_CCOBJS = $(addprefix $(OBJDIR)/,$(patsubst %.cc,%.$(OBJ_EXTN),$(filter %.cc, $(SRCS))))

OBJS_OBJS = $(OBJS_COBJS) $(OBJS_CPPOBJS) $(OBJS_CCOBJS) 

#create dependency lists
set-e;DEPS_CDEPS = $(addprefix $(OBJDIR)/,$(patsubst %.c,%.$(DEP_EXTN),$(filter %.c, $(SRCS))))
set-e;DEPS_CPPDEPS = $(addprefix $(OBJDIR)/,$(patsubst %.cpp,%.$(DEP_EXTN),$(filter %.cpp, $(SRCS))))
set-e;DEPS_CCDEPS = $(addprefix $(OBJDIR)/,$(patsubst %.cc,%.$(DEP_EXTN),$(filter %.cc, $(SRCS))))
DEPS =  $(DEPS_CDEPS)$(DEPS_CPPDEPS) $(DEPS_CCDEPS)


ifneq ($(MAKECMDGOALS),clean)
	TMPDEPS := $(shell $(MKPATH) $(OBJDIR)) # To force dir creation
	-include $(DEPS)
endif

$(OBJDIR)/%.$(DEP_EXTN): %.cc
	@set -e; rm -f $@; \
	$(CXX) -MM $(COPTIONS) $(CFLAGS)  $(INCLUDES) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/\1.$(OBJ_EXTN) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$ 

$(OBJDIR)/%.$(DEP_EXTN): %.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $(COPTIONS) $(CFLAGS)  $(INCLUDES) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/\1.$(OBJ_EXTN) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$ 

$(OBJDIR)/%.$(DEP_EXTN): %.c
	@set -e; rm -f $@; \
	echo "$(CC) -MM $(COPTIONS) $(CFLAGS)  $(INCLUDES) $<"; \
	$(CC) -MM $(COPTIONS) $(CFLAGS)  $(INCLUDES) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/\1.$(OBJ_EXTN) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$ 

.SUFFIXES : .c .cpp .cc

$(OBJS_CPPOBJS): $(OBJDIR)/%.$(OBJ_EXTN): %.cpp
	$(ECHO_START)"Compiling CPP $<" $(ECHO_END)
	$(QUIET) $(CXX) $(COPTIONS) $(CFLAGS)  $(INCLUDES) -o $@ -c $< 


$(OBJS_CCOBJS): $(OBJDIR)/%.$(OBJ_EXTN): %.cc
	$(ECHO_START)"Compiling CC $<" $(ECHO_END)
	$(QUIET) $(CXX) $(COPTIONS) $(CFLAGS)  $(INCLUDES) -o $@ -c $< 

$(OBJS_COBJS): $(OBJDIR)/%.$(OBJ_EXTN): %.c
	$(ECHO_START)"Compiling C $<" $(ECHO_END)
	$(QUIET) $(CC) $(COPTIONS) $(CFLAGS)  $(INCLUDES) -o $@ -c $< 


$(BINDIR):
	$(QUIET) -$(MKPATH) $@

ifneq (, $(EXTRA_LIBS))
EXTRA_LIBS2=$(EXTRA_LIBS)
else
EXTRA_LIBS2=
endif
	
$(BIN): $(OBJS_OBJS) $(LIBDIR)/$(LIB) $(EXTRA_LIBS2)
	$(ECHO_START)"Linking and creating binary $(BIN)" $(ECHO_END)
	$(QUIET) $(LL) $(LDFLAGS) -o $@  $(OBJS_OBJS) $(LIBDIR)/$(LIB) $(EXTRA_LIBS2) -lm

$(BINDIR)/$(BIN): $(BIN)
	$(ECHO_START)"Installing $(BIN)" $(ECHO_END)
	$(QUIET) $(CP) $(BIN) $(BINDIR)/$(BIN)

run: 
	@echo "Testing $(BIN)"; 
	@xt-run --turbo $(BIN) > /dev/null 2>&1 ;

app_build:  $(BIN)

app_install: $(BINDIR)/$(BIN)

app_clean: 
	$(QUIET) $(RM) .\$(BIN)	

info: 
	echo $(EXTRA_LIBS2)

