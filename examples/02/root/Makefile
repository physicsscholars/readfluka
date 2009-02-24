include ../../../Makefile.inc

#read_mgdraw: read_mgdraw.o
#	@echo "Linking $@"
#	@$(GCC) $(CXXFLAGS) $^ $(ROOTCFLAGS) -L -lReadFluka -o $@ `root-config --libs`

read_mgdraw: read_mgdraw.cxx
	@echo "Compiling $@"
	@$(GCC) $(CXXFLAGS) -I$(READFLUKA_SYS)/src $^ $(ROOTCFLAGS) -L $(READFLUKA_SYS) -lReadFluka -o $@ `root-config --libs`


read_mgdraw.o: read_mgdraw.cxx
	@echo "Compiling $@"
	@$(GCC) -c $(CXXFLAGS) -I../../../libReadFluka/src/ $< $(ROOTCFLAGS) -o $@

clean:
	@rm -fv *~ *.o read_mgdraw
