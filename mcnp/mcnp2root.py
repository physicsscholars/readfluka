#! /usr/bin/env python

import sys
from pymcnp import *


def main():
    verbose = '-v' in sys.argv
    if verbose:
        sys.argv.remove('-v')

    fname_in = sys.argv[1]
    fname_out = fname_in + ".root"

#    conv = Converter(fname_in)
#    conv = ROOTConverter(fname_in)
#    print "MCNP version: ", conv.GetMCNPVersion()
#    print "Input file: ", conv.GetInputFileName()
    print "ROOT file: ", fname_out
    sys.exit(0)
    file = TFile(fname_out, "recreate")
    conv.GetTally(11).GetHistogram(1).Write()
    file.Close()

"""
    conv.GetTally(14)
    conv.GetTally(24)
    conv.GetTally(34)
    conv.GetTally(21)
    conv.GetTally(31)
    conv.GetTally(41)
    conv.GetTally(51)
"""
#    conv.PrintMaterialComposition()
#    conv.PrintProjectiles()

#    fout = TFile(fname_out, "RECREATE")
#    GetHistogram(fname_in).Write()
#    fout.Close()
    


if __name__ == "__main__":
    sys.exit(main())
