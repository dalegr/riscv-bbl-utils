# GitHub info about user
%global gituser     dalegr
%global gitname     riscv-bbl-utils

Summary:   A disassembly framework
Name:      riscv-bbl-utils
Version:   0.1
Release:   alt1
License:   LGPL v.3
Group:     Development/Tools
Url:       http://github.com/%{gituser}/%{gitname}
Source:    http://github.com/%{gituser}/%{gitname}/archive/v%{version}.zip
Packager:  Nikita Ermakov <arei@altlinux.org>

BuildRequires: gdisk

%description
This is the utils for RISC-V Berkeley Bootloader (BBL).

%prep
%setup

%build
DESTDIR=%{buildroot} CFLAGS="%{optflags}" make %{?_smp_mflags}

%install
DESTDIR=%{buildroot} make install

%changelog
* Tue Aug 28 2018 Nikita Ermakov <arei@altlinux.org> %{version}-%{release}
- Initial build for ALT Linux Sisyphus
