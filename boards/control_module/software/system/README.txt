
Run check_compile.sh automatically via cron to download the latest
release and run it:

Put into /etc/crontab:

*/1 * * * * pinball	/home/pinball/check_compile.sh >/home/pinball/log/cron.log 2>&1


Allow the pinball user to run the pinball_controller software as root
(necessary for some permission thingies):

Put into /etc/sudoers:

pinball ALL=(ALL) NOPASSWD: /home/pinball/pinball/boards/control_module/software/pinball_controller



