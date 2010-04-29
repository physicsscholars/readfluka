# $URL$
# $Id$
# $Date$

include Makefile.inc

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

tar: clean
	cd .. && tar jcf readfluka.$(date).tar.bz2 readfluka --exclude-tag-all=entries
