#include "klib.h"

//#ifndef __ISA_NATIVE__
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char *start = s;

  while(*s)
    s++;
  return s - start;
}

char *strcpy(char* dst,const char* src) {
  char *s = dst;
  
  while ((*dst++ = *src++) != '\0') 
      ;

  return s;
}

char* strncpy(char* dst, const char* src, size_t n) {
  char *dscan;
  const char *sscan;
  dscan = dst;
  sscan = src;
  while (n > 0) {
    --n;
    if ((*dscan++ = *sscan++) == '\0')
      break;
  }
  while (n-- > 0)
    *dscan++ = '\0';
  return dst;
}

char* strcat(char* dst, const char* src) {
  char *s = dst;

  while(*dst)
    dst++;

  while ((*dst++ = *src++) != '\0')
    ;
  return s;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

int strncmp(const char* s1, const char* s2, size_t n) {
  if (n == 0) 
    return 0;

  while (n-- != 0 && *s1 == *s2) {
    if (n == 0 || *s1 == '\0') 
      break;
    s1++;
    s2++;
  }
  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

void* memset(void* v, int c, size_t n) {
  char *s = (char *) v;
  while (n--){
    *s++ = (char) c;
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  char *dst = (char *) out;
  char *src = (char *) in;

  void *save = out;

  while (n--) {
    *dst++ = *src++;
  }

  return save;
}

int memcmp(const void* s1, const void* s2, size_t n){
  unsigned char *m1 = (unsigned char *) s1;
  unsigned char *m2 = (unsigned char *) s2;

  while (n--) {
    if (*m1 != *m2) {
      return *m1 - *m2;
    }
    m1++;
    m2++;
  }
  return 0;
}

#endif
