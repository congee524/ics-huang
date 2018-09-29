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
    printf("#1\n");
    if (free_ == NULL) {
        printf("the watchpoint pool has been full!\n");
        assert(0);
    }
    printf("#2\n");
    WP* n_wp = NULL;
    n_wp = free_;
    free_ = free_->next;
    printf("#2.5\n"); 
    n_wp->next = NULL;
    printf("#2.6\n");
    n_wp->nv = value;
    printf("#2.7\n");
    assert(n_wp->expr);
    strcpy(n_wp->expr, expr);
    
    printf("#3\n");
    if (head != NULL) {
        n_wp->next = head;
    }
    head = n_wp;

    return n_wp;
}

 
