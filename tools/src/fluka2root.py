#! /usr/bin/python -Qwarn
import sys, getopt, re, string, os

def usage():
    print main.__doc__

def notsupported():
    printincolor("fluka2root:\tFree-format input is not yet supported.\n\t\tYou might consider writing a feature request at http://readfluka.googlecode.com.")

def str2int(s):
    try:
        ret = int(s)
    except ValueError:
        ret = int(float(s))
    return ret

def printincolor(s,col=37):
    """
Print a string with a given color using ANSI/VT100 Terminal Control Escape Sequences
http://www.termsys.demon.co.uk/vtansi.htm
    """
    print "\033[1;%dm%s\033[0m" % (col, s)

def main(argv=None):
    """
fluka2root - a script to convert the output of all FLUKA estimators (supported by readfluka) to a single ROOT file.
usage:\tfluka2root file.inp [N] [M]
\tN - number of previous run plus 1. The default value is 1.
\tM - number of final run plus 1. The default value is N.
    """
    if argv is None:
        argv = sys.argv

    if len(argv) is 1:
        usage()
        sys.exit(1)

    estimators = {"EVENTDAT" : [], "USRBDX" : [], "USRBIN" : []} # dictionary of supported estimators and their file units
    opened = {} # dictionary of the opened units (if any)
    
    inpname = argv[1]
    N = 1
    if len(argv) > 2:
        try:
            N = str2int(argv[2])
        except ValueError:
            print main.__doc__
            sys.exit(1)
    M = N
    if len(argv) == 4:
        try:
            M = str2int(argv[3])
        except ValueError:
            print main.__doc__
            sys.exit(1)
#    print N, M

    inp = open(argv[1], "r")
#    print "Input file: %s" % inpname
    isname = False
    for line in inp.readlines():
        if re.search("\AFREE", line):
            notsupported()
            sys.exit(2)

#    inp.seek(0)
#    for line in inp.readlines():
        if isname is True:
            name = line[0:10].strip()
            opened[str2int(unit)] = name
            isname = False

        if re.search("\AOPEN", line):
            unit = line[11:20].strip()
            isname = True

    if len(opened):
        print "Opened units: ", opened

    inp.seek(0)
    print "Supported estimators:"
    for line in inp.readlines():
        for e in estimators:
            if re.search("\A%s" % e, line) and not re.search("\&", line[70:80]):
#                print line.strip()
                unit = line[30:40].strip()
                name = line[70:80].strip()
                print "\t", e, unit, name
                if str2int(unit)<0: # we are interested in binary files only
                    if not unit in estimators[e]:
                        estimators[e].append(unit)

    print estimators

    for e in estimators:
        for i in estimators[e]:
            i = str2int(i)
            if i<0: # we are interested in binary files only
                if i in opened:
                    estimators[e] = [str("_%s" % opened[i])] # !!! what if we have 2 units for the same estimator? !!!
                else:
                    estimators[e] = ["_fort.%d" % abs(i)]
    print estimators

    inp.close()

# run converters
    return_value = 0
    for i in range(N, M+1):
        binfilename = ""
        rootfilenames = []
        command = ""
        for e in estimators:
            for s in estimators[e]:
                binfilename = inpname.replace(".inp", "%.3d%s" % (i, s))
                rootfilenames.append(binfilename + ".root")
                command =  "%s2root %s" % (e.lower(), binfilename)
                printincolor(command)
                return_value = os.system(command)
                if return_value is not 0:
                    print "warning: " % return_value
                    sys.exit(return_value)
# hadd
        command = "hadd %s %s" % (inpname.replace(".inp", "%.3d%s" % (i, ".root")), string.join(rootfilenames))
        printincolor(command)
        return_value = os.system(command)
# remove tmp files
        if return_value is 0:
            command = "rm -v %s" % string.join(rootfilenames)
            printincolor(command)
            os.system(command)

    sys.exit(return_value)

if __name__ == "__main__":
    sys.exit(main())
