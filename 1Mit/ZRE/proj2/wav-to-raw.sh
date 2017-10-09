#!/bin/bash

cd data/

for i in {1..40};
do
    sox -t wav hello/$i.wav -t raw -r 8000 -c 1 -s hello-raw/$i.raw resample
done

for i in {1..46};
do
    sox -t wav no/$i.wav -t raw -r 8000 -c 1 -s no-raw/$i.raw resample
done

for i in {1..30};
do
    sox -t wav yes/$i.wav -t raw -r 8000 -c 1 -s yes-raw/$i.raw resample
done

cd ..