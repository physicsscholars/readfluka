include ../Makefile.inc

BINARY := eventdat2txt  usrbin2txt  usrbdx2txt  mgdraw2txt resnuclei2txt
ROOT   := eventdat2root usrbin2root usrbdx2root

targets := $(BINARY) $(ROOT)

all:	$(targets)
binary: $(BINARY)
root:   $(ROOT)

%2root: src/%2root.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -lReadFlukaROOT -o $@ $(ROOTLIBS)

%2txt: src/%2txt.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKA_SYS)/src $^ -L $(READFLUKA_SYS) -lReadFluka -o $@

clean:
	@rm -fv {./,src/}*~ $(targets)
