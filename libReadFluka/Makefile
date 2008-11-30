include ../Makefile.inc

all:	ray rdbin libReadFluka.so


######################## libReadFluka ############
libReadFluka.so: obj/ReadFluka.o
	@echo "Compiling $@"
	@$(GCC) $(SOFLAGS) $^ -o $@


obj/ReadFluka.o: src/ReadFluka.cxx src/ReadFluka.h
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

clean:
	@rm -fv *~ obj/* *.map ray rdbin *.so