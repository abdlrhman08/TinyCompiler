#include "scanner.h"

#include <stdio.h>

int main()
{
    open_compilation("main.tiny");

    Token tk = next_token();

    printf("The current value is %lu \n", tk.num_val);
}
