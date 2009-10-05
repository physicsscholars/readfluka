include ../Makefile.inc

TARGETS=eventdat2root usrbin2root usrbdx2txt usrbdx2root

all : $(TARGETS)

#EVENTDAT
eventdat2root: src/eventdat2root.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -o $@ `root-config --libs`

# USRBIN
usrbin2root: src/usrbin2root.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -lReadFlukaROOT -o $@ `root-config --libs`


# USRBDX
usrbdx2txt: src/usrbdx2txt.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKA_SYS)/src $^ -L $(READFLUKA_SYS) -lReadFluka -o $@

usrbdx2root: src/usrbdx2root.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -lReadFlukaROOT -o $@ `root-config --libs`


clean:
	@rm -fv *~ $(TARGETS)
