%include %{version_path}/versions.spec
Name: dummy_dependency_base
Version: %{dummy_base_version}
Release: 0
License: UFO Moviez all rights reserved.
Packager: ufo
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
#mkdir -p %{buildroot}/etc/systemd/system
mkdir -p %{buildroot}/tmp/
#cp /CodeBase/CBUpdate/rpm_build/test/ufo_test.service %{buildroot}/etc/systemd/system
#cp /CodeBase/CBUpdate/rpm_build/test/ufo_test_1.service %{buildroot}/tmp/
cp /CodeBase/CBUpdate/rpm_build/test/ufo_test_1.file %{buildroot}/tmp/
cp /CodeBase/CBUpdate/rpm_build/test/ufo_test_2.file %{buildroot}/tmp/

%files
#%attr(0644,root,root) /etc/systemd/system/ufo_test.service
#%attr(0644,root,root) /tmp/ufo_test_1.service
%attr(0644,root,root) /tmp/ufo_test_1.file
%attr(0644,root,root) /tmp/ufo_test_2.file

%pre
if [ "$1" -gt 0 ]
then
    echo "Install or Update : $1"
fi

%post
#systemctl enable ufo_test.service 
#systemctl daemon-reload 
#systemctl enable /tmp/ufo_test_1.service 

%preun
#%systemd_preun ufo_test.service 
if [ "$1" -eq 0 ]
then
    echo "Downgrade or remove : $1"
fi
#systemctl disable ufo_test_1.service 
#systemctl daemon-reload 

%postun
