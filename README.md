# N-Body Simulation parallelized with Open MP and OpenCL
Jonas Jaszkowic and Richard Remus

## Building
You need _CMake_ and _Make_ to build the project. GTest sources will be pulled automatically.

Also you have to have _OpenCL_ installed and _g++-6_ should be available at `/usr/bin/g++-6`, otherwise you should update `CMakeLists.txt` to your needs.

The application provides hints on how to use its CLI, just try it out.

The build was tested on MacOS Sierra and in Docker running Debian 9.0 _"Stretch"_. 

## Visualizations

We provided two visualizations with Processing in the directories `realtime_visualization` and `replay_visualization` respectively. 

You might also want to take a look at the bash scripts in these directories to run the visualization more comfortable.