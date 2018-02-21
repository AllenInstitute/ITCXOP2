#!/bin/sh
symchk=/c/Program\ Files\ \(x86\)/Windows\ Kits/8.1/Debuggers/x64/symchk.exe

for i in `ls XOP/*.xop`
do
  MSYS_NO_PATHCONV=1 "${symchk}" ${i} /s PDB
done
