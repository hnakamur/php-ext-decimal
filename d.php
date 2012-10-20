<?php

require 'pure/Number.php';

preg_match(Number::$pattern, '2.3', $matches);
preg_match(Number::$pattern, '-3.23e+01', $matches);
preg_match(Number::$pattern, '-infinity', $matches);
var_dump($matches);
if (count($matches) > 0) {
    echo 'matches[0]=' . $matches[0] . "\n";
    echo 'sign=' . $matches['sign'] . "\n";
    echo 'decimal=' . $matches['decimal'] . "\n";
    echo 'exponent=' . $matches['exponent'] . "\n";
}
?>
