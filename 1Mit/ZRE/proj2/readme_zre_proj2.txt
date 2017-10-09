Autor: Marek Marusic (xmarus05)
Email: xmarus05@stud.fit.vutbr.cz

Rozpoznavane slova:
YES, NO, HELLO

Data:
stiahnute po jednom z http://www.freesound.org a upravene na wav a potom na raw
frekvencia 8khz, 1 kanal, 16 bit na vzorek
Data su od roznych ludi a s roznymi intonaciami
YES - 30 vzorkov (20 train, 10 test)
NO - 46 vzorkov (36 train, 10 test)
HELLO - 40 vzorkov (30 train, 10 test)

Postup:
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


Vysledky (vysledky aj HVite, aby bolo vidiet, ze problem bol iba so slovami NO a HELLO):

$ HVite -T 1 -d hmm1 -S scripts/test_htk.scp -i mlf/testout.mlf -w net/network dics/dictionary lists/models
Read 3 physical / 3 logical HMMs
Read lattice with 6 nodes / 7 arcs
Created network with 11 nodes / 12 links
File: data/test/hello-raw/31.mfc
HELLO  ==  [36 frames] -69.7055 [Ac=-2509.4 LM=0.0] (Act=8.3)
File: data/test/hello-raw/32.mfc
HELLO  ==  [42 frames] -65.5031 [Ac=-2751.1 LM=0.0] (Act=8.4)
File: data/test/hello-raw/33.mfc
NO  ==  [65 frames] -78.5130 [Ac=-5103.3 LM=0.0] (Act=8.6)
File: data/test/hello-raw/34.mfc
HELLO  ==  [36 frames] -72.1174 [Ac=-2596.2 LM=0.0] (Act=8.3)
File: data/test/hello-raw/35.mfc
NO  ==  [73 frames] -67.3232 [Ac=-4914.6 LM=0.0] (Act=8.7)
File: data/test/hello-raw/36.mfc
HELLO  ==  [48 frames] -63.9321 [Ac=-3068.7 LM=0.0] (Act=8.5)
File: data/test/hello-raw/37.mfc
NO  ==  [42 frames] -79.9609 [Ac=-3358.4 LM=0.0] (Act=8.4)
File: data/test/hello-raw/38.mfc
HELLO  ==  [99 frames] -65.1435 [Ac=-6449.2 LM=0.0] (Act=8.8)
File: data/test/hello-raw/39.mfc
HELLO  ==  [90 frames] -64.4600 [Ac=-5801.4 LM=0.0] (Act=8.7)
File: data/test/hello-raw/40.mfc
HELLO  ==  [101 frames] -63.1645 [Ac=-6379.6 LM=0.0] (Act=8.8)
File: data/test/no-raw/37.mfc
HELLO  ==  [47 frames] -68.3100 [Ac=-3210.6 LM=0.0] (Act=8.5)
File: data/test/no-raw/38.mfc
NO  ==  [124 frames] -78.7574 [Ac=-9765.9 LM=0.0] (Act=8.8)
File: data/test/no-raw/39.mfc
NO  ==  [107 frames] -69.2091 [Ac=-7405.4 LM=0.0] (Act=8.8)
File: data/test/no-raw/40.mfc
NO  ==  [57 frames] -74.4856 [Ac=-4245.7 LM=0.0] (Act=8.6)
File: data/test/no-raw/41.mfc
HELLO  ==  [73 frames] -68.3712 [Ac=-4991.1 LM=0.0] (Act=8.7)
File: data/test/no-raw/42.mfc
HELLO  ==  [52 frames] -74.3589 [Ac=-3866.7 LM=0.0] (Act=8.5)
File: data/test/no-raw/43.mfc
NO  ==  [38 frames] -76.3729 [Ac=-2902.2 LM=0.0] (Act=8.4)
File: data/test/no-raw/44.mfc
NO  ==  [47 frames] -76.4305 [Ac=-3592.2 LM=0.0] (Act=8.5)
File: data/test/no-raw/45.mfc
NO  ==  [40 frames] -73.7394 [Ac=-2949.6 LM=0.0] (Act=8.4)
File: data/test/no-raw/46.mfc
YES  ==  [31 frames] -79.9727 [Ac=-2479.2 LM=0.0] (Act=8.2)
File: data/test/yes-raw/21.mfc
YES  ==  [31 frames] -79.7705 [Ac=-2472.9 LM=0.0] (Act=8.2)
File: data/test/yes-raw/22.mfc
YES  ==  [63 frames] -64.8271 [Ac=-4084.1 LM=0.0] (Act=8.6)
File: data/test/yes-raw/23.mfc
YES  ==  [74 frames] -58.1744 [Ac=-4304.9 LM=0.0] (Act=8.7)
File: data/test/yes-raw/24.mfc
YES  ==  [66 frames] -74.9134 [Ac=-4944.3 LM=0.0] (Act=8.6)
File: data/test/yes-raw/25.mfc
YES  ==  [85 frames] -70.2352 [Ac=-5970.0 LM=0.0] (Act=8.7)
File: data/test/yes-raw/26.mfc
YES  ==  [78 frames] -72.2726 [Ac=-5637.3 LM=0.0] (Act=8.7)
File: data/test/yes-raw/27.mfc
YES  ==  [64 frames] -72.3794 [Ac=-4632.3 LM=0.0] (Act=8.6)
File: data/test/yes-raw/28.mfc
YES  ==  [142 frames] -67.9604 [Ac=-9650.4 LM=0.0] (Act=8.8)
File: data/test/yes-raw/29.mfc
YES  ==  [86 frames] -70.6332 [Ac=-6074.5 LM=0.0] (Act=8.7)
File: data/test/yes-raw/30.mfc
YES  ==  [94 frames] -65.6106 [Ac=-6167.4 LM=0.0] (Act=8.7)

$ HResults -I mlf/test.mlf lists/models mlf/testout.mlf
====================== HTK Results Analysis =======================
  Date: Sun May  7 23:52:48 2017
  Ref : mlf/test.mlf
  Rec : mlf/testout.mlf
------------------------ Overall Results --------------------------
SENT: %Correct=76.67 [H=23, S=7, N=30]
WORD: %Corr=76.67, Acc=76.67 [H=23, D=0, S=7, I=0, N=30]
===================================================================

Zaver:
Rozpoznavanie fungovalo na slova YES velmi dobre, pretoze sa dostatocne lisi od ostatnych rozpoznavanych.
Pri NO a HELLO bol mensi problem, pretoze maju podobny tvar signalu a rozpoznavane data bolo narocne rozpoznat
aj pre cloveka. Data taktiez boli nahovorene s roznymi "naladami" a roznymi prizvukmi preto nebolo najjednoduchsie rozoznat niektore
z danych slov (napr hello-raw/33.raw nahovorila pravdepodobne osoba asijskeho povodu),
pri no-raw/37.raw som mal aj ja problem zo zaciatku odlisit ci sa jedna o HELLO alebo NO.