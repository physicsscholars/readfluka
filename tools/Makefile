include ../Makefile.inc

eventdat2root: src/eventdat2root.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -o $@ `root-config --libs`

clean:
	@rm -fv *~ eventdat2root