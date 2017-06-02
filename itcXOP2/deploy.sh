#!/bin/sh

set -e

cp VC2015/itcXOP2.xop    ../../../XOPs-IP7/
cp VC2015/Release/itcXOP2.pdb    PDBs

cp VC2015/itcXOP2-64.xop ../../../XOPs-IP7-64bit/
cp VC2015/x64/Release/itcXOP2-64.pdb PDBs
