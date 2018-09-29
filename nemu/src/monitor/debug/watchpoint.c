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

WP* new_wp(char *expr, uint32_t value) {
    if (free_ == NULL) {
        printf("the watchpoint pool has been full!\n");
        assert(0);
    }
    WP* n_wp;
    n_wp = free_;
    free_ = free_->next;

    n_wp->next = NULL;
    n_wp->nv = value;
    strcpy(n_wp->expr, expr);

    if (head != NULL) {
        n_wp->next = head;
    }
    head = n_wp;

    return n_wp;
}

 
