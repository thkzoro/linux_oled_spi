TOPDIR := $(PWD)
$(warning $(TOPDIR)) 
LIBDIR := $(TOPDIR)/lib
CROSS := arm-linux-
LIBNAME 	:= liboled.a
SRCDIRS		:=	$(TOPDIR)/lib
EXTER_INCLUDE 	+= -I $(TOPDIR)/include
SRCEXTS   := .c

# The flags used by the cpp (man cpp for more).
# CPPFLAGS  := -Wall -Werror # show all warnings and take them as errors
CPPFLAGS  += -Wall -Werror -O2 -Os -g
CPPFLAGS  += $(EXTER_CPPFLAGS) $(EXTER_INCLUDE) 
CPPFLAGS  +=  -D_REENTRANT

#REVISION := $(shell sed -n "4p" $(PWD)/.svn/entries)
#ifneq ($(REVISION),)
#CPPFLAGS += -DSVN_VERSION=$(REVISION)
#endif

# The compiling flags used only for C.
# If it is a C++ program, no need to set these flags.
# If it is a C and C++ merging program, set these flags for the C parts.

CFLAGS    := 
CFLAGS    +=

AFLAGS := -r
AFLAGS += 

OBJCOPYFLAGS = -R .comment -R .stab -R .stabstr

## Implict Section: change the following only when necessary.
##=============================================================================
# The C program compiler. Uncomment it to specify yours explicitly.
#CC      = gcc
CC      = $(CROSS)gcc
AR    = $(CROSS)ar
STRIP	= $(CROSS)strip
NM = $(CROSS)nm
OBJCOPY = $(CROSS)objcopy

# The command used to delete file.
RM        = rm -f

## Stable Section: usually no need to be changed. But you can add more.
##=============================================================================
SHELL   = /bin/sh
SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
OBJS    = $(foreach x,$(SRCEXTS), \
      $(patsubst %$(x),%.o,$(filter %$(x),$(SOURCES))))
DEPS    = $(patsubst %.o,%.d,$(OBJS))

.PHONY : all objs clean cleanall rebuild

all : $(LIBNAME)

# Rules for creating the dependency files (.d).
#---------------------------------------------------
%.d : %.c
	@$(CC) -MM -MD $(EXTER_INCLUDE) $(CFLAGS) $(EXTER_CPPFLAGS) $<

# Rules for producing the objects.
#---------------------------------------------------
objs : $(OBJS)

%.o : %.c %.d
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<
	@echo $(CC) $<

$(LIBNAME) : $(OBJS)
	$(AR) $(AFLAGS) $(LIBNAME) $(notdir $(OBJS))
#	$(CC)  -fPIC -shared -o $(LIBNAME) $(notdir $(OBJS))
	

-include $(DEPS)

rebuild: clean all

clean :
	@$(RM) *.o *.d *.a *.so

cleanall: clean
	@$(RM) $(LIBNAME)
	
