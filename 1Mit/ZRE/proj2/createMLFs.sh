#!/bin/bash
echo \#\!MLF\!\# > mlf/train.mlf

./make_mlf.pl YES data/train/yes-raw/* >> mlf/train.mlf
./make_mlf.pl NO data/train/no-raw/* >> mlf/train.mlf
./make_mlf.pl HELLO data/train/hello-raw/* >> mlf/train.mlf

echo \#\!MLF\!\# > mlf/test.mlf
./make_mlf.pl YES data/test/yes-raw/* >> mlf/test.mlf
./make_mlf.pl NO data/test/no-raw/* >> mlf/test.mlf
./make_mlf.pl HELLO data/test/hello-raw/* >> mlf/test.mlf
