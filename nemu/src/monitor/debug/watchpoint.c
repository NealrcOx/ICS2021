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
	int Num = free_ -> NO;
	free_ = free_->next;
	wp_pool[Num].next =head;
	head = &wp_pool[Num];
	return head;
	}

//free a watchpoint NO is n
void free_wp(int n){
	WP *wp;
	if(head->NO == n){
		wp = head->next;
		head->next = free_;
		free_ = head;
		head = wp;
	//	return ;
	}
	else{
	WP * wp1,* wp2,* wp3;
	for(wp1 = head, wp2 = head->next; wp2->NO != n; wp1 = wp1->next,wp2 = wp2->next);
	wp3 = &wp_pool[n];
	wp1 -> next = wp3 ->next;
	wp3 -> next = free_;
	free_ = wp3;
	}
}
