%include %{version_path}/versions.spec
Name: dummy_dependency_main
Version: %{dummy_main_version} 
Release: 0
License: UFO Moviez all rights reserved.
Packager: ufo
Group: Development/Tools
Summary: This is dummy dependency base package.
#BuildArch: noarch

Requires(pre): dummy_dependency_base >= %{dummy_dependency_base}

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
