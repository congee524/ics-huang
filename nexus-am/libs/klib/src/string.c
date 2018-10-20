#include "klib.h"

#ifndef __ISA_NATIVE__

size_t strlen(const char *s) {
  return 0;
}

char *strcpy(char* dst,const char* src) {
  return NULL;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
  return NULL;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

void* memset(void* v, int c, size_t n) {
  char *s = (char *) v;
  while (n--){
    *s++ = (char) c;
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  return NULL;
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
