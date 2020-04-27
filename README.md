# clothfx
The cloth simulation is based on Baraff-Witkin work describing three energy functions: stretch, shear and bend.

The cloth simulation is based on Baraff-Witkin work describing three energy functions: stretch, shear and bend.
Although they describe constraint and penalty methods, a simple susbtitute (yet unstable) is the implementation 
of spring between 2 linked particles in a triangle mesh. 

The main goals of this project is to have fun migrating the original `alpha` branch project while adding new 
features when migration is done.


# Dependencies
* Visual Studio 2017, running only with x86 architecture and Debug
* [freeglut](http://freeglut.sourceforge.net/) is a free-software/open-source alternative to the OpenGL Utility Toolkit (GLUT) library

# Known issues
* Energy functions should be at equilibrium with links;
* The system doesn't stabilize when is grounded;
* Some particles may stick to the surface of the sphere when damping is `kDamp > 0`

# References
* [Large Steps in Cloth Simulation](https://www.cs.cmu.edu/~baraff/papers/sig98.pdf), David Baraff and Andrew Witkin