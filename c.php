<?php

require 'pure/Context.php';

$c = new Context(Context::INIT_DECIMAL128);
$c->round = Context::ROUND_UP;
echo $c . "\n";
var_dump($c);
?>
