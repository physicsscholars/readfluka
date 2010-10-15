#! /usr/bin/python -Qwarn
import sys, getopt, re, string, os
import tempfile

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

    printincolor("Note that the corresponding histograms from different ROOT files will be summed up but not averaged unless it is not been implemented in the ROOT's hadd.", 33)

    estimators = {"EVENTDAT" : [], "USRBDX" : [], "USRBIN" : [], "RESNUCLE" : []} # dictionary of supported estimators and their file units
#    estimators = {"RESNUCLE" : []} # dictionary of supported estimators and their file units
    opened = {} # dictionary of the opened units (if any)
    out_root_files = [] # list of output ROOT files
    
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
            if e == "EVENTDAT": # EVENTDAT card has a different format than the other estimators
                if re.search("\A%s" % e, line):
                    unit = line[10:20].strip()
                    name = line[70:80].strip()
                    print "\t", e, unit,name
                    if str2int(unit)<0: # we are interested in binary files only
                        if not unit in estimators[e]:
                            estimators[e] = ["_%s" % name]
            else:
                if re.search("\A%s" % e, line) and not re.search("\&", line[70:80]):
                    if e == "RESNUCLE":
                        unit = line[20:30].strip()
                    else:
                        unit = line[30:40].strip()
                    name = line[70:80].strip()
                    print "\t", e, unit, name
                    if str2int(unit)<0: # we are interested in binary files only
                        if not unit in estimators[e]:
                            estimators[e].append(unit)

    print estimators

# Convert units in the file names:
    for e, units in estimators.iteritems():
        if e == "EVENTDAT":
            continue
        for u in units:
            iu = str2int(u)
            if iu<0: # we are interested in binary files only
                if iu in opened:
                    units[units.index(u)] = str("_%s" % opened[iu])
                else:
                    units[units.index(u)] = "_fort.%d" % abs(iu)

    print estimators

    inp.close()

# run converters
    return_value = 0
    resnuclei_binary_files = []
    usrbin_binary_files = []
    for run in range(N, M+1):
        binfilename = ""
        rootfilenames = []
        command = ""
        for e in estimators:
            for s in estimators[e]:
                binfilename = inpname.replace(".inp", "%.3d%s" % (run, s))
                if os.path.isfile(binfilename):
                    if re.search("RESNUCLE", e): # RESNUCLE = RESNUCLEi = RESNUCLEI
                        e = "RESNUCLEI"
                        resnuclei_binary_files.append(binfilename)
                    if re.search("USRBIN", e):
                        usrbin_binary_files.append(binfilename)
                    else:
                        rootfilenames.append(binfilename + ".root")
                        command =  "%s2root %s" % (e.lower(), binfilename)
                        printincolor(command)
                        return_value = os.system(command)
                        if return_value is not 0:
                            print "warning: " % return_value
                            sys.exit(return_value)
                        else:
                            printincolor("WARNING: can't open file %s" % binfilename, 33)
# hadd within one sample
        print rootfilenames
        out_root_file = inpname.replace(".inp", "%.3d%s" % (run, ".root"))
        command = "hadd %s %s" % (out_root_file, string.join(rootfilenames))
        printincolor(command)
        return_value = os.system(command)
# remove tmp files
        if return_value is 0:
            command = "rm -v %s" % string.join(rootfilenames)
            printincolor(command)
            return_value = os.system(command)
            if return_value is 0:
                out_root_files.append(out_root_file)
            else:
                sys.exit(return_value)

    if len(resnuclei_binary_files): # usrsuw to sum RESNUCLEI
        usrsuwfile = inpname.replace(".inp", "%.3d-%.3d_usrsuw" % (N, M) )
        fd, temp_path = tempfile.mktemp()
        tmpfile = open(temp_path, "w")
        print tmpfile.name
        for f in resnuclei_binary_files:
            tmpfile.write("%s\n" % f)
        tmpfile.write("\n")
        tmpfile.write("%s\n" % usrsuwfile)
        tmpfile.close()
#        os.close(fd)
#        os.remove(temp_path)
        command = "cat %s | $FLUTIL/usrsuw" % tmpfile.name
        print command
        os.system(command)
        os.unlink(tmpfile.name)
        command = "usrsuw2root %s" % usrsuwfile
        os.system(command)
        out_root_files.append("%s.root" % usrsuwfile)

    if len(usrbin_binary_files): # usbsuw to sum RESNUCLEI
        usbsuwfile = inpname.replace(".inp", "%.3d-%.3d_usbsuw" % (N, M) )
        fd, temp_path = tempfile.mkstemp()
        tmpfile = open(temp_path, "w")
        print tmpfile.name
        for f in resnuclei_binary_files:
            tmpfile.write("%s\n" % f)
        tmpfile.write("\n")
        tmpfile.write("%s\n" % usbsuwfile)
        tmpfile.close()
#        os.close(fd)
#        os.remove(temp_path)
        command = "cat %s | $FLUTIL/usbsuw" % tmpfile.name
        print command
        os.system(command)
        os.unlink(tmpfile.name)
        command = "usbsuw2root %s" % usbsuwfile
        print command
#        os.system(command)
#        out_root_files.append("%s.root" % usbsuwfile)

    print out_root_files
    if return_value is 0 and len(out_root_files)>1:
        out_root_file = inpname.replace(".inp", ".root");
        command = "hadd %s %s" % (out_root_file, string.join(out_root_files))
        printincolor(command)
        return_value = os.system(command)
        if return_value is 0:
            command = "rm -v %s" % string.join(out_root_files)
            printincolor(command)
            return_value = os.system(command)

    sys.exit(return_value)

if __name__ == "__main__":
    sys.exit(main())
