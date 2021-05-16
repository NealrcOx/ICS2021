#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

//new a watchpint
WP * new_wp(){
if(free_ == NULL){
	//log("Not has enough watchpoint\n");
	assert(0);
	return NULL;
}
	WP * wp = free_;
	free_ = free_->next;

	wp->next = head;
	head = wp;

	return head;
	}

//free a watchpoint NO is n
void free_wp(int n){
	if(head->NO == n){
		WP * wp = head->next;
		head->next = free_;
		free_ = head;
		head = wp;
		return ;
	}
	WP * pwt = NULL;
	for(pwt = head ; pwt->next != NULL ; pwt = pwt->next){
		if(pwt->next->NO == n){
			WP * wp = pwt->next;
			pwt->next = wp->next;
			wp->next = free_;
			free_ = wp;
			return ;
		}
	}
}
