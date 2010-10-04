#!/usr/bin/python

import sys, re, random

def usage():
    print "This script clones FLUKA input files by putting the different numbers in WHAT(3) of the RANDOMIZe card."
    print "Usage: clone_inp.py old_file.inp new_file.inp"
#    print "Example: for i in `seq 9`; do ../macros/clone_inp.py e200_0_.inp `basename e200_0_.inp 0_.inp`"$i"_.inp; done"
    sys.exit(1)


def main(argv=None):
    if argv is None:
        argv = sys.argv

        if len(sys.argv) is not 3:
            usage()
            sys.exit(1)

        fin_name = sys.argv[1]
        fout_name = sys.argv[2]

        fin = open(fin_name, 'r')
        fout = open(fout_name, 'w');
        
        for line in fin.readlines():
            line = line.strip()
            if re.search("^RANDOMIZ", line):
                words = line.split()
                fout.write("%-9.8s %9.8s%9.0f.0\n" % (words[0], words[1], random.random()*2e+6))
            else:
                fout.write("%s\n" % line)
        sys.exit(0)

if __name__ == "__main__":
    sys.exit(main())

