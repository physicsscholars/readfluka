#! /usr/bin/env python
# Plots two histograms with average and maximum CPU time used to follow a projectile in FLUKA

import sys,time,os,re
from ROOT import ROOT, TH1F, TFile
from array import array

def GetHistogram(fname):
    file = open(fname)
    x = []  # upper border of the energy bin
    x.append(0.0)
    y = []  # flux
    ey = [] # relative error
    line_number = 0
    lines_in_header = 10
    hname = ""
    htitle = ""
    xtitle = "x"
    particles = ""
    nentries = 0
    for line in file.readlines():
        line_number = line_number+1
        if line_number == 1:
            words = line.split()
            hname = "h%s%s" % (words[0], words[1])
            nentries = int(words[4])
            print hname, nentries
        elif line_number == 2:
            htitle = line
            print htitle
        elif line_number == 3:
            words = line.split()
            particles = words[1:]
            htitle = "%s: %s" % (particles, htitle)
        elif line_number == lines_in_header:
            xtitle = line
            if re.search("energy", xtitle):
                xtitle = xtitle + " [MeV]"

        if line_number<=lines_in_header:
            continue

        words = line.split()
        if words[0] == "total":
            break
#        print words
        x.append(float(words[0]))
        y.append(float(words[1]))
        ey.append(float(words[1])*float(words[2]))
#    print line_number-lines_in_header, "lines read", len(x)
    
    nbins = len(y)
    h = TH1F(hname, "%s;%s" % (htitle, xtitle), nbins, array('f', x))
    for i in range(nbins):
        h.SetBinContent(i+1, y[i]/(x[i+1]-x[i]))
        h.SetBinError(i+1,  ey[i]/(x[i+1]-x[i]))
#        he.SetBinContent(i+1, x[i+1]*y[i]/(x[i+1]-x[i]))
    h.SetEntries(nentries)
    return h

def main():
    fname_in = sys.argv[1]
    fname_out = fname_in.replace(".txt", ".root")
    fout = TFile(fname_out, "RECREATE")
    GetHistogram(fname_in).Write()
    fout.Close()
    


if __name__ == "__main__":
    sys.exit(main())
