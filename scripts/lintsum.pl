#!/usr/bin/perl

open(IN, "<", $ARGV[0]);

my %list = ();

my $file = "";
my $count = 0;
foreach my $line (<IN>){
	if($line =~ /\< checking (.*) \>/){
		$file = $1;
		$count = 0;
	}elsif($line =~ /Finished checking --- (\d+) code warnings/){
		$count = int($1);
		printf "%-40s %d\n", $file, $count;
	}elsif($line =~ "Finished checking --- no warnings"){
		$count = 0;
		printf "%-40s %d\n", $file, $count;
	}elsif($line =~ /\*\*\* Cannot continue\./){
		printf "%-40s Error\n", $file, $count;
	}
}
