Summary: Displays where a particular program in your path is located.
Name: which
Version: 2.5
Release: 1
Copyright: distributable
Group: Applications/System
Source: ftp://ftp.gnu.org/gnu/which/which-2.5.tar.gz
Prefix: /usr
Buildroot: /var/tmp/which-root

%description
The which command shows the full pathname of a specified program, if
the specified program is in your PATH.

%prep
%setup

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=/usr
make

%install
make prefix=$RPM_BUILD_ROOT/usr install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%doc EXAMPLES README
/usr/bin/which
/usr/man/man1/which.1

%changelog
* Tue May 14 1999 Carlo Wood <carlo@runaway.xs4all.nl>
- which version 2.5
- Moved assignment of CFLAGS to the configure line, using RPM_OPT_FLAGS now.

* Tue May 13 1999 Carlo Wood <carlo@runaway.xs4all.nl>
- which version 2.4

* Tue May 10 1999 Carlo Wood <carlo@runaway.xs4all.nl>
- which version 2.3

* Tue Apr 16 1999 Carlo Wood <carlo@runaway.xs4all.nl>
- which version 2.2

* Tue Apr 09 1999 Carlo Wood <carlo@runaway.xs4all.nl>
- which version 2.1

* Tue Apr 06 1999 Carlo Wood <carlo@runaway.xs4all.nl>
- which version 2.0
