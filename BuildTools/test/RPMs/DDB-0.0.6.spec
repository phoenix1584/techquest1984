%include HelperMacros.def
Name: dummy_dependency_base
Version: 0.0.6 
Release: 0
License: creative commons.
Packager: jinx 
Group: Development/Tools
Summary: This is dummy dependency base package.
#BuildArch: noarch

%{?systemd_requires}
BuildRequires: systemd
BuildRoot: /CodeBase/CBUpdate/rpm_build/cbsoftware_pkg/tmp/%{name}-%{version}

%global debug_package %{nil}

%description
%{summary}

%prep

%build

%install
mkdir -p %{buildroot}/tmp/DDB-tests/
cp /tmp/file_3.txt %{buildroot}/tmp/DDB-tests/  

%files
%dir %attr(0644,root,root) /tmp/DDB-tests 
%attr(0644,root,root) /tmp/DDB-tests/file_3.txt

%pre
%DeployTraceMacro Install_or_upgrade $1

%post
%DeployTraceMacro Install_or_upgrade $1

%preun
%DeployTraceMacro Downgrade_or_remove $1

%postun
%DeployTraceMacro Downgrade_or_remove $1
