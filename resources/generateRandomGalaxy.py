#!/usr/bin/python
import random
import sys

args = sys.argv;
if (len(args) < 2):
    print("Arguments missing!")
    print("Usage: ./generateRandomGalaxy.py [num_bodies] [out_file]")
    sys.exit();

num_bodies=int(args[1])
print 'Generating random galaxy with {} bodies.'.format(num_bodies)

radius_universe=3.0
file_buffer = "";
file_name=args[2]

MASS_SCALE = 1.98892e+30;
LENGTH_SCALE =  149597870700;
TIME_SCALE = 86400 * 365.25;

for i in range(0,num_bodies):
    line = "";

    if i == 0:
        mass = 1.0 * MASS_SCALE;
        px = random.uniform(-radius_universe, radius_universe) * LENGTH_SCALE;
        py = random.uniform(-radius_universe, radius_universe) * LENGTH_SCALE;
        vx = 0.0;
        vy = 0.0;
    else:
        mass = 1.0 * MASS_SCALE;
        px = random.uniform(-radius_universe, radius_universe) * LENGTH_SCALE;
        py = random.uniform(-radius_universe, radius_universe) * LENGTH_SCALE;
        vx = 0.0;
        vy = 0.0;

    line_vals = [mass,px,py,vx,vy];
    line += " ".join(map(str,line_vals));
    line += "\n";
    file_buffer += line;

f = open(file_name, 'w');
f.write(file_buffer);
f.close();

print 'Done. Generated random galaxy with %d bodies, output file was written to file %s.' % (num_bodies, file_name)
