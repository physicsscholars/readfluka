include ../Makefile.inc

TARGETS=eventdat2root usrbin2root

all : $(TARGETS)

eventdat2root: src/eventdat2root.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -o $@ `root-config --libs`

usrbin2root: src/usrbin2root.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -lReadFlukaROOT -o $@ `root-config --libs`


usxsuw2txt: src/usxsuw2txt.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKA_SYS)/src $^ -L $(READFLUKA_SYS) -lReadFluka -o $@


clean:
	@rm -fv *~ $(TARGETS)