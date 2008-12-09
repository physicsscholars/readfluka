include ../../Makefile.inc
DIFF=diff --ignore-all-space --ignore-blank-lines -i

all: reader

run: ex01_.inp
	$(FLUTIL)/rfluka -e $(FLUPRO)/flukahp -N0 -M1 `basename $^ .inp`

reader: reader.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKASYS)/src $^ -L $(READFLUKASYS) -lReadFluka -o $@

test: reader
	@echo -en "Test USRBIN:\t"
	@./reader ex01_001_usrbin.bin > test_usrbin.log
	@$(DIFF) test_usrbin.log ex01_001_usrbin.txt && echo "done" || echo "failed"

clean:
	@echo "Cleaning..."
	@find . -name "*~" -delete
	@rm -fvr obj/* *.so reader_root reader

cleandata:
	@rm -fvr *~ fort.* *.{log,out,err,bin,txt} {ranex01,fluka_}* *{_geolog,eventdat}
