<?

  $n = 10000;
  $suspects = ($n + 1) * (log($n) + log(log($n)));

  $prime = 2;
  $counter = 0;

  $line = "+-----------------------------+\n";
  $head = "|      Count            Prime |\n";

  print $line;
  print $head;
  print $line;

  while ($counter < $n)
  {
    $counter++;
    printf("|%11d%17d |\n", $counter, $prime);

    $i = $prime;
    while ($i < $suspects)
    {
      $suspect[$i] = 1;
      $i += $prime;
    }

    while ($suspect[++$prime] != 0);
  }

  print $line;

?>
