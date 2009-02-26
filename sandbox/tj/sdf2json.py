import simplejson
import sys

line = sys.stdin.readline()
title = line.strip()
line = sys.stdin.readline()
line = sys.stdin.readline()
line = sys.stdin.readline()
natoms,nbonds,rest = line.strip().split(None,2)

radius = {"C":1.4, "N":1.3, "O":1.2, "H":1.0,
          "F":1.0, "Cl":2.0, "P":2.0, "S":2.0}
sphere = list()
coords = list()
for i in range(0,int(natoms)):
  line = sys.stdin.readline()
  x,y,z,symbol,rest = line.strip().split(None,4)
  coords.append([float(x),float(y),float(z)])
  sphere.append([coords[i],radius[symbol]])

cylinder = list()
for i in range(0,int(nbonds)):
  line = sys.stdin.readline()
  a,b,type,rest = line.strip().split(None,3)
  cylinder.append([coords[int(a)-1], coords[int(b)-1], 0.25])

#primitives = [title,{"atoms":["sphere",sphere],"bonds":["cylinder",cylinder]}]
primitives = ["sphere",sphere, "cylinder",cylinder]
print simplejson.dumps(primitives)
