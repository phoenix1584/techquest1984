#!/bin/bash
LOCAL_REPO_PATH=/home/subodh/CodeBase/LocalRepo
yum -y remove rpmflowtracer dummy_dependency_base
sleep 2
echo "$(date) : Starting Sequence"
mcho "############################################[ START - TRANSACTION BOUNDARY ]################################################################"
yum -y install rpmflowtracer
echo "############################################[ END - TRANSACTION BOUNDARY ]################################################################"
sleep 2
echo "############################################[ START - TRANSACTION BOUNDARY ]################################################################"
yum -y install dummy_dependency_base-0.0.1 
echo "############################################[ END - TRANSACTION BOUNDARY ]################################################################"
sleep 2
echo "############################################[ START - TRANSACTION BOUNDARY ]################################################################"
yum -y install dummy_dependency_base-0.0.2 
echo "############################################[ END - TRANSACTION BOUNDARY ]################################################################"
sleep 2
echo "############################################[ START - TRANSACTION BOUNDARY ]################################################################"
yum -y install dummy_dependency_base-0.0.3 
echo "############################################[ END - TRANSACTION BOUNDARY ]################################################################"
sleep 2
echo "############################################[ START - TRANSACTION BOUNDARY ]################################################################"
yum -y install dummy_dependency_base-0.0.4 
echo "############################################[ END - TRANSACTION BOUNDARY ]################################################################"
sleep 2
echo "############################################[ START - TRANSACTION BOUNDARY ]################################################################"
yum -y install dummy_dependency_base-0.0.5 
echo "############################################[ END - TRANSACTION BOUNDARY ]################################################################"
sleep 2
echo "############################################[ START - TRANSACTION BOUNDARY ]################################################################"
yum -y install dummy_dependency_base-0.0.6 
echo "############################################[ END - TRANSACTION BOUNDARY ]################################################################"
sleep 2

rm -f $LOCAL_REPO_PATH/{dummy_dependency_base-0.0.2*.rpm,dummy_dependency_base-0.0.3*.rpm,dummy_dependency_base-0.0.4*.rpm,dummy_dependency_base-0.0.5*.rpm}
createrepo --update $LOCAL_REPO_PATH
yum repolist
# Going from 0.0.6 => 0.0.1
sleep 2
echo "############################################[ START - TRANSACTION BOUNDARY ]################################################################"
yum -y downgrade dummy_dependency_base
echo "############################################[ END - TRANSACTION BOUNDARY ]################################################################"
echo "$(date) : Finished Sequence"
