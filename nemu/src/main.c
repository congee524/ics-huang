#include <stdio.h>
#include "monitor/expr.h"
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
    /* Initialize the monitor. */
    int is_batch_mode = init_monitor(argc, argv);

    /* test the expr.c */
    char str_tmp[65530];
    FILE *fp;
    if ((fp = fopen("/home/congee/ics2018/nemu/tools/gen-expr/input", "r")) == NULL) {
        printf("open file failed!\n");
        return -1;
    }
    
    while (!feof(fp)) {
        uint32_t res;
        fscanf(fp, "%u", &res);
        fgets(str_tmp, 65530, fp);
        bool success = true;
        if (res == expr(str_tmp, &success)) {
            printf("right!\n");
        } else {
            printf("wrong!\n");
        }
    }
    fclose(fp);
    /* Receive commands from user. */
    ui_mainloop(is_batch_mode);

    return 0;
}
