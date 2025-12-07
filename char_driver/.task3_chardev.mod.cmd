savedcmd_task3_chardev.mod := printf '%s\n'   task3_chardev.o | awk '!x[$$0]++ { print("./"$$0) }' > task3_chardev.mod
