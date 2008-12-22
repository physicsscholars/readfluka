include ../Makefile.inc

all:	libReadFluka.so


######################## libReadFluka ############
libReadFluka.so: obj/UsrBin.o obj/UsrYield.o obj/Base.o
	@echo "Compiling $@"
	@$(GCC) $(SOFLAGS) $^ -o $@


obj/Base.o: src/Base.cxx src/Base.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@

obj/UsrBin.o: src/UsrBin.cxx src/UsrBin.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@

obj/UsrYield.o: src/UsrYield.cxx src/UsrBin.h
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) $< -o $@

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
	@rm -fv *~ */*~ obj/* *.map ray rdbin *.so
