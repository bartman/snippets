#!/usr/bin/tclsh

set n [lindex $argv 0]

if {$n > 2} {
  set suspects [expr [expr $n + 1] * [expr log($n) + log(log($n))]]
} else {
  set suspects 0
}

set prime 2
set counter 0

set line "+-----------------------------+"
set head "|      Count            Prime |"

puts $line
puts $head
puts $line

for {set i 1} {$i < $suspects} {incr i 2} {
  set suspect($i) 1
}

while {$counter < $n} {
  incr counter
  puts [format "|%11d%17d |" $counter $prime]

  set i $prime
  while {$i < $suspects} {
    if {[info exists suspect($i)]} {
      unset suspect($i)
    }
    incr i $prime
  }

  incr prime
  while {![info exists suspect($prime)]} {
    if {$prime > $suspects} {
      break
    }
    incr prime
  }
}

puts $line
