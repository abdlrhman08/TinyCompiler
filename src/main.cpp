#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "parser.hpp"

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
    Parser parser;
    parser.setTokensList(tokens, token_count);
    parser.run();
    parser.printParseTree(parser.parse_tree, 0);
    parser.clearTables();

    return 0;
}