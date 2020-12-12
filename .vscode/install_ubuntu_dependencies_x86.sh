#!/bin/bash  

apt install gcc
apt install g++
apt install clang

dpkg --add-architecture i386
apt-get update

apt install libc6-dev:i386
apt install libgcc-5-dev:i386
apt install libstdc++-5-dev:i386





