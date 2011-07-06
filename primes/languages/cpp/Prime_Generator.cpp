#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "Prime_Generator.h"

Prime_Generator::Prime_Generator(const int a)
{
	set_size(a);
	temp = new char [get_size()];
}

Prime_Generator::~Prime_Generator(void) {delete [] temp;}

int Prime_Generator::get_size (void) const {return size;}

char Prime_Generator::get_temp (const int i) const {return temp[i];}

void Prime_Generator::set_size (const int s) {size = (int)(s*(log(s)+log(log(s))));}

void Prime_Generator::set_temp (const int i) {temp[i] = FALSE;}

void Prime_Generator::cat (const int req)
{
	int prime=BASE_CASE;
	int index, count=0;
	
	cout << BAR << "\n|	Count		Prime |\n" << BAR;
	
	while(++count < req){
		index=prime;
		cout << '|';
		cout.width(11);
		cout << count;
		cout.width(17);
		cout << prime << " |\n";
		
		while( (index += prime) < get_size() )
		  set_temp(index);
		
		while(get_temp(++prime));
	}
	
	cout << BAR << '\n';
}

int main(int argc, char* argv[]){

	int req = atoi(argv[1])+1;
	Prime_Generator * PG = new Prime_Generator(req);
	
	PG->cat(req);
	
	exit(0);
}
