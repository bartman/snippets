#!/usr/bin/perl -w
use strict;

my(
	$currentPrime,
	$primeCount,
	@numbers,
	$count,
	$arraySize,
	$numToFind
);

$primeCount = 0;
$currentPrime = 2;
$numToFind = $ARGV[0];
$arraySize = $numToFind * (log($numToFind) + log( log($numToFind) ));

print "+-----------------------------+"."\n|	Count		Prime |\n";
while($primeCount++ < $numToFind){
	$count = $currentPrime;
	printf "|%11d%17d |\n", $primeCount, $currentPrime;
	while( ($count += $currentPrime) < $arraySize ){
		$numbers[$count] = 1;
	};
	while($numbers[++$currentPrime]){};
};
print "+-----------------------------+\n";
