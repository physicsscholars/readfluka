include ../../Makefile.inc


reader: reader.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -lReadFlukaROOT -o $@ `root-config --libs`


run:
	$(FLUTIL)/rfluka -e $(FLUTIL)/flukahp -N0 -M1 ex03_
