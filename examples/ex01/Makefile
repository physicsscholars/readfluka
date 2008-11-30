include ../../Makefile.inc

READFLUKASYS=../../libReadFluka

all: reader

run: ex01_.inp
	$(FLUTIL)/rfluka -e $(FLUPRO)/flukahp -N0 -M1 `basename $^ .inp`

reader: reader.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKASYS)/src $^ -L $(READFLUKASYS) -lReadFluka -o $@

clean:
	@echo "Cleaning..."
	@find . -name "*~" -delete
	@rm -fvr obj/* *.so reader_root reader
	@rm -fvr *~ fort.* *.{log,out,err,bin,txt} {ranex01,fluka_}* *{_geolog,eventdat}
