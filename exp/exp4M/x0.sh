#!/bin/sh
date
mkdir 0
cd 0; ../exp $1 0 32; cd ..
date
mkdir 1
cd 1; ../exp $1 1 32; cd ..
date
mkdir 2
cd 2; ../exp $1 2 32; cd ..
date
mkdir 3
cd 3; ../exp $1 3 32; cd ..
date
mkdir 0-1
cd 0-1
../exp 1 ../0/ ../1/
date
../exp 3 ../0/ ../1/
date
cd ..
mkdir 2-3
cd 2-3
../exp 1 ../2/ ../3/
date
../exp 3 ../2/ ../3/
date
cd ..
mkdir 0-3
cd 0-3
../exp 1 ../0-1/ ../2-3/
date
../exp 3 ../0-1/ ../2-3/
date
../exp $1 32 32
cd ..
