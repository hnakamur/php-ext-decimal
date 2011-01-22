#ifndef PHP_DECIMAL_H
#define PHP_DECIMAL_H 1

#define PHP_DECIMAL_VERSION "0.1"
#define PHP_DECIMAL_EXTNAME "Decimal"

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(decimal)
    zend_object_value defaultContext;
ZEND_END_MODULE_GLOBALS(decimal)

#ifdef ZTS
#define DECIMAL_G(v) TSRMG(decimal_globals_id, zend_decimal_globals *, v)
#else
#define DECIMAL_G(v) (decimal_globals.v)
#endif

extern zend_module_entry decimal_module_entry;
#define phpext_decimal_ptr &decimal_module_entry

PHP_METHOD(DecimalContext, __construct);
PHP_METHOD(DecimalContext, getDefaultContext);
PHP_METHOD(DecimalContext, init);

PHP_METHOD(Decimal, __construct);
PHP_METHOD(Decimal, __toString);
PHP_METHOD(Decimal, abs);
PHP_METHOD(Decimal, add);
PHP_METHOD(Decimal, and);
PHP_METHOD(Decimal, compare);
PHP_METHOD(Decimal, compareSignal);
PHP_METHOD(Decimal, compareTotal);
PHP_METHOD(Decimal, compareTotalMag);
PHP_METHOD(Decimal, divide);
PHP_METHOD(Decimal, divideInteger);
PHP_METHOD(Decimal, exp);
PHP_METHOD(Decimal, fma);
PHP_METHOD(Decimal, invert);
PHP_METHOD(Decimal, ln);
PHP_METHOD(Decimal, logb);
PHP_METHOD(Decimal, log10);
PHP_METHOD(Decimal, max);
PHP_METHOD(Decimal, maxMag);
PHP_METHOD(Decimal, min);
PHP_METHOD(Decimal, minMag);
PHP_METHOD(Decimal, minus);
PHP_METHOD(Decimal, multiply);
PHP_METHOD(Decimal, nextMinus);
PHP_METHOD(Decimal, nextPlus);
PHP_METHOD(Decimal, nextToward);
PHP_METHOD(Decimal, or);
PHP_METHOD(Decimal, plus);
PHP_METHOD(Decimal, power);
PHP_METHOD(Decimal, quantize);
PHP_METHOD(Decimal, remainder);
PHP_METHOD(Decimal, remainderNear);
PHP_METHOD(Decimal, rescale);
PHP_METHOD(Decimal, rotate);
PHP_METHOD(Decimal, sameQuantum);
PHP_METHOD(Decimal, scaleB);
PHP_METHOD(Decimal, shift);
PHP_METHOD(Decimal, squareRoot);
PHP_METHOD(Decimal, subtract);
PHP_METHOD(Decimal, toIntegralExact);
PHP_METHOD(Decimal, toIntegralValue);
PHP_METHOD(Decimal, xor);
PHP_METHOD(Decimal, isCanonical);
PHP_METHOD(Decimal, isFinite);
PHP_METHOD(Decimal, isInfinite);
PHP_METHOD(Decimal, isNaN);
PHP_METHOD(Decimal, isNegative);
PHP_METHOD(Decimal, isNormal);
PHP_METHOD(Decimal, isQNaN);
PHP_METHOD(Decimal, isSNaN);
PHP_METHOD(Decimal, isSpecial);
PHP_METHOD(Decimal, isSubnormal);
PHP_METHOD(Decimal, isZero);
PHP_METHOD(Decimal, reduce);
PHP_METHOD(Decimal, trim);
PHP_METHOD(Decimal, toEngString);

PHP_MINIT_FUNCTION(decimal);
PHP_RINIT_FUNCTION(decimal);

#include "decNumber/decContext.h"
#include "decNumber/decNumber.h"

typedef struct {
    zend_object  std;
    decContext  *context;
} decimal_context_object;

typedef struct {
    zend_object std;
    decNumber  *number;
} decimal_number_object;

#endif
