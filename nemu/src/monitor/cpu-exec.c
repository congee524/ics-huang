#include "nemu.h"
#include "monitor/monitor.h"
#include "monitor/watchpoint.h"

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10

int nemu_state = NEMU_STOP;

void exec_wrapper(bool);

static uint64_t g_nr_guest_instr = 0;

void nr_guest_instr_add(uint32_t n) {
    g_nr_guest_instr += n;
}

void monitor_statistic() {
    Log("total guest instructions = %ld", g_nr_guest_instr);
}

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
    if (nemu_state == NEMU_END || nemu_state == NEMU_ABORT) {
        printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
        return;
    }
    nemu_state = NEMU_RUNNING;

    bool print_flag = n < MAX_INSTR_TO_PRINT;
    for (; n > 0; n --) {
        /* Execute one instruction, including instruction fetch,
         * instruction decode, and the actual execution. */
        exec_wrapper(print_flag);
        nr_guest_instr_add(1);
        //printf("0xc is 0x%08x at eip 0x%08x\n", vaddr_read(reg_l(5) + 0xc, 4), cpu.eip);
#ifdef DEBUG
        /* TODO: check watchpoints here. */
        WP *p = NULL;
        p = check_watchpoint(p);
        while (p != NULL) {
            printf("%d-th watchpointer '%s' has changed!\n%-11d-> %-11d\n", 
                    p->NO, p->expr, p->ov, p->nv);
            if (nemu_state == NEMU_RUNNING) {
                nemu_state = NEMU_STOP;
                return;
            }
            p = check_watchpoint(p);
        }
        //do {
        //    p = check_watchpoint(p);
        //} while (p != NULL);
#endif

#ifdef HAS_IOE
        extern void device_update();
        device_update();
#endif

        if (nemu_state != NEMU_RUNNING) {
            if (nemu_state == NEMU_END) {
                printflog("\33[1;31mnemu: HIT %s TRAP\33[0m at eip = 0x%08x\n\n",
                        (cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip - 1);
                monitor_statistic();
                return;
            }
            else if (nemu_state == NEMU_ABORT) {
                printflog("\33[1;31mnemu: ABORT\33[0m at eip = 0x%08x\n\n", cpu.eip);
                return;
            }
        }
    }

    if (nemu_state == NEMU_RUNNING) { nemu_state = NEMU_STOP; }
}
