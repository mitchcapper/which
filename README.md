Install
=======

You will need an ANSI C compiler (like gcc) to compile this package.

Just type `make`, followed by `make install`.

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



# Binary Builds

For Windows binary builds and build scripts see [mitchcapper/WIN64LinuxBuild Scripts · GitHub](https://github.com/mitchcapper/WIN64LinuxBuild)
