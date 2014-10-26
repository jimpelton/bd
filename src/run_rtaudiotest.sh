#!/bin/bash

export LD_LIBRARY_PATH=../../common/rtaudio/lib:$LD_LIBRARY_PATH

./a.out "$@"

