#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php/Zend/zend_exceptions.h"
#include "php_decimal.h"

ZEND_DECLARE_MODULE_GLOBALS(decimal)

/* {{{ Module struct */
zend_module_entry decimal_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_DECIMAL_EXTNAME,
    NULL,
    PHP_MINIT(decimal),
    NULL,
    PHP_RINIT(decimal),
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_DECIMAL_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DECIMAL
ZEND_GET_MODULE(decimal)
#endif

zend_class_entry *decimal_ce_DecimalContext;
zend_class_entry *decimal_ce_Decimal;
zend_class_entry *decimal_ce_DecimalException;

static zend_object_handlers decimal_context_handlers;
static zend_object_handlers decimal_handlers;
static zend_object_handlers decimal_exception_handlers;

ZEND_BEGIN_ARG_INFO_EX(arginfo_context_clearStatus, 0, 0, 0)
    ZEND_ARG_INFO(0, status)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_context_create, 0, 0, 1)
    ZEND_ARG_INFO(0, kind)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_context_init, 0, 0, 1)
    ZEND_ARG_INFO(0, kind)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_context_restoreStatus, 0, 0, 0)
    ZEND_ARG_INFO(0, status)
    ZEND_ARG_INFO(0, mask)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_context_saveStatus, 0, 0, 0)
    ZEND_ARG_INFO(0, mask)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_context_setPrecision, 0, 0, 1)
    ZEND_ARG_INFO(0, precision)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_context_setRounding, 0, 0, 1)
    ZEND_ARG_INFO(0, rounding)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_context_setStatus, 0, 0, 1)
    ZEND_ARG_INFO(0, status)
ZEND_END_ARG_INFO()

const zend_function_entry decimal_context_class_functions[] = {
    PHP_ME(DecimalContext, __construct,
        arginfo_context_create, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
    PHP_ME(DecimalContext, getDefaultContext, NULL,
        ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(DecimalContext, init, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};


ZEND_BEGIN_ARG_INFO_EX(arginfo_number_create, 0, 0, 1)
    ZEND_ARG_INFO(0, val)
    ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_number_unary, 0, 0, 0)
    ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_number_binary, 0, 0, 1)
    ZEND_ARG_INFO(0, rhs)
    ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_number_ternary, 0, 0, 2)
    ZEND_ARG_INFO(0, rhs)
    ZEND_ARG_INFO(0, fhs)
    ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

const zend_function_entry decimal_number_class_functions[] = {
    PHP_ME(Decimal, __construct,
            arginfo_number_create, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, __toString, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, abs, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, add, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, divide, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, divideInteger, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, exp, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, fma, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, invert, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, ln, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, logb, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, log10, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, max, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, maxMag, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, min, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, minMag, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, minus, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, multiply, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, nextMinus, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, nextPlus, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, nextToward, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, or, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, plus, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, quantize, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, remainder, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, remainderNear, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, rescale, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, rotate, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, sameQuantum, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, scaleB, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, shift, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, squareRoot, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, subtract, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, toIntegralExact, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, toIntegralValue, arginfo_number_unary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, xor, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isCanonical, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isFinite, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isInfinite, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isNaN, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isNegative, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isNormal, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isQNaN, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isSNaN, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isSpecial, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isSubnormal, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, isZero, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, reduce, arginfo_number_binary, ZEND_ACC_PUBLIC)
    PHP_ME(Decimal, trim, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

/*
 * Number
 */
#include "decNumber/decNumberLocal.h"

#define decimal_get_context_obj(defaultCondition, zcontext) \
    ((decimal_context_object *)zend_object_store_get_object_by_handle( \
        ((defaultCondition) ? DECIMAL_G(defaultContext).handle \
                            : Z_OBJ_HANDLE_P(zcontext)) \
        TSRMLS_CC \
    ))

static decNumber *_decimal_allocDecNumber(long numdigits)
{
    uInt needbytes;
    needbytes = sizeof(decNumber) + (D2U(numdigits) - 1) * sizeof(Unit);
    return (decNumber *)emalloc(needbytes);
}

static void decimal_number_object_free(zend_object *object TSRMLS_DC)
{
    decimal_number_object* dco;

    dco = (decimal_number_object*)object;
    zend_object_std_dtor(&dco->std TSRMLS_CC);
    if (dco->number) {
        efree(dco->number);
    }
    efree(dco);
}

static zend_object_value
decimal_number_object_new(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    decimal_number_object *intern;

    intern = emalloc(sizeof(decimal_number_object));
    zend_object_std_init(&(intern->std), ce TSRMLS_CC);
    zend_hash_copy(intern->std.properties,
        &ce->default_properties, (copy_ctor_func_t) zval_add_ref,
        NULL, sizeof(zval *));

    retval.handle = zend_objects_store_put(
        intern,
        (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)decimal_number_object_free,
        NULL TSRMLS_CC);
    retval.handlers = &decimal_handlers;
    return retval;
}

PHPAPI int decimal_number_initialize(decimal_number_object *number_obj,
    char *val, decimal_context_object *context_obj)
{
    decContext *context;
    decNumber *number;

    context = context_obj->context;
    number = _decimal_allocDecNumber(context->digits);
    number = decNumberFromString(number, val, context);
    number_obj->number = number;
    return SUCCESS;
}

static decimal_number_object *decimal_convert_str_to_number(char *val,
    decimal_context_object *context_obj TSRMLS_DC)
{
    zend_object_value number_ov;
    decimal_number_object *number_obj;
    decContext *context;
    decNumber *number;

    number_ov = decimal_number_object_new(decimal_ce_Decimal TSRMLS_CC);
    number_obj = zend_object_store_get_object_by_handle(
        number_ov.handle TSRMLS_CC);

    if (decimal_number_initialize(number_obj, val, context_obj) == FAILURE) {
        return NULL;
    }

    return number_obj;
}

#define decimal_get_number_obj(zval_ptr) \
    ((decimal_number_object *)zend_object_store_get_object(zval_ptr TSRMLS_CC))

static decimal_number_object *decimal_convert_zval_to_number(zval *zarg,
    decimal_context_object *context_obj TSRMLS_DC)
{
	switch (Z_TYPE_P(zarg)) {
	case IS_STRING:
        return decimal_convert_str_to_number(Z_STRVAL_P(zarg), context_obj
            TSRMLS_CC);
	case IS_OBJECT:
		if (instanceof_function(Z_OBJCE_P(zarg), decimal_ce_Decimal
            TSRMLS_CC))
        {
            return decimal_get_number_obj(zarg);
        }
	}
    return NULL;
}

static int decimal_convert_arg_to_number(zval *zarg, int arg_pos,
    decimal_context_object *context_obj, decimal_number_object **number_obj
    TSRMLS_DC)
{
    *number_obj = decimal_convert_zval_to_number(zarg, context_obj TSRMLS_CC);
    if (*number_obj == NULL) {
		zend_error(E_WARNING, "%s() expects parameter %d to be %s, %s given",
			get_active_function_name(TSRMLS_C), arg_pos,
			"resource(Decimal) or String", zend_zval_type_name(zarg));
		return FAILURE;
    }
    return SUCCESS;
}

#define RETURN_DECIMAL(zval_ptr, number_ov)    \
    ALLOC_INIT_ZVAL(zval_ptr);                 \
    Z_TYPE_P(zval_ptr) = IS_OBJECT;            \
    Z_OBJVAL_P(zval_ptr) = number_ov;          \
    RETURN_ZVAL(zval_ptr, 0, 1);

/* {{{ proto Decimal::__construct(string val, DecimalContext context)
   Creates new Decimal object from number string
*/
PHP_METHOD(Decimal, __construct)
{
    char *val;
    long val_len;
    zval *zcontext;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|O",
        &val, &val_len, &zcontext, decimal_ce_DecimalContext)
        == SUCCESS
    ) {
        decimal_number_initialize(
            decimal_get_number_obj(getThis()),
            val,
            decimal_get_context_obj(ZEND_NUM_ARGS() < 2, zcontext));
    }
}
/* }}} */

static decimal_number_object *decimal_number_new_result(
    int32_t digits, zend_object_value *number_ov TSRMLS_DC)
{
    decimal_number_object *result_obj;

    *number_ov = decimal_number_object_new(decimal_ce_Decimal TSRMLS_CC);
    result_obj = zend_object_store_get_object_by_handle(
        number_ov->handle TSRMLS_CC);
    result_obj->number = _decimal_allocDecNumber(digits);
    return result_obj;
}

typedef int32_t (*decimal_unary_bool_func_t)(const decNumber *a, decContext *context);
typedef decNumber *(*decimal_unary_func_t)(decNumber *res, const decNumber *a, decContext *context);
typedef decNumber *(*decimal_binary_func_t)(decNumber *res, const decNumber *a, const decNumber *b, decContext *context);
typedef decNumber *(*decimal_ternary_func_t)(decNumber *res, const decNumber *a, const decNumber *b, const decNumber *c, decContext *context);

static void decimal_apply_unary_bool(decimal_unary_bool_func_t func,
    INTERNAL_FUNCTION_PARAMETERS)
{
    zval *zcontext;
    decimal_context_object *context_obj;
    decimal_number_object *this_obj;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|O",
        &zcontext, decimal_ce_DecimalContext) == FAILURE
    ) {
        RETURN_NULL();
    }

    this_obj = decimal_get_number_obj(getThis());
    context_obj = decimal_get_context_obj(ZEND_NUM_ARGS() < 1, zcontext);

    RETURN_BOOL(func(this_obj->number, context_obj->context));
}

static void decimal_apply_unary(decimal_unary_func_t func,
    INTERNAL_FUNCTION_PARAMETERS)
{
    zval *zcontext;
    decimal_context_object *context_obj;
    decimal_number_object *this_obj, *result_obj;
    zend_object_value result_ov;
    zval *zretval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|O",
        &zcontext, decimal_ce_DecimalContext) == FAILURE
    ) {
        RETURN_NULL();
    }

    this_obj = decimal_get_number_obj(getThis());
    context_obj = decimal_get_context_obj(ZEND_NUM_ARGS() < 1, zcontext);

    result_obj = decimal_number_new_result(context_obj->context->digits,
        &result_ov TSRMLS_CC);
    result_obj->number = func(result_obj->number, this_obj->number,
        context_obj->context);
    RETURN_DECIMAL(zretval, result_ov);
}

static void decimal_apply_binary(decimal_binary_func_t func,
    INTERNAL_FUNCTION_PARAMETERS)
{
    zval *za, *zcontext;
    decimal_context_object *context_obj;
    decimal_number_object *this_obj, *b_obj, *result_obj;
    zend_object_value result_ov;
    zval *zretval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|O",
        &za, &zcontext, decimal_ce_DecimalContext) == FAILURE
    ) {
        RETURN_NULL();
    }

    this_obj = decimal_get_number_obj(getThis());
    context_obj = decimal_get_context_obj(ZEND_NUM_ARGS() < 1, zcontext);
    if (decimal_convert_arg_to_number(za, 1, context_obj, &b_obj TSRMLS_CC)
        == FAILURE
    ) {
        RETURN_NULL();
    }

    result_obj = decimal_number_new_result(context_obj->context->digits,
        &result_ov TSRMLS_CC);
    result_obj->number = func(result_obj->number, this_obj->number,
        b_obj->number, context_obj->context);
    RETURN_DECIMAL(zretval, result_ov);
}

/* {{{ proto Decimal::abs([DecimalContext context])
   Returns the absolute value of this.
*/
PHP_METHOD(Decimal, abs)
{
    decimal_apply_unary(decNumberAbs, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::add(mixed rhs [, DecimalContext context])
   Returns the result of adding this to rhs.
*/
PHP_METHOD(Decimal, add)
{
    decimal_apply_binary(decNumberAdd, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::and(mixed rhs [, DecimalContext context])
   Returns the result of the digit-wise logical and of this and rhs.
*/
PHP_METHOD(Decimal, and)
{
    decimal_apply_binary(decNumberAnd, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::compare(mixed rhs [, DecimalContext context])
   Compares this and rhs.
   Returns -1 if this < rhs, 0 if this == rhs (this - rhs == 0),
   1 if this > rhs, NaN if one or both is NaN.
*/
PHP_METHOD(Decimal, compare)
{
    decimal_apply_binary(decNumberCompare, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::compareSignal(mixed rhs [, DecimalContext context])
   Compares this and rhs.
   Returns -1 if this < rhs, 0 if this == rhs (this - rhs == 0),
   1 if this > rhs,
   throws DecimalException if one or both is NaN.
*/
PHP_METHOD(Decimal, compareSignal)
{
    decimal_apply_binary(decNumberCompareSignal, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::compareTotal(mixed rhs [, DecimalContext context])
   Compares this and rhs using the IEEE 754 total ordering.
   Returns the result of comparison of this and rhs. -1 if this < rhs,
   0 if this == rhs (this - rhs == 0), 1 if this > rhs.

   Total order: -NaN < -sNaN < -Infinity < -finites < -0 < +0 < +finites < +Infinity < +sNaN < +NaN. Also, 1.000 < 1.0 (etc.) and NaNs are ordered by payload.
*/
PHP_METHOD(Decimal, compareTotal)
{
    decimal_apply_binary(decNumberCompareTotal, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::compareTotalMag(mixed rhs [, DecimalContext context])
   Compares the magnitude of this and rhs using the IEEE 754 total ordering.
   It is identical to compareTotal except the signs of this and rhs are
   taken to non-nagative.
*/
PHP_METHOD(Decimal, compareTotalMag)
{
    decimal_apply_binary(decNumberCompareTotalMag, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::divide(mixed rhs [, DecimalContext context])
   Returns the result of dividing this by rhs.
*/
PHP_METHOD(Decimal, divide)
{
    decimal_apply_binary(decNumberDivide, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::divideInteger(mixed rhs [, DecimalContext context])
   Returns the integer part of the result of dividing this by rhs.
   Throws DecimalException if the result has more digits than
   $context->presicion.
*/
PHP_METHOD(Decimal, divideInteger)
{
    decimal_apply_binary(decNumberDivideInteger, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::exp(mixed rhs [, DecimalContext context])
   Returns the result of e raised to the power of rhs, rounded if necessary
   using $context->precision and DecimalContext::ROUND_HALF_EVEN.

   Finite results will always be full precision and inexact, except when
   rhs is a zero or -Infinity (giving 1 or 0 respectively). Inexact results
   will almost always be correctly rounded, but may be up to 1 ulp (unit in
   last place) in error in rare cases.

   This is a mathematical function; the 10^6 restrictions on precision and
   range apply as described above.
*/
PHP_METHOD(Decimal, exp)
{
    decimal_apply_unary(decNumberExp, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::fma(mixed rhs, mixed fhs [, DecimalContext context])
   Returns the result of multiplying this by rhs then adding fhs to that
   intermediate result. It is equivalent to a multiplication followed by an
   addition except that the intermediate result is not rounded and will not
   cause overflow or underflow. That is, only the final result is rounded and
   checked.

   This is a mathematical function; the 10^6 restrictions on precision and
   range apply as described above.
*/
PHP_METHOD(Decimal, fma)
{
    decimal_apply_ternary(decNumberFMA, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::invert([DecimalContext context])
*/
PHP_METHOD(Decimal, invert)
{
    decimal_apply_unary(decNumberInvert, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::ln([DecimalContext context])
*/
PHP_METHOD(Decimal, ln)
{
    decimal_apply_unary(decNumberLn, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::logb(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, logb)
{
    decimal_apply_unary(decNumberLogB, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::log10(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, log10)
{
    decimal_apply_unary(decNumberLog10, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::max(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, max)
{
    decimal_apply_binary(decNumberMax, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::maxMag(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, maxMag)
{
    decimal_apply_binary(decNumberMaxMag, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::min(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, min)
{
    decimal_apply_binary(decNumberMin, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::minMag(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, minMag)
{
    decimal_apply_binary(decNumberMinMag, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::minus([DecimalContext context])
*/
PHP_METHOD(Decimal, minus)
{
    decimal_apply_unary(decNumberMinus, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::multiply(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, multiply)
{
    decimal_apply_binary(decNumberMultiply, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::nextMinus([DecimalContext context])
*/
PHP_METHOD(Decimal, nextMinus)
{
    decimal_apply_unary(decNumberNextMinus, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::nextPlus([DecimalContext context])
*/
PHP_METHOD(Decimal, nextPlus)
{
    decimal_apply_unary(decNumberNextPlus, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::nextToward(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, nextToward)
{
    decimal_apply_binary(decNumberNextToward, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::or(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, or)
{
    decimal_apply_binary(decNumberOr, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::plus([DecimalContext context])
*/
PHP_METHOD(Decimal, plus)
{
    decimal_apply_unary(decNumberPlus, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::power(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, power)
{
    decimal_apply_binary(decNumberPower, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::quantize(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, quantize)
{
    decimal_apply_binary(decNumberQuantize, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::remainder(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, remainder)
{
    decimal_apply_binary(decNumberRemainder, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::remainderNear(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, remainderNear)
{
    decimal_apply_binary(decNumberRemainderNear, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::rescale(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, rescale)
{
    decimal_apply_binary(decNumberRescale, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::rotate(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, rotate)
{
    decimal_apply_binary(decNumberRotate, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::sameQuantum(mixed rhs [, DecimalContext context])
   The context will be only used for converting rhs to Decimal.
*/
PHP_METHOD(Decimal, sameQuantum)
{
    zval *za, *zcontext;
    decimal_context_object *context_obj;
    decimal_number_object *this_obj, *b_obj, *result_obj;
    zend_object_value result_ov;
    zval *zretval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|O",
        &za, &zcontext, decimal_ce_DecimalContext) == FAILURE
    ) {
        RETURN_NULL();
    }

    this_obj = decimal_get_number_obj(getThis());
    context_obj = decimal_get_context_obj(ZEND_NUM_ARGS() < 1, zcontext);
    if (decimal_convert_arg_to_number(za, 1, context_obj, &b_obj TSRMLS_CC)
        == FAILURE
    ) {
        RETURN_NULL();
    }

    result_obj = decimal_number_new_result(context_obj->context->digits,
        &result_ov TSRMLS_CC);
    result_obj->number = decNumberSameQuantum(result_obj->number,
        this_obj->number, b_obj->number);
    RETURN_DECIMAL(zretval, result_ov);
}
/* }}} */

/* {{{ proto Decimal::scaleB(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, scaleB)
{
    decimal_apply_binary(decNumberScaleB, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::shift(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, shift)
{
    decimal_apply_binary(decNumberShift, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::squareRoot([DecimalContext context])
*/
PHP_METHOD(Decimal, squareRoot)
{
    decimal_apply_unary(decNumberSquareRoot, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::subtract(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, subtract)
{
    decimal_apply_binary(decNumberSubtract, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::toIntegralExact([DecimalContext context])
*/
PHP_METHOD(Decimal, toIntegralExact)
{
    decimal_apply_unary(decNumberToIntegralExact, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::toIntegralValue([DecimalContext context])
*/
PHP_METHOD(Decimal, toIntegralValue)
{
    decimal_apply_unary(decNumberToIntegralValue, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::xor(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, xor)
{
    decimal_apply_binary(decNumberXor, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::isCanonical()
*/
PHP_METHOD(Decimal, isCanonical)
{
    RETURN_TRUE;
}
/* }}} */

/* {{{ proto Decimal::isFinite()
*/
PHP_METHOD(Decimal, isFinite)
{
    decimal_number_object *this_obj;
    this_obj = decimal_get_number_obj(getThis());
    RETURN_BOOL(decNumberIsFinite(this_obj->number));
}
/* }}} */

/* {{{ proto Decimal::isInfinite()
*/
PHP_METHOD(Decimal, isInfinite)
{
    decimal_number_object *this_obj;
    this_obj = decimal_get_number_obj(getThis());
    RETURN_BOOL(decNumberIsInfinite(this_obj->number));
}
/* }}} */

/* {{{ proto Decimal::isNaN()
*/
PHP_METHOD(Decimal, isNaN)
{
    decimal_number_object *this_obj;
    this_obj = decimal_get_number_obj(getThis());
    RETURN_BOOL(decNumberIsNaN(this_obj->number));
}
/* }}} */

/* {{{ proto Decimal::isNegative()
*/
PHP_METHOD(Decimal, isNegative)
{
    decimal_number_object *this_obj;
    this_obj = decimal_get_number_obj(getThis());
    RETURN_BOOL(decNumberIsNegative(this_obj->number));
}
/* }}} */

/* {{{ proto Decimal::isNormal([DecimalContext context])
*/
PHP_METHOD(Decimal, isNormal)
{
    decimal_apply_unary_bool(decNumberIsNormal, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::isQNaN()
*/
PHP_METHOD(Decimal, isQNaN)
{
    decimal_number_object *this_obj;
    this_obj = decimal_get_number_obj(getThis());
    RETURN_BOOL(decNumberIsQNaN(this_obj->number));
}
/* }}} */

/* {{{ proto Decimal::isSNaN()
*/
PHP_METHOD(Decimal, isSNaN)
{
    decimal_number_object *this_obj;
    this_obj = decimal_get_number_obj(getThis());
    RETURN_BOOL(decNumberIsSNaN(this_obj->number));
}
/* }}} */

/* {{{ proto Decimal::isSpecial()
*/
PHP_METHOD(Decimal, isSpecial)
{
    decimal_number_object *this_obj;
    this_obj = decimal_get_number_obj(getThis());
    RETURN_BOOL(decNumberIsSpecial(this_obj->number));
}
/* }}} */

/* {{{ proto Decimal::isSubnormal([DecimalContext context])
*/
PHP_METHOD(Decimal, isSubnormal)
{
    decimal_apply_unary_bool(decNumberIsSubnormal, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::isZero()
*/
PHP_METHOD(Decimal, isZero)
{
    decimal_number_object *this_obj;
    this_obj = decimal_get_number_obj(getThis());
    RETURN_BOOL(decNumberIsZero(this_obj->number));
}
/* }}} */

/* {{{ proto Decimal::reduce(mixed rhs [, DecimalContext context])
*/
PHP_METHOD(Decimal, reduce)
{
    decimal_apply_unary(decNumberReduce, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto Decimal::trim()
*/
PHP_METHOD(Decimal, trim)
{
    decimal_number_object *this_obj, *result_obj;
    zend_object_value result_ov;
    zval *zretval;

    this_obj = decimal_get_number_obj(getThis());

    result_obj = decimal_number_new_result(this_obj->number->digits,
        &result_ov TSRMLS_CC);
    result_obj->number = decNumberCopy(result_obj->number, this_obj->number);
    result_obj->number = decNumberTrim(result_obj->number);
    RETURN_DECIMAL(zretval, result_ov);
}
/* }}} */

/* {{{ proto Decimal::__toString()
   convert Decimal object to string
*/
PHP_METHOD(Decimal, __toString)
{
    decimal_number_object *number_obj;
    decNumber *number;
    char *buf;
    size_t tmp_size, actual_size;

    number_obj = (decimal_number_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    number = number_obj->number;
    tmp_size = number->digits + 14;
    buf = emalloc(tmp_size);
    buf = decNumberToString(number, buf);
    actual_size = strlen(buf) + 1;
    if (actual_size != tmp_size) {
        buf = erealloc(buf, actual_size);
    }
    RETURN_STRING(buf, 0);
}

/*
 * Context
 */

/* {{{ decimal_context_read_property */
zval *decimal_context_read_property(zval *object, zval *member, int type TSRMLS_DC)
{
	decimal_context_object *obj;
	zval *retval;
	zval tmp_member;
    long value = -1;

 	if (member->type != IS_STRING) {
		tmp_member = *member;
		zval_copy_ctor(&tmp_member);
		convert_to_string(&tmp_member);
		member = &tmp_member;
	}

	obj = (decimal_context_object *)zend_objects_get_address(object TSRMLS_CC);

#define GET_VALUE_FROM_STRUCT(n,m)            \
	if (strcmp(Z_STRVAL_P(member), m) == 0) { \
		value = obj->context->n;              \
	}
	GET_VALUE_FROM_STRUCT(digits,   "precision");
	GET_VALUE_FROM_STRUCT(emax,     "emax");
	GET_VALUE_FROM_STRUCT(emin,     "emin");
	GET_VALUE_FROM_STRUCT(round,    "rounding");
	GET_VALUE_FROM_STRUCT(status,   "status");
	GET_VALUE_FROM_STRUCT(traps,    "traps");
	GET_VALUE_FROM_STRUCT(clamp,    "clamp");
	GET_VALUE_FROM_STRUCT(extended, "extended");

	ALLOC_INIT_ZVAL(retval);
	Z_SET_REFCOUNT_P(retval, 0);

	if (value == -1) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unknown property (%s)", Z_STRVAL_P(member));
	}

	ZVAL_LONG(retval, value);

	if (member == &tmp_member) {
		zval_dtor(member);
	}

	return retval;
}
/* }}} */

/* {{{ decimal_context_write_property */
void decimal_context_write_property(zval *object, zval *member, zval *value TSRMLS_DC)
{
	decimal_context_object *obj;
	zval tmp_member, tmp_value;
	int found = 0;

 	if (member->type != IS_STRING) {
		tmp_member = *member;
		zval_copy_ctor(&tmp_member);
		convert_to_string(&tmp_member);
		member = &tmp_member;
	}
	obj = (decimal_context_object *)zend_object_store_get_object(
        object TSRMLS_CC);

#define SET_VALUE_FROM_STRUCT(n,m)            \
	if (strcmp(Z_STRVAL_P(member), m) == 0) { \
		if (value->type != IS_LONG) {         \
			tmp_value = *value;               \
			zval_copy_ctor(&tmp_value);       \
			convert_to_long(&tmp_value);      \
			value = &tmp_value;               \
		}                                     \
		found = 1;                            \
		obj->context->n = Z_LVAL_P(value); \
		if (value == &tmp_value) {         \
			zval_dtor(value);              \
		}                                  \
	}

	SET_VALUE_FROM_STRUCT(digits,   "precision");
	SET_VALUE_FROM_STRUCT(emax,     "emax");
	SET_VALUE_FROM_STRUCT(emin,     "emin");
	SET_VALUE_FROM_STRUCT(round,    "rounding");
	SET_VALUE_FROM_STRUCT(status,   "status");
	SET_VALUE_FROM_STRUCT(traps,    "traps");
	SET_VALUE_FROM_STRUCT(clamp,    "clamp");
	SET_VALUE_FROM_STRUCT(extended, "extended");

	if (!found) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unknown property (%s)", Z_STRVAL_P(member));
	}

	if (member == &tmp_member) {
		zval_dtor(member);
	}
}
/* }}} */

static HashTable *decimal_context_get_properties(zval *object TSRMLS_DC)
{
	HashTable *props;
	zval *zv;
	decimal_context_object *context_obj;


	context_obj = (decimal_context_object *)zend_object_store_get_object(
        object TSRMLS_CC);

	props = zend_std_get_properties(object TSRMLS_CC);

#define DECIMAL_CONTEXT_HASH_ADD_PROPERTY(f,n) \
	MAKE_STD_ZVAL(zv); \
	ZVAL_LONG(zv, context_obj->context->f); \
	zend_hash_update(props, n, strlen(n) + 1, &zv, sizeof(zval), NULL);

	DECIMAL_CONTEXT_HASH_ADD_PROPERTY(digits,   "precision");
	DECIMAL_CONTEXT_HASH_ADD_PROPERTY(emax,     "emax");
	DECIMAL_CONTEXT_HASH_ADD_PROPERTY(emin,     "emin");
	DECIMAL_CONTEXT_HASH_ADD_PROPERTY(round,    "rounding");
	DECIMAL_CONTEXT_HASH_ADD_PROPERTY(status,   "status");
	DECIMAL_CONTEXT_HASH_ADD_PROPERTY(traps,    "traps");
	DECIMAL_CONTEXT_HASH_ADD_PROPERTY(clamp,    "clamp");
	DECIMAL_CONTEXT_HASH_ADD_PROPERTY(extended, "extended");

	return props;
}

static void decimal_context_object_free(zend_object *object TSRMLS_DC)
{
    decimal_context_object* dco = (decimal_context_object*)object;
    zend_object_std_dtor(&dco->std TSRMLS_CC);
    if (dco->context) {
        efree(dco->context);
    }
    efree(dco);
}

static zend_object_value
decimal_context_object_new(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    decimal_context_object *intern;

    intern = emalloc(sizeof(decimal_context_object));
    zend_object_std_init(&(intern->std), ce TSRMLS_CC);
    zend_hash_copy(intern->std.properties,
        &ce->default_properties, (copy_ctor_func_t) zval_add_ref,
        NULL, sizeof(zval *));

    retval.handle = zend_objects_store_put(
        intern,
        (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)decimal_context_object_free,
        NULL TSRMLS_CC);
    retval.handlers = &decimal_context_handlers;
    return retval;
}

PHPAPI int decimal_context_initialize(decimal_context_object *context_obj,
    long kind)
{
    decContext *context;

    context = (decContext *)emalloc(sizeof(decContext));
    decContextDefault(context, kind);
    context_obj->context = context;
    return SUCCESS;
}

/* {{{ proto DecimalContext::__construct(long kind)
   Creates new DecimalContext object
*/
PHP_METHOD(DecimalContext, __construct)
{
    long kind;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &kind)
        == SUCCESS
    ) {
        decimal_context_initialize(zend_object_store_get_object(
            getThis() TSRMLS_CC), kind);
    }
}
/* }}} */


/* {{{ proto DecimalContext::getDefault()
   Returns the default DecimalContext
*/
PHP_METHOD(DecimalContext, getDefaultContext)
{
    zval *retval;

    ALLOC_INIT_ZVAL(retval);
    Z_TYPE_P(retval) = IS_OBJECT;
    Z_OBJVAL_P(retval) = DECIMAL_G(defaultContext);
    RETURN_ZVAL(retval, 0, 1);
}
/* }}} */

/* {{{ proto DecimalContext::init(long kind)
   Initialize a DecimalContext object
*/
PHP_METHOD(DecimalContext, init)
{
    long kind;
    decimal_context_object *context_obj;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &kind)
        == FAILURE
    ) {
        RETURN_FALSE;
    }

    context_obj = zend_object_store_get_object(getThis() TSRMLS_CC);
    decContextDefault(context_obj->context, kind);
    RETURN_TRUE;
}
/* }}} */

PHP_MINIT_FUNCTION(decimal)
{
    zend_class_entry ce_context, ce_decimal, ce_decimal_exception;
    zend_class_entry *ce_exception;

    INIT_CLASS_ENTRY(ce_context, "DecimalContext",
        decimal_context_class_functions);
    decimal_ce_DecimalContext = zend_register_internal_class(
        &ce_context TSRMLS_CC);
    decimal_ce_DecimalContext->create_object = decimal_context_object_new;
    memcpy(&decimal_context_handlers, zend_get_std_object_handlers(),
        sizeof(zend_object_handlers));
	decimal_context_handlers.read_property = decimal_context_read_property;
	decimal_context_handlers.write_property = decimal_context_write_property;
	decimal_context_handlers.get_properties = decimal_context_get_properties;

#define REGISTER_CONTEXT_CONST_L(const_name, value) \
	zend_declare_class_constant_long(decimal_ce_DecimalContext, const_name, sizeof(const_name)-1, value TSRMLS_CC);

	REGISTER_CONTEXT_CONST_L("INIT_BASE",       DEC_INIT_BASE);
	REGISTER_CONTEXT_CONST_L("INIT_DECIMAL32",  DEC_INIT_DECIMAL32);
	REGISTER_CONTEXT_CONST_L("INIT_DECIMAL64",  DEC_INIT_DECIMAL64);
	REGISTER_CONTEXT_CONST_L("INIT_DECIMAL128", DEC_INIT_DECIMAL128);

	REGISTER_CONTEXT_CONST_L("ROUND_CEILING",   DEC_ROUND_CEILING);
	REGISTER_CONTEXT_CONST_L("ROUND_UP",        DEC_ROUND_UP);
	REGISTER_CONTEXT_CONST_L("ROUND_HALF_UP",   DEC_ROUND_HALF_UP);
	REGISTER_CONTEXT_CONST_L("ROUND_HALF_EVEN", DEC_ROUND_HALF_EVEN);
	REGISTER_CONTEXT_CONST_L("ROUND_HALF_DOWN", DEC_ROUND_HALF_DOWN);
	REGISTER_CONTEXT_CONST_L("ROUND_DOWN",      DEC_ROUND_DOWN);
	REGISTER_CONTEXT_CONST_L("ROUND_FLOOR",     DEC_ROUND_FLOOR);
	REGISTER_CONTEXT_CONST_L("ROUND_05UP",      DEC_ROUND_05UP);
	REGISTER_CONTEXT_CONST_L("ROUND_MAX",       DEC_ROUND_MAX);

    REGISTER_CONTEXT_CONST_L("CONVERSION_SYNTAX",    DEC_Conversion_syntax);
    REGISTER_CONTEXT_CONST_L("DIVISION_BY_ZERO",     DEC_Division_by_zero);
    REGISTER_CONTEXT_CONST_L("DIVISION_IMPOSSIBLE",  DEC_Division_impossible);
    REGISTER_CONTEXT_CONST_L("DIVISION_UNDEFINED",   DEC_Division_undefined);
    REGISTER_CONTEXT_CONST_L("INSUFFICIENT_STORAGE", DEC_Insufficient_storage);
    REGISTER_CONTEXT_CONST_L("INEXACT",              DEC_Inexact);
    REGISTER_CONTEXT_CONST_L("INVALID_CONTEXT",      DEC_Invalid_context);
    REGISTER_CONTEXT_CONST_L("INVALID_OPERATION",    DEC_Invalid_operation);
    REGISTER_CONTEXT_CONST_L("LOST_DIGITS",          DEC_Lost_digits);
    REGISTER_CONTEXT_CONST_L("OVERFLOW",             DEC_Overflow);
    REGISTER_CONTEXT_CONST_L("CLAMPED",              DEC_Clamped);
    REGISTER_CONTEXT_CONST_L("ROUNDED",              DEC_Rounded);
    REGISTER_CONTEXT_CONST_L("SUBNORMAL",            DEC_Subnormal);
    REGISTER_CONTEXT_CONST_L("UNDERFLOW",            DEC_Underflow);

    REGISTER_CONTEXT_CONST_L("MAX_DIGITS", DEC_MAX_DIGITS);
    REGISTER_CONTEXT_CONST_L("MIN_DIGITS", DEC_MIN_DIGITS);
    REGISTER_CONTEXT_CONST_L("MAX_EMAX",   DEC_MAX_EMAX);
    REGISTER_CONTEXT_CONST_L("MIN_EMAX",   DEC_MIN_EMAX);
    REGISTER_CONTEXT_CONST_L("MAX_EMIN",   DEC_MAX_EMIN);
    REGISTER_CONTEXT_CONST_L("MIN_EMIN",   DEC_MIN_EMIN);
    REGISTER_CONTEXT_CONST_L("MAX_MATH",   DEC_MAX_MATH);

    REGISTER_CONTEXT_CONST_L("IEEE_754_DIVISION_BY_ZERO",  DEC_IEEE_754_Division_by_zero);
    REGISTER_CONTEXT_CONST_L("IEEE_754_INEXACT",           DEC_IEEE_754_Inexact);
    REGISTER_CONTEXT_CONST_L("IEEE_754_INVALID_OPERATION", DEC_IEEE_754_Invalid_operation);
    REGISTER_CONTEXT_CONST_L("IEEE_754_OVERFLOW",          DEC_IEEE_754_Overflow);
    REGISTER_CONTEXT_CONST_L("IEEE_754_UNDERFLOW",         DEC_IEEE_754_Underflow);

    REGISTER_CONTEXT_CONST_L("ERRORS",      DEC_Errors);
    REGISTER_CONTEXT_CONST_L("NANS",        DEC_NaNs);
    REGISTER_CONTEXT_CONST_L("INFORMATION", DEC_Information);


    INIT_CLASS_ENTRY(ce_decimal, "Decimal", decimal_number_class_functions);
    decimal_ce_Decimal = zend_register_internal_class(&ce_decimal TSRMLS_CC);
    decimal_ce_Decimal->create_object = decimal_number_object_new;
    memcpy(&decimal_handlers, zend_get_std_object_handlers(),
        sizeof(zend_object_handlers));

	ce_exception = zend_exception_get_default(TSRMLS_C);
    INIT_CLASS_ENTRY(ce_decimal_exception, "DecimalException", NULL);
    decimal_ce_DecimalException = zend_register_internal_class_ex(
        &ce_decimal_exception, ce_exception, ce_exception->name TSRMLS_CC);
    decimal_ce_DecimalException->create_object = ce_exception->create_object;

    ZEND_INIT_MODULE_GLOBALS(decimal, NULL, NULL);

    return SUCCESS;
}

static int decimal_create_default_context(TSRMLS_D)
{
    zend_object_value context_ov;
    decimal_context_object *context_obj;
    zval *zcontext;

    context_ov = decimal_context_object_new(
        decimal_ce_DecimalContext TSRMLS_CC);
    context_obj = zend_object_store_get_object_by_handle(
        context_ov.handle TSRMLS_CC);
    if (decimal_context_initialize(context_obj, DEC_INIT_DECIMAL128)
        == FAILURE
    ) {
        return FAILURE;
    }
    DECIMAL_G(defaultContext) = context_ov;
    return SUCCESS;
}

PHP_RINIT_FUNCTION(decimal)
{
    if (decimal_create_default_context(TSRMLS_C) == FAILURE) {
        return FAILURE;
    }
    return SUCCESS;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: fdm=marker
 * vim: et sw=4 ts=4
 */
