%include HelperMacros.def
Name: dummy_dependency_main
Version: 0.0.1 
Release: 0
License: Creative Commons 
Packager: jinx 
Group: Development/Tools
Summary: This is dummy dependency base package.
#BuildArch: noarch

Requires: dummy_dependency_base 

#BuildRequires: %{nil}
BuildRoot: /CodeBase/CBUpdate/rpm_build/cbsoftware_pkg/tmp/%{name}-%{version}

%global debug_package %{nil}

%description
%{summary}

%prep

%build

%install

%files

%pre

%post

%preun

%postun
