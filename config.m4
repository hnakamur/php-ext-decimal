PHP_ARG_ENABLE(decimal, whether to enable Decimal Number Arithmetic support,
  [ --enable-decimal   Enable Decimal Number Arithmetic support])

if test "$PHP_DECIMAL" = "yes"; then
  AC_DEFINE(HAVE_DECIMAL, 1, [Whether you have Decimal Number Arithmetic])

  AC_DEFINE(DECSUBSET, 1,
      [Enable X3.274 subset arithmetic (Must be 1)])

  AC_DEFINE(DECNUMDIGITS, 34,
      [The default number of digits that can be held in the structure])

  PHP_DECIMAL_SOURCES="decimal.c decNumber/decNumber.c decNumber/decContext.c"
  PHP_NEW_EXTENSION(decimal, $PHP_DECIMAL_SOURCES, $ext_shared)
fi
