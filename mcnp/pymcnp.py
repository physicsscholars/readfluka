#! /usr/bin/env python -W all

import sys,time,os,re,string
from ROOT import ROOT, TH1F, TFile
from array import array

def info(string):
    print >> sys.stderr, "\033[32m%s\033[0m" % string

def warning(string):
    print >> sys.stderr, "\033[33mWARNING: %s\033[0m" % string

def error(string):
    print >> sys.stderr, "\033[31mERROR: %s\033[0m" % string
    sys.exit(1)


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
    data = {}                        # dictionary for all the data
    number = None
    type = None
    title = None                     # description of type
    particles = None
    printing_order = None
    surfaces = None
    cells = None
    time_bins = []
    energy_bins = []
    nps = 0                          # number of particles

    is_header_found = False
    is_data_found = False
    is_time_bins_found = False
    is_energy_bins_found = False

    
    def __init__(self, data, number):
        self.number = number
#        print len(self.time_bins), self.time_bins
        del self.time_bins[:]
        del self.energy_bins[:]
        strtmp = "" # a temproary string

        iline = 1
        header_start = None
        header_end = None
        data_start = None
        data_end = None
# first, search for the tally description:
        for line in data:
            if re.search("\A1tally  %d " % number, line): # space after %d is mandatory
                self.is_header_found = True
                header_start = iline
                print "\nfound the header of tally %d" % number

            if self.is_header_found and iline>header_start and re.search("\A1", line):
                header_end = iline-1
                break
            iline = iline+1

        if self.is_header_found:
            info("header of tally %d was found between lines %d and %d" % (number, header_start, header_end))
        else:
            error("tally %d not found" % number)
            return

        # analyse the header
        for line in data[header_start:header_end]: # lines with the current tally only
            if self.type is None:
                if re.search("tally type", line):
                    words = line.split()
                    self.type = words[2]
                    print "\ttype: %s" % self.type
                    
                    self.title = string.join(words[3:])
                    print "\ttitle: %s" % self.title


            if self.particles is None:
                if re.search('particle\(s\)', line):
                    self.particles = line.split()[1:]
                    print "\tparticle(s): ", self.particles
                    
            if self.printing_order is None:
                if re.search("order of printing:", line):
                    self.printing_order = line.split()[3:]
                    print "\tprinting order:", self.printing_order

            if self.surfaces is None:
                if re.search("surfaces", line):
                    self.surfaces = line.split()[1:]
                    print "\tsurfaces:", self.surfaces

            if self.cells is None:
                if line[1:6] == "cells":
                    strtmp = line[7:]
                    strtmp = strtmp.replace("(", "")
                    strtmp = strtmp.replace(")", "")
                    self.cells = strtmp.split()
                    print "\tcells: %s" % self.cells

#            if re.search("time  bins", line): print "there are time bins", len(self.time_bins)
#            if re.search("energy bins", line): print "there are energy bins", len(self.energy_bins)

            if len(self.time_bins) is 0:
                if re.search("time  bins", line):
                    self.is_time_bins_found = True
#                    print "time bins found"
                
            if self.is_time_bins_found:
                if re.search(" to ", line):
                    words = line.split()
                    if words[3] == "shakes":
#                        error("Cannot read time bins in tally %d: format error" % number)
                        self.time_bins.append(words[0])
                        self.time_bins.append(words[2])

            if len(self.energy_bins) is 0:
                if line[1:12] == "energy bins":
                    self.is_energy_bins_found = True

            if self.is_energy_bins_found:
                if re.search (" to ", line):
                    words = line.split()
                    if words[3] == "mev":
                        self.energy_bins.append(words[0])
                        self.energy_bins.append(words[2])

        if self.is_time_bins_found:
            if len(self.time_bins) % 2:
                error("number of time borders is not a multiple of 2: %s" % len(self.time_bins))
            print "\t%d time bins from  %s to %s shakes" % (len(self.time_bins)/2, self.time_bins[0], self.time_bins[-1])
        if self.is_energy_bins_found:
            if len(self.energy_bins) % 2:
                error("number of energy borders is not a multiple of 2")
            print "\t%d energy bins from  %s to %s MeV" % (len(self.energy_bins)/2, self.energy_bins[0], self.energy_bins[-1])

# Now let's find the tally's data
        for iline, line in enumerate(data[header_end+1:], header_end+1):
            if re.search("\A1tally  %d " % number, line):
                self.is_data_found = iline
                self.nps = line.split()[4]
                data_start = iline+1
                print "\nfound data of tally %d" % number

            if self.is_data_found and iline>data_start and re.search("\A1", line):
                data_end = iline
                break

        
        if self.is_data_found:
            info("data of tally %d found between lines %d and %d" % (number, data_start, data_end))
        else:
            error("data of tally %d not found" % number)
            return

        # analyse the data
        for line in data[data_start:data_end]:
            # check the data and compare it with the header information
            if re.search("tally type", line):
                words = line.split()
                if self.type != words[2]: error("Tally %d data format error: types from the header and the data mismatch: %s != %s" %(number, self.type, words[2]) )
                if self.title != string.join(words[3:]): error("Tally %d data format error: titles from the header and the data mismatch: %s != %s" % (number, self.title, string.join(words[3:])))

            if line[11:19] == "particle":
                words = line.split()
                if self.particles != words[1:]: error("Data format error in tally %d: particles from the header and the data mismatch:\nheader:\t%s\ndata:\t%s" % (number, string.join(self.particles), string.join(words[1:])) )

            # loop through all surfaces
            for isurface in range(len(self.surfaces)):
                if re.search("\A surface  %s" % int(self.surfaces[isurface]), line):
                    print line.rstrip()
        

    def GetTimeBins(self, number):
        """
        Return the time bins [shakes]
        """
        for line in self.mcnp_file_contents:
            if re.search("1tally  %d" % number, line):
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
#        self.mcnp_file.close()

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

    def PrintMaterialComposition(self):
        """
        Prints the input file section with material composition
        """
        start = False
        for line in self.mcnp_file_contents:
            if start and re.search("\A1", line):
                break
            if start:
                print line.rstrip()
            if re.search("\A1material composition", line):
                start = True

    def PrintProjectiles(self):
        """
        Prints the input file section with information about incident particles
        """
        start = False
        i = 0
        x = 0.0
        print "incident particles:"
        for line in self.mcnp_file_contents:
            if start and len(line)>5:
                print line.rstrip()
                words = line.split()
                x = words[1]
                i = i+1
            if start and i is 50:
                break
            if re.search("nps    x          y          z", line):
                start = True
