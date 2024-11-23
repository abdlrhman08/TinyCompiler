#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("No tiny file given \n");
        printf("Usage: TinyCompiler [*.tiny]\n");
        return 1;
    }
    if (open_compilation(argv[1]) !=  0) {
        print_error();
        return 1;
    }
    Token* tokens = tokenize();
    if (tokens == NULL) {
        print_error();
        return 1;
    }

    size_t token_count = get_token_count();
    printf("Tokens: %zu\n", token_count);
    for (size_t i = 0; i < token_count; i++) {
        print_token(&tokens[i]);
    }

    return 0;
}
