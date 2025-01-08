#!/usr/bin/bash
git clone https://gitlab.com/bztsrc/posix-uefi.git
cd source
ln -s ../posix-uefi/uefi
cd ..
cd posix-uefi/uefi
make
cd ..
