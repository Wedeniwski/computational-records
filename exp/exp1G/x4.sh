#!/bin/sh
cd 0-15
#date
#../exp2X 0 $1 4 4
date
../div2
date
../div3 exp-p1.tmp x.tmp
date
../exp2 0 $1 5 4
sh ../x5.sh $1
sh ../x5.sh $1
sh ../x5.sh $1
sh ../x5.sh $1
sh ../x5.sh $1
sh ../x5.sh $1
sh ../x5.sh $1
../exp2 0 $1 5 4
date
../exp2 0 $1 5 4
date
../exp2 $1
date
cd ..
