#!/bin/sh

make clean && make all
./main
open "output.tga"