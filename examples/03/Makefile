include ../../Makefile.inc


reader: reader.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -lReadFlukaROOT -o $@ $(ROOTLIBS)


run:
	$(FLUTIL)/rfluka -e $(FLUPRO)/flukahp -N0 -M1 ex03_

read:
	./reader ex03_001_eventdat ex03_001_usrbin ex03.root

clean:
	@rm -fv *~ reader

cleandata:
	@rm -fv ran* *.{log,out,err,root,11,15,6} *_{usrbin,eventdat,dump}
