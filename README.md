RISC-V Trace profiler
=====================

### riscv-trace

- Install JUCE and the Projucer (link)
- JUCE requires some dependencies. In order to install them, run the command:

```
$ sudo apt update
$ sudo apt install libasound2-dev libjack-jackd2-dev \
	ladspa-sdk \
 	libcurl4-openssl-dev  \
  	libfreetype6-dev \
   	libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
    	libwebkit2gtk-4.0-dev \
     	libglu1-mesa-dev mesa-common-dev`
```

- Open the Projucer utility (located in JUCE folder)

In Projucer:
- Set up global paths to library:

	- File -> Global Paths...
	- Specify paths to JUCE and JUCE Modules (Where JUCE was installed by user)
	- Click Re-scan JUCE Modules
	- Close Global Paths window

- Open JUCE project file (File -> Open -> RISCV_Trace/riscv-trace/RISCV_Trace.jucer)
- Open Modules section and make sure that "Use global path" is ticked for each module. This is necessary for the Projucer to generate project files for Windows and macOS, as well as a Makefile for Linux.
- Save changes (File -> Save)

- In order to build the project, cd to RISCV_Trace/riscv-trace/Builds/LinuxMakefile, then type make

### riscv-isa-sim

**Build**

In file RISCV_Trace/riscv-isa-sim/riscv/processor.cc set TRACE_INFO_FILEPATH global variable to the trace save path. Decoded commands will be written there.

Set RISCV environment variable to the RISC-V tools install path.

```
$ apt-get install device-tree-compiler libboost-regex-dev
$ mkdir build
$ cd build
$ ../configure --prefix=$RISCV
$ make
$ [sudo] make install
```

### riscv-tests

Write the code of your program in file RISCV_Trace/riscv-tests/benchmarks/program/program_main.c

**Build**

Set RISCV environment variable to the RISC-V tools install path.

```
$ cd riscv-tests
$ autoconf
$ ./configure --prefix=$RISCV/target
$ make
$ make install
```

### Full usage pipeline


почему то файл с трассой не сохраняется в модифицированном спайке на линуксе...
