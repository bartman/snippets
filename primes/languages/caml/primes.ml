
let guess x =
  int_of_float ( (float (x+1)) *. ( log( float x ) +. log( log( float x ) ) ) );;

let main () =
  let request = int_of_string Sys.argv.(1) in
  let estimate = guess request in
  let prime=2 in
  let counter=0 in
  let temp=0 in
  
  let np = Array.make estimate 0 in
  
  print_string "+-----------------------------+";
  print_newline();
  print_string "|      Count            Prime |";
  print_newline();
  print_string "+-----------------------------+";
  print_newline();

  let c = ref counter in
  let p = ref prime in
  let i = ref temp in
  
  while !c < request do
    c := !c + 1;
 
    Printf.printf "|%11d%17d |\n" !c !p;
    
    i := !p * 2;
    while !i < estimate do
      np.(!i) <- 1;
      i := !i + !p;
    done;
    
    p := !p + 1;
    while np.(!p) == 1 do
      p := !p + 1;
    done;

  done;

  print_string "+-----------------------------+";
  print_newline();
   
  exit 0;;

main ();;


(* File fib.ml
let rec fib n =
  if n < 2 then 1 else fib(n-1) + fib(n-2);;
let main () =
  let arg = int_of_string Sys.argv.(1) in
  print_int(fib arg);
  print_newline();
  exit 0;;
main ();;
*)

