include ../Makefile.inc

BINARY := eventdat2txt  usrbin2txt  usrbdx2txt  mgdraw2txt resnuclei2txt   usrsuw2txt   usbsuw2txt   usxsuw2txt  ustsuw2txt
ROOT   := eventdat2root usrbin2root usrbdx2root            resnuclei2root  usrsuw2root  usbsuw2root  usxsuw2root ustsuw2root

targets := $(BINARY) $(ROOT) rdbin rdbdx ray

all:	$(targets)
binary: $(BINARY)
root:   $(ROOT)

%2root: src/%2root.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -lReadFlukaROOT -o $@ $(ROOTLIBS)

%2txt: src/%2txt.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKA_SYS)/src $^ -L $(READFLUKA_SYS) -lReadFluka -o $@

%:      src/%.f
	@echo "Compiling $@"
	$(FLUTIL)/lfluka -o $@ $< && rm -fv $@.map


clean:
	@rm -fv {./,src/}*~ $(targets)
