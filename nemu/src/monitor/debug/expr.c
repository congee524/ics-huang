#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
    TK_NOTYPE = 256, TK_EQ, TK_NEQ, DEC, HEX, AND, OR, REG, DEREF

        /* TODO: Add more token types */

};

static struct rule {
    char *regex;
    int token_type;
} rules[] = {

    /*TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +", TK_NOTYPE},       // spaces
    {"\\+", '+'},            // plus
    {"\\-", '-'},            // minus
    {"\\*", '*'},            // multiply, pointer dereference
    {"/", '/'},              // divide
    {"\\(", '('},            // left bracket
    {"\\)", ')'},            // right bracket
    {"==", TK_EQ},           // equal
    {"!=", TK_NEQ},          // not equal
    {"[0-9]+", DEC},         // decimal number
    {"0x[0-9a-fA-F]+", HEX}, // hexadecimal number
    {"&&", AND},             // logical and
    {"\\|\\|", OR},          // logical or
    {"\\$\\w{3}", REG},      // register
    // firstly, i use \\d but error, and then i modify to [0-9]
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; i++) {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0) {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

typedef struct token {
    int type;
    char str[32];
} Token;

Token tokens[50005];
int nr_token;

void empty_token_str(void) {
    for (int i = 0; i < 32; i++) {
        tokens[nr_token].str[i] = '\0';
    }
}

static bool make_token(char *e) {
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_token = 0;

    while (e[position] != '\0') {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i ++) {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;


                Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
                        i, rules[i].regex, position, substr_len, substr_len, substr_start);

                position += substr_len;

                /* TODO: Now a new token is recognized with rules[i]. Add codes
                 * to record the token in the array `tokens'. For certain types
                 * of tokens, some extra actions should be performed.
                 */

                switch (rules[i].token_type) {
                    case DEC:
                        if (substr_len > 10) {
                            printf("the number is overflow!\n");
                            assert(0);
                        } 
                        empty_token_str();
                        strncpy(tokens[nr_token].str, substr_start, substr_len);
                        tokens[nr_token].type = rules[i].token_type;
                        nr_token++;
                        break;
                    case HEX:
                        if (substr_len > 8) {
                            printf("the hex number is overflow!\n");
                            assert(0);
                        }
                        empty_token_str(); 
                        strncpy(tokens[nr_token].str, substr_start, substr_len);
                        tokens[nr_token].type = rules[i].token_type;
                        break;
                    case REG:
                        empty_token_str();
                        strncpy(tokens[nr_token].str, substr_start + 1, substr_len);
                        tokens[nr_token].type = rules[i].token_type;
                        break;
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '(':
                    case ')':
                    case TK_EQ:
                    case TK_NEQ:
                    case AND:
                    case OR:
                        // printf("nr_token: %d\n", nr_token);
                        tokens[nr_token].type = rules[i].token_type;
                        break;
                    case TK_NOTYPE:
                        nr_token--;
                }
                if (tokens[nr_token].type == '*' && (nr_token == 0 
                            || ((tokens[nr_token - 1].type != DEC) 
                            && (tokens[nr_token - 1].type != HEX)))) {
                    tokens[nr_token].type = DEREF;
                }
                nr_token++;
                break;
            }
        }
        if (i == NR_REGEX) {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }
    return true;
}

bool check_parentheses(int p, int q) {
    if (tokens[p].type != '(' || tokens[q].type != ')') {
        return false;
    }
    int cnt_l = 0, cnt_r = 0;
    while (p < q) {
        if (tokens[p].type == '(') {
            cnt_l++;
        } else if (tokens[p].type == ')') {
            cnt_r++;
        }
        if (cnt_l <= cnt_r) {
            return false;
        }
        p++;
    }
    cnt_r++;
    if (cnt_l == cnt_r) {
        return true;
    } else {
        return false;
    }
}

uint32_t eval(int p, int q) {
    if (p > q) {
        printf("Bad expression!\n");
        assert(0);
        return 0;
    } else if (p == q) {
        uint32_t operand = 0;
        if (tokens[p].type == DEC) {
            sscanf(tokens[p].str, "%u", &operand);
            // printf("%d~%d, operand: %d\n", p, q, operand);
        } else if (tokens[p].type == HEX) {
            sscanf(tokens[p].str, "%x", &operand);
        } else if (tokens[p].type == REG) {
            if (!strcmp(tokens[p].str, "eax")) {
                return cpu.eax;
            } else if (!strcmp(tokens[p].str, "ecx")) {
                return cpu.ecx;
            } else if (!strcmp(tokens[p].str, "edx")) {
                return cpu.edx;
            } else if (!strcmp(tokens[p].str, "ebx")) {
                return cpu.ebx;
            } else if (!strcmp(tokens[p].str, "esp")) {
                return cpu.esp;
            } else if (!strcmp(tokens[p].str, "ebp")) {
                return cpu.ebp;
            } else if (!strcmp(tokens[p].str, "esi")) {
                return cpu.esi;
            } else if (!strcmp(tokens[p].str, "edi")) {
                return cpu.edi;
            } else if (!strcmp(tokens[p].str, "eip")) {
                return cpu.eip;
            } else {
                printf("unknown register name!\n");
                assert(0);
            }
        } else {
            printf("operand loss!\n");
            assert(0);
        }
        return operand;
    } else if ((tokens[p].type == DEREF) && 
                ((p + 1 == q) || check_parentheses(p + 1, q))) {
        uint32_t addr;
        addr = eval(p + 1, q);
        return vaddr_read(addr, 4);
    } else if (check_parentheses(p, q)) {
        // the expression is surrounded by a matched pair of parentheses. 
        return eval(p + 1, q - 1);
    } else {
        //  remember that the main operation is the right one
        int op_type = 0, op_posi = 0, op_prio = 15; // the priority of the operation
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == '(') {
                int cnt = 0;
                cnt++;
                while(cnt && i <= q) {
                    i++;
                    if (tokens[i].type == '(') {
                        cnt++;
                    } else if (tokens[i].type == ')'){
                        cnt--;
                    }
                }
                if (i < q) {
                    i++;
                }
            }
            if (tokens[i].type == '*' && op_prio >= 3) {
                op_type = '*';
                op_posi = i;
                op_prio = 3;            
                // printf("p = %d, q = %d, i = %d, op_type = %d\n ", p, q, i, op_type);
            } else if (tokens[i].type == '/' && op_prio >= 3) {
                op_type = '/';
                op_posi = i;
                op_prio = 3;
                // printf("p = %d, q = %d, i = %d, op_type = %d\n ", p, q, i, op_type);
            } else if (tokens[i].type == '+' && op_prio >= 2) {
                op_type = '+';
                op_posi = i;
                op_prio = 2;
                // printf("p = %d, q = %d, i = %d, op_type = %d\n ", p, q, i, op_type);
            } else if (tokens[i].type == '-' && op_prio >= 2) {
                op_type = '-';
                op_posi = i;
                op_prio = 2;
                // printf("p = %d, q = %d, i = %d, op_type = %d\n ", p, q, i, op_type);
            } else if (tokens[i].type == TK_EQ && op_prio >= 7) {
                op_type = TK_EQ;
                op_posi = i;
                op_prio = 7;
            } else if (tokens[i].type == TK_NEQ && op_prio >= 7) {
                op_type = TK_NEQ;
                op_posi = i;
                op_prio = 7;
            } else if (tokens[i].type == AND && op_prio >= 11) {
                op_type = AND;
                op_posi = i;
                op_prio = 11;
            } else if (tokens[i].type == OR && op_prio >= 11) {
                op_type = OR;
                op_posi = i;
                op_prio = 11;
            }
        }
        uint32_t val1 = 0, val2 = 0;
        // printf("%d~%d, op_posi: %d\n", p, q, op_posi);
        val1 = eval(p, op_posi - 1);
        val2 = eval(op_posi + 1, q);
        // printf("val1 = %u, val2 = %u\n", val1, val2);
        switch (op_type) {
            case '*':
                return val1 * val2;
            case '/':
                if (val2 == 0) {
                    printf("warning! '/0' has happened.\n");
                    return 1;
                }
                return val1 / val2;
            case '+':
                return val1 + val2;
            case '-':
                return val1 - val2;
            case TK_EQ:
                return val1 == val2;
            case TK_NEQ:
                return val1 != val2;
            case AND:
                return val1 && val2;
            case OR:
                return val1 || val2;
            default:
                printf("wrong op_type!\n");
                assert(0);
        }
    }
} 

uint32_t expr(char *e, bool *success) {
    if (!make_token(e)) {
        *success = false;
        return 0;
    } else {
        return eval(0, nr_token - 1);
    }

    /* TODO: Insert codes to evaluate the expression. */
    // TODO();
    return 0;
}
