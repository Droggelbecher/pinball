#!/bin/bash

ONLINE_HOST=github.com
REPO_URL=https://github.com/Droggelbecher/pinball.git
REPO_DIR=/home/pinball/pinball
LOCKFILE=/home/pinball/check_compile.lock

LOGFILE=/home/pinball/log/check_compile.log

function online() {
	ping -c 4 $ONLINE_HOST
}

function running() {
	pgrep pinball_controller
}

function not() {
	if $1; then
		false
	else
		true
	fi
}

function log() {
	echo $(date +'[%Y-%m-%d %H:%M]') $@ >> $LOGFILE
}

function loglines() {
	while read n; do
		log $n
	done
}

log "$0 starting."

if [ "$(hostname)" != "raspberrypi" ]; then
	log "Hostname is not 'pinball'. Sure you know what you are doing?"
	exit 1;
fi

if [ -e $LOCKFILE ]; then
	log "$LOCKFILE exists, not running compile/run check."
	exit 1;
fi


touch $LOCKFILE

if not running; then
	log "Controller doesnt seem to be running."

	#if online; then
		#log "It seems we are online :-)"

		#if [ -e $REPO_DIR ]; then
			#log "Repo exists, pulling for news..."

			#cd $REPO_DIR
			#git pull

		#else
			#log "No repo found, cloning..."
			#mkdir -p $REPO_DIR
			#git clone $REPO_URL $REPO_DIR
		#fi
	#fi

	cd $REPO_DIR/boards/control_module/software

	log "Trying to compile..."
	make 2>&1 | loglines

	if not running; then
		log "Still not running, lets give it a go :-)"
		sudo ./pinball_controller 2>&1 | loglines
	fi
else
	log "Controller already running, not doing anything."
fi

rm $LOCKFILE

