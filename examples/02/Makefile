include ../../Makefile.inc

all: read_mgdraw

ex02: mgdraw.f
	@echo "Linking $@"
	@$(FLUTIL)/lfluka -o $@ -m fluka $< 

run: ex02
	$(FLUTIL)/rfluka -e ex02 -N0 -M1 ex02_

read_mgdraw: read_mgdraw.cxx
	@echo "Compiling $@"
	@$(GCC) -I$(READFLUKA_SYS)/src $^ -L $(READFLUKA_SYS) -lReadFluka -o $@

clean:
	@echo "Cleaning..."
	@find . -name "*~" -delete
	@rm -fvr obj/* *.so read_mgdraw flukahp.map

cleandata:
	@rm -fvr *~ fort.* *.{log,out,err,bin,txt} {rantest,fluka_}* *{_geolog,eventdat}
