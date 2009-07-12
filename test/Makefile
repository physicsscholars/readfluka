include ../Makefile.inc
DIFF=diff --ignore-all-space --ignore-blank-lines -i

all: test_usrbin

run: test_001_usrbin.bin

test_001_usrbin.bin: $(FLUPRO)/flukahp test_.inp
	$(FLUTIL)/rfluka -e $(FLUPRO)/flukahp -N0 -M1 `basename test_.inp .inp`

$(FLUPRO)/flukahp:
	cd $(FLUPRO) && flutil/lfluka -m fluka

read_usrbin: read_usrbin.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKA_SYS)/src $^ -L $(READFLUKA_SYS) -lReadFluka -o $@


read_usryield: read_usryield.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKA_SYS)/src $^ -L $(READFLUKA_SYS) -lReadFluka -o $@

read_mgdraw: read_mgdraw.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKA_SYS)/src $^ -L $(READFLUKA_SYS) -lReadFluka -o $@

read_eventdat: read_eventdat.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKA_SYS)/src $^ -L $(READFLUKA_SYS) -lReadFluka -o $@


test_usrbin: run read_usrbin
	@echo -en "Test USRBIN:\t"
	@./read_usrbin test_001_usrbin.bin > test_usrbin.log
	@$(DIFF) test_usrbin.log test_001_usrbin.txt && echo "done" || echo "failed"

test_eventdat: run read_eventdat.cxx
	@echo -en "Test EVENTDAT:\t"
	@./read_eventdat test_001_evdat.bin > test_evdat.bin.log
	@perl -pi -e "s/\n/ /g" test_evdat.bin.log  && echo a >> test_evdat.bin.log
	@cp test_001_evdat.txt test_001_evdat.txt.log
	@perl -pi -e "s/\n/ /g" test_001_evdat.txt.log  && echo a >> test_001_evdat.txt.log
	@$(DIFF) test_evdat.bin.log test_001_evdat.txt.log && echo "done" || echo "failed"

clean:
	@echo "Cleaning..."
	@find . -name "*~" -delete
	@rm -fvr obj/* *.so read_usrbin_root read_usrbin read_usryield read_mgdraw read_eventdat

cleandata:
	@rm -fvr *~ fort.* *.{log,out,err,bin,txt} {rantest,fluka_}* *{_geolog,eventdat}
