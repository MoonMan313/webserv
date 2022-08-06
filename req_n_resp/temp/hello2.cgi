#!/usr/bin/perl
print "<html><head><title>HELLO</title></head>\n";
print "<body>\n";
print "<h2>Bonjour bonjiur cgi</h2>\n";
$method = $ENV{'REQUEST_METHOD'};
$test = $ENV{'to'};
print "<h2>check method name<h2>\n";
print $method;
print $test;
print "\n</body>\n";
print "</html>\n";
