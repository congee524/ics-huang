#include "klib.h"
#include <stdarg.h>

#ifndef __ISA_NATIVE__

int printf(const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    unsigned int t;
    char ch;
    char *ptr = NULL;
    char buffer[50];
    char Representation[] = "0123456789abcdef";

    while (*fmt) {
        if (*fmt == '%') {
            switch (*++fmt) {
                case 'd':
                    va_arg(arg, int);
                    ptr = &buffer[49];
                    *ptr = '\0';
                    do {
                        *--ptr = Representation[t % 10];
                        t /= 10;
                    } while (t);
                    while (*ptr) {
                        _putc(*ptr);
                        ptr++;
                    }
                    break;
                case 'x':
                    t = va_arg(arg, unsigned int);
                    ptr = &buffer[49];
                    *ptr = '\0';
                    do {
                        *--ptr = Representation[t % 16];
                        t /= 16;
                    } while (t);
                    while (*ptr) {
                        _putc(*ptr);
                        ptr++;
                    }
                    break;       
                case 'c':
                    ch = (char) va_arg(arg, int);
                    _putc(ch);
                    break;
                case 's':
                    ptr = va_arg(arg, char *);
                    while (*ptr) {
                        _putc(*ptr);
                        ptr++;
                    }
                default:
                    assert(0);
            }
        } else {
            _putc(*fmt);
        }
        fmt++;
    }
    va_end(arg);
    return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
    return 0;
}

int sprintf(char *out, const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    unsigned int t;
    char *ptr = NULL;
    char *start = out;
    char buffer[50];
    char Representation[] = "0123456789abcdef";

    while (*fmt) {
        if (*fmt == '%') {
            switch (*++fmt) {
                case 'd':
                    t = va_arg(arg, int);
                    ptr = &buffer[49];
                    *ptr = '\0';
                    do {
                        *--ptr = Representation[t % 10];
                        t /= 10;
                    } while (t);
                    while (*ptr) {
                        *start = *ptr;
                        start++;
                        ptr++;
                    }
                    break;
                case 'x':
                    t = va_arg(arg, unsigned int);
                    ptr = &buffer[49];
                    *ptr = '\0';
                    do {
                        *--ptr = Representation[t % 16];
                        t /= 16;
                    } while (t);
                    while (*ptr) {
                        *start++ = *ptr++;
                    }
                    break;       
                case 'c':
                    *start++ = (char) va_arg(arg, int);
                    break;
                case 's':
                    ptr = va_arg(arg, char *);
                    while (*ptr) {
                        *start++ = *ptr++;
                    }
                default:
                    assert(0);
            }
        } else {
            *start = *fmt;
            start++;
        }
        fmt++;
    }
    *start = '\0';
    va_end(arg);
    return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
    return 0;
}

#endif
