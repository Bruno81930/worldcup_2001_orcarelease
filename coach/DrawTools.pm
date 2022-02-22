package DrawTools;

require Exporter;
use GD;
use strict;

our @ISA = qw(Exporter);
our @EXPORT = qw(draw_field draw_player draw_player_in_region $regwidth $regheight $playersize $pitch_length $pitch_width);

our $regwidth = 100;
our $regheight = 100;
our $playersize = 20;
our $pitch_length = 105;
our $pitch_width = 68;
our %offsets;

sub draw_player_in_region {
  my ($player, $x, $y, $im, $color) = @_;
  
  my $xoffset = $offsets{$x}->{$y};
  $offsets{$x}->{$y} += $playersize;
  my $yoffset = 0;

  draw_player($player,
	      ($x * $regwidth) + $playersize + $xoffset,
	      ($y * $regheight) + $playersize + $yoffset,
	      $im,
	      $color);
}

sub draw_player {
  my ($player, $x, $y, $im, $color) = @_;

  my $black = $im->colorAllocate(0, 0, 0);

  $im->arc($x, $y, $playersize, $playersize, 0, 360, $color);
  $im->fill($x, $y, $color);
  $im->string(gdLargeFont, $x - int($playersize * (1/3)), $y - int($playersize * (1/3)), $player, $black);
}

sub draw_field {
  my ($magnify) = @_;
  
  my $width = $pitch_length * $magnify;
  my $height = $pitch_width * $magnify;

  my $im = new GD::Image($width, $height);

  my $green = $im->colorAllocate(0, 150, 0);

  $im->fill(int($width / 2), int($height / 2), $green);

  return $im;
}

1;

