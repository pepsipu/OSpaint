gdb -ex "set arch i386:intel" -ex "symbol-file build/bins/ospaint.bin" -ex "gef-remote -q localhost:1234" -ex "break paint_main" -ex "c"
