# SimBenchmark [![Build Status](https://travis-ci.com/jslee02/SimBenchmark.svg?branch=dart)](https://travis-ci.com/jslee02/SimBenchmark)

SimBenchmark is a project providing the reliable benchmark results of state-of-the-art physics engines used for contact simulation for various robotic tasks.

We evaluated the most widely-used physics engines for robotics and machine learning applications. The list of the engines is as follows:

- RaiSim (Jemin Hwangbo, Dongho Kang et al.)
- [Bullet Physics](http://bulletphysics.org/)
- [Open Dynamics Engine](http://www.ode.org/)
- [Multi-Joint dynamics with Contact (a.k.a. MuJoCo)](http://mujoco.org/)
- [DART Sim](https://dartsim.github.io/)

You can see the details in [webpage](https://leggedrobotics.github.io/SimBenchmark/).

![ANYmal PD Control Benchmark](https://leggedrobotics.github.io/SimBenchmark/about/anymal.gif)

## Overview

The project is consist of common part(interface) and libraries which corresponds each simulation engine.

## Installation
 
- Ubuntu 16.04 LTS only.
- To use visualization, add environmental variable "RAI_GRAPHICS_OPENGL_ROOT" that points to lib/raiGraphics
- Matlab R2018a is required for plotting
- MuJoCo is proprietary. You need a license for testing MuJoCo.
- RaiSim is proprietary, and currently unreleased.

### Install with bash script

- Run install.sh script by ```./install.sh ```
- The dependencies are downloaded in ```lib``` directory.

### Install manually

- install Bullet Physics
    - turn on double precision and shared library option
    - Build and install library into local
- install ODE
    - Build and install library into local
- install raiGraphics
    - Build install library into local
- install raiCommons
    - Build install library into local
- download MuJoCo v.1.5 in lib directory
    - ```lib/mjpro150```
    - put ```mjkey.txt``` in ```lib/mjpro150/mjkey.txt```

## Test and Results

We designed the following tests for the evaluation 

- Rolling test: friction model test
- Bouncing test: single-body elastic collision test
- 666 balls test: single-body hard contact test
- Elastic 666 balls test: single-body energy test
- ANYmal PD control test: articulated-robot-system speed test for quadrupedal robot
- Atlas PD control test: articulated-robot-system speed test for bipedal robot
- ANYmal momentum test: articulated-robot-system momentum test
- ANYmal energy test: articulated-robot-system energy test

Please see [our webpage](https://leggedrobotics.github.io/SimBenchmark/) for more details. 

<!-- ## Citation

If you want to refer the benchmark result in an academic publication, please consider citing as 
 -->
## Contact

- Note that this benchmark is done by Dongho Kang and Jemin Hwangho who are the developers of RaiSim.
- If you have any concern, please contact [Dongho Kang](mailto:kangd@ethz.ch)
