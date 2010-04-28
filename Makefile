# $URL$
# $Id$
# $Date$

all:
	make -C libReadFluka
	make -C tools

binary:
	make -C libReadFluka binary
	make -C tools binary

root:
	make -C libReadFluka root
	make -C tools root

clean:
	make -C libReadFluka clean
	make -C tools clean

