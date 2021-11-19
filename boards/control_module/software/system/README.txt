

# 1. Configure network

Set local IP Address to eg 10.0.0.1
Pinball has addr 10.0.0.42 ssh pinball/pinball

# 2. Deploy

Run deploy.sh to copy over the sourcecode

# 3. Compile

Log into to pinball machine go to /home/pinball/sw and run

dub build -b release

Hint: build as "pinball" user, that one has all the dub packages

# 4. Run

./control_module

Hint: Run only as root b/c access to BCM device

# Cron job (outdated)


Run check_compile.sh automatically via cron to download the latest
release and run it:

Put into /etc/crontab:

	*/1 * * * * pinball	/home/pinball/check_compile.sh >/home/pinball/log/cron.log 2>&1


Allow the pinball user to run the pinball_controller software as root
(necessary for some permission thingies):


Put into /etc/sudoers:

	pinball ALL=(ALL) NOPASSWD: /home/pinball/pinball/boards/control_module/software/pinball_controller
	pinball ALL=(ALL) NOPASSWD: /usr/bin/killall pinball_controller


Put into /etc/rc.local:

	rm /home/pinball/check_compile.lock

