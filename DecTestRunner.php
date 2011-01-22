<?php

class DecTestRunner {
    private $fileContext;
    private static $roundingMap = null;
    private static $statusMap = null;
    private static $methodMap = null;
    private static $numberPattern = '/\A(?P<sign>[-+])?((?P<decimal>(\d+\.\d*|\.?\d+))(e(?P<exponent>[-+]?\d+))?|(?<infinity>inf(inity)?)|(?P<nan>s?nan)(?P<payload>\d*))\Z/i';

    public function __construct() {
        if (self::$roundingMap === null) {
            self::$roundingMap = self::createRoundingMap();
        }

        if (self::$statusMap === null) {
            self::$statusMap = self::createStatusMap();
        }

        if (self::$methodMap === null) {
            self::$methodMap = self::createMethodMap();
        }

        $this->fileContext = new DecimalContext(DecimalContext::INIT_BASE);
        $this->fileContext->traps = 0;
    }

    private static function createRoundingMap() {
        return array(
            'ceiling' => DecimalContext::ROUND_CEILING,
            'down' => DecimalContext::ROUND_DOWN,
            'floor' => DecimalContext::ROUND_FLOOR,
            'half_down' => DecimalContext::ROUND_HALF_DOWN,
            'half_even' => DecimalContext::ROUND_HALF_EVEN,
            'half_up' => DecimalContext::ROUND_HALF_UP,
            'up' => DecimalContext::ROUND_UP,
            '05up' => DecimalContext::ROUND_05UP
        );
    }

    private static function createStatusMap() {
        return array(
            DecimalContext::CONVERSION_SYNTAX => 'Conversion_syntax',
            DecimalContext::DIVISION_BY_ZERO => 'Division_by_zero',
            DecimalContext::DIVISION_IMPOSSIBLE => 'Division_impossible',
            DecimalContext::DIVISION_UNDEFINED => 'Division_undefined',
            DecimalContext::INSUFFICIENT_STORAGE => 'Insufficient_storage',
            DecimalContext::INEXACT => 'Inexact',
            DecimalContext::INVALID_CONTEXT => 'Invalid_context',
            DecimalContext::INVALID_OPERATION => 'Invalid_operation',
            DecimalContext::LOST_DIGITS => 'Lost_digits',
            DecimalContext::OVERFLOW => 'Overflow',
            DecimalContext::CLAMPED => 'Clamped',
            DecimalContext::ROUNDED => 'Rounded',
            DecimalContext::SUBNORMAL => 'Subnormal',
            DecimalContext::UNDERFLOW => 'Underflow'
        );
    }

    private static function createMethodMap() {
        return array(
            'abs' => 'abs'
        );
    }

    public function processFile($file) {
        $handle = fopen($file, 'r');
        while (($line = fgets($handle)) !== false) {
            $tokens = $this->tokenize($line);
            if (count($tokens) === 0) {
                continue;
            }

            if ($this->isTestCase($tokens)) {
                $this->processTestCase($tokens);
            } elseif ($this->isDirective($tokens)) {
                $this->processDirective($tokens);
            }
        }
        fclose($handle);
    }

    public function tokenize($line) {
        $pattern = '/\s*((?P<squot>\'(?:[^\']|\'\')*\')|(?P<dquot>"(?:[^"]|"")*")|(?P<comment>--.*)|(?P<colon>:)|(?P<arrow>->)|(?P<word>[^\s:]+))/';
        $tokens = array();
        $offset = 0;
        while (preg_match($pattern, $line, $matches, PREG_OFFSET_CAPTURE,
            $offset)
        ) {
            if (array_key_exists('word', $matches)
                    && $matches['word'][1] !== -1) {
                $tokens[] = array('word', $matches[1][0]);
            } elseif (array_key_exists('squot', $matches)
                    && $matches['squot'][1] !== -1) {
                $tokens[] = array('quoted',
                                  $this->unquoteSingle($matches[1][0]));
            } elseif (array_key_exists('dquot', $matches)
                    && $matches['dquot'][1] !== -1) {
                $tokens[] = array('quoted',
                                 $this->unquoteDouble($matches[1][0]));
            } elseif (array_key_exists('colon', $matches)
                    && $matches['colon'][1] !== -1) {
                $tokens[] = array('colon', $matches[1][0]);
            } elseif (array_key_exists('arrow', $matches)
                    && $matches['arrow'][1] !== -1) {
                $tokens[] = array('arrow', $matches[1][0]);
            } elseif (array_key_exists('comment', $matches)
                    && $matches['comment'][1] !== -1) {
                //$tokens[] = array('comment', $matches[1][0]);
            }
            $offset = $matches[1][1] + strlen($matches[1][0]);
        }
        return $tokens;
    }

    private function unquoteSingle($text) {
        return preg_replace("/''/", "'", trim($text, "'"));
    }

    private function unquoteDouble($text) {
        return preg_replace('/""/', '"', trim($text, '"'));
    }

    private function isDirective($tokens) {
        foreach ($tokens as $token) {
            if ($token[1] === ':') {
                return true;
            }
        }
        return false;
    }

    private function processDirective($tokens) {
/*
        echo 'directive:';
        foreach ($tokens as $token) {
           echo ' ' . $token[1];
        }
        echo "\n";
*/
        switch (strtolower($tokens[0][1])) {
            case 'precision':
                $this->fileContext->precision = (integer)$tokens[2][1];
                break;
            case 'rounding':
                $this->fileContext->rounding
                    = self::$roundingMap[$tokens[2][1]];
                break;
            case 'maxexponent':
                $this->fileContext->emax = (integer)$tokens[2][1];
                break;
            case 'minexponent':
                $this->fileContext->emin = (integer)$tokens[2][1];
                break;
            case 'extended':
                $this->fileContext->extended = (integer)$tokens[2][1];
                break;
            case 'clamp':
                $this->fileContext->clamp = (integer)$tokens[2][1];
                break;
            case 'version':
                // noop
                break;
            default:
                throw new InvalidArgumentException('Invalid directive: '
                    . $tokens[0][1]);
                break;
        }
    }

    private function strToRoundingMode($str) {
        switch ($str) {
            case 'ceiling':
        }
    }

    private function isTestCase($tokens) {
        foreach ($tokens as $token) {
            if ($token[1] === '->') {
                return true;
            }
        }
        return false;
    }

    private function processTestCase($tokens) {
#        echo 'testcase:';
#        foreach ($tokens as $token) {
#           echo ' ' . $token[1];
#        }
#        echo "\n";

        $id = $tokens[0][1];
        $operator = $tokens[1][1];

        $operands = array();
        for ($i = 2; $tokens[$i][1] !== '->'; ++$i) {
            $operands[] = $tokens[$i][1];
        }

        ++$i;
        $result = $tokens[$i][1];
        ++$i;
        $conditions = array();
        for (; $i < count($tokens); ++$i) {
            $conditions[] = $tokens[$i][1];
        }

        $this->runTestCase($id, $operator, $operands, $result, $conditions);
    }

    private function runTestCase($id, $operator, $operands, $result,
            $conditions) {
        $this->fileContext->status = 0;

        $this->printTestCase($id, $operator, $operands, $result, $conditions);
        $actualStatus = array();
        $actual = $this->callMethod($operator, $operands);
        if ($this->fileContext->status !== 0) {
            $actualStatus = self::convertStatusToText($this->fileContext->status);
        }

        $expected = $this->strToNum($result);
        $actualStr = (string)$actual;
#        $expectedStr = (string)$expected;
        $expectedStr = $result;
        if ($actualStr === $expectedStr) {
            if (self::isSameStatus($actualStatus, $conditions)) {
#                echo $id . " matched.\n";
            } else {
#                echo $id . ' unmatched. status=' . implode(' ', $actualStatus)
#                    . ', expected=' . implode(' ', $conditions) . "\n";
#                $this->printTestCase($id, $operator, $operands, $result,
#                    $conditions);
                echo $id . ' unmatched. status=';
                print_r($actualStatus);
                echo ', expected=';
                print_r($conditions);
                print_r($this->fileContext);
            }
        } else {
#            $this->printTestCase($id, $operator, $operands, $result,
#                $conditions);
            echo $id . ' unmatched. actual=[' . $actualStr . "]\n";
            echo $id . '          expected=[' . $expectedStr . "]\n";
            print_r($this->fileContext);
        }
    }

    private function printTestCase($id, $operator, $operands, $result,
            $conditions) {
        echo 'testcase: ' . $id . ' ' . $operator . ' '
            . implode(' ', $operands) . ' -> ' . $result . ' '
            . implode(' ', $conditions) . "\n";
    }

    private function callMethod($operator, $operands) {
        $operator = strtolower($operator);
        if ($operator === 'apply') {
            return new Decimal($operands[0], $this->fileContext);
        }

        $numbers = array();
        if ($operator === 'tosci' || $operator === 'toeng') {
            foreach ($operands as $operand) {
                $numbers[] = new Decimal($operand, $this->fileContext);
            }
        } else {
            foreach ($operands as $operand) {
                $numbers[] = $this->strToNum($operand);
            }
        }

        if ($numbers[0] === null) {
            return null;
        }

        switch ($operator) {
        case 'abs':
            return $numbers[0]->abs($this->fileContext);
        case 'add':
            return $numbers[0]->add($numbers[1], $this->fileContext);
        case 'and':
            return $numbers[0]->and($numbers[1], $this->fileContext);
        case 'compare':
            return $numbers[0]->compare($numbers[1], $this->fileContext);
        case 'comparesig':
            return $numbers[0]->compareSignal($numbers[1], $this->fileContext);
        case 'comparetotal':
            return $numbers[0]->compareTotal($numbers[1], $this->fileContext);
        case 'comparetotmag':
            return $numbers[0]->compareTotalMag($numbers[1], $this->fileContext);
        case 'divide':
            return $numbers[0]->divide($numbers[1], $this->fileContext);
        case 'divideint':
            return $numbers[0]->divideInteger($numbers[1], $this->fileContext);
        case 'exp':
            return $numbers[0]->exp($this->fileContext);
        case 'fma':
            return $numbers[0]->fma($numbers[1], $numbers[2], $this->fileContext);
        case 'invert':
            return $numbers[0]->invert($this->fileContext);
        case 'ln':
            return $numbers[0]->ln($this->fileContext);
        case 'log10':
            return $numbers[0]->log10($this->fileContext);
        case 'logb':
            return $numbers[0]->logb($this->fileContext);
        case 'max':
            return $numbers[0]->max($numbers[1], $this->fileContext);
        case 'maxmag':
            return $numbers[0]->maxMag($numbers[1], $this->fileContext);
        case 'min':
            return $numbers[0]->min($numbers[1], $this->fileContext);
        case 'minmag':
            return $numbers[0]->minMag($numbers[1], $this->fileContext);
        case 'minus':
            return $numbers[0]->min($this->fileContext);
        case 'multiply':
            return $numbers[0]->multiply($numbers[1], $this->fileContext);
        case 'nextminus':
            return $numbers[0]->nextMinus($this->fileContext);
        case 'nextplus':
            return $numbers[0]->nextPlus($this->fileContext);
        case 'nexttoward':
            return $numbers[0]->nextToward($numbers[1], $this->fileContext);
        case 'or':
            return $numbers[0]->or($numbers[1], $this->fileContext);
        case 'plus':
            return $numbers[0]->plus($this->fileContext);
        case 'power':
            return $numbers[0]->power($numbers[1], $this->fileContext);
        case 'quantize':
            return $numbers[0]->quantize($numbers[1], $this->fileContext);
        case 'reduce':
            return $numbers[0]->reduce($this->fileContext);
        case 'remainder':
            return $numbers[0]->remainder($numbers[1], $this->fileContext);
        case 'remaindernear':
            return $numbers[0]->remainderNear($numbers[1], $this->fileContext);
        case 'rescale':
            return $numbers[0]->rescale($numbers[1], $this->fileContext);
        case 'rotate':
            return $numbers[0]->rotate($numbers[1], $this->fileContext);
        case 'samequantum':
            return $numbers[0]->sameQuantum($numbers[1], $this->fileContext);
        case 'scaleb':
            return $numbers[0]->scaleB($numbers[1], $this->fileContext);
        case 'shift':
            return $numbers[0]->shift($numbers[1], $this->fileContext);
        case 'squareroot':
            return $numbers[0]->squareRoot($this->fileContext);
        case 'subtract':
            return $numbers[0]->subtract($numbers[1], $this->fileContext);
        case 'tointegral':
            return $numbers[0]->toIntegralValue($this->fileContext);
        case 'tointegralx':
            return $numbers[0]->toIntegralExact($this->fileContext);
        case 'tosci':
            return (string)$numbers[0];
        case 'toeng':
            return $numbers[0]->toEngString();
        case 'trim':
            return $numbers[0]->trim($this->fileContext);
        case 'xor':
            return $numbers[0]->xor($numbers[1], $this->fileContext);
        default:
            throw new InvalidArgumentException('Unknown operator: '
                . $operator);
        }
    }

    private static function isSameStatus($actual, $expected) {
        sort($actual);
        sort($expected);
        return $actual == $expected;
    }

    private function strToNum($str) {
        if (strncmp($str, '#', 1) === 0) {
            if ($str === '#') {
                return null;
            } else {
                throw new InvalidArgumentException('Hex notation not supported: ' . $str);
            }
        } else {
            $c = self::cloneContext($this->fileContext);
            $c->precision = 99;
#            $c->precision = (strpos($matches['decimal'], '.') !== false
#                    ? strlen($matches['decimal']) - 1
#                    : strlen($matches['decimal']));
            $c->status = 0;
            $c->traps = 0;
            $n = new Decimal($str, $c);
            if ($c->status !== 0) {
#                echo 'strToNum s=' . $str . ', status='
#                    . implode(' ', self::convertStatusToText($c->status))
#                    . "\n";
#                print_r($c);
            }
            return $n;
        }
    }

#    private function strToNum($str) {
#        preg_match(self::$numberPattern, $str, $matches);
#        if (count($matches) !== 0) {
#            $c = self::cloneContext($this->fileContext);
#            $c->precision = 99;
##            $c->precision = (strpos($matches['decimal'], '.') !== false
##                    ? strlen($matches['decimal']) - 1
##                    : strlen($matches['decimal']));
#            $c->status = 0;
#            $c->traps = 0;
##            echo 'strToNum s=' . $str . "\n";
##            print_r($c);
#            $n = new Decimal($str, $c);
#            if ($c->status !== 0) {
##                echo 'strToNum s=' . $str . ', status='
##                    . implode(' ', self::convertStatusToText($c->status))
##                    . "\n";
##                print_r($c);
#            }
#            return $n;
#        } elseif (strncmp($str, '#', 1) === 0) {
#            if ($str === '#') {
#                return null;
#            } else {
#                throw new InvalidArgumentException('Hex notation not supported: ' . $str);
#            }
#        } else {
#            throw new InvalidArgumentException('Not a valid number: ' . $str);
#        }
#    }

    private static function convertStatusToText($status) {
        $texts = array();
        foreach (self::$statusMap as $flag => $text) {
            if (($status & $flag) !== 0) {
                $texts[] = $text;
            }
        }
        return $texts;
    }

    private static function cloneContext($context) {
        $c = new DecimalContext(DecimalContext::INIT_BASE);
        $c->precision = $context->precision;
        $c->rounding = $context->rounding;
        $c->emax = $context->emax;
        $c->emin = $context->emin;
        $c->extended = $context->extended;
        $c->clamp = $context->clamp;
        return $c;
    }
}

$runner = new DecTestRunner();
#$runner->processFile('dectest/abs.decTest');
#$runner->processFile('dectest/add.decTest');
#$runner->processFile('dectest/and.decTest');
#$runner->processFile('dectest/base.decTest');
#$runner->processFile('dectest/clamp.decTest');
#$runner->processFile('dectest/compare.decTest');
#$runner->processFile('dectest/comparesig.decTest');
#$runner->processFile('dectest/comparetotal.decTest');
#$runner->processFile('dectest/comparetotmag.decTest');
#$runner->processFile('dectest/comparetotmag.decTest');
#copy.decTest
#copyabs.decTest
#copynegate.decTest
#copysign.decTest
#$runner->processFile('dectest/ddAbs.decTest');
#$runner->processFile('dectest/ddAdd.decTest');
#$runner->processFile('dectest/ddAnd.decTest');
#$runner->processFile('dectest/ddBase.decTest');

#$runner->processFile('dectest/ddCanonical.decTest');
#$runner->processFile('dectest/ddClass.decTest');

#$runner->processFile('dectest/ddCompare.decTest');
#$runner->processFile('dectest/ddCompareSig.decTest');
#$runner->processFile('dectest/ddCompareTotal.decTest');
#$runner->processFile('dectest/ddCompareTotalMag.decTest');
#$runner->processFile('dectest/ddCopy.decTest');
#$runner->processFile('dectest/ddCopyAbs.decTest');
#$runner->processFile('dectest/ddCopyNegate.decTest');
#$runner->processFile('dectest/ddCopySign.decTest');
#$runner->processFile('dectest/ddDivide.decTest');
#$runner->processFile('dectest/ddDivideInt.decTest');
#$runner->processFile('dectest/ddEncode.decTest');
#$runner->processFile('dectest/ddFMA.decTest');
#$runner->processFile('dectest/ddInvert.decTest');
#$runner->processFile('dectest/ddLogB.decTest');
#$runner->processFile('dectest/ddMax.decTest');
#$runner->processFile('dectest/ddMaxMag.decTest');
#$runner->processFile('dectest/ddMin.decTest');
#$runner->processFile('dectest/ddMinMag.decTest');
#$runner->processFile('dectest/ddMinus.decTest');
#$runner->processFile('dectest/ddMultiply.decTest');
#$runner->processFile('dectest/ddNextMinus.decTest');
#$runner->processFile('dectest/ddNextPlus.decTest');
#$runner->processFile('dectest/ddNextToward.decTest');
#$runner->processFile('dectest/ddOr.decTest');
#$runner->processFile('dectest/ddPlus.decTest');
#$runner->processFile('dectest/ddQuantize.decTest');
#$runner->processFile('dectest/ddReduce.decTest');
#$runner->processFile('dectest/ddRemainder.decTest');
#$runner->processFile('dectest/ddRemainderNear.decTest');
#$runner->processFile('dectest/ddRotate.decTest');
#$runner->processFile('dectest/ddSameQuantum.decTest');
#$runner->processFile('dectest/ddScaleB.decTest');
#$runner->processFile('dectest/ddShift.decTest');
#$runner->processFile('dectest/ddSubtract.decTest');
#$runner->processFile('dectest/ddToIntegral.decTest');
#$runner->processFile('dectest/ddXor.decTest');
#$runner->processFile('dectest/decDouble.decTest');
#$runner->processFile('dectest/decQuad.decTest');
#$runner->processFile('dectest/decSingle.decTest');
#$runner->processFile('dectest/divide.decTest');
#$runner->processFile('dectest/divideint.decTest');
#$runner->processFile('dectest/dqAbs.decTest');
#$runner->processFile('dectest/dqAdd.decTest');
#$runner->processFile('dectest/dqAnd.decTest');
#$runner->processFile('dectest/dqBase.decTest');
#$runner->processFile('dectest/dqCanonical.decTest');
#$runner->processFile('dectest/dqClass.decTest');
#$runner->processFile('dectest/dqCompare.decTest');
#$runner->processFile('dectest/dqCompareSig.decTest');
#$runner->processFile('dectest/dqCompareTotal.decTest');
#$runner->processFile('dectest/dqCompareTotalMag.decTest');
#$runner->processFile('dectest/dqCopy.decTest');
#$runner->processFile('dectest/dqCopyAbs.decTest');
#$runner->processFile('dectest/dqCopyNegate.decTest');
#$runner->processFile('dectest/dqCopySign.decTest');
#$runner->processFile('dectest/dqDivide.decTest');
#$runner->processFile('dectest/dqDivideInt.decTest');
#$runner->processFile('dectest/dqEncode.decTest');
#$runner->processFile('dectest/dqFMA.decTest');
#$runner->processFile('dectest/dqInvert.decTest');
#$runner->processFile('dectest/dqLogB.decTest');
#$runner->processFile('dectest/dqMax.decTest');
#$runner->processFile('dectest/dqMaxMag.decTest');
#$runner->processFile('dectest/dqMin.decTest');
#$runner->processFile('dectest/dqMinMag.decTest');
#$runner->processFile('dectest/dqMinus.decTest');
#$runner->processFile('dectest/dqMultiply.decTest');
#$runner->processFile('dectest/dqNextMinus.decTest');
#$runner->processFile('dectest/dqNextPlus.decTest');
#$runner->processFile('dectest/dqNextToward.decTest');
#$runner->processFile('dectest/dqOr.decTest');
#$runner->processFile('dectest/dqPlus.decTest');
#$runner->processFile('dectest/dqQuantize.decTest');
#$runner->processFile('dectest/dqReduce.decTest');
#$runner->processFile('dectest/dqRemainder.decTest');
#$runner->processFile('dectest/dqRemainderNear.decTest');
#$runner->processFile('dectest/dqRotate.decTest');
#$runner->processFile('dectest/dqSameQuantum.decTest');
#$runner->processFile('dectest/dqScaleB.decTest');
#$runner->processFile('dectest/dqShift.decTest');
#$runner->processFile('dectest/dqSubtract.decTest');
#$runner->processFile('dectest/dqToIntegral.decTest');
#$runner->processFile('dectest/dqXor.decTest');
#$runner->processFile('dectest/dsBase.decTest');
#$runner->processFile('dectest/dsEncode.decTest');
#$runner->processFile('dectest/exp.decTest');
#$runner->processFile('dectest/fma.decTest');
#$runner->processFile('dectest/inexact.decTest');
#$runner->processFile('dectest/invert.decTest');
#$runner->processFile('dectest/ln.decTest');
#$runner->processFile('dectest/log10.decTest');
#$runner->processFile('dectest/logb.decTest');
#$runner->processFile('dectest/max.decTest');
#$runner->processFile('dectest/maxmag.decTest');
#$runner->processFile('dectest/min.decTest');
#$runner->processFile('dectest/minmag.decTest');
#$runner->processFile('dectest/minus.decTest');
#$runner->processFile('dectest/multiply.decTest');
#$runner->processFile('dectest/nextminus.decTest');
#$runner->processFile('dectest/nextplus.decTest');
#$runner->processFile('dectest/nexttoward.decTest');
#$runner->processFile('dectest/or.decTest');
#$runner->processFile('dectest/plus.decTest');
#$runner->processFile('dectest/power.decTest');
#$runner->processFile('dectest/powersqrt.decTest');
#$runner->processFile('dectest/quantize.decTest');
#$runner->processFile('dectest/randombound32.decTest');
#$runner->processFile('dectest/randoms.decTest');
#$runner->processFile('dectest/reduce.decTest');
#$runner->processFile('dectest/remainder.decTest');
#$runner->processFile('dectest/remaindernear.decTest');
#$runner->processFile('dectest/rescale.decTest');
#$runner->processFile('dectest/rotate.decTest');
#$runner->processFile('dectest/rounding.decTest');
#$runner->processFile('dectest/samequantum.decTest');
#$runner->processFile('dectest/scaleb.decTest');
#$runner->processFile('dectest/shift.decTest');
#$runner->processFile('dectest/squareroot.decTest');
#$runner->processFile('dectest/subtract.decTest');
#$runner->processFile('dectest/testall.decTest');
#$runner->processFile('dectest/tointegral.decTest');
#$runner->processFile('dectest/tointegralx.decTest');
#$runner->processFile('dectest/trim.decTest');
$runner->processFile('dectest/xor.decTest');
