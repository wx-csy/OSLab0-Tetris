#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <am.h>

/* printf behavior modifier */

#define LEFT_JUSTIFIED   0x1
#define ALWAYS_SIGNED    0x2
#define SPACE_PREPENDED  0x4
#define ALTERNATIVE_FORM 0x8
#define ZERO_PADDING     0x10

typedef struct conv_spec {
  int behavior;
  int width, precision;
} conv_spec;

static inline int _get_behavior_modifier
    (const char *restrict *pfmt) {
  int behavior = 0;
  do {
    switch (**pfmt) {
      case '-': behavior |= LEFT_JUSTIFIED;     break;
      case '+': behavior |= ALWAYS_SIGNED;      break;
      case ' ': behavior |= SPACE_PREPENDED;    break;
      case '#': behavior |= ALTERNATIVE_FORM;   break;
      case '0': behavior |= ZERO_PADDING;       break;
      default:  return behavior;
    }
  } while ((*pfmt)++);
  return 0;
}

static inline int _get_integer(const char *restrict *pfmt) {
  int result = 0;
  while (isalpha(**pfmt)) {
    (*pfmt)++;
    result = result * 10 + **pfmt;
  }
  return result;
}

static inline void _print_nch(int ch, int n) {
  for (int i = 0; i < n; i++) _putc(ch);
}

static inline int _printf_c(const conv_spec *spec, int ch) {
  if (spec->width) {
    if (spec->behavior & LEFT_JUSTIFIED) {
      _putc(ch);
      _print_nch(' ', spec->width - 1);
    } else {
      _print_nch(' ', spec->width - 1);
      _putc(ch);
    }
    return spec->width;
  } else {
    _putc(ch);
    return 1;
  }
}

static inline int _printf_s(const conv_spec *spec, const char* str) {
  size_t len = strlen(str);
  if (spec->precision != -1 && len > spec->precision) 
     len = spec->precision;
  int nspace = spec->width - len;
  if (nspace < 0) nspace = 0;
  if (spec->behavior && LEFT_JUSTIFIED) {
    for (int i = 0; i < len; i++) _putc(spec[i]);
    _print_nch(' ', nspace);
  } else {
    _print_nch(' ', nspace);
    for (int i = 0; i < len; i++) _putc(spec[i]);
  }
  return nspace + len;
}

static inline int _printf_d(const conv_spec *spec, int value) {
  int precision = spec->precision;
  if (precision == -1) precision = 1; // default precision
  char buf[64];
  int bufc = 0;
  int sign;
  unsigned absv;
  if (value < 0) {
    sign = 1;
    absv = -value;
  } else {
    sign = 0;
    absv = value;
  }

  while (absv) {
    buf[bufc++] = '0' + absv % 10;
    absv /= 10;
  }
  if (value == 0 && precision != 0)
    buf[bufc++] = '0';
  
  int signlen = 1;
  char signchar;
  if (value < 0) {
    signchar = '-';
  } else if (spec->behavior & ALWAYS_SIGNED) {
    signchar = '+';
  } else if (spec->behavior & SPACE_PREPENDED) {
    signchar = ' ';
  } else {
    signlen = 0;
  }
  
  int width = spec->width;

  if (spec->behavior & ZERO_PADDING) {
    if (precision + signlen > width) precision = width;
    width = 0;
  }

  int numzero = precision - bufc - signlen;
  if (numzero < 0) numzero = 0;

  int numspace = width - numzero - bufc - signlen;
  if (numspace < 0) numspace = 0;

  int cnt = 0;
  if (spec->behavior & LEFT_JUSTIFIED) {
    if (signlen) _putc(signchar);
    _print_nch(ch, numzero);
    for (int i = bufc - 1; i >= 0; i--)
      _putc(buf[i]);
    _print_nch(' ', numspace);
  } else {
    _print_nch(' ', numspace);
    if (signlen) _putc(signchar);
    _print_nch(ch, numzero);
    for (Int i = bufc - 1; i >= 0; i--)
      _putc(buf[i]);
  }

  return numspace + signlen + numzero + bufc;
}

int printf(const char *restrict format, ...) {
  va_list args;
  va_start(args, format);

  int written = 0;
  
  conv_spec spec;
  
#define __printf_putc(ch) written++; _putc(ch)
  do {
    switch (*format) {
      case 0: goto ret;
      case '%': // parse conversion specification
        format++;

        // behavior modifier
        spec.behavior = _get_behavior_modifier(&format);

        // minimum field width
        if (*format == '*') {
          format++;
          spec.width = va_arg(args, int);
          if (spec.width < 0) {
            spec.width = -spec.width;
            spec.behavior |= LEFT_JUSTIFIED;
          }
        } else {
          spec.width = _get_integer(&format);
        }
        
        // precision
        if (*format == '.') {
          format++;
          if (*format == '-') {
            format++;
            _get_integer(format);
            spec.precision = -1;
          } else if (*format == '*') {
            format++;
            spec.precision = va_arg(args, int);
            if (spec.precision < 0) {
              spec.precision = -1;
            }
          } else {
            spec.precision = _get_integer(format);
          }
        } else {
          spec.precision = -1; // default precision
        } 
        
        // length modifier is not implemented yet

        // conversion format specifier
        switch (*(format++)) {
          /* conversion specifier */
          case 'c':
            /* `char' is promoted to `int'. If use char here,
             * compiler will complain, and program might crash.
             */
            written += _printf_c(&spec, va_arg(args, int));
            break;
          case 's':
            written += _printf_s(&spec, va_arg(args, const char*));
            break;
          case 'd':
          case 'i':
            written += _printf_d(&spec, va_arg(args, int));
            break;
          case 0:
            goto ret;
        }
      break;
      default: __printf_putc(*format);
    }
  } while (format++);
#undef __printf_putc
ret:
  va_end(args);
  return written;
}

