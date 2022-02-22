#!/usr/local/bin/perl -w

use strict;

my $min = -52;
my $max = 52;
my $step = 2;
my $angle_step = 5;
my $max_distance = 20;

die "usage: ./genHeader functioname\n" unless $ARGV[0];

print "#Koord:";
for (my $i = $min; $i <= $max; $i += $step) {
  print " *k$i", ($i < $max ? "," : "");
}
print ".\n";

print "#Team:", join(", ", map { "*t$_" } 0 .. 11), ".\n";

print "#Angle:";
for (my $i = 0; $i <= 360; $i += $angle_step) {
  print " *a$i", ($i < 360 ? "," : "");
}
print ".\n";

print "#Distance:";
for (my $i = 0; $i <= $max_distance; $i += $step) {
  print " *d$i", ($i < $max_distance ? "," : "");
}
print ".\n\n";

print "*greaterKoord(Koord,Koord) \#\#\n";
for (my $i = $max; $i >= $min; $i -= $step) {
  for (my $j = $i - $step; $j >= $min; $j -= $step) {
    print "k$i,k$j\n";
  }
}
print ".\n";

print "*greaterTeam(Team,Team) \#\#\n";
for (my $i = 11; $i >= 0; $i--) {
  for (my $j = $i - 1; $j >= 0; $j--) {
    print "t$i,t$j\n";
  }
}
print ".\n";

print "*greaterDistance(Distance, Distance) \#\#\n";
for (my $i = $max_distance; $i >= 0; $i -= $step) {
  for (my $j = $i - $step; $j >= 0; $j -= $step ) {
    print "d$i,d$j\n";
  }
}
print ".\n";

print "*greaterAngle(Angle, Angle) \#\#\n";
for (my $i = 360; $i >= 0; $i -= $angle_step) {
  for (my $j = $i - $angle_step; $j >= 0; $j -= $angle_step) {
    print "a$i,a$j\n";
  }
}
print ".\n";

print "$ARGV[0](Koord,Koord,Team,Team,Koord,Koord,Distance,Angle,Distance,Angle) " . "#" x 10 . "\n";
print "§\n";
