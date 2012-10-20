<?php

function runPop($a) {
    array_pop($a);
    print_r($a);
}

$a = array(2, 3);
runPop($a);
print_r($a);


