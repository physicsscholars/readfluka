# $URL$
# $Id$
# $Date$

date=`date +%y%m%d`
PWD=$(shell pwd)

ROOTCONFIG :=	root-config
ARCH       :=	$(shell $(ROOTCONFIG) --arch)
PLATFORM   :=	$(shell $(ROOTCONFIG) --platform)

ROOTCFLAGS :=	$(shell $(ROOTCONFIG) --cflags)
ROOTGLIBS  := $(shell root-config --glibs)
ROOTLIBS   := $(shell $(ROOTCONFIG) --libs)
#OPT         = -g
MEMCHECK = $(shell $(ROOTCONFIG) --new --libs)
#MEMCHECK = ""

CXXFLAGS		=	$(OPT) -Wall -fPIC
LDFLAGS			=	$(OPT)
SOFLAGS			=	-shared
GCC = g++


#clean_common:
#	@rm -fv *~ *.o obj/* */*~
