Robotics Operating System (ROS): C++ vs Python
==============================================

Author: Ori Novanda (cargmax-at-gmail.com)

This respository contains a homebrew benchmark program to show the performance of C++ and Python programs when running as ROS nodes.

The program compares the aimed (programmed) publish rates and the achieved message publish rates. It also counts for missing/dropped messages.

I provided seperate publisher and subscriber nodes for each language. Comparisons can be done within the same language or cross languages through these combinations:
* C++ publisher - C++ subscriber
* C++ publisher - Phyton subscriber
* Phyton publisher - Phyton subscriber
* Python publisher - C++ subscriber

Results
-------

A try-out was done by running the program on a laptop with Celeron SU2300 processor, Ubuntu 16.04 64-bit and ROS Kinetic.

The results are located in the "result" directory in this repository.

Note
----

The programs are not fully optimised. Differrent results might be achieved by optimising the code to the respective programming languages.

