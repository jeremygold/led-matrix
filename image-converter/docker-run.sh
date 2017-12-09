#!/bin/bash

docker run --rm \
    --name image-converter \
    --mount type=bind,source="$(pwd)",target=/convert \
    jjanzic/docker-python3-opencv python /convert/converter.py

