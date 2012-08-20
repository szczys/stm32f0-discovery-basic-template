
# Specify remote target
target remote localhost:3333

# Reset to known state
monitor reset halt
monitor wait_halt 500

# Set a breakpoint at main().
break main

# Run to the breakpoint.
continue
