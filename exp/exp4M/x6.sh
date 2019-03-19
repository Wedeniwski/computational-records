#!/bin/sh
date
mkdir 24
cd 24; ../exp $1 24 32; cd ..
date
mkdir 25
cd 25; ../exp $1 25 32; cd ..
date
mkdir 26
cd 26; ../exp $1 26 32; cd ..
date
mkdir 27
cd 27; ../exp $1 27 32; cd ..
date
mkdir 24-25
cd 24-25
../exp 1 ../24/ ../25/
date
../exp 3 ../24/ ../25/
date
cd ..
mkdir 26-27
cd 26-27
../exp 1 ../26/ ../27/
date
../exp 3 ../26/ ../27/
date
cd ..
mkdir 24-27
cd 24-27
../exp 1 ../24-25/ ../26-27/
date
../exp 3 ../24-25/ ../26-27/
date
../exp $1 32 32
cd ..
