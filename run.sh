#!/bin/bash

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

IMG="$DIR/build/bootfd.img"
SYM="$DIR/build/kernel"

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
    -serial file:log \
    $args -s -S  \
    -drive file=$IMG,index=0,if=floppy,format=raw \
    2>/dev/null &

  pid=$!

  # Wait a little
  sleep 0.5

  # Setup GDB script
  GDB_SCRIPT=$(mktemp)
  cat << EOF > $GDB_SCRIPT
target remote localhost:1234
symbol-file $SYM

break kernel_main
break lbreak
continue
EOF

  # Run GDB
  echo "QEMU Running with pid=$pid"
  gdb -q --command $GDB_SCRIPT

  # Kill QEMU
  [[ -d /proc/$pid ]] && tell kill -9 $pid
  rm $GDB_SCRIPT
}

main $@
