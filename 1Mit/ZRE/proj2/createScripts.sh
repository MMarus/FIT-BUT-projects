#!/usr/bin/env bash

ls data/train/*/*.raw > scripts/train.scp
ls data/test/*/*.raw > scripts/test.scp
#+ edited by hand added mfc