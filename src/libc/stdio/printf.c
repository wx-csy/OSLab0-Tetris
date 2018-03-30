#include <stdio.h>
#include <stdarg.h>
#include <am.h>

/* printf behavior modifier */
#define LEFT_JUSTIFIED   0x1
#define ALWAYS_SIGNED    0x2
#define SPACE_PREPENDED  0x4
#define ALTERNATIVE_FORM 0x8
#define ZERO_PADDING     0x10

static inline int _get_behavior_modifier
    (const char **pfmt) {
  int behavior = 0;
  while (**pfmt) {
    switch (**pfmt) {
      case '-': *behavior |= LEFT_JUSTIFIED;     break;
      case '+': *behavior |= ALWAYS_SIGNED;      break;
      case ' ': *behavior |= SPACE_PREPENDED;    break;
      case '#': *behavior |= ALTERNATIVE_FORM;   break;
      case '0': *behavior |= ZERO_PADDING;       break;
      default:  return behavior;
    }
    (*pfmt)++;
  }
  return -1;
}

static inline int _get_integer(int *result) {

}

int printf(const char *restrict format, ...) {
  va_list args;
  va_start(args, format);

  int written = 0;
  
  int behavior;
  int width, precision;

  char *p_string;
  int p_int;
  unsigned p_unsigned;
#define __printf_putc(ch) written++; _putc(ch)
  do {
    switch (*format) {
      case 0: goto ret;
      case '%': // parse conversion specification
        format++;
        behavior = _get_behavior_modifier(&format);
        if (behavior == -1) goto ret;
        
        switch (*format) {
          /* conversion specifier */
          case 'c':
            /* `char' is promoted to `int', so if use char here,
             * compiler will complain, and program might crash.
             */
            p_int = va_arg(args, int);
            __printf_putc(p_int);
            break;
          case 's':
            p_string = va_arg(args, char*);
            while (*p_string) {
              __printf_putc(*p_string);
              p_string++;
            }
            break;
          case 'd':
            p_int = va_arg(args, int);
            if (p_int < 0) {
              p_unsigned = -p_int;
              __printf_putc('-');
            } else {
              p_unsigned = p_int;
            }
            goto print_u;
          case 'u':
            p_unsigned = va_arg(args, unsigned);
print_u:          
            if (p_unsigned == 0) {
              __printf_putc('0');
            } else {
              while (p_unsigned) {
                __printf_putc('0' + p_unsigned % 10);
                p_unsigned /= 10;
              }
            }
            break;
          case 'x':
            p_unsigned = va_arg(args, unsigned);
            if (p_unsigned == 0) {
              __printf_putc('0');
            } else {
              while (p_unsigned) {
                unsigned tmp = p_unsigned & 0xf;
                p_unsigned >>= 4;
                if (tmp < 10) {
                  __printf_putc('0' + tmp);
                } else {
                  __printf_putc('a' + tmp - 10);
                }
              }            
            }
            break;
          case 'X':
            p_unsigned = va_arg(args, unsigned);
            if (p_unsigned == 0) {
              __printf_putc('0');
            } else {
              while (p_unsigned) {
                unsigned tmp = p_unsigned & 0xf;
                p_unsigned >>= 4;
                if (tmp < 10) {
                  __printf_putc('0' + tmp);
                } else {
                  __printf_putc('A' + tmp - 10);
                }
              }            
            }
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

