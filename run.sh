#!/bin/bash

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

IMG="$DIR/build/bootfd.img"
SYM="$DIR/build/kernel"

LOG="$DIR/log_$(date +%s%3N)"

function tell() { echo "$@"; $@; }

function main() {
  # Parse command line
  args=
  [[ "$1" == "-v" ]] && shift 1 && args="-d int"

  # Make the peoject
  [[ "$(pwd)" != "$DIR" ]] && tell cd "$DIR"
  tell make

  [[ "$?" != "0" ]] && exit $?

  # Run QEMU
  qemu-system-i386 \
    -no-shutdown \
    -no-reboot \
    -serial file:"$LOG" \
    $args -s -S  \
    -drive file=$IMG,index=0,if=floppy,format=raw \
    2>/dev/null &

  q_pid=$!

  # Run the log listener
  tell touch "$LOG"
  termite --title="qemu-system-i386-log" --exec="tail -F $LOG" &

  l_pid=$!

  # Wait a little
  sleep 0.5

  # Setup GDB script
  GDB_SCRIPT=$(mktemp)
  cat << EOF > $GDB_SCRIPT
target remote localhost:1234
symbol-file $SYM

break kernel_main
break lbreak
break panic
continue
EOF

  # Run GDB
  echo "QEMU Running with pid=$q_pid"
  gdb -q --command $GDB_SCRIPT
  rm $GDB_SCRIPT

  # Kill programs
  [[ -d /proc/$l_pid ]] && tell kill -9 $l_pid
  [[ -d /proc/$q_pid ]] && tell kill -9 $q_pid
}

main $@
