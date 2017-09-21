#rpmbuild -v -bb --define "_topdir /tmp" --define "version <Major.Minor.Patch>" rpm_flow_tracer.spec
%include HelperMacros.def 
Name: rpmflowtracer 
Version: 0.0.7 
Release: 0
License: Creative commons. 
Packager: jinx 
Group: Development/Tools
Summary: Test package for tracing RPM workflow.
#BuildArch: i686 

#BuildRequires: %{nil}
BuildRoot: %{name}-%{version}

%global debug_package %{nil}

%description
%{summary}

%prep

%build

%files

%pretrans
%TracerMacro "[START]_pretrans_directive" 

%pre
%TracerMacro "pre_directive" 

%install

%files

%post
%TracerMacro "post_directive" 

%triggerin -- dummy_dependency_base 
%TracerMacro "triggerin1_directive" 

%triggerin -- rpmflowtracer < %{version}
%TracerMacro "triggerin2_directive" 

%triggerun --  dummy_dependency_base
%TracerMacro "triggerun1_directive" 

%triggerun --  rpmflowtracer < %{version}
%TracerMacro "triggerun2_directive" 

%preun 
%TracerMacro "preun_directive" 

%postun
%TracerMacro "postun_directive" 

%triggerpostun --  dummy_dependency_base
%TracerMacro "triggerpostun1_directive" 

%triggerpostun --  rpmflowtracer < %{version}
%TracerMacro "triggerpostun2_directive" 

%posttrans
%TracerMacro "[END]_posttrans_directive" 
