#!/bin/bash

if [ "$1" == "" ] 
then
    echo "USAGE: $0 <filename>"
    exit 1
fi

filename="$1"
outfile=`basename $filename '.png'`.h

docker run --rm \
    --name image-converter \
    --mount type=bind,source="$(pwd)",target=/convert \
    jjanzic/docker-python3-opencv python /convert/converter.py $filename \
    > $outfile

