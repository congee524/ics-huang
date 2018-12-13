#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
	char expr[200];
	long long old_value;
	long long new_value;
	bool change_flag;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */


} WP;

WP* new_wp();
void add_wp(WP* target_wp, char *exp);
WP* search_wp(int wp_NO);
void free_wp(int wp_NO);
void info_wp();
bool check_wp();
void print_wp();
void update_wp();

#endif
