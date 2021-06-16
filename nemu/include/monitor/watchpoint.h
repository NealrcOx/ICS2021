#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[128];
  uint32_t old_val,new_val;

} WP;
//add define function for watchpoint setting

int set_watchpoint(char *e);

bool delete_watchpoint(int NO);

void list_watchpoint(void);

WP * scan_watchpoint(void);

#endif
