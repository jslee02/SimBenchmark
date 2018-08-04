#!/usr/bin/env bash

set -ex

sudo add-apt-repository ppa:ubuntu-toolchain-r/test

# Install DART
if [ $(lsb_release -sc) = "trusty" ]; then
  $SUDO apt-add-repository ppa:libccd-debs -y
  $SUDO apt-add-repository ppa:fcl-debs -y
fi
$SUDO apt-add-repository ppa:dartsim -y

$SUDO apt-get update -q

$SUDO apt-get install gcc-7 g++-7 -y

$SUDO apt-get install \
  libdart6-all-dev \
  libassimp-dev \
  libboost-all-dev \
  libeigen3-dev \
  libglew-dev \
  libglm-dev \
  libsdl2-dev \
  libsdl2-ttf-dev \
  libsoil-dev \
  libyaml-cpp-dev \
  -y
