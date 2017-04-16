#!/bin/bash
# mkimg.sh
# aoneill - 03/10/17

# Inspiration and aid taken from Finnbarr P. Murphy, at
# https://goo.gl/mjEqNI

# Tell the user of an action and preform it
function tell() { echo "$@"; $@; }

# Global parameters
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Grub requirements
GRUB_SRC=/usr/lib/grub/x86_64-unknown
LOOP_DEV=$(/usr/bin/losetup -f)
MNT=$(mktemp -d)

# Image requirements
IMAGE_FILE=$(mktemp).img
IMAGE_DIR=$(mktemp -d)
BOOT="/boot"
GRUB="${BOOT}/grub"

function make() {
  # Capture arguments
  kernel=$1
  output=$2

  # Setup the image
  tell dd if=/dev/zero of=${IMAGE_FILE} bs=512 count=2880
  tell mkdir -p "${IMAGE_DIR}${BOOT}" "${IMAGE_DIR}${GRUB}"

  # Copy legacy grub bootloader over
  tell cp "${GRUB_SRC}/stage1" "${IMAGE_DIR}${GRUB}"
  tell cp "${GRUB_SRC}/stage2" "${IMAGE_DIR}${GRUB}"
  tell cp "${DIR}/menu.lst"    "${IMAGE_DIR}${GRUB}"
  tell cp "$kernel"            "${IMAGE_DIR}${BOOT}"

  # Make an MSDOS filesystem
  tell /usr/bin/losetup "${LOOP_DEV}" "${IMAGE_FILE}"
  tell /usr/bin/mkdosfs "${LOOP_DEV}"

  # Deploy files to the img
  tell mount "${LOOP_DEV}" -o loop $MNT
  tell chmod 777 $MNT
  tell cp -aR "${IMAGE_DIR}/*" "$MNT"
  tell umount "$MNT"

  # Instruct grub to create the bootloader
  cat <<EOF | /usr/bin/grub --batch --device-map=/dev/null
device (fd0) ${LOOP_DEV}
root (fd0)
setup (fd0)
quit
EOF

  # Detach loop dev
  tell /usr/bin/losetup -d "${LOOP_DEV}"

  # Deploy and cleanup
  tell mv "${IMAGE_FILE}" "$output"
  tell rm -rf "${IMAGE_DIR}"
}

function usage() {
  cat << EOF
usage: $0 [KERNEL] [OUTPUT]"
EOF

  exit 1
}

[[ "$(whoami)" != "root" ]] && echo "err: Need root!" && usage
if [[ "$#" -eq "2" ]]; then
  make $1 $2
else
  usage
fi
