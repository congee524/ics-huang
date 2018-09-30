#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
    int NO;
    struct watchpoint *next;
    uint32_t nv;        // new value
    uint32_t ov;        // old value
    bool open;          // to judge whether the watchpoint is open
    char expr[65536];         // put the expression
    /* TODO: Add more members if necessary */


} WP;

void init_wp_pool();
WP* new_wp(char *, uint32_t);
void free_wp(char *);
#endif
