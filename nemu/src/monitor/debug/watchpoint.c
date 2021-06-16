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
// set watchpoint
int set_watchpoint(char *e){
	bool success;
	int result = expr(e, &success);
	if(success == false){
		printf("Wrong Expr!\n");
		assert(0);
	}
	WP *wp = new_wp();
	printf("Set watchpoint #%d\n",wp -> NO);
	strcpy(wp -> expr, e);
	printf("expr = %s\n", wp -> expr);
	wp -> old_val = result;
	printf("old value = Ox%08x\n", wp -> old_val);
	return 1;
}
// delete watchpoint
bool delete_watchpoint(int NO){
	if(head == NULL){
		printf("There isn't no watchpoint set!\n");
		return false;
	}
	free_wp(NO);
	printf("Watchpoint %d deleted!\n",NO);
	return true;
}
//list watchpoint
void list_watchpoint(void){
	if(head == NULL){
		printf("There isn't no watchpoint!\n");
		assert(0);
	}
	printf("No old value expr!\n");
	WP * wp = head;
	for( ; wp != NULL ; wp = wp -> next){
		printf("%d	Ox%08x %s\n",wp -> NO, wp -> old_val, wp -> expr);
	}
}
// scan watchpoint
WP * scan_watchpoint(void){
	bool success;
	WP *wp = head;
	int result;
	int eip_num;
	for( ; wp != NULL ; wp = wp -> next){
		result = expr(wp -> expr, &success);
		if(result != wp -> old_val){
			wp -> new_val = result;
			eip_num = expr("$eip", &success);
			printf("Hit watchpoint %d at addrss Ox%08x\n",wp ->NO, eip_num);
			return wp;
		}
	}
	return NULL;
}
