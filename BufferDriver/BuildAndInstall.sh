#!/bin/sh
# This script cleans the folder, makes the project, removes the BufferDriver if it's installed, and then installs the 
# new BufferDriver.

make clean
make
sudo rmmod BufferDriverReader
sudo rmmod BufferDriverWriter
sudo insmod BufferDriverWriter.ko
sudo insmod BufferDriverReader.ko