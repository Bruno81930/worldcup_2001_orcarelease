#!/usr/local/bin/perl -I/Daten/collin/Studium/CL-KI/Projekt/src/orca/coach

use GD;
use DrawTools;

use strict;

my @regions;

open(INPUT, $ARGV[0]) || die $!;
open(OUTPUT, ">$ARGV[1]") || die $!;

while (<INPUT>) {
  last if /^=start/;
}

$/ = "=\n";

while (<INPUT>) {
  last if /^=end/;
  my @column = grep { !/^=/ } split/\n/;
  $regions[@regions] = \@column;
}

my $image_width = $regwidth * @{$regions[0]};
my $image_height = $regheight * @regions;

my $im = new GD::Image($image_width, $image_height);
my $green = $im->colorAllocate(0, 150, 0);
my $black = $im->colorAllocate(0, 0, 0);
my $red = $im->colorAllocate(200, 0, 0);

$im->fill(int($image_width / 2), int($image_height / 2), $green);

for (my $i = 0; $i < @{$regions[0]}; $i++) {
  $im->line($i * $regwidth, 0, $i * $regwidth, $image_height - 1, $black);
}
for (my $i = 0; $i < @regions; $i++) {
  $im->line(0, $i * $regheight, $image_width - 1, $i * $regheight, $black);
}

for (my $y = 0; $y < @regions; $y++) {
  for (my $x = 0; $x < @{$regions[$y]}; $x++) {
    if ($regions[$y]->[$x] =~ /\S/) {
      my @players = split/\s+/, $regions[$y]->[$x];
      foreach my $player (@players) {
	&draw_player_in_region($player, $x, $y, $im, $red);
      }
    }
  }
}

binmode OUTPUT;
print OUTPUT $im->png;
close(OUTPUT);
close(INPUT);
