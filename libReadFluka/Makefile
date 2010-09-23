include ../Makefile.inc

all:	binary root

binary:	libReadFluka.so
root:	libReadFlukaROOT.so


######################## libReadFluka ############
libReadFluka.so: obj/UsrBin.o obj/UsrYield.o obj/EventDat.o obj/Mgdraw.o obj/UsrBdx.o obj/UsxSuw.o obj/ResNuclei.o obj/Base.o
	@echo "Compiling $@"
	@$(GCC) $(SOFLAGS) $^ -o $@

obj/Base.o: src/Base.cxx src/Base.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@

obj/EventDat.o: src/EventDat.cxx src/EventDat.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@

obj/UsrBin.o: src/UsrBin.cxx src/UsrBin.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@

obj/UsrYield.o: src/UsrYield.cxx src/UsrYield.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@

obj/Mgdraw.o: src/Mgdraw.cxx src/Mgdraw.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@

obj/UsrBdx.o: src/UsrBdx.cxx src/UsrBdx.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@

obj/UsxSuw.o: src/UsxSuw.cxx src/UsxSuw.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@

obj/ResNuclei.o: src/ResNuclei.cxx src/ResNuclei.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@


########################## libReadFlukaROOT #####

libReadFlukaROOT.so: obj/ROOT_UsrBin.o obj/ROOT_UsrBdx.o
	@echo "Compiling $@"
	@$(GCC) $(SOFLAGS) $^ -o $@

obj/ROOT_%.o: src/ROOT_%.cxx src/ROOT_%.h src/%.cxx src/%.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< $(ROOTCFLAGS) -o $@



########################## rdbin and ray #########
%:	src/%.f
	@echo "Compiling $@"
	$(FLUTIL)/lfluka -o $@ $<

##################################################

#install: libReadFluka.so
#	@echo "Installing $< to $(MYLIBS)"
#	@ln -sf `pwd`/$< $(MYLIBS)

clean:
	@rm -fv *~ obj/* */*~  *.map ray rdbin *.so
