#!/bin/bash

file=$1

if [[ -n "$file"  ]]; then
	echo 'Starting visualization...'
	./replay_visualization "$file"
	echo 'Done.'
else
	echo -e "Usage: ./nbbody_with_replay_vis.sh [input file]

	- input file must be output log of simulation
	"
fi

