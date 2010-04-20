# $URL$
# $Id$
# $Date$

all:
	make -C libReadFluka
	make -C tools

clean:
	make -C libReadFluka clean
	make -C tools clean

