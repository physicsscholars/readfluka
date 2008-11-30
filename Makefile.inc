date=`date +%y%m%d`
PWD=$(shell pwd)

ROOTCONFIG :=	root-config
ARCH       :=	$(shell $(ROOTCONFIG) --arch)
PLATFORM   :=	$(shell $(ROOTCONFIG) --platform)

ROOTCFLAGS :=	$(shell $(ROOTCONFIG) --cflags)
ROOTGLIBS  := $(shell root-config --glibs)
OPT         = -g
MEMCHECK = $(shell $(ROOTCONFIG) --new --libs)
#MEMCHECK = ""


DllSuf			= so
CXXFLAGS		=	$(OPT) -Wall -fPIC
LDFLAGS			=	$(OPT)
SOFLAGS			=	-shared
GCC = g++

ifeq ($(ARCH),win32gcc)
DllSuf	=	dll
ExeSuf	=	.exe
CXXFLAGS	=	$(OPT) -pipe -Wall -fPIC -Woverloaded-virtual -I/usr/X11R6/include
LDFLAGS	=	$(OPT) -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc -L/usr/X11R6/lib
SOFLAGS	=	-shared -D_DLL -Wl,--export-all-symbols
SOFLAGS = -shared `root-config --cflags --libs`
endif
