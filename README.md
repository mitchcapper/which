Install
=======

For official release sources you will need an ANSI C compiler (like gcc) to compile this package.

Just type `make`, followed by `make install`.  See [INSTALL](INSTALL) for more details.

For git clones you will need to follow some basic maintainer instructions at the bottom for first time compiles.

History
=======

The main difference with version 1.0 by Paul Vixie is that this
version will not return directory names as being executables
and that by default it will expand a leading "./" and "~/" to
its full path on output.

The -all option has been added in example of a version of which
on Ultrix.  They use `-a' as option.

The --read-alias idea has been copied from a version of which by
Maarten Litmaath called `which-v6', he was using `-i' as option
which stands for `interactive'.

2.4 added better windows support.

Manual page
===========

See the [man page](MAN.md) for more details.


Maintainer Instructions
=======================
   If you are not using the release sources but are using the git
sources directly there are a couple extra steps to compile.

   Make sure you have all the git submodules installed by running
git submodule update --init --recursive
in the root of the project.

   Next run `autoreconf -si` in the which root to generate the
install scripts.

   You must run configure with `--enable-maintainer-mode` the first time
in order to generate the man file when you first run make, otherwise
make will not succeed.


# Binary Builds

For Windows binary builds and build scripts see [mitchcapper/WIN64LinuxBuild Scripts · GitHub](https://github.com/mitchcapper/WIN64LinuxBuild)
