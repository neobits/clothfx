# clothfx
The cloth simulation is based on Baraff-Witkin work describing three energy functions: stretch, shear and bend.
Although they describe constraint and penalty methods, a simple susbtitute (yet unstable) is the implementation of spring between 2 linked particles in a triangle mesh. 

This a project done around 2010, so it has old sintax and pretty basic C++98

# Dependencies
* Visual Studio 2017, running only with x86 architecture and Debug
* [freeglut](http://freeglut.sourceforge.net/) is a free-software/open-source alternative to the OpenGL Utility Toolkit (GLUT) library

# Known issues
* Energy functions should be at equilibrium with links;
* The system doesn't stabilize when is grounded;

# References
* [Large Steps in Cloth Simulation](https://www.cs.cmu.edu/~baraff/papers/sig98.pdf), David Baraff and Andrew Witkin