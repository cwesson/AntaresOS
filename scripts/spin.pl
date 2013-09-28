#!/usr/bin/perl

use Time::HiRes qw(usleep);
use sigtrap qw(handler int_handler INT);

my @spin = ("/", "-", "\\", "|");

while(1){
	foreach $char (@spin){
		print $char;
		$|=1;
		usleep(500000);
		print "\b";
	}
}

sub int_handler{
	print "\b\b\b";
	$|=1;
	exit(0);
}
