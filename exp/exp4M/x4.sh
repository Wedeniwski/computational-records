#!/bin/sh
date
mkdir 16
cd 16; ../exp $1 16 32; cd ..
date
mkdir 17
cd 17; ../exp $1 17 32; cd ..
date
mkdir 18
cd 18; ../exp $1 18 32; cd ..
date
mkdir 19
cd 19; ../exp $1 19 32; cd ..
date
mkdir 16-17
cd 16-17
../exp 1 ../16/ ../17/
date
../exp 3 ../16/ ../17/
date
cd ..
mkdir 18-19
cd 18-19
../exp 1 ../18/ ../19/
date
../exp 3 ../18/ ../19/
date
cd ..
mkdir 16-19
cd 16-19
../exp 1 ../16-17/ ../18-19/
date
../exp 3 ../16-17/ ../18-19/
date
../exp $1 32 32
cd ..
