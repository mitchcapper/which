Summary: Displays where a particular program in your path is located.
Name: which
Version: 2.0
Release: 1
Copyright: distributable
Group: Applications/System
Source: ftp://metalab.unc.edu/pub/Linux/utils/shell/which-2.0.tar.gz
Prefix: /usr
Buildroot: /var/tmp/which-root

%description
The which command shows the full pathname of a specified program, if
the specified program is in your PATH.

%prep
%setup -n which

%build
make prefix=/usr

%install
make prefix=$RPM_BUILD_ROOT/usr install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%doc EXAMPLES README
/usr/bin/which
/usr/man/man1/which.1

%changelog
* Tue Apr 06 1999 Carlo Wood <carlo@runaway.xs4all.nl>
- which version 2.0
