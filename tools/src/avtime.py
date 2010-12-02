#! /usr/bin/env python
# Plots two histograms with average and maximum CPU time used to follow a projectile in FLUKA

import sys,time,os,re
from ROOT import ROOT, TH1F, TH2F, TCanvas, THStack, TLegend, TString, gStyle

def GetWeight(fname):
#    print fname
    weight = 1
    file = open(fname, 'r')
    for line in file.readlines():
        if re.search("Total number of primaries run", line):
            words = line.split()
            try:
                return float(words[5])
            except:
                return float(-1)
    print "weight not found in %s" % fname
    return float(-1)

def GetTime(fname, search, number):
#    print fname
    file = open(fname, 'r')
    for line in file.readlines():
        if re.search(search, line):
            words = line.split()
            try:
                return float(words[int(number)])
            except:
                return float(-1)
    print "time not found in %s" % fname
    return float(-1)

def GetAverageTime(fname):
    return GetTime(fname,"Average CPU time used to follow a primary particle", 9)

def GetMaximumTime(fname):
    return GetTime(fname, "Maximum CPU time used to follow a primary particle", 9)

def main():
    dir = sys.argv[1]
    gStyle.SetOptStat("imr")

    maxtimes = []
    avtimes  = []
    weights  = []
    
    for root, dirs, files in os.walk(dir):
        for fout in files:
            if re.search(".out\Z", fout):
                print fout
                path = os.path.join(dir, fout)

                av_time = GetAverageTime(path)
                if (av_time>0):
                    max_time = GetMaximumTime(path)
                    if (max_time>0):
                        weight = GetWeight(path)
                
                if (av_time>0) and (max_time>0) and (weight>0):
                    avtimes.append(av_time)
                    maxtimes.append(max_time)
                    weights.append(weight)
#                    hAverage.Fill(av_time, weight)
#                    hMaximum.Fill(max_time, weight)
#                    hMaxVsAv.Fill(av_time, max_time, weight)


#    hMaximum.SetBit(2048)
#    hMaximum.SetLineColor(ROOT.kRed)
#    hMaximum.SetLineWidth(2)

    hMaxVsAv = TH2F("hMaxVsAv", "Maximum vs Average CPU time;average time [sec];maximum time [sec]",
                    20, float(min(avtimes))*0.99, float(max(avtimes))*1.01,
                    20, float(min(maxtimes))*0.99, float(max(maxtimes))*1.01)
    
    n = len(avtimes)
    for i in range(n):
        hMaxVsAv.Fill(avtimes[i], maxtimes[i], weights[i])


    c1 = TCanvas("c1", "CPU Time", -1)
    c1.Divide(2, 1)

    c1.cd(1)
    hAverage = hMaxVsAv.ProjectionX("Average")
    hAverage.SetLineColor(ROOT.kBlack)
    hMaximum = hMaxVsAv.ProjectionY("Maximum")
    hMaximum.SetLineColor(ROOT.kRed)
    hs = THStack("hs", "CPU Time;time per event [sec];number of events simulated")
    hs.Add(hAverage)
    hs.Add(hMaximum)
    hs.Draw("nostack")
    
    leg = TLegend(0.68, 0.74, 0.97, 0.94)
    leg.AddEntry(hAverage, "average: %.0f #pm %.0f" % (hAverage.GetMean(), hAverage.GetMeanError()), "l")
    leg.AddEntry(hMaximum, "maximum: %.0f #pm %.0f" % (hMaximum.GetMean(), hMaximum.GetMeanError()), "l")
    leg.Draw()
    
#    ROOT.gPad.SetGrid()

    c1.cd(2)
    hMaxVsAv.Draw("col")

    c1.Update()
    c1.Print("avtime.C")
    time.sleep(60)

if __name__ == "__main__":
    sys.exit(main())
