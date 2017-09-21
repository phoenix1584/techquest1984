#!/bin/bash

# Step 1  Generate the RPM Flow tracer (RFT.rpm) that will monitor the events of dummy_dep_base (DDB.rpm) rpms workflows.
# Step 2  Create file file_1.txt with text DDB-0.0.1 that will be part of the rpm DDB-0.0.1.rpm generated using DDB-0.0.1.spec
# Step 3  Generate DDB-0.0.1.rpm
# Step 4  Create file_2.txt with text DDB-0.0.2 to be packaged in DDB-0.0.2 along with "unmodified" file_1.txt
# Step 5  Generate DDB-0.0.2.rpm using DDB-0.0.2.spec 
# Step 6  Append file_1.txt with text DDB-0.0.3 to be packaged in DDB-0.0.3 along with "unmodified" file_2.txt
# Step 7  Generate DDB-0.0.3.rpm using DDB-0.0.3.spec
# Step 8  Create file_3.txt with text DDB-0.0.4 to be packaged in CCB-0.0.4 with unmodified file_2.txt and NO file_1.txt (deprecation).
# Step 9  Generate DDB-0.0.4.rpm using DDB-0.0.4.spec
# Step 10 Append text of file_2.txt AND file_3.txt with DDB-0.0.5 
# Step 11 Generate DDB-0.0.5.rpm using DDB-0.0.5.spec
# Step 12 Generate DDB-0.0.6.rpm using DDB-0.0.6.spec

function RPMBuilder(){
	echo "Builder invoked for spec file $1"
	rpmbuild -bb --define "_topdir  /home/subodh/CodeBase/rpm_factory" $1
}

rm -f /tmp/file_*.txt
#1
RPMBuilder rpm_flow_tracer.spec 

#2
echo "$(date) : DDB-0.0.1" >> /tmp/file_1.txt
sleep 5

#3
RPMBuilder DDB-0.0.1.spec

#4 
sleep 5
echo "$(date) : DDB-0.0.2" >> /tmp/file_2.txt

#5
RPMBuilder DDB-0.0.2.spec

#6
sleep 5
echo "$(date) : DDB-0.0.3" >> /tmp/file_1.txt

#7
RPMBuilder DDB-0.0.3.spec

#8
sleep 5
echo "$(date) : DDB-0.0.4" >> /tmp/file_3.txt

#9
RPMBuilder DDB-0.0.4.spec

#10
sleep 5
echo "$(date) : DDB-0.0.5" >> /tmp/file_2.txt
echo "$(date) : DDB-0.0.5" >> /tmp/file_3.txt

#11
RPMBuilder DDB-0.0.5.spec

#12
RPMBuilder DDB-0.0.6.spec
