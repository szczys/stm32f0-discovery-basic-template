
target remote localhost:3333
mon reset halt
mon wait_halt 500
break main
cont
