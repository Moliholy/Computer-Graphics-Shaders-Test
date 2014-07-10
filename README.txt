Computer Graphics and Visualization
===================================


Assignment 3: Walking in the air
Author: José Molina Colmenero





How to compile
==============

Open a terminal and type the following commands:
	$ cd Debug
	$ make clean
	$ make all




Usage
=====

By default the program will load an sphere and three lights, one of them in movement.
Camera controls are the following:

	When camera movement is normal:
	- i or I: reset initial conditions

	- w: move forward
	- s: move backward
	- a: move left
	- d: move right
	- z: move up
	- x: move down

	- W: negative rotation around X edge
	- S: positive rotation around X edge
	- A: positive rotation around Y edge
	- D: negative rotation around Y edge
	- Z: positive rotation around Z edge
	- X: negative rotation around Z edge


	When camera movement is smooth:
	- a: smooth movement to the left
	- d: smooth movement to the right



Also, it is possible to use the mouse for the same purpose:

	When camera movement is normal:
	- Mouse wheel forward: move forward
	- Mouse wheel backward: move backward
	- Mouse up: move up
	- Mouse down: move down
	- Mouse right: move right
	- Mouse left: move left

	- SHIFT + mouse up: negative rotation around X edge
	- SHIFT + mouse down: positive rotation around X edge
	- SHIFT + mouse left: positive rotation around Y edge
	- SHIFT + mouse right: negative rotation around Y edge

	When camera movement is smooth:
	- Mouse left: smooth movement to the left
	- Mouse right: smooth movement to the right