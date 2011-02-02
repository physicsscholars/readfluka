#! /usr/bin/env python

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


class Tally():
    """
    A class for tally
    """
    data = None
    number = None
    type = None
    particles = None
    printing_order = None
    surfaces = None
    time_bins = []
    
    def __init__(self, data, number):
        self.number = number

        is_tally_found = False
        is_time_bins_found = False

        iline = 1
        line_start = None
        line_end = None
        for line in data:
            if re.search("\A1tally  %d" % number, line):
                is_tally_found = True
                line_start = iline
                print "found tally %d" % number
                print "line start", line_start

            if is_tally_found and iline>line_start and re.search("1tally", line):
                line_end = iline-1
                print "line end:", line_end
                break
            iline = iline+1

        if not is_tally_found:
            print "tally %d not found" % number
            return

        for line in data[line_start:line_end]: # lines with the current tally only
            if self.type is None:
                if re.search("tally type", line):
                    self.type = line.split()[2]
                    print "\ttype: ", self.type

            if self.particles is None:
                if re.search('particle\(s\)', line):
                    self.particles = line.split()[1:]
                    print "\tparticles: ", self.particles
                    
            if self.printing_order is None:
                if re.search("order of printing:", line):
                    self.printing_order = line.split()[3:]
                    print self.printing_order

            if self.surfaces is None:
                if re.search("surfaces", line):
                    self.surfaces = line.split()[1:]
                    print self.surfaces

            if len(self.time_bins) is 0:
                if re.search("time  bins", line):
                    is_time_bins_found = True
                
            if is_time_bins_found:
                if re.search(" to ", line):
                    words = line.split()
                    self.time_bins.append(words[0])
                    self.time_bins.append(words[2])
        print self.time_bins

    def GetTimeBins(self, tally):
        """
        Return the time bins [shakes]
        """
        for line in self.mcnp_file_contents:
            if re.search("1tally  %d" % tally, line):
                if re.search("  time  bins", line):
                    print line



class Converter():
    """
    A class to convert the MCNP output to the ROOT format
    """
    mcnp_fname = None
    mcnp_file  = None
    mcnp_file_contents = [] # contents of mcnp_file

    root_fname = None
    
    def __init__(self, mcnp_fname, root_fname="out.root"):
        self.mcnp_fname = mcnp_fname
        self.root_fname = root_fname
        self.mcnp_file  = open(mcnp_fname)
        self.mcnp_file_contents = self.mcnp_file.readlines()

    def GetMCNPVersion(self):
        """
        Return the MCNP version as reported in the output file
        """
        return self.mcnp_file_contents[0].split()[2]

    def GetInputFileName(self):
        """
        Return the input file name
        """
        return self.mcnp_file_contents[2]

    def GetTally(self, N):
        """
        Return a tally number N
        """
        return Tally(self.mcnp_file_contents, N)

def main():
    fname_in = sys.argv[1]
    fname_out = fname_in.replace(".txt", ".root")

    conv = Converter(fname_in, fname_out)
    print "MCNP version: ", conv.GetMCNPVersion()
    print "Input file: ", conv.GetInputFileName()
    conv.GetTally(11)
    conv.GetTally(14)

#    fout = TFile(fname_out, "RECREATE")
#    GetHistogram(fname_in).Write()
#    fout.Close()
    


if __name__ == "__main__":
    sys.exit(main())
