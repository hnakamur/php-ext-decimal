<?php
# DecimalContext::getDefaultContext()->init(DecimalContext::INIT_DECIMAL128);
$c = new DecimalContext(DecimalContext::INIT_DECIMAL128);
print_r($c);
$n = new Decimal('NaN22', $c);
print_r($c);
