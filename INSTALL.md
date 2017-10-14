# INSTALL GEMMA: Genome-wide Efficient Mixed Model Association

## Check version

Simply run gemma once installed

    gemma

and it should give you the version.

## GEMMA dependencies

GEMMA runs on Linux and MAC OSX and the runtime has the following
dependencies:

* C++ tool chain >= 4.9
* GNU Science library (GSL) 1.x (note that 2.x is not yet supported)
* blas/openblas
* lapack
* [Eigen3 library](http://eigen.tuxfamily.org/dox/)
* zlib

See below for installation on Guix.

## Install GEMMA

### Debian and Ubuntu

Travis-CI uses Ubuntu for testing. Check the test logs for version numbers.

[![Build Status](https://travis-ci.org/genetics-statistics/GEMMA.svg?branch=master)](https://travis-ci.org/genetics-statistics/GEMMA)

Current settings can be found in [travis.yml](.travis.yml).

### Bioconda

(Note Bioconda install is a work in [progress](https://github.com/genetics-statistics/GEMMA/issues/52)

Recent versions of GEMMA can be installed with
[BioConda](http://ddocent.com/bioconda/) without root permissions using the following
command

    conda install gemma

### GNU Guix

The GNU Guix package manager can install recent versions of [GEMMA](https://www.gnu.org/software/guix/packages/g.html)
using the following command

    guix package -i gemma

To build GEMMA from source you can opt to install the build tools with GNU Guix

    guix package -i make gcc linux-libre-headers gsl eigen openblas lapack glibc ld-wrapper

### Install from source

Install listed dependencies and run

	make -j 4

(the -j switch builds on 4 cores).

if you get an Eigen error you may need to override the include
path. E.g. to build GEMMA on GNU Guix with shared libs the following
may work

    make EIGEN_INCLUDE_PATH=~/.guix-profile/include/eigen3 WITH_OPENBLAS=1

another example overriding optimization and LIB flags (so as to link
against gslv1) would be

    make EIGEN_INCLUDE_PATH=~/.guix-profile/include/eigen3 WITH_OPENBLAS=1 GCC_FLAGS="-Wall" LIBS="$HOME/opt/gsl1/lib/libgsl.a $HOME/opt/gsl1/lib/libgslcblas.a -L$HOME/.guix-profile/lib -pthread -llapack -lblas -lz"

to run GEMMA tests

	time make check

You can run gemma in the debugger with, for example

	gdb --args \
		./bin/gemma -g example/mouse_hs1940.geno.txt.gz \
		-p example/mouse_hs1940.pheno.txt -a example/mouse_hs1940.anno.txt \
		-snps example/snps.txt -nind 400 -loco 1 -gk -debug -o myoutput

Note that if you get <optimized out> warnings on inspecting variables you
should compile with GCC_FLAGS="" to disable optimizations (-O3). E.g.

    make EIGEN_INCLUDE_PATH=~/.guix-profile/include/eigen3 WITH_OPENBLAS=1 GCC_FLAGS=

Other options, such as compiling with warnings, are listed in the
Makefile.

## Run tests

GEMMA includes the shunit2 test framework (version 2.0).

    make check

or

    ./run_tests.sh

## Optimizing performance

### OpenBlas

Linking against a built-from-source OpenBlas is a first optimization
step because it will optimize code for the local architecture (on my
workstation it easily doubles speed). When you check the output .log
file of GEMMA after a run, it will tell you how the linked-in OpenBlas
was compiled.

To link a new version, compile OpenBlas as per
[instructions](http://www.openblas.net/).  You can start with the default:

    make -j 4

or play with the switches

    make USE_THREAD=1 NUM_THREADS=16 -j 4

rendering for example:

        OpenBLAS build complete. (BLAS CBLAS)
        OS               ... Linux
        Architecture     ... x86_64
        BINARY           ... 64bit
        C compiler       ... GCC  (command line : gcc)
        Library Name     ... libopenblas_haswellp-r0.3.0.dev.a (Multi threaded; Max num-threads is 16)

        To install the library, you can run "make PREFIX=/path/to/your/installation install".

This generates a static library which you can link using the full path
with using the GEMMA Makefile:

    time env OPENBLAS_NUM_THREADS=4 make EIGEN_INCLUDE_PATH=~/.guix-profile/include/eigen3 LIBS="~/tmp/OpenBLAS/libopenblas_haswellp-r0.3.0.dev.a -lgsl -lgslcblas -pthread -lz  -llapack" WITH_OPENBLAS=1 -j 4 fast-check

    make EIGEN_INCLUDE_PATH=~/.guix-profile/include/eigen3 LIBS="~/tmp/OpenBLAS/libopenblas_haswellp-r0.3.0.dev.a -lgsl -lgslcblas -pthread -lz  -llapack" WITH_OPENBLAS=1 -j 4 unittests

NOTE: we should make this easier.
