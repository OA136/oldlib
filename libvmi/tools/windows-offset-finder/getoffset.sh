#!/bin/bash
#$1 is domain_name of VM
g++ -o getGUID getGUID.cpp
./getGUID $1.dd | python downloadPDB.py $1
python pdb_name.py $1 | python dumpPDB.py -o $1.txt
python createConfig.py -f $1.txt
