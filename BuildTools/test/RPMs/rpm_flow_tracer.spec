#rpmbuild -v -bb --define "_topdir /tmp" --define "version <Major.Minor.Patch>" rpm_flow_tracer.spec
Name: rpmflowtracer 
Version: %{version} 
Release: 0
License: Test Licence.
Packager: Test 
Group: Development/Tools
Summary: Test package for tracing RPM workflow.
#BuildArch: i686 

#BuildRequires: %{nil}
BuildRoot: %{name}-%{version}
%define log_file /tmp/rpm_trace.txt

%global debug_package %{nil}

%description
%{summary}

%prep

%build

%files

%pretrans
rm -f %{log_file}
echo "[START] $(date) %{version} pretrans directive" >> %{log_file}

%pre
echo "$(date) %{version} pre directive" >> %{log_file}

%install

%files

%post
echo "$(date) %{version} post directive" >> %{log_file}

%triggerin -- cbsm 
echo "$(date) %{version} triggerin1 directive" >> %{log_file}

%triggerin -- cbsm
echo "$(date) %{version} triggerin2 directive" >> %{log_file}

%triggerun -- cbsm
echo "$(date) %{version} triggerun1 directive" >> %{log_file}

%triggerun -- cbsm
echo "$(date) %{version} triggerun2 directive" >> %{log_file}

%preun 
echo "$(date) %{version} preun directive" >> %{log_file}

%postun
echo "$(date) %{version} postun directive" >> %{log_file}

%triggerpostun -- cbsm
echo "$(date) %{version} triggerpostun1 directive" >> %{log_file}

%triggerpostun -- cbsm
echo "$(date) %{version} triggerpostun2 directive" >> %{log_file}

%posttrans
echo "[END] $(date) %{version} posttrans directive" >> %{log_file}
