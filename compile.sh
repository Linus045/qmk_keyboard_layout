echo Sourcing python virtual environment
source ~/qmk/venv/bin/activate

KEYMAPS_DIR='./keyboards/splitkb/aurora/sofle_v2/rev1/keymaps'


echo Converting JSON keymap to C
qmk json2c -o "$KEYMAPS_DIR/Linus045/keymap.c" "$KEYMAPS_DIR/Linus045/linus045_keymap.json"

echo Appending keymap_extras.c to keymap.c
cat "$KEYMAPS_DIR/Linus045/keymap_extras.c" >> "$KEYMAPS_DIR/Linus045/keymap.c"

echo Compiling keymap Linus045 for Sofle V2 and waiting to flash to device
qmk flash -e CONVERT_TO=liatris -kb splitkb/aurora/sofle_v2/rev1 -km Linus045

echo Deleting generated keymap.c
rm "$KEYMAPS_DIR/Linus045/keymap.c"
