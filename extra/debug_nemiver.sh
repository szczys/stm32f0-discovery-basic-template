#!/bin/bash

# location of OpenOCD Board .cfg files (only used with 'make program')
OPENOCD_BOARD_DIR=/usr/share/openocd/scripts/board

# start xterm with openocd in the background
xterm -e openocd -f $OPENOCD_BOARD_DIR/stm32f0discovery.cfg -f extra/stm32f0-openocd.cfg -f extra/debug.cfg &

# save the PID of the background process
XTERM_PID=$!

# wait a bit to be sure the hardware is ready
sleep 2

# execute some initialisation commands via gdb
arm-none-eabi-gdb --batch --command=extra/run.gdb main.elf

# start the gdb gui
nemiver --remote=localhost:3333 --gdb-binary="$(which arm-none-eabi-gdb)" main.elf

# close xterm when the user has exited nemiver
kill $XTERM_PID
