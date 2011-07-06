Program Primes(input, output);
Var 
	tmp, parm : LongInt;

procedure compute( request : LongInt, estimate : LongInt );
Var 
	prime, i, counter : LongInt;
	not_primes_size : LongInt;
	not_primes : array [ 0 .. estimate ] of Byte;
begin
	{ initialize variables }
	not_primes_size := estimate ;
	
	prime := 2;
	counter := 0;
	
	{ show header }
	WriteLn( '+-----------------------------+' );
	WriteLn( '|      Count            Prime |' );
	WriteLn( '+-----------------------------+' );
	
	{ run loop }
	while ( counter < request ) do
	begin
		counter := counter + 1;
	
		WriteLn( '|', counter:11, prime:17, ' |' );
	
		{ mark primes }
		i := prime * 2;
		while ( i < not_primes_size ) do
		begin
			not_primes[i] := 1;
			i := i + prime;
		end;
		
		{ find next prime }
		prime := prime + 1;
		while ( not_primes[prime] ) do
		begin
			prime := prime + 1;
		end;
	end;

	WriteLn( '+-----------------------------+' );
end;


Begin
	WriteLn( 'ParamCount: ', ParamCount );
	if ParamCount = 0 then
	begin
		WriteLn('Specify number of primes to run.');
		halt(1);
	end
	else begin

		{ read the command line parameter }
		tmp := 1;
		strread(ParamStr(1),tmp,tmp,parm);
		
		{ guesstimate the size of the array needed }
		tmp := (parm+1) * (ln(parm) + ln(ln(parm)));

		compute(parm, tmp);
	end

End.
