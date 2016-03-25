#!/bin/sh

DEPLOY_HOST = 10.0.0.42
DEPLOY_USER = pinball
DEPLOY_DIR = /home/pinball/pinball
DEPLOY_SCRIPT_DIR = /home/pinball

ssh ${DEPLOY_USER}@${DEPLOY_HOST} "sudo killall ${TARGET}; mkdir -p ${DEPLOY_DIR}"
rsync -CHaPx ../../../ ${DEPLOY_USER}@${DEPLOY_HOST}:${DEPLOY_DIR}/ --exclude datasheets/ --exclude casing/ --delete --exclude obj/ --exclude pinball_controller
