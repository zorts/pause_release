
UNAME_KERNEL  := $(shell uname -s)
UNAME_MACHINE := $(shell uname -m)

# To make everything be rebuilt if the Makefile changes, uncomment
#ALL_DEPEND := Makefile

ifeq "${UNAME_KERNEL}" "OS/390"
OSTYPE := zOS
else
ifeq "${UNAME_KERNEL}" "Linux"
ifeq "${UNAME_MACHINE}" "s390x"
OSTYPE := zLinux
else
OSTYPE := Linux
endif
else
$(error "Unknown kernel type ${UNAME_KERNEL}")
endif
endif

EMPTY = 

#DEBUG = -g
ifneq "${DEBUG}"  "-g"
  OPT = -O2
  DEBUG = -DNDEBUG
endif

ifeq "${OSTYPE}" "zLinux"
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

CPPFLAGS = \
 -I. \
 -m64 \
 -Wall \
 -D__STDC_FORMAT_MACROS \
 -D_XOPEN_SOURCE=600 \
 ${EMPTY}

CC = gcc
CFLAGS = \
 ${OPT} ${DEBUG} \
 -std=c99 \
 ${EMPTY}

CXX = g++
CXXFLAGS = \
 ${OPT} ${DEBUG} \
 ${EMPTY}

LINK = g++ -m64 -lpthread ${DEBUG}
endif

ifeq "${OSTYPE}" "zOS"
DEPFLAGS = -MT $@ -MG -qmakedep=gcc -MF $(DEPDIR)/$*.Td

SYS_INCLUDE = /usr/include

CPPFLAGS = \
 -I. -I${SYS_INCLUDE} \
 -D${OSTYPE} \
 -D_XOPEN_SOURCE=600 -D_XOPEN_SOURCE_EXTENDED=1 \
 -D__STDC_FORMAT_MACROS \
 -D__MV17195__ \
 ${EMPTY}

CC = xlc \
 -q64 \
 -qasm \
 -qlanglvl=extc99 \
 -qseverity=e=CCN3296 -qseverity=e=CCN3950 \
 -qnocse -qnosearch \
 ${EMPTY}

ifneq "${LIST}" ""
LISTOPT = -V -qlist=${basename $<}.lst
endif

CFLAGS = \
 ${OPT} ${DEBUG} ${LISTOPT} \
 -qsuppress=CCN4108\
 ${EMPTY}

CXX := xlC \
 -q64 \
 -qasm \
 -qasmlib="//'SYS1.MACLIB'" \
 -Wc,xplink -Wl,xplink \
 -qlanglvl=longlong \
 ${EMPTY}

CXXFLAGS = \
 ${OPT} ${DEBUG} ${LISTOPT} \
 -qsuppress=CCN6639 \
 ${EMPTY}

LINK = xlC ${DEBUG} -q64
endif

# List all source files (NOT headers) in SRCS; these will be
# processed to have dependencies auto-generated
SRCS = \
 mvsalloc.cpp \
 options.c \
 pauserel.cpp \
 pe.cpp \
 timer.cpp \
 waitpost.cpp \
 ${EMPTY}

# Targets start here

ifeq "${OSTYPE}" "zOS"
PROGRAMS := pauserel
endif

default:  ${PROGRAMS}

pauserel: pauserel.o \
 pe.o \
 timer.o \
 waitpost.o \
 mvsalloc.o \
 options.o \
 ${EMPTY}
	${LINK} -o $@ $^

clean:
	rm -f ${PROGRAMS} *.o *.dbg *.lst *.ad $(DEPDIR)/*

veryclean: clean
	rm -f *~ 
	rmdir $(DEPDIR)

# dependency generation code
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

%.o : %.c
%.o : %.c $(DEPDIR)/%.d ${ALL_DEPEND}
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d ${ALL_DEPEND}
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
