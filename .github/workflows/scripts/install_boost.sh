#!/bin/bash

# yum install -y wget
pwd
cd /
mkdir boost
cd boost
wget --no-check-certificate 'https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz'
tar xf boost_1_82_0.tar.gz
cd boost_1_82_0
./bootstrap.sh
./b2
export BOOST_ROOT="/boost/boost_1_82_0"