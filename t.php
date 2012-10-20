<?php

while (($line = fgets(STDIN)) !== false) {
    // Remove comment
    $pos = strpos($line, '--');
    if ($pos !== false) {
        $line = substr($line, 0, $pos);
    }

    $pos = strpos($line, '->');
    if ($pos !== false) {
        // test
        list($lhs, $rhs) = explode('->', $line);

        $id = strtok($lhs, ' ');
        $operation = strtok(' ');
        $operands = array();
        while (($tok = strtok(' ')) !== false) {
            $operands[] = $tok;
        }

        $result = strtok($rhs, ' ');
        $conditions = array();
        while (($tok = strtok(' ')) !== false) {
            $conditions[] = $tok;
        }

        echo "TEST: $id,$operation," . implode('|', $operands) . "-> $result," . implode('|', $conditions) . "\n";
        continue;
    }

    $pos = strpos($line, ':');
    if ($pos !== false) {
        // directive
        $keyword = strtok($line, ': ');
        $value = strtok(': ');

        echo 'DIR: keyword=' . $keyword . ', value=' . $value . "\n";
    }
}
