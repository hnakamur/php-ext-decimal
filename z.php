<?php
list($a, $b) = array(1, 2, 3);
echo '#1 a=' . $a . ', b=' . $b . "\n";
//list($a, $b, $c) = array(1, 2);
//echo '#1 a=' . $a . ', b=' . $b . ', c=' . $c ."\n";

$tok = preg_split('/ +/', '  1  2   3  ');
var_dump($tok);

$tok = strtok('  1  2    3   ', ' ');
echo '#1 (' . $tok . ")\n";
$tok = strtok(' ');
echo '#2 (' . $tok . ")\n";
$tok = strtok(' ');
echo '#3 (' . $tok . ")\n";
$tok = strtok(' ');
echo '#4 (' . $tok . ")\n";
$tok = strtok(' ');
echo '#5 (' . $tok . ")\n";

$a = array();
$a[] = 'a';
$a[] = 'b';
var_dump($a);

$directive = strtok('   a :  b  ', ': ');
$value = strtok(': ');
echo "dir=" . $directive . ", value=" . $value . ".\n";

