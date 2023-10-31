#!/bin/sh
git remote add assignments-base https://github.com/cu-ecen-aeld/aesd-assignments.git
git fetch assignments-base
git merge assignments-base/master
git submodule update --init --recursive