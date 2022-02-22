#!/usr/local/bin/perl -w

use GD;
use DrawTools;
use strict;

my $magnify = 10;

while (my $file = <$ARGV[0]*>) {
  open(INPUT, $file) || die "odd.";
  my $im = draw_field($magnify);
  my $black = $im->colorAllocate(0, 0, 0);
  my $white = $im->colorAllocate(255, 255, 255);
  my $red = $im->colorAllocate(200, 0, 0);
  my $blue = $im->colorAllocate(0, 0, 200);
  my $yellow = $im->colorAllocate(0, 200, 200);
  my $orange = $im->colorAllocate(200, 200, 0);
  
  while(<INPUT>) {
    my @data = split;
    if ($data[0] eq "p") {
      my $color = $data[1] > 10 ? $red : $blue;
      if ($data[4] == 1) {
	$color = $yellow;
      }
      elsif ($data[4] == 2) {
	$color = $orange;
      }
      $data[1] = $data[1] > 10 ? $data[1] - 11 : $data[1];
      $data[1]++;
      draw_player($data[1], ($data[2] + $pitch_length / 2) * $magnify,
		  ($data[3] + $pitch_width / 2) * $magnify,
		  $im, $color);
    }
    elsif($data[0] eq "b") {
      draw_player(" ", ($data[1] + $pitch_length / 2) * $magnify, 
		  ($data[2] + $pitch_width / 2) * $magnify, $im, $white);
    }
  }
  close(INPUT);
  open(OUTPUT, "|pngtopnm | ppmtogif >$file.gif") || die "cannot write $file.png";
  print OUTPUT $im->png;
  close(OUTPUT);

}


