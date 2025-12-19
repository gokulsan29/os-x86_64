## Sources used
- The base setup was done using https://wiki.osdev.org/Limine_Bare_Bones

## Dependencies
```
sudo apt-get install qemu-system xorriso clang
```

# How to Build
1. Clone the repository `git clone https://github.com/gokulsan29/os-x86_64.git --recursive`
  - Alternatively
  ```
  git clone https://github.com/gokulsan29/os-x86_64.git
  git submodule init
  git submodule update
  ```
2. Run `make` to build the ISO image. `make all-hdd` can be used to build the HDD image
3. To run the OS using `qemu`:
```
make run # runs the OS using BIOS as firmware and an ISO image
make run-hdd # runs the OS using BIOS as firmware and a HDD image
```

