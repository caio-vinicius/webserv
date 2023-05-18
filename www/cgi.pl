#!/usr/bin/perl

$name = $ENV{'QUERY_STRING'};
print "<html><head><title>Hello!</title></head><body>";
print "<h1>Hello, $name!</h1>";
print "</body></html>";
