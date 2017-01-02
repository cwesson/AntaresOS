#!/usr/bin/perl

use constant {
	START_X => 8,
	START_Y => 4,
	SIZE_X => 17,
	SIZE_Y => 9
};

my @field;

my @values = (
	' ', '.', 'o', '+', '=', '*', 'B', '0', 'X', '@', '%', '&', '#', '/', '^', 'S', 'E'
);

sub isxdigit($){
	my ($c) = @_;
	return (($c >= '0' && $c <= '9')
			|| ($c >= 'a' && $c <= 'f')
			|| ($c >= 'A' && $c <= 'F'));
}

my $x = START_X;
my $y = START_Y;

my $line = <STDIN>;
my $len = length($line);

for(my $i = 0; $i < $len; ){
	my @word = (0, 0);
	for(my $j = 0; $j < 2; $j++){
		my $c = substr($line, $i++, 1);
		if(isxdigit($c)){
			$word[$j] = hex($c);
		}else{
			$j--;
		}
	}
	my @move = (0, 0, 0, 0);
	$move[0] = $word[1] & 0x03;
	$move[1] = $word[1] >> 2;
	$move[2] = $word[0] & 0x03;
	$move[3] = $word[0] >> 2;
	
	for(my $j = 0; $j < 4; $j++){
		if($move[$j] & 0x02){
			$y++;
		}else{
			$y--;
		}
		if($move[$j] & 0x01){
			$x++;
		}else{
			$x--;
		}
	
		if($x > SIZE_X-1){
			$x = SIZE_X-1;
		}elsif($x < 0){
			$x = 0;
		}
		if($y > SIZE_Y-1){
			$y = SIZE_Y-1;
		}elsif($y < 0){
			$y = 0;
		}
		$field[$x][$y]++;
		if($field[$x][$y] > 14){
			$field[$x][$y] = 14;
		}
	}
}
	
$field[START_X][START_Y] = 15;
$field[$x][$y] = 16;

printf("+-----------------+\n");
for($y = 0; $y < SIZE_Y; $y++){
	print('|');
	for($x = 0; $x < SIZE_X; $x++){
		print($values[$field[$x][$y]]);
	}
	printf("|\n");
}
printf("+-----------------+\n");

