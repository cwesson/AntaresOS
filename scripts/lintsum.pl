#!/usr/bin/perl

my $file = "";
my $line = -1;
my %count = ();
foreach my $in(<STDIN>){
	if($in =~ /\[(.+)\:(\d+)\]\:\s+\((\w+)\)/){
		$file = $1;
		$line = $2;
		$type = $3;
		if(defined($count{$type})){
			$count{$type}++;
		}else{
			$count{$type} = 1;
		}
		if($type eq "error"){
			$stderr.print $in;
		}
	}
}

my $total = 0;
foreach my $key (keys(%count)){
	printf "%-11s  %3d\n", $key, $count{$key};
	$total += $count{$key};
}
print "----------------\n";
printf "%-11s  %3d\n", "TOTAL", $total;

