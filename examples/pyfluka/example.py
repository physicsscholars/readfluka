#! /usr/bin/python -Qwarn

from pyfluka import *

def main(argv=None):
   """
	This script generates a FLUKA input file, similar to $FLUPRO/example.inp
	The geometry description is saved in a separate file, example.geo
	See comments in readfluka/tools/src/pyfluka.py for documentation of the class Project
   """
   if argv is None:
        argv = sys.argv

   beam_ekin = 50

   # FILE UNITS
   usrbdx_unit   = -77.0
   
   example = Project("example", "Charged pion fluence inside and around a proton-irradiated Be target", "A simple Be target inside vacuum", "example.inp")

   example.Line("BEAM", -beam_ekin, None, None, None, None, None, "PROTON")
#
# note that the previous line can be also written in a shorter way:
# example.Line("BEAM", -beam_ekin, sdum="PROTON")
#
   example.Line("BEAMPOS",  0.0,  0.0,  -50.0)
   example.Line("GEOBEGIN", None, None, 25, 26, sdum="COMBNAME")
   example.RawLine("example.geo")
   example.RawLine("geolog")
# just an empty comment line to separate the input file sections
   example.Comment("")

#                                                              GEOMETRY DESCRIPTION
   # BODIES
   example.RPP("body1", -5000000, 5000000, -5000000, 5000000, -5000000, 5000000)
   example.RPP("body2", -1000000, 1000000, -1000000, 1000000, -1000000, 1000000)
   example.RPP("body3", -10,      10, -10, 10, 0, 5)
   example.XYP("body4", 2.5)

   example.end()

   # REGIONS
   # note that we describe the regions and set their materials in one line:
   reg = example.Region("regBH1", "+body1 -body2", "BLCKHOLE", "black hole")
   reg = example.Region("regVA2", "+body2 -body3", "VACUUM", "vacuum around", 50)
   reg = example.Region("regBE3", "+body3 +body4", "BERYLLIU", "Be target 1st half")
   reg = example.Region("regBE4", "+body3 -body4", "BERYLLIU", "Be target 2nd half")

   example.end()


   example.Line("GEOEND", 101, 101, 111, -101, -101, -101, "DEBUG")
   example.Line("GEOEND", 200, 200, 200, sdum="&")

# MATERIALS
   example.Comment("")
   example.Line("MATERIAL",  4.0, 9.0122, 1.848,  5.0, sdum="BERYLLIU")

# this method prints the material assigment described in the lines with example.Region()
   example.ASSIGNMAT()

   example.Open(usrbdx_unit, "usrbdx")
   example.Comment("")

   example.Line("EMFCUT", -0.01, 0.01, 1.0, "BERYLLIU", sdum="PROD-CUT")
   example.USRBDX(99.0, "PIONS+-", usrbdx_unit, "regBE3", "regBE4", 400, "piFlueUD",
                  50, 0, 50, 0, 0, 10)
                 

#   example.Line("EVENTDAT", eventdat_unit, sdum="eventdat")
   example.Line("SCORE", "ENERGY", "BEAMPART")
   example.Line("RANDOMIZ", 1)
   example.Line("START", 2000)

   example.close()


if __name__ == "__main__":
    sys.exit(main())
