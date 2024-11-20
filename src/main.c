#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    open_compilation("main.tiny");

    Token* tokens = tokenize();
    size_t token_count = get_token_count();

    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; i++) {
        Token tk = tokens[i];
        printf("Token %zu: type = %i, string_val = %s, num_Val = %llu\n", i, tk.token_type, tk.string_val, tk.num_val);
    }

    free(tokens);

    return 0;
}
