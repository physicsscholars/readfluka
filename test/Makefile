include ../Makefile.inc
DIFF=diff --ignore-all-space --ignore-blank-lines -i

all: test_usrbin

run:   $(FLUPRO)/flukahp test_.inp
	$(FLUTIL)/rfluka -e $(FLUPRO)/flukahp -N0 -M5 test_

$(FLUPRO)/flukahp:
	cd $(FLUPRO) && flutil/lfluka -m fluka

usxsuw: run
	./usxsuw.sh

ustsuw: run
	./ustsuw.sh

read_usryield: read_usryield.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKA_SYS)/src $^ -L $(READFLUKA_SYS) -lReadFluka -o $@

test_usrbin: run
	@echo -en "Test USRBIN:\t"
	@$(READFLUKA_SYS)/../tools/usrbin2txt test_001_usrbin.bin > test_usrbin.log
	@$(DIFF) test_usrbin.log test_001_usrbin.txt && echo "done" || echo "failed"

test_eventdat: run
	@echo -en "Test EVENTDAT:\t"
	@./test_eventdat.sh

clean:
	@echo "Cleaning..."
	@find . -name "*~" -delete
	@rm -fvr obj/* *.so read_usrbin_root read_usrbin read_usryield

cleandata:
	@rm -fvr *~ *test_00* fluka_* *.lis *_usxsuw
