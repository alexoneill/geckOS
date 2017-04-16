# geckOS

A toy operating system with threading, paging, device drivers, and more!

`geckOS` is targeted for Intel IA32 x86 processors and is written primarily
in `C` and `x86 Assembly`. It runs best on a simulated `i386` system!

## Requirements

Complaining and using `geckOS` requires the following packages:

* [dosfstools](https://github.com/dosfstools/dosfstools): For formatting
  the final `geckOS` image filesystem
* [GRUB Legacy](https://www.gnu.org/software/grub/grub-legacy.html):
  For providing a bootloader for `geckOS` compatible for the targeted system
* [QEMU](http://www.qemu-project.org/): For simulating and testing `geckOS`
  - This may require installing additional emulator support for the `i386`
    system

The other dependencies should be standard across most Unix installs, such as
`gcc`, `make`, `gzip`, `sudo`, etc.

## Building geckOS

Building `geckOS` is as simple as the following:

```bash
cd geckOS/
make
```

Note, this will prompt you for your `sudo` password to run the GRUB
installation during the image build process.

If you are nervous about randomly cloning a Github project, trying to compile
it, only to be asked for `sudo`, you can look at what commands are run as
root by inspecting `mk/mkimg/mkimg.sh`.

You may need to tweak or modify the following paths:

* `GRUB_SRC` in `mk/mkimg/mkimg.sh`: This points to the location for
  GRUB Legacy's boot stages (this seems to work on Arch Linux with the
  [`grub-legacy`](https://aur.archlinux.org/packages/grub-legacy/) AUR package!)

