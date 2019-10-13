Name:           yafm
Version:        1.1
Release:        1%{?dist}
Summary:        Yet Another File Manager
License:        BSD4
Packager: 	    Reberti Carvalho Soares
URL:            https://github.com/KisuYami/YAFM
Source0:        yafm-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  libncurses6

%description
An file manager write in C

%prep
%setup -q

%build
%{make_build}

%install
%{make_install} PREFIX=%{_prefix}

%files
/usr/bin/yafm
