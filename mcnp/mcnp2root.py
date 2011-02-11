#! /usr/bin/env python

import sys
from pymcnp import *


def main():
    verbose = '-v' in sys.argv
    if verbose:
        sys.argv.remove('-v')

    fname_in = sys.argv[1]
    fname_out = fname_in + ".root"

    conv = Converter(fname_in)
    conv.GetTally(14)

"""
    conv = ROOTConverter(fname_in)
    print "MCNP version:\t", conv.GetMCNPVersion()
    print "Input file:\t", conv.GetInputFileName()
    print "ROOT file:\t", fname_out

    tally = conv.GetTally(11)
    surfaces = tally.surfaces
 
    file = TFile(fname_out, "recreate")
    for surface in surfaces:
        print surface
        tally.GetHistogram(int(surface)).Write()
    file.Close()
"""


"""
    conv.GetTally(14)
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
