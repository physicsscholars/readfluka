#! /usr/bin/python -Qwarn

from pyfluka import *

def main(argv=None):
   """
	Generates a FLUKA input file, similar to $FLUPRO/example.inp
	The geometry description is saved in a separate file, example.geo
   """
   if argv is None:
        argv = sys.argv

   X0 = 0.0
   Y0 = 0.0
   Z0 = 0.0

   # all dimensions are in centimeters

   beam_ekin = 50

   target_width  = 2   # cm
   target_height = 2   # cm
   target_length = 10  # cm

   target_half_dimentions = [target_width/2.0, target_height/2.0, target_length/2.0]

   ############################################################
   # FILE UNITS
   usrbin_type   =  10.0
   eventdat_unit = -75.0
   usrbin_unit   = -76.0
   usrbdx_unit   = -77.0
   usrtrack_unit = -78.0
   
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
   example.Comment("")

#                                                              GEOMETRY DESCRIPTION
   # BODIES
   example.RPP("body1", -5000000, 5000000, -5000000, 5000000, -5000000, 5000000)
   example.RPP("body2", -1000000, 1000000, -1000000, 1000000, -1000000, 1000000)
   example.RPP("body3", -10,      10, -10, 10, 0, 5)
   example.XYP("body4", 2.5)

   example.end()

   # REGIONS
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

   example.ASSIGNMAT()

   example.Open(eventdat_unit, "eventdat")
   example.Open(usrbin_unit, "usrbin")
   example.Open(usrbdx_unit, "usrbdx")
   example.Open(usrtrack_unit, "usrtrack")
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
