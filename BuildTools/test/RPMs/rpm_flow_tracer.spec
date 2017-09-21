#rpmbuild -v -bb --define "_topdir /tmp" --define "version <Major.Minor.Patch>" rpm_flow_tracer.spec
%include HelperMacros.def 
Name: rpmflowtracer 
Version: 0.0.1 
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

%preun 
%TracerMacro "preun_directive" 

%postun
%TracerMacro "postun_directive" 

%posttrans
%TracerMacro "[END]_posttrans_directive" 

# Triggers for DDB-0.0.1
%triggerin -- dummy_dependency_base = 0.0.1 
%TracerMacro "trigger_install_DDB_0.0.1" 

%triggerun --  dummy_dependency_base = 0.0.1
%TracerMacro "trigger_uninstall_DDB_0.0.1" 

%triggerpostun --  dummy_dependency_base = 0.0.1
%TracerMacro "trigger_post_uninstall_DDB_0.0.1" 

# Triggers for DDB-0.0.2
%triggerin -- dummy_dependency_base = 0.0.2 
%TracerMacro "trigger_install_DDB_0.0.2" 

%triggerun --  dummy_dependency_base = 0.0.2
%TracerMacro "trigger_uninstall_DDB_0.0.2" 

%triggerpostun --  dummy_dependency_base = 0.0.2
%TracerMacro "trigger_post_uninstall_DDB_0.0.2" 

# Triggers for DDB-0.0.3
%triggerin -- dummy_dependency_base = 0.0.3 
%TracerMacro "trigger_install_DDB_0.0.3" 

%triggerun --  dummy_dependency_base = 0.0.3
%TracerMacro "trigger_uninstall_DDB_0.0.3" 

%triggerpostun --  dummy_dependency_base = 0.0.3

# Triggers for DDB-0.0.4
%triggerin -- dummy_dependency_base = 0.0.4 
%TracerMacro "trigger_install_DDB_0.0.4" 

%triggerun --  dummy_dependency_base = 0.0.4
%TracerMacro "trigger_uninstall_DDB_0.0.4" 

%triggerpostun --  dummy_dependency_base = 0.0.4
%TracerMacro "trigger_post_uninstall_DDB_0.0.4" 

# Triggers for DDB-0.0.5
%triggerin -- dummy_dependency_base = 0.0.5 
%TracerMacro "trigger_install_DDB_0.0.5" 

%triggerun --  dummy_dependency_base = 0.0.5
%TracerMacro "trigger_uninstall_DDB_0.0.5" 

%triggerpostun --  dummy_dependency_base = 0.0.5
%TracerMacro "trigger_post_uninstall_DDB_0.0.5" 

# Triggers for DDB-0.0.6
%triggerin -- dummy_dependency_base = 0.0.6 
%TracerMacro "trigger_install_DDB_0.0.6" 

%triggerun --  dummy_dependency_base = 0.0.6
%TracerMacro "trigger_uninstall_DDB_0.0.6" 

%triggerpostun --  dummy_dependency_base = 0.0.6
%TracerMacro "trigger_post_uninstall_DDB_0.0.6" 
%TracerMacro "trigger_post_uninstall_DDB_0.0.6" 
