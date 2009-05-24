include ../../Makefile.inc

writer.o: writer.c
	$(GCC) -c $< $(ROOTCFLAGS) -o $@

source.o: source.f
	$(FLUTIL)/fff $<

ex01: writer.o source.o
	$(FLUTIL)/lfluka -o ex01 -m fluka $(ROOTLIBS) $^
#/home/kbat/usr/local/fluka/flutil/lfluka -o ex01 -m fluka -L/home/kbat/usr/local/root/lib/root -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lm -ldl  -L$FLUPRO writer.o source.o


run:
	$(FLUTIL)/rfluka -e $(FLUTIL)/flukahp -N0 -M1 ex03_
