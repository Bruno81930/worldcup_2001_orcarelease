#!/usr/bin/perl -w

use strict;
use Digest::MD5;
use Getopt::Long;

$| = 1;

my ($config_file, $verbose, $interactive, $uploadonly, $upload, $untar);
$upload = 1;
$untar = 1;
my %cmd_config;
GetOptions('file=s' => \$config_file,
	   'teamtar1=s' => \$cmd_config{teamtar1},
	   'teamtar2=s' => \$cmd_config{teamtar2},
	   'teamsh1=s' => \$cmd_config{teamsh1},
	   'teamsh2=s' => \$cmd_config{teamsh2},
	   'address1=s' => \$cmd_config{address1},
	   'address2=s' => \$cmd_config{address2},
	   'login1=s' => \$cmd_config{login1},
	   'login2=s' => \$cmd_config{login2},
	   'dir1=s' => \$cmd_config{dir1},
	   'dir2=s' => \$cmd_config{dir2},
	   'identity=s' => \$cmd_config{identity},
	   'serveraddress=s' => \$cmd_config{serveraddress},
	   'serverdir=s' => \$cmd_config{serverdir},
	   'servertar=s' => \$cmd_config{servertar},
	   'serverlogin=s' => \$cmd_config{serverlogin},
	   'interactive_config' => \$interactive,
	   'verbose' => \$verbose,
	   'upload!' => \$upload,
	   'untar!' => \$untar,
	   'uploadonly' => \$uploadonly);

while (-e "gauntlet.stop") {
  print "waiting because auf gauntlet.stop...\n" if $verbose;
  sleep 300;
}


my %config;
if ($config_file) { #load config
  open(INPUT, $config_file) || die "cannot open $config_file: $!";
  while (<INPUT>) {
    chomp;
    my ($key, $value) = split/§/;
    $config{$key} = $value;
  }
  close INPUT;

  if ($interactive) {
    &interactive();
  }

  foreach my $key (keys %cmd_config) { #commandline overwrites config-file
    if ($cmd_config{$key}) {
      $config{$key} = $cmd_config{$key};
    }
  }

}

my $start_time = time();

for my $team (1..2) {
  upload($config{"teamtar" . $team}, $config{"login" . $team}, 
	 $config{"address" . $team}, $config{"dir" . $team}, "team$team");
}
upload($config{servertar}, $config{serverlogin}, $config{serveraddress}, $config{serverdir}, "server");

exit if $uploadonly;

my ($serverpid, $team1pid, $team2pid, $trainerpid);
if ($serverpid = fork()) {
  sleep 5;
}
elsif (defined $serverpid) { #we are the newborn
  start_server();
  exit;
}
else {
  die "cannot fork: $!\n";
}
if ($team1pid = fork()) {
  sleep 5;
}
elsif (defined $team1pid) {
  start_team(1, "silent");
  exit;
}
else {
  die "cannot fork: $!\n";
}
if ($team2pid = fork()) {
  sleep 60;  #give teams enough time to position themselves (when the trainer connects, the game starts)
}
elsif (defined $team2pid) {
  start_team(2, "silent");
  exit;
}
else {
  die "cannot fork: $!\n";
}
if ($trainerpid = fork()) {
  waitpid($trainerpid, 0);
  print "done\n";
}
elsif (defined $trainerpid) {
  my ($errorcode, $result1, $result2, $history) = start_trainer("silent");
  if ($errorcode == 139) {
    print "trainer had a segfault\n";
  }
  print "trainer is done (errorcode = $errorcode)\n";
  cleanup($config{serveraddress}, $config{serverlogin});
  sleep 5;
  cleanup($config{address1}, $config{login1});
  cleanup($config{address2}, $config{login2});
  my $teamname1 = get_teamname_from_tar($config{teamtar1});
  my $teamname2 = get_teamname_from_tar($config{teamtar2});
  open(RESULTS, ">>gameStarter.results") || die "cannot append to gameStarer.results: $!";
  chomp($result1, $result2);
  my $context = new Digest::MD5;
  open(TEAM1, $config{teamtar1}) || die "cannot open " . $config{teamtar1} . ": $!";
  $context->addfile(*TEAM1);
  my $team1md5 = $context->hexdigest();
  close(TEAM1);

  $context->reset();
  open(TEAM2, $config{teamtar2}) || die "cannot open " . $config{teamtar2} . ": $!";
  $context->addfile(*TEAM1);
  my $team2md5 = $context->hexdigest();
  close(TEAM2);
  
  print RESULTS join(";", 
		     $start_time, 
		     time(),
		     join(",", $teamname1, $team1md5, $result1),
		     join(",", $teamname2, $team2md5, $result2),
		     $history), "\n";
  close RESULTS;
  exit;
}
else {
  die "cannot fork: $!\n";
}

sub cleanup {
  my ($host, $login) = @_;
  print "cleaning $host...\n";
  my $cmd = "ssh -i " . $config{identity} . " -l " . $login . " " . $host . 
      " 'ps -fu $login' |";
  local *SSH;
  open(SSH, $cmd) || die "cannot spawn ssh: $!";
  my %pids = ();
  $_ = <SSH>;
  while (<SSH>) {
    my ($uid, $pid, $ppid, $c, $stime, $tty, $time, $cmd) = split/\s+/;
    if ($cmd !~ /^\-bash/) {
      $pids{$pid} = $cmd;
    }
  }
  close(SSH);
  #print "  killing " . scalar(keys %pids) . " processes (" . join(", ", values %pids) .  ")\n";
  system("ssh -i " . $config{identity} . " -l " . $login . " " . $host . " 'kill " . 
	 join(" ", keys %pids) . "' 2>/dev/null >/dev/null")
      if keys(%pids) > 0;
}

sub start_server {
  print "trying to kill old server...\n" if $verbose;
  local *SSH;
  open(SSH, "ssh -i " . $config{identity} . " -l " . $config{serverlogin} . " " . $config{serveraddress} .
       " killall soccerserver 2>&1 " . "|") || die "cannot spawn ssh: $!";
  while(<SSH>) {
    print "  killall: ", $_ if $verbose;
  }
  close(SSH);
  print "done\n" if $verbose;

  print "starting server...\n" if $verbose;
  open(SSH, "ssh -i " . $config{identity} . " -l " . $config{serverlogin} . " " . $config{serveraddress} .
       " " . $config{serverdir} . "/server/soccerserver -sfile " . 
       $config{serverdir} . "/server/server.conf" . 
       " -pfile " . $config{serverdir} . "/server/player.conf" . 
       " -coach_w_referee 2>&1 " . "|")
      || die "cannot spawn ssh: $!";
  while (<SSH>) {
    print "Server: ", $_ if $verbose;
  }
  close(SSH);
  open(SSH, ssh_cmd($config{serverlogin}, $config{serveraddress},
		    "mv /home/orca/record.log " . $config{serverlogdir} . "/$start_time.log" .
		    " && bzip2 " . $config{serverlogdir} . "/$start_time.log"))
      || die "cannot spawn ssh: $!";
  while(<SSH>) {
    print "Server: ", $_ if $verbose;
  }
  close(SSH);
}

sub ssh_cmd {
  my ($login, $address, $command) = @_;

  my $cmd = "ssh -i " . $config{identity} . " -l " . $login . " " . $address . " '" . $command . "' 2>&1 |";
  return $cmd;
}

sub start_trainer {
  my ($silent) = @_;
  print "starting trainer...\n";
  my $cmd = "ssh -i " . $config{identity} . " -l " . $config{serverlogin} . " " . 
      $config{serveraddress} . " 'cd " . $config{serverdir} . "/server/; ./onlinecoach " .
	  ($silent ? "" : "-v ") .  "-T -p 6001 -h " . $config{serveraddress} . " -t Team1' 2>&1 " . "|";
  local *SSH;
  open(SSH, $cmd) || die "cannot spawn ssh: $!";
  my ($result1, $result2, $history);
  while (<SSH>) {
    if (/^l,/) {
      $result1 = $_;
    }
    elsif (/^r,/) {
      $result2 = $_;
    }
    elsif (/^x,/) {
      $history = $_;
      next;
    }
    print "Trainer: $_";
  }
  close(SSH);
  my $errorcode = $?;

  return ($errorcode, $result1, $result2, $history);
}

sub start_team {
  my ($team, $silent) = @_;

  my $teamname = get_teamname_from_tar($config{"teamtar" . $team});
  print "starting $teamname...\n";
  my $cmd = "ssh -i " . $config{identity} . " -l " . $config{"login" . $team} . " " . 
      $config{"address" . $team} . " 'cd " . $config{"dir" . $team} . "/team$team/; ./" .
	  $config{"teamsh" . $team} . " " . $config{serveraddress} . " $teamname " . 
	      ($silent ? "-v" : "") . "' 2>&1 " . "|";
  local *SSH;
  #print $cmd, "\n";
  open(SSH, $cmd) || die "cannot spawn ssh: $!";
 while (<SSH>) {
    print "$teamname: ", $_;
  }
  close(SSH);
}

sub equal {
  my ($file, $login, $host, $dir) = @_;

  my $remote_file = get_file_from_path($file);
  open SSH, "ssh -i " . $config{identity} . " -l $login $host 'ls -l $dir/$remote_file' 2>/dev/null |"
      || die "cannot spawn ssh: $!";
  my (undef, undef, undef, undef, $size, undef, undef, undef, undef, undef) = split/\s+/, <SSH>;
  close SSH;
  return defined $size ? $size == -s $file : undef;
}

sub upload {
  my ($file, $login, $host, $dir, $type) = @_;

  if ($upload) {
    if (equal($file, $login, $host, $dir)) {
      print "Remote file $file has the same size as the local one. skipping.\n" if $verbose;
    }
    else {
      print "Uploading $file..." if $verbose;
      my $remote_file = get_file_from_path($file);
      system("scp -i " . $config{identity} . " $file " . $login . "@" . $host . ":" . $dir . 
	     ">>gameStarter.log 2>>gameStarter.log");
      print "done\n";
    }
  }

  untar($file, $login, $host, $dir, $type);
}

sub untar {
  my ($file, $login, $host, $dir, $type) = @_;

  return unless $untar;

  my $remote_file = get_file_from_path($file);
  my $untar_cmd = "ssh -i " . $config{identity} . " -l " . $login . " " . $host . 
      " 'cd $dir; rm -rf $type; mkdir $type; cd $type; mkdir Logfiles; tar -xzf ../$remote_file' >>gameStarter.log 2>>gameStarter.log";
  print "Untaring $file..." if $verbose;
  system($untar_cmd);
  print "done\n" if $verbose;
}

sub ask {
  my ($question, $default) = @_;

  print $question . ($default ? " [$default]" : "") . "? ";
  my $answer = <STDIN>;
  chomp $answer;
  return $answer ? $answer : $default;
}

sub choice {
  my ($question, $choices) = @_;

  print $question, "?\n";
  my $i = 1;
  print "  ", $i++, ": ", $_, "\n" foreach @$choices;
  print "\nPlease enter your choice: ";
  my $choice = <STDIN>;
  chomp $choice;

  choice($question, $choices) if ($choice < 1 || $choice > @$choices);

  $choice--;
  return $choices->[$choice];
}

sub get_file_from_path {
  return (($_[0] =~ m!/([^/]+)$!) ? $1 : $_[0]);
}

sub get_teamname_from_tar {
  my ($name) = $_[0] =~ m!/([^/]+)\.tgz$!;

  return $name;
}

sub interactive {
  $config{teamtar1} = &ask("What is the filename of the tgz containing the first team");
  my @files = map { /(\S+)$/ } grep { /start/ } `tar -tzvf $config{teamtar1}`;
  $config{teamsh1} = &ask("What is the filename (contained in the tgz) to start the first team with",
			  $files[0]);
  $config{teamtar2} = &ask("What is the filename of the tgz containing the second team");
  @files = map { /(\S+)$/ } grep { /start/ } `tar -tzvf $config{teamtar2}`;
  $config{teamsh2} = &ask("What is the filename (contained in the tgz) to start the second team with",
			  $files[0]);
  $config{servertar} = &ask("What is the filename of the tgz containing the server");
  
  for my $team (1..2) {
    $config{"address" . $team} = &ask("What is the IP-Address (or hostname) of the machine for team $team");
    $config{"login" . $team} = &ask("What is your login on the machine for team $team");
    $config{"dir" . $team} = 
	&ask("In which directory shall I put the tgz for team $team (absolute path, please)");
  }
  $config{serveraddress} = &ask("What is the IP-Address (or hostname) of the machine for the server");
  $config{serverlogin} = &ask("What is your login on the machine for the server");
  $config{serverdir} = &ask("In which directory shall I put the server-tgz (absolute path, please)");

  $config{identity} = &ask("What is the path to your SSH-identity file", $ENV{'HOME'} . "/.ssh/identity");

  if (&choice("Do you want to save this configuration", ["Yes", "No"]) eq "Yes") {
    my $filename = &ask("What is the filename for this configuration file");
    open OUTPUT, ">$filename" || die "cannot write to $filename: $!";
    print OUTPUT $_ . "§" . $config{$_} . "\n" foreach sort keys %config;
    close OUTPUT;
    print "If you want to run $0 with this configuration, simply supply $filename as the first argument\n";
  }
}

