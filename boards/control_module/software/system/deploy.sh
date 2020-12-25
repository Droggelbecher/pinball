#!/bin/bash

DEPLOY_HOST=10.0.0.42
DEPLOY_USER=pinball
DEPLOY_DIR=/home/pinball/pinball
DEPLOY_SCRIPT_DIR=/home/pinball

LOCAL_DIR=/home/henning/repos/pinball
TARGET=control_module

ssh ${DEPLOY_USER}@${DEPLOY_HOST} "sudo killall ${TARGET}; mkdir -p ${DEPLOY_DIR}"
rsync -CHaPx $LOCAL_DIR/ ${DEPLOY_USER}@${DEPLOY_HOST}:${DEPLOY_DIR}/ \
	--exclude datasheets/ \
	--exclude casing/ \
	--exclude obj/ \
	--exclude pinball_controller \
	--exclude control_module/software/control_module \
	--exclude Pinball \
	--exclude control_module/software/Makefile \
	--exclude CMakeFiles \
	--exclude CMakeCache.txt \
	--exclude cmake_install.cmake \
	--exclude .dep \
	--exclude 'scores.csv' \
	--exclude 'log-*' \
	--exclude '*.o' \
	--exclude '*.swp'
