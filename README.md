# Majora's Mask: Recompiled Mod Template

This is an example mod for Majora's Mask: Recompiled that can be used as a template for creating mods. It has a basic build system, headers, sample code, and a mod config toml.

### Writing mods
See [this document](https://hackmd.io/fMDiGEJ9TBSjomuZZOgzNg) for an explanation of the modding framework, including how to write function patches and perform interop between different mods.

### Tools
You'll need to install `clang` and `make` to build this template.
* On Windows, using [chocolatey](https://chocolatey.org/) to install both is recommended. The packages are `llvm` and `make` respectively.
* On Linux, these can both be installed using your distro's package manager.
* On MacOS, these can both be installed using Homebrew. Apple clang won't work, as you need a mips target for building the mod code.

On Linux and MacOS, you'll need to also ensure that you have the `zip` utility installed.

You'll also need to build [N64Recomp](https://github.com/N64Recomp/N64Recomp) for the `RecompModTool` utility.

### Building
* First, run `make` (with an optional job count) to build the mod code itself.
* Oops, all done.
