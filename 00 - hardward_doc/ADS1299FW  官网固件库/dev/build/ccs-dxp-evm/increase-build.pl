# Increase the build number in a build

use strict;
use Cwd;
use IO::FILE;

my $versionfile;
my $newversionfile;
my $readtext;
my @version1;
my @version2;
   
   $versionfile = "version.h";
   $newversionfile = "temp.h";

  #open version file
  open (VERSION, "< $versionfile") or die "Error opening $versionfile\n";
  open (OUTVERSION, "> $newversionfile") or die "Error opening $newversionfile\n";
  
  while ( <VERSION> ) {
    $readtext = $_;
    if ($readtext =~ /FW_VERSION/) {
        @version1 = split /"/, $readtext;
        @version2 = split /\./, $version1[1];
        ++$version2[2];
        $version2[0] = join '.' , @version2;
        $readtext = $version1[0] . '"' . $version2[0] . '"' . $version1[2];
        
    }
    print OUTVERSION "$readtext";
  }
  close VERSION;
  close OUTVERSION;
  
  unlink $versionfile;
  
  rename $newversionfile, $versionfile;
  
  
  
