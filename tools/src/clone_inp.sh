#! /bin/bash


if [ $# -ne 2 -o  ! -f $1  ]; then
    echo "usage: $0 file.inp Ninps"
    exit 1
fi

inp=$1
n=$2

for i in `seq 0 $n`; do ~/fluka/calet/macros/clone_inp.py $inp `basename $inp .inp`"$i"_.inp; done

echo "do not forget to remove $inp"
