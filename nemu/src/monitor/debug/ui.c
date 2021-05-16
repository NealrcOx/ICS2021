#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <string.h>

//#include <stdbool>
//typedef enum _bool { false = 0, true = 1, } bool;

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  { "si", "singel step run", cmd_si },
  { "info", "display all register", cmd_info },
  { "x", "Evaluate the expr and print memeroy addressed from expr in hex", cmd_x },
  { "p", "Solve the regular expr", cmd_p },
  { "w", "Stop the programming when the value of expr changes", cmd_w},
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}
//archieve the si command
static int cmd_si(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int  n = 0;
//  int  i;
  char ch;

  if (arg == NULL) {
    /* no argument given */
/*    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description); */
	  n = 1;
	  cpu_exec(n);
  }
  else {
       while (*arg != '\0'){
	       ch = *arg++;
	       if(ch < '0' || ch > '9'){
	      		n = -1;
		       break;	
	       }
	       n = n * 10 + (ch - '0');
       }
       if(n == 0){
	       n = 1;
       }
       cpu_exec(n);
  }
  return 0;
}
//achieve the info command
static int cmd_info(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  if(arg == NULL){
	  printf("Must input one argument!\n");
	  return 0;
  }
  else if(! strcmp(arg, "r")){
	 	for(int i = R_EAX ; i < R_EDI ; i++){
			printf("%-10s:\t%-#15x\t%3u", regsl[i], reg_l(i), reg_l(i));
		        printf("\n");
		}
		  for(int i = R_AX ; i < R_DI ; i++){
			printf("%-10s:\t%-#15x\t%3u", regsw[i], reg_w(i), reg_w(i));
			printf("\n");
		  }
	              for(int i = R_AL ; i < R_BH ; i++){
			printf("%-10s:\t%-#15x\t%3u", regsb[i], reg_b(i), reg_b(i));
			printf("\n");
		      }
			}
  else if(!strcmp(arg, "w")){
	  printf("pa1.3 will achieve this commad\n");
	  }
  else{
  	printf("eg:<info r> or <info w>\n");
	}
	return 0;
  }
//achieve the cmd_x command

static int cmd_x(char *args) {
  /* extract the first argument */
	char * arg1 = strtok(args, " ");
	char * arg2 = strtok(NULL, " ");
	if(arg1 == NULL || arg2 == NULL){
		printf("Must input two argument!\n");
		return 0;
	}
	int n = atoi(arg1);
	vaddr_t addr;
	sscanf(arg2,"%x",& addr);
//	vaddr_t byteAddr = addr;
	printf("address   Dword block ...  Byte sequence \n");
	for(int i = 0 ; i < n ; i++) {
		printf("%#0x  %#010x  ...  ",addr + 4 * i,vaddr_read(addr + 4 * i, 4));
		vaddr_t byteAddr = addr + 4 * i;
		for(int j = 0 ; j < 4 ; j++) {
			printf("%02x ", vaddr_read(byteAddr + j, 1));
		}
		printf("\n");
	}
     return 0;
}

//achieve the cmd_p command
static int cmd_p(char *args){
   	char * arg = args;
	if(arg == NULL){
		printf("Must input one argument!\n");
		return 0;
	}
	bool success = true;
	uint32_t val = expr(arg, &success);
	if(success){
		printf("%d\n", val);
	}
	else {
		printf("Bad expr!\n");
	}
	return 0;
}
//achieve the cmd_w command
static int cmd_w(char * args){
	char * arg = args;
	if(arg == NULL){
		printf("Must input one argument!\n");
		return 0;
	}
	bool success = true;
	uint32_t val = expr(arg, &success);
	if(!success){
		printf("bad expr!\n");
	}
		WP * wp = new_wp();
		wp->expr = (char *)malloc(strlen(arg) + 1);
		strcpy(wp->expr, arg);
		wp->expr[strlen(arg)] = 0;
		wp->val = val;
		return 0;
}
	

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
