#!/usr/bin/env bash

HCopy -T 1 -C cfg/hcopy.conf -S scripts/train.scp
HCopy -T 1 -C cfg/hcopy.conf -S scripts/test.scp

HCompV -T 7 -I mlf/train.mlf -l YES -m -S scripts/train_htk.scp -M hmm0 proto/YES
HCompV -T 7 -I mlf/train.mlf -l NO -m -S scripts/train_htk.scp -M hmm0 proto/NO
HCompV -T 7 -I mlf/train.mlf -l HELLO -m -S scripts/train_htk.scp -M hmm0 proto/HELLO

HRest -T 7 -I mlf/train.mlf -l YES -S scripts/train_htk.scp -M hmm1 hmm0/YES
HRest -T 7 -I mlf/train.mlf -l NO -S scripts/train_htk.scp -M hmm1 hmm0/NO
HRest -T 7 -I mlf/train.mlf -l HELLO -S scripts/train_htk.scp -M hmm1 hmm0/HELLO
HParse net/oldnetwork net/network


HVite -T 1 -d hmm1 -S scripts/test_htk.scp -i mlf/testout.mlf -w net/network dics/dictionary lists/models
HResults -I mlf/test.mlf lists/models mlf/testout.mlf