<?php
$context = new DecimalContext(DecimalContext::INIT_DECIMAL64);
$a = new Decimal('12');
$b = new Decimal('7.00', $context);
$c = $a->add($b, $context);
echo $c . "\n";

$d = new Decimal('0.1', $context);
$e = $c->add($d, $context);
echo $e . "\n";

//$context->traps = DecimalContext::DIVISION_BY_ZERO;
$g = new Decimal('1', $context);
$f = $g->divide(new Decimal('0', $context), $context);
echo $f . "\n";
?>
