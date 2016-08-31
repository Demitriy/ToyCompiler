
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum token {
    NUMBER,
    BINOP,
    LBRACE,
    RBRACE,
    NEWLINE,
    EOFL,
    ERROR
    };
struct node {
    struct node* left;
    struct node* right;
    int token;
/*  united  */
    union {
        char op;
        int number;
    } value;
};
void bypass(int number, struct node* treenode) {
    int i;
    if (treenode != NULL) {
        number+=4;
        bypass(number,treenode->right);
        for(i = 0; i < number; ++i) {
        printf(" ");
        }
        switch (treenode->token) {
            case NUMBER: printf("%d\n", treenode->value.number);break;
            case BINOP:  printf("%c\n", treenode->value.op);break;
        }
        bypass(number,treenode->left);
    }
    return;
}
int otvet,value = 0;
int counting(struct node* treenode) {
    char operation;
    int cur_value = 0;
    if (treenode != NULL) {
        if (treenode->token == BINOP) {
            operation = treenode->value.op; 
            cur_value = counting(treenode->left);
        } else {
            cur_value = treenode->value.number;
            return cur_value;
        }
        switch (operation) {
            case '+' : cur_value = cur_value + counting(treenode->right);break;
            case '-' : cur_value = cur_value - counting(treenode->right);break;
            case '*' : cur_value = cur_value * counting(treenode->right);break;
            case '/' : cur_value = cur_value / counting(treenode->right);break;
        }
    }
    return cur_value;
}   

int token;
int binop = 0;
int digit(int chr) {
    if ((chr <= '9') && (chr >= '0')) {
       return chr;
    } else  {
       return 0;
    }
}
int sum(int chr) {
    int b = 0;
    while(digit(chr)) {
        b = b * 10 +  chr - '0';
        chr = getchar();
    }
    ungetc(chr,stdin);
    return b;
}
int operation(int chr) {
    if ((chr == '*') || (chr == '-') || (chr == '+') || (chr ==  '/')) {
        binop = chr;
        return BINOP;
    }
    return ERROR;
}
int gettoken() {
    int chr = getchar();
    while (chr != EOF) {
        if (chr == ' ') {
            chr = getchar();
            continue;
        }
        if (digit(chr)) {
            value = sum(chr);
            return NUMBER;
        }
        if (chr == '(') {     //
            return LBRACE;    //
        }                     //
        if (chr == ')') {     //
            return RBRACE;    //
        }                     //
        if (operation(chr)) {
           return operation(chr);
        }
    }
    return EOFL;
}
struct node* expression();
struct node* factor() {
    struct node* new_node = NULL;
    if (token == NUMBER) {
        new_node = malloc(sizeof(struct node));
        new_node->token = NUMBER;
        new_node->value.number = value;
        new_node->left = NULL;
        new_node->right = NULL; 
        token = gettoken();
    }
    if (token == LBRACE) {
        token = gettoken();
        new_node = expression();
        token = gettoken();
    }
    return new_node;
}
struct node* term() {
    struct node* new_node = factor();
    struct node* cur_node = NULL;
    int binopdivmul = binop;/* binop div or mul */
    if ((token == BINOP) && ((binopdivmul == '*') || (binopdivmul == '/')))  {
        cur_node = malloc(sizeof(struct node));
        cur_node->token = BINOP;
        cur_node->value.op = binopdivmul;
        cur_node->left = new_node;
        token = gettoken();
        cur_node->right = term();
        return cur_node;
    }
    return new_node;
}
struct node* expression() {
    struct node* new_node = term();
    struct node* cur_node = NULL;   
    int binopplmin = binop; /* binop plus or minus */
    if ((token == BINOP) && ((binopplmin == '+') || (binopplmin == '-'))) {
        cur_node = malloc(sizeof(struct node));
        cur_node->value.op = binopplmin;
        cur_node->token = BINOP;
        cur_node->left = new_node; 
        token = gettoken();
        cur_node->right = expression();
        return cur_node;
    }
    return new_node;
}
void header() {
    printf("extern finish\n");
    printf("extern write\n");
    printf("global _start\n");
    printf("_start:\n");
    return;
}
void footer() {
    printf("call write\n");
    printf("add esp,4\n");
    printf("call finish\n");
    return;
}
void creationofcode(struct node* treenode) {
    char operation;
    if (treenode != NULL) {
        if (treenode->token == BINOP) {
            operation = treenode->value.op; 
            creationofcode(treenode->left);
        } else {
            printf("push %d\n",treenode->value.number);
            return;
        }
        creationofcode(treenode->right);
        printf("pop ebx\n");
        printf("pop eax\n");
        switch (operation) {
            case '+' : printf("add eax,ebx\n");break;
            case '-' : printf("sub eax,ebx\n");break;
            case '*' : printf("mov edx,0\n"); printf("mul ebx\n");break;
            case '/' : printf("mov edx,0\n"); printf("div ebx\n");break;
        }
        printf("push eax\n");
    }
    return;
}
struct node* root = NULL;
void creationasm() {
    header();
    creationofcode(root);
    footer();
    return;
}
/*int main() {
    int token = gettoken();
    while(token != EOFL) {
        switch (token) {
            case NUMBER : printf("\nNUMBER , %d", value);break;
            case BINOP : printf("\nBINOP , %c", binop);break;
            case LBRACE : printf("\nLBRACE");break;
            case RBRACE : printf("\nRBRACE");break;
            case ERROR : printf("\nERROR");break;
        }
        token = gettoken();
    }
printf("\nEOFL");
return 0;
}*/
int main(int argc, char *argv[]) {
    token = gettoken();
    root = expression();
    int i;
    for(i = 0; i < argc; ++i) {
        if (strcmp(argv[i],"bypass") == 0) {
            bypass(0, root);
        } 
        if (strcmp(argv[i], "count") == 0) {
            printf("%d",counting(root));
            printf("\n");
        } 
        if (strcmp(argv[i],"asm") == 0) {
            creationasm();
        }
    }
    return 0;
}
