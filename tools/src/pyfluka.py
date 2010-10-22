import sys, getopt, re, string, math
from array import array
from string import *

global _project_name
_project_name = "CMP"

global _file_geo_name
_file_geo_name = _project_name.lower() + ".geo"

global _file_geo
_file_geo =  open(_file_geo_name, "w")

global _rotdefi_i
_rotdefi_i = int(1)

def CheckName(name):
    'checks BODY/REGION name for FLUKA com'
    if len(str(name)) > 8:
        print >>sys.stderr, "name", name, "is too long"
        return 1
    if name[0] not in string.letters:
        print >>sys.stderr, "name:", name
        print >>sys.stderr, "the first character of body's name must be alphabetical"
        return 2
    return 0
        
class BODY:
    'class for a body'
    code = None
    name = None

    def __init__(self):
        CheckName(self.name)
    
    def Print(self, name, w1, w2, w3, w4, w5, w6, comment):
        CheckName(name)
        self.name = name
        if comment is None:
            print >> _file_geo, "%5s %8s %#9.4g %5.3f %5.3f %5.3f %5.3f %5.3f" %( self.code, name, w1, w2, w3, w4, w5, w6 )
        else:
            print >> _file_geo, "%5s %8s %#9.4g %#9.4g %#9.4g %#10.3f %#9.3f %#9.3f      ! %s" %( self.code, name, w1, w2, w3, w4, w5, w6, comment )

#############################

class RPP(BODY):
    '''
    RPP - rectangular parallelepiped
    It is possible to use both FLUKA and GEANT3 notations in constructor.
    FLUKA:  name, xmin,    xmax,    ymin,    ymax, zmin, zmax
    GEANT3: name, xcenter, ycenter, zcenter, array
            array is an array of 3 numbers of half-dimentions
    '''
    code = 'RPP'
    xmin = None
    xmax = None
    ymin = None
    ymax = None
    zmin = None
    zmax = None
    comment = None
    # GEANT3 notation:        x     y     z   array  comment
    def __init__(self, name, xmin, xmax, ymin, ymax, zmin=None, zmax=None, comment=None):
        self.name = name
        BODY.__init__(self)
        if zmax is not None:               # then FLUKA notation
            self.xmin = xmin
            self.xmax = xmax
            self.ymin = ymin
            self.ymax = ymax
            self.zmin = zmin
            self.zmax = zmax
            self.comment = comment
        else:                              # then GEANT3 notation
            self.xmin = xmin - ymax[0]
            self.xmax = xmin + ymax[0]
            self.ymin = xmax - ymax[1]
            self.ymax = xmax + ymax[1]
            self.zmin = ymin - ymax[2]
            self.zmax = ymin + ymax[2]
            self.comment = zmin
        self.Print()

    def Print(self):
        print >> _file_geo, "%5s %8s %#9.4f %9.4f %9.4f %9.4f %9.4f %9.4f" % ( self.code, self.name, self.xmin, self.xmax, self.ymin, self.ymax, self.zmin, self.zmax )

#############################

class SPH(BODY):
    'SPH - sphere'
    x = None
    y = None
    z = None
    R = None
    def __init__(self, name, x, y, z, R, comment=None):
        self.code = "SPH"
        self.name = name
        BODY.__init__(self)
        self.x = x
        self.y = y
        self.z = z
        self.R = R
        self.comment = comment
        self.Print()

    def Print(self):
        print >> _file_geo, "%5s %8s %9.3f %9.3f %9.3f %9.3f" % (self.code, self.name, self.x, self.y, self.z, self.R)

#############################

class XYP(BODY):
    'XYP - plane perpendicular to the z-axis'
    z = None
    def __init__(self, name, z, comment=None):
        self.code = "XYP"
        self.name = name
        self.z = z
        self.Print(name, z, comment)

    def Print(self, name, z, comment):
        print >> _file_geo, "%5s %8s %9.3f" % (self.code, self.name, z)


class YZP(BODY):
    'YZP - plane perpendicular to the x-axis'
    x = None
    def __init__(self, name, x, comment=None):
        self.code = "YZP"
        self.name = name
        self.x = x
        self.Print(name, x, comment)

    def Print(self, name, x, comment):
        print >> _file_geo, "%5s %8s %9.3f" % (self.code, self.name, x)

class XZP(BODY):
    'XZP - plane perpendicular to the y-axis'
    y = None
    def __init__(self, name, y, comment=None):
        self.code = "XZP"
        self.name = name
        self.y = y
        self.Print(name, y, comment)

    def Print(self, name, y, comment):
        print >> _file_geo, "%5s %8s %9.3f" % (self.code, self.name, y)

#############################

class ZCC(BODY):
    'ZCC - cyllinder parallel to z-axis'
    x = None
    y = None
    R = None
    def __init__(self, name, x, y, R, comment=None):
        self.code = "ZCC"
        self.x = x
        self.y = y
        self.R = R
        self.Print(name, x, y, R, comment)

    def Print(self, name, x, y, R, comment):
        self.name = name
        print >> _file_geo, "%5s %8s %9.3f %9.3f %9.3f" % (self.code, self.name, x, y, R)

class XCC(BODY):
    'XCC - cyllinder parallel to x-axis'
    y = None
    z = None
    R = None
    def __init__(self, name, y, z, R, comment=None):
        self.code = "XCC"
        self.y = y
        self.z = z
        self.R = R
        self.Print(name, y, z, R, comment)

    def Print(self, name, y, z, R, comment):
        self.name = name
        print >> _file_geo, "%5s %8s %9.3f %9.3f %9.3f" % (self.code, self.name, y, z, R)

class YCC(BODY):
    'YCC - cyllinder parallel to y-axis'
    def __init__(self, name, x, z, R, comment=None):
        self.code = "YCC"
        self.Print(name, x, z, R, comment)


    def Print(self, name, y, z, R, comment):
        self.name = name
        print >> _file_geo, "%5s %8s %9.3f %9.3f %9.4f" % (self.code, self.name, y, z, R)        

############################

_reg_counter = 0
global _materials
_materials = []

class Region:
    'describes a region as a combination of bodies/zones'
    name = None
    naz = 5
    title = None
    comment = None
    material = None
    counter = 0

    def __init__(self, name, title, material="", comment=None, naz=5):
        global _reg_counter
        _reg_counter = _reg_counter+1
        self.counter = _reg_counter
        
        CheckName(name)
        self.name = name
        self.title = title
        self.comment = comment
        self.material = material
        if self.material is not "":
            CheckName(self.material)
            _materials.append("%9s%11s" % (self.material, self.name))
        if naz is not 0:
            self.naz = int(naz)
        self.Print()

    def Print(self):
        if self.comment is not None:
            print >> _file_geo,  '* region', self.counter, '-', self.comment
        words = self.title.split()
        len_of_line = 0
        for i in range(len(words)):
            len_of_line += len(words[i]) + 1
            if len_of_line > 70:
                words[i] = words[i] + "\n" + " "*12
                len_of_line = 0
#            if i and not i % 15: words[i] = words[i] + "\n" + " "*12
#            if i and not i % 13: words[i] = words[i] + "  "
        self.title = join(words)
        print >> _file_geo, "%9s %3d  %s" % (self.name, self.naz, self.title)
    
###############################
        
def ASSIGNMAT(fname=None):
    material_to_skip = "lattice"
    
    if fname is None:
        for i in range( len(_materials) ):
            if material_to_skip not in _materials[i]:
                print >> _file_geo, "ASSIGNMAT %16s" %_materials[i]
    else:
        file = open(fname, "w")
        file.write("! generated by pyfluka.py\n")
        for i in range( len(_materials) ):
            if material_to_skip not in _materials[i]:
                file.write("ASSIGNMAT %16s\n" %_materials[i])
        file.close()
    if len(_materials) != _reg_counter:
        print >>sys.stderr, "Warning: number of regions (%d) is not equal to one of materials (%d)" % (_reg_counter, len(_materials))

########################################

def LATTICE(container, cell, sdum=""):
    mode = "a"
    file = open(_lattice_file, mode)
    file.write("%-9.8s %-9.8s                     %-9.8s                       %-.8s\n" % ("LATTICE", container, cell, sdum))
    file.close()

        
def ROTDEFI(index, polar, azimutal, x, y, z):
    mode = "a"
#    if _rotdefi_i is 0: mode = "w"
    file = open(_rotdefi_file, mode)
#    if _rotdefi_i is 0: file.write("! generated by pyfiuka.py\n")
    file.write("%-9.8s  %9.1f %9.1f %9.1f %9.4f%9.4f %9.4f\n" % ("ROT-DEFI", index, polar, azimutal, x, y, z))
    file.close()

    
########################################

global _estimators
_estimators = []

class USRBIN:
    classname = 'USRBIN'
    type = None
    particle = None
    unit = None
    xmax = None
    ymax = None
    zmax = None
    name = None
    xmin = None
    ymin = None
    zmin = None
    nx   = None
    ny   = None
    nz   = None

    def __init__(self, type, particle, unit, xmax, ymax, zmax, name, xmin, ymin, zmin, nx, ny, nz):
        self.type = type
        self.particle = particle
        self.unit = unit
        self.xmax = xmax
        self.ymax = ymax
        self.zmax = zmax
        self.name = name
        CheckName(name)
        self.xmin = xmin
        self.ymin = ymin
        self.zmin = zmin
        self.nx = nx
        self.ny = ny
        self.nz = nz
        _estimators.append(self)

    def GetLine(self):
        return "%-9.8s %9.1f %9s %9.1f %9.3f %9.3f %9.3f %-.8s\n%-9.8s %9.3f %9.3f %9.3f %9.1f %9.1f %9.1f &\n*" % (self.classname, self.type, self.particle, self.unit, self.xmax, self.ymax, self.zmax, self.name, self.classname, self.xmin, self.ymin, self.zmin, self.nx, self.ny, self.nz)

    def Print(self):
        print >> _file_geo, self.GetLine()

class EVENTBIN(USRBIN):
    classname = 'EVENTBIN'

class USRYIELD:
    classname = 'USRYIELD'
    quantity = None
    particle = None
    unit = None
    upstream = None
    downstream = None
    norma = None
    sdum = None
    max1 = None
    min1 = None
    n1   = None
    max2 = None
    min2 = None
    type = None

    def __init__(self, quantity, particle, unit, upstream, downstream, norma, sdum, max1, min1, n1, max2, min2, type):
        self.quantity = quantity
        self.particle = particle
        self.unit = unit
        self.upstream = upstream
        self.downstream = downstream
        self.norma = norma
        self.sdum = sdum
        CheckName(sdum)
        self.max1 = max1
        self.min1 = min1
        self.n1 = n1
        self.max2 = max2
        self.min2 = min2
        self.type = type
        _estimators.append(self)

    def GetLine(self):
        return "%-9.8s %9.1f %9s %9.1f %9s %9s %9.1f %-.8s\n%-9.8s %9.3f %9.3f %9.1f %9.3f %9.3f %9.1f &\n*" % (self.classname, self.quantity, self.particle, self.unit, self.upstream, self.downstream, self.norma, self.sdum, self.classname, self.max1, self.min1, self.n1, self.max2, self.min2, self.type)
#        return "%-9.8s %9.1f %9s %9.1f %9s %9s %9.1f %-.8s \n*" % (self.classname, self.quantity, self.particle, self.unit, self.upstream, self.downstream, self.norma, self.sdum)

    def Print(self):
        print >> _file_geo, self.GetLine()
    
#############################

def LINE(fname, name, w1, w2, w3, w4, w5, w6, sdum):
    print >> _file_geo, "%s%5.1f%10s%5.1f%5.3f%5.3f%5.3f%s\n" % (name, w1, w2, w3, w4, w5, w6, sdum)

def PRINT_FILE(fname, where=sys.stdout):
    file = open(fname, "r")
    for line in file.readlines():
        line = line.rstrip()
        print >> where, line

def ESTIMATORS(fname=None):
    if fname is None:
        for i in range( len(_estimators) ):
            print >> _file_geo, _estimators[i].GetLine()
    else:
        file = open(fname, "w")
        file.write("! generated by pyfluka.py\n")
        for i in range( len(_estimators) ):
            file.write(_estimators[i].GetLine() + '\n')
        file.close()


#def RANDOMIZE(fname):
#    import random
#    file = open(fname, "w")
#    file.write("! generated by pyfluka.py\n")
#    file.write("%-9.8s %9.1f%9.0f.0\n" % ("RANDOMIZE", 1.0, random.random()*2e+6) )
#    file.close
