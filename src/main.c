#include "scanner.h"

#include <stdio.h>

int f () {
    return 5;
}

int main()
{
    open_compilation("main.tiny");

    Token tk = next_token();

    printf("The current value is %i \n", tk.token_type);
}
