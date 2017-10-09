#!/usr/bin/perl

# use: make_mlf LABEL signal files
# does not output the header #!MLF!#

$label = shift @ARGV;
@files = @ARGV; 

foreach $file (@files) {
    ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
                      $atime,$mtime,$ctime,$blksize,$blocks)
                          = stat($file);
    $htktime  = $size / 2 * 1250;  
    $file =~ s/[.]raw/.lab/;
    print "\"$file\"\n";
    print "0\t$htktime\t$label\n";
    print ".\n";
}


