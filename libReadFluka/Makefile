include ../Makefile.inc

all:	libReadFluka.so libReadFlukaROOT.so


######################## libReadFluka ############
libReadFluka.so: obj/UsrBin.o obj/UsrYield.o obj/EventDat.o obj/Mgdraw.o obj/Base.o
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

########################## libReadFlukaROOT #####

libReadFlukaROOT.so: obj/ROOT_UsrBin.o
	@echo "Compiling $@"
	@$(GCC) $(SOFLAGS) $^ -o $@

obj/ROOT_UsrBin.o: src/ROOT_UsrBin.cxx src/ROOT_UsrBin.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< $(ROOTCFLAGS) -o $@


########################## rdbin #################
obj/rdbin.o:	src/rdbin.f
	@echo "Compiling $@"
	@$(FLUTIL)/fff $< && mv -f $(?:.f=.o) $@


rdbin: obj/rdbin.o
	@echo "Compiling $@"
	$(FLUTIL)/lfluka -o rdbin obj/rdbin.o

########################## ray ###################
obj/ray.o:	src/ray.f
	@echo "Compiling $@"
	@$(FLUTIL)/fff $< && mv -f $(?:.f=.o) $@


ray: obj/ray.o
	@echo "Compiling $@"
	$(FLUTIL)/lfluka -o ray obj/ray.o
##################################################

#install: libReadFluka.so
#	@echo "Installing $< to $(MYLIBS)"
#	@ln -sf `pwd`/$< $(MYLIBS)

clean:
	@rm -fv *~ obj/* */*~  *.map ray rdbin *.so
