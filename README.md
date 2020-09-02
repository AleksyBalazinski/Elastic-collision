# Elastic collision
This program is a simulation of a real-world elastic collision in two dimensions. User can specify masses and velocities of two objects and then set their initial positions and trajectories by changing the angle at which the objects are fired. 
### How to use this program?
In order to move an object (which is only possible before hitting space - a signal for the program to start simulation) click on a desired object (red or green) with left mouse button and then using arrows on a keyboard change its position and initial direction of velocity. Thus the full list of controls looks like that:
_Up arrow_ - move an object up
_Down arrow_ - move an object down
_Right arrow_ - rotate velocity vector clockwork wise
_Left arrow_ - rotate velocity vector counter-clockwork wise
### What are the mechanics behind this program?
The program uses formulas which can be found [here](https://en.wikipedia.org/wiki/Elastic_collision) to show objects' behavior after the collision and cmath library for sin, cos and arctan functions. 
