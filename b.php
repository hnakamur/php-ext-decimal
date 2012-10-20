<?php
$context = DecimalContext::getDefaultContext();
$context->precision = 2;
$context->rounding = DecimalContext::ROUND_HALF_UP;
$a = new Decimal('2.35', $context);
echo $a . "\n";
?>
