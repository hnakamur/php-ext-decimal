<?php
$context = DecimalContext::getDefaultContext();
var_dump($context);
$a = new Decimal('-12', $context);
echo $a . "\n";
echo $a->abs($context) . "\n";
?>
