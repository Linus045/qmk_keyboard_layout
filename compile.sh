#!/bin/bash

echo Sourcing python virtual environment
source ~/qmk/venv/bin/activate

KEYMAPS_DIR='./keyboards/splitkb/aurora/sofle_v2/rev1/keymaps'


qmk generate-autocorrect-data ${KEYMAPS_DIR}/Linus045/autocorrect_dictionary.txt || exit

echo 'Compiling keymap Linus045 for Sofle V2 and waiting to flash to device'
qmk flash -e CONVERT_TO=liatris -kb splitkb/aurora/sofle_v2/rev1 -km Linus045
