#!/usr/local/bin/perl -T

use strict;
use CGI qw(:standard);

my $image_dir = "/Daten/collin/Studium/CL-KI/Projekt/images";

$ENV{PATH} = "";

print header,
    start_html('Teampositions Analyser'),
    h1('Teampositions Analyser'),
    start_form,
    hidden('file', time() . ".png"),
    textarea('positions', '', 30, 100), p,
    submit,
    end_form,
    hr;

if (param()) {
  my $file = param('file');
  my $lines = param('positions');
  $lines =~ s/\r//g;

  $file =~ /^(\d+\.png)$/;
  $file = $1;
  open(my $data, ">$image_dir/$file.tmp") || die $!;
  print $data $lines;
  close($data);
  system("/Daten/collin/Studium/CL-KI/Projekt/src/orca/coach/teampositions.pl ". 
	 "$image_dir/$file.tmp ".
	 "$image_dir/$file");
  print img({src => 'http://collin.rogowski.de/projekt/images/' . $file});
}

print end_html;
    
