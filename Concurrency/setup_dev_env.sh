# Copyright (c) 2016 
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#!/bin/bash 
ENV_FILE="platform.env"

#Supported Platforms list
P_X_86_64=x86_64 
P_X_86_32=x86_32 

#Default path on ubuntu 14.04 64 bit
DEF_SYS_ROOT="/"
DEF_SYS_INC="/usr/include"
DEF_SYS_LIB="/usr/lib"
DEF_TOOL_CHAIN_PATH="/usr/bin"
DEF_BOOST_PATH="${DEF_SYS_LIB}/boost"
DEF_POCO_PATH="${DEF_SYS_LIB}"
DEF_ZMQ_PATH="${DEF_SYS_LIB}"
DEF_PROTOC_PATH="/usr/local/include/google/protobuf/"
DEF_GSOAP_PATH="${DEF_SYS_LIB}"
CROSS_COMPILE=0

RESP_YES=Yes
RESP_NO=no
RESP_Y=Y
RESP_N=N
ERROR="[ERROR] : "
PATH_ERR_MSG="Path not found."
OPT_ERR_MSG="Invalid option provided."

error_exit(){
    echo " ${ERROR} $1"
    exit 
}

#path_finder var var_path
path_finder(){
    if [ ! -e $1 ]; then
        error_exit "${PATH_ERR_MSG}"
    fi
}

# File writing
env_generator(){
    CODE_GCC_VER_CMP48=$(echo `gcc -dumpversion | cut -f1-2 -d.` \>= 4.8 | bc)
    CODE_GCC_VERSION=$(echo `gcc -dumpversion`)
    echo "Writing to file ${ENV_FILE}"
    echo "######## ENVIRONMENT PATHS #################" >> ${ENV_FILE}
    echo "TARGET_PLATFORM=${TARGET_PLATFORM}" >> ${ENV_FILE}
    echo "CODE_BASE_DIR=${CODE_BASE_DIR}" >> ${ENV_FILE} 
    echo "TOOL_CHAIN_PATH=${TOOL_CHAIN_PATH}" >> ${ENV_FILE} 
    echo "CODE_SYS_ROOT=${CODE_SYS_ROOT}" >> ${ENV_FILE} 
    echo "CODE_GCC_VER_CMP48=${CODE_GCC_VER_CMP48}" >> ${ENV_FILE}
    echo "CODE_GCC_VERSION=${CODE_GCC_VERSION}" >> ${ENV_FILE}
    echo "CROSS_COMPILE=${CROSS_COMPILE}" >> ${ENV_FILE}
    echo "platform.env created succesfully !!!"
    echo -e $(cat platform.env)
}

# path_selector <Framework Name> <Default System Path> <custom libpath>
# Usage:  path_selector Boost ${DEF_SYS_LIB} stage/lib
path_selector(){
    echo "Enter path of base path of $1 Libraries [Default : $2]"
    read BASE_DIR
    if [ ! -z ${BASE_DIR} ]; then
        path_finder "${BASE_DIR}/$3"
        echo $1"_BASE_DIR=${BASE_DIR}/" >> ${ENV_FILE}
    fi
}

echo "======= Build enviroment creation script ============"

USER=$(echo `whoami`)
CODE_ARCH=$(uname -m)
if [ $USER == "root" ];then
    error_exit "Compiling as root is illegal !!!!"
elif [ $USER == "jenkins" ]; then
    echo "Using default settings for file generation"
    rm -f ./platform.env
    CODE_BASE_DIR=${PWD}
    #FIXME : How to convey cross compilation to automated build setup
    TARGET_PLATFORM=${CODE_ARCH}
    TOOL_CHAIN_PATH=${DEF_TOOL_CHAIN_PATH}
    CODE_SYS_ROOT=${DEF_SYS_ROOT} 
    DEF_BOOST_PATH=${DEF_SYS_LIB}/boost
else
    # Base Path
    echo "Please enter path for working directory [Default : ${PWD}]"
    read CODE_BASE_DIR
    if [ -z $CODE_BASE_DIR ];then
        CODE_BASE_DIR=${PWD}
    else
        path_finder "${CODE_BASE_DIR}"
    fi

    if [ -e "${CODE_BASE_DIR}/platform.env" ];then
        echo "platform.env already exists. Do you want to replace it ?? [${RESP_YES}/${RESP_NO}] : Default [${RESP_NO}]"
        read REPLACE
        if [[ ${REPLACE} == ${RESP_YES} ]]
        then
            echo "Copying older file to bk.platform.env"
            cp platform.env bk.platform.env
        else
            error_exit "Refer to README.txt."
        fi
    fi
    echo "#CREATION DATE : $(date) #" > ${ENV_FILE} 
    echo "######## BUILD SETUP ${HOSTNAME} #################" >> ${ENV_FILE}

    # Arch selection
    if [ ${CODE_ARCH} == ${P_X_86_64} ] ; then 
        echo "Input the target architecture: [${P_X_86_64} / ${P_X_86_32}]"
        read TARGET_PLATFORM
        if [[ ${TARGET_PLATFORM} != ${P_X_86_64} && ${TARGET_PLATFORM} !=  ${P_X_86_32} ]];then
            error_exit "${OPT_ERR_MSG}"
        fi
        if [ ${TARGET_PLATFORM} == ${P_X_86_32} ]; then
            CROSS_COMPILE=1
        fi
    fi

    #platform specific setup
    echo "Input the path for ${CODE_ARCH} Binutils. [Default : ${DEF_TOOL_CHAIN_PATH}]"
    read TOOL_CHAIN_PATH
    if [ -z ${TOOL_CHAIN_PATH} ]; then
        TOOL_CHAIN_PATH=${DEF_TOOL_CHAIN_PATH}
    else
        path_finder "${TOOL_CHAIN_PATH}"
    fi

    echo "Input the path for ${CODE_ARCH} Sysroot/Rootfs. [Default : ${DEF_SYS_ROOT}]" 
    read CODE_SYS_ROOT
    if [ -z CODE_SYS_ROOT ]; then
        CODE_SYS_ROOT=${DEF_SYS_ROOT} 
    else
        path_finder "${CODE_SYS_ROOT}"
    fi

    #Set Framework Paths
    path_selector "BOOST" ${DEF_SYS_LIB} "stage/lib/"
    path_selector "POCO" ${DEF_SYS_LIB} "lib/Linux/${CODE_ARCH}/"
    path_selector "ZMQ" ${DEF_SYS_LIB} ".libs/"
    path_selector "PROTO_BUFF" ${DEF_SYS_LIB} "src/.libs"

    # Third Party Libraries
    LD_CONFIG_FILE="CODEapp-x86_64.conf"
    
    echo "Do you want to update ${LD_CONFIG_FILE} ? [${RESP_YES}/${RESP_NO}] : Default [${RESP_NO}]"
    read RESPONSE
    if [[ ${RESPONE} == ${RESP_YES} ]];then
        echo "${DEF_BOOST_PATH}" >> ${LD_CONFIG_FILE}
    else 
        echo "No changes made to the file" 
    fi
fi

env_generator
