#!/bin/bash

file=$1
iterations=$2
simulator=$3

if [[ -n "$file" && -n "$iterations" && -n "$simulator" ]]; then
	mkfifo nbody-pipe
	../cmake-build-debug/nbody_psys17 "$file" "$iterations" "$simulator" > nbody-pipe &
	echo 'Starting Visualisation...'
	./nbody_visualization_cli nbody-pipe
	echo 'Exiting and cleaning up...'
	rm nbody-pipe
	echo 'Done.'
else
	echo -e "Usage: ./nbody_with_vis [input file] [iterations] [simulator]

    - use -1 for infinite iterations
    - possible simulators are omp and ocl
    "
fi
