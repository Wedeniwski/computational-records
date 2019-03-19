#!/bin/sh
date
mkdir 20
cd 20; ../exp $1 20 32; cd ..
date
mkdir 21
cd 21; ../exp $1 21 32; cd ..
date
mkdir 22
cd 22; ../exp $1 22 32; cd ..
date
mkdir 23
cd 23; ../exp $1 23 32; cd ..
date
mkdir 20-21
cd 20-21
../exp 1 ../20/ ../21/
date
../exp 3 ../20/ ../21/
date
cd ..
mkdir 22-23
cd 22-23
../exp 1 ../22/ ../23/
date
../exp 3 ../22/ ../23/
date
cd ..
mkdir 20-23
cd 20-23
../exp 1 ../20-21/ ../22-23/
date
../exp 3 ../20-21/ ../22-23/
date
../exp $1 32 32
cd ..
