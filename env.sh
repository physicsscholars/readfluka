#! /bin/bash

export READFLUKA_SYS=$PWD/libReadFluka
export LD_LIBRARY_PATH=$READFLUKA_SYS:$LD_LIBRARY_PATH
export PATH=$PATH:$READFLUKA_SYS/../tools
