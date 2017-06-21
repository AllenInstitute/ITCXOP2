#!/bin/sh

# This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

set -e

cp VC2015/itcXOP2.xop    ../../../XOPs-IP7/
cp VC2015/Release/itcXOP2.pdb    PDBs

cp VC2015/itcXOP2-64.xop ../../../XOPs-IP7-64bit/
cp VC2015/x64/Release/itcXOP2-64.pdb PDBs
