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

        header_start = None
        header_end = None
        data_start = None
        data_end = None
# first, search for the tally description:
        for iline, line in enumerate(data, 1):
            if re.search("\A1tally  %d " % number, line): # space after %d is mandatory
                self.is_header_found = True
                header_start = iline
                print "\nfound the header of tally %d" % number

            if self.is_header_found and iline>header_start and re.search("\A1", line):
                header_end = iline-1
                if data[iline-2].strip() != "total bin": error("Format error - can't find the 'total bin' line in the end of tally %d" % number)
                break

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
            if self.time_bins[0] == "-i": # ??? -i means zero ???
                self.time_bins[0] = "0.0"
            print "\t%d time bins from  %s to %s shakes" % (len(self.time_bins)/2, self.time_bins[0], self.time_bins[-1])
        if self.is_energy_bins_found:
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
        data_start_line = None
        data_end_line   = None
        for iline, line in enumerate(data[data_start:data_end], data_start+1):
            if iline<data_end_line: continue
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
                thesurface = int(self.surfaces[isurface])
                if re.search("\A surface  %s " % thesurface, line): # space after %s is mandatory
#                    print line.rstrip()
                    time_or_energy = data[iline].strip()
                    if  time_or_energy == "time":
                        nbins = len(self.time_bins)/2
                    elif time_or_energy == "energy":
                        nbins = len(self.energy_bins)/2
                    else:
                        error("neither time nor energy in the data section")
                    
                    data_start_line = iline+1
                    data_end_line   = iline+1+nbins
                    if data[data_end_line].split()[0] != "total": error("format error after reading 1D values")
                    self.data[thesurface] = self.Get1Dvalues(data[data_start_line:data_end_line])
                    print "the sufrace", thesurface
#        print self.data
#        print iline

        

    def GetTimeBins(self, number):
        """
        Return the time bins [shakes]
        """
        for line in self.mcnp_file_contents:
            if re.search("1tally  %d" % number, line):
                if re.search("  time  bins", line):
                    print line

    def Get1Dvalues(self, lines):
        """
        Reads 1D values in the tally's data section from the lines in the argument and returns the list of values.
        Note that you should specify the lines to read from - the method should be called from constructor.
        """
        values = []
        del values[:]
        for line in lines:
#            print line.rstrip()
            words = line.split()
            values.append((float(words[1]), float(words[2])))
        return tuple(values)

    def Is1D(self):
        """
        Return True if Tally is one-dimentional, otherwise return False
        """
        if len(self.time_bins) * len(self.energy_bins) == 0: return True
        return False

    def ResampleBins(self, old_bins):
        """
        The 'old_bins' tuple has the format of the MCNP output file:
        x0 x1
        x1 x2
        x2 x3
        but in order to build a histogram we need x0, x1, x2, x3 ..., so here we resample it
        Also convert the bin values to float.
        """
        new_bins = []
        for i, bin in enumerate(old_bins):
            if i==0 or i%2:
                new_bins.append(float(bin))
        return new_bins





class Converter():
    """
    A class to read the MCNP output
    """
    mcnp_fname = None
    mcnp_file  = None
    mcnp_file_contents = [] # contents of mcnp_file

    def __init__(self, mcnp_fname):
        self.mcnp_fname = mcnp_fname
        self.mcnp_file  = open(self.mcnp_fname)
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


class ROOTTally(Tally):
    """
    A class for Tally with ROOT methods
    """
    def __init__(self, data, number):
        Tally.__init__(self, data, number)

    def GetHistogram1D(self, surface):
        """
        Return TH1 for the given surface
        """

        x = []
        bins = []
        xtitle = None
        if len(self.energy_bins): 
            bins = self.energy_bins
            xtitle = "Energy [MeV]"
        else:
            bins = self.time_bins
            xtitle = "Time [shakes]"
        
        print "bins", bins
        x = self.ResampleBins(bins) # len(x) = number of bins + 1
        nbins = len(x)-1
        h = TH1F("t%ds%d" % (self.number, surface), "%s;%s" % (self.title, xtitle), nbins, array('f', x)) 

        warning("the histogram must be scaled by the bin width (remove 1.0 below)")
        for i in range(nbins):
            value = self.data[surface][i]
            xwidth = 1.0 #x[i+1] - x[i]
            y = float(value[0])/xwidth
            ey = float(value[1])/xwidth * y # absolute error
            h.SetBinContent(i+1, y)
            h.SetBinError(i+1, ey)
            
#        h.Draw("hist,e")
#        h.Print("all")
#        time.sleep(60)
        return h

    def GetHistogram(self, surface):
        print "histogram for surface", surface
        print self.data[surface]
        if self.Is1D():                             # for 1D histograms
            return self.GetHistogram1D(surface)
            
            


class ROOTConverter(Converter):
    """
    A class to convert the MCNP output to the ROOT format
    """

    def __init__(self, mcnp_fname):
        Converter.__init__(self, mcnp_fname) # you must always explicitly call the appropriate method in the ancestor class

    def GetTally(self, N):
        """ Return a tally number N """
        return ROOTTally(self.mcnp_file_contents, N)
