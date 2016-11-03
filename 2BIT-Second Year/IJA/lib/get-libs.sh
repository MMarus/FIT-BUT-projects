#!/usr/bin/env bash
BASEDIR=$(dirname $0)
echo $BASEDIR
wget -O $BASEDIR/data.zip    http://www.stud.fit.vutbr.cz/~xjezov01/data.zip
unzip $BASEDIR/data.zip -d $BASEDIR
echo "libs downloaded"