#!/bin/sh
date
mkdir 12
cd 12; ../exp $1 12 32; cd ..
date
mkdir 13
cd 13; ../exp $1 13 32; cd ..
date
mkdir 14
cd 14; ../exp $1 14 32; cd ..
date
mkdir 15
cd 15; ../exp $1 15 32; cd ..
date
mkdir 12-13
cd 12-13
../exp 1 ../12/ ../13/
date
../exp 3 ../12/ ../13/
date
cd ..
mkdir 14-15
cd 14-15
../exp 1 ../14/ ../15/
date
../exp 3 ../14/ ../15/
date
cd ..
mkdir 12-15
cd 12-15
../exp 1 ../12-13/ ../14-15/
date
../exp 3 ../12-13/ ../14-15/
date
../exp $1 32 32
cd ..
