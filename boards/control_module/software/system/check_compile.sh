#!/bin/bash

ONLINE_HOST=github.com
REPO_URL=https://github.com/Droggelbecher/pinball.git
REPO_DIR=/home/pinball/pinball
LOCKFILE=/home/pinball/check_compile.lock

if [ -e $LOCKFILE ]; then
	echo "$LOCKFILE exists, not running compile/run check."
	exit 1;
fi

touch $LOCKFILE

if [ ! -e $LOCKFILE ]; then
	echo "$LOCKFILE does not exist right after touching it WTF?"
	exit 1;
fi

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

if [ "$(hostname)" != "raspberrypi" ]; then
	echo "Hostname is not 'pinball'. Sure you know what you are doing?"
	exit 1;
fi

if not running; then
	if [ -e $REPO_DIR ]; then
		cd $REPO_DIR
		git pull

	elif online; then
		mkdir -p $REPO_DIR
		git clone $REPO_URL $REPO_DIR
	fi

	cd $REPO_DIR/boards/control_module/software
	make
	sudo ./pinball_controller >/dev/null 2>&1
fi

rm $LOCKFILE

