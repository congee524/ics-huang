#include <stdio.h>
#include "monitor/expr.h"
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
    /* Initialize the monitor. */
    int is_batch_mode = init_monitor(argc, argv);

    /* test the expr.c: firstly i use fgets, but the line feeds will be read and it cannot be recognized, so i use fscanf */
    /* 
    FILE *fp;
    if ((fp = fopen("/home/congee/ics2018/nemu/tools/gen-expr/input", "r")) == NULL) {
        printf("open file failed!\n");
        return -1;
    }
    uint32_t res, ans;
    bool success = true;
    while (~fscanf(fp, "%u", &res)) {
        char str_tmp[65530];
        fscanf(fp, "%[^\n]%*c", str_tmp);
        success = true;
        ans = expr(str_tmp, &success);
        printf("the true answer is %u. the answer of expr is %u.\n", res, ans);
    }
    fclose(fp);
    */
    /* Receive commands from user. */

    ui_mainloop(is_batch_mode);

    return 0;
}
