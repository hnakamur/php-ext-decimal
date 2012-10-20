<?php
$context = new Decimal_Context(4);
$a = new Decimal_Number('-1.2', $context);
$b = $a->abs($context);
echo $b->toString() . "\n";
?>
