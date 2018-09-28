#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

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

static int cmd_si(char *args) {
    int i = 0;
    if (args == NULL) {
        i = 1;
    } else {
        sscanf(args, "%d", &i);
    }
    cpu_exec(i);
    return 0;    
}

static int cmd_info(char *args) {
    if (args[0] == 'r') {
        printf("EAX:%x %d\n", cpu.eax, cpu.eax);
        printf("ECX:%x %d\n", cpu.ecx, cpu.ecx);
        printf("EDX:%x %d\n", cpu.edx, cpu.edx);
        printf("EBX:%x %d\n", cpu.ebx, cpu.ebx);
        printf("ESP:%x %d\n", cpu.esp, cpu.esp);
        printf("EBP:%x %d\n", cpu.ebp, cpu.ebp);
        printf("ESI:%x %d\n", cpu.esi, cpu.esi);
        printf("EDI:%x %d\n", cpu.edi, cpu.edi);
    }
    return 0;
}

static int cmd_x(char *args) {
    char *n = strtok(args, " ");
    char *expr = n + strlen(n) + 1;
    int i = 0, addr = 0;
    sscanf(n, "%d", &i);
    sscanf(expr, "%x", &addr);
    for (; i > 0; i--) {
        printf("0x%x: 0x", addr);
        for (int j = 0; j < 4; j++) {
            printf("%02x", pmem[addr]);
            addr++;
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}

static int cmd_p(char *args) {
    bool success = true;
    int ans = 0;
    ans = expr(args, &success);
    if (!success) {
        printf("expression is none!\n");
    } else {
        printf("the value is %d\n", ans);
    }
    return 0;
}

static void test_expr(void) {
    FILE *fp = popen("/home/congee/ics2018/nemu/tools/gen-expr/input", "r");
    if (fp == NULL) {
        printf("empty input!\n");
        assert(0);
    }
    char str_tmp[65530];
    while (!feof(fp)) {
        str_tmp[0] = '\0';
        fgets(str_tmp, 65530, fp);
        char *res = strtok(str_tmp, " ");
        char *exp = res + strlen(res) + 1;
        bool succ = true;
        int ans = expr(exp, &succ);
        printf("pause!\n");
        if(!succ) {
            printf("expression is none!\n");
        } else {
            printf("the value is %d.\n the result is %s.\n", ans, res);
        }
    }
    fclose(fp);
}

    static int cmd_help(char *args);

    static struct {
        char *name;
        char *description;
        int (*handler) (char *);
    } cmd_table [] = {
        { "help", "Display informations about all supported commands", cmd_help },
        { "c", "Continue the execution of the program", cmd_c },
        { "q", "Exit NEMU", cmd_q },
        { "si", "excute the same command for n times", cmd_si },
        { "info", "print the state of the program", cmd_info },
        { "x", "examine the memory", cmd_x },
        { "p", "solve the value of the expression", cmd_p },
        /* TODO: Add more commands */

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

    void ui_mainloop(int is_batch_mode) {
        if (is_batch_mode) {
            cmd_c(NULL);
            return;
        }

        test_expr();

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
