#!/bin/bash

file=$1
iterations=$2

if [[ -n "$file" && -n "$iterations" ]]; then
	mkfifo nbody-pipe
	../cmake-build-debug/nbody_psys17 "$file" "$iterations" > nbody-pipe &
	echo 'Starting Visualisation...'
	./nbody_visualization nbody-pipe
	echo 'Exiting and cleaning up...'
	rm nbody-pipe
	echo 'Done.'
else
	echo -e "Usage:\\n ./nbody_with_vis [input file] [iterations]\\n Use -1 for infinite iterations"

fi
