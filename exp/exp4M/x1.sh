#!/bin/sh
date
mkdir 4
cd 4; ../exp $1 4 32; cd ..
date
mkdir 5
cd 5; ../exp $1 5 32; cd ..
date
mkdir 6
cd 6; ../exp $1 6 32; cd ..
date
mkdir 7
cd 7; ../exp $1 7 32; cd ..
date
mkdir 4-5
cd 4-5
../exp 1 ../4/ ../5/
date
../exp 3 ../4/ ../5/
date
cd ..
mkdir 6-7
cd 6-7
../exp 1 ../6/ ../7/
date
../exp 3 ../6/ ../7/
date
cd ..
mkdir 4-7
cd 4-7
../exp 1 ../4-5/ ../6-7/
date
../exp 3 ../4-5/ ../6-7/
date
../exp $1 32 32
cd ..
