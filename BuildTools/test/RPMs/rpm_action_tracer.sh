#!/bin/bash
LOCAL_REPO_PATH=/home/subodh/CodeBase/LocalRepo

yum -y install rpmflowtracer
yum -y install dummy_dep_base-0.0.1 
yum -y install dummy_dep_base-0.0.2 
yum -y install dummy_dep_base-0.0.3 
yum -y install dummy_dep_base-0.0.4 
yum -y install dummy_dep_base-0.0.5 
yum -y install dummy_dep_base-0.0.6 

rm -f $LOCAL_REPO_PATH/{dummy_dep_base-0.0.2,dummy_dep_base-0.0.3,dummy_dep_base-0.0.4,dummy_dep_base-0.0.5}
createrepo --update $LOCAL_REPO_PATH
yum repolist
# Going from 0.0.6 => 0.0.1
yum downgrade dummy_dep_base
