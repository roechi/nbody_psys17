#!/usr/bin/python
import random

num_bodies=1000
radius_universe=3.0
file_buffer = "";
file_name="random_galaxy.txt"

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

print(file_buffer)
