# DanpheOS

A small hobby Operating System developed out of learning curiosity.

The OS supports:

- Booting with Grub multi-bootloader
- 32-bit protected mode
- Paging and memory protection
- Fault handling
- Keyboard and VGA drivers
- Timer and interrupts handling
- Virtual and physical memory management is their lowest levels
- etc.


I mean to add some new features including multithreading and disk drivers some time in future.

The project is developed in C and assembly combined. It was initially built and tested in Windows through Bochs virtual emulator, but is being ported to be compiled in Linux systems and being tested on Qemu emulator.

### Old OS project

In the `old` branch in this git repository, you can find my old OS project that inspires this one. That one even features a *custom* Bootloader that can enable 32-bit and paging and load the kernel by itself in multi-stages. However due to wide support, use and power of Grub, I restarted the new project without a custom bootloader.
