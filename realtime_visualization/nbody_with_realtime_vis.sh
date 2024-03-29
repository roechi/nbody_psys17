#!/bin/bash

file=$1
iterations=$2
simulator=$3

if [[ -n "$file" && -n "$iterations" && -n "$simulator" ]]; then
	mkfifo nbody-pipe
	../cmake-build-debug/nbody_psys17 "$file" "$iterations" "$simulator" > nbody-pipe &
	echo 'Starting Visualisation...'
	./realtime_visualization nbody-pipe
	echo 'Exiting and cleaning up...'
	rm nbody-pipe
	echo 'Done.'
else
	echo -e "Usage: ./nbody_with_vis [input file] [iterations] [simulator]
	
	- input file must be valid start configuration for simulator
    	- use -1 for infinite iterations
    	- possible simulators are omp and ocl
    	"
fi
