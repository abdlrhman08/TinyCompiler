#include <stdint.h>

enum TokenType {
    SEMICOLON,
    IF,
    THEN,
    END,
    REPEAT,
    UNTIL,
    IDENTIFIER,
    ASSIGN,
    READ,
    WRITE,
    LESSTHAN,
    EQUAL,
    PLUS,
    MINUS,
    MULT,
    DIV,
    OPENBRACKET,
    CLOSEDBRACKET,
    NUMBER,
    UNKNOWN,
    ELSE,
};

#ifndef __cplusplus
typedef enum TokenType TokenType;
#endif

#include <stddef.h> 

struct Token {
    TokenType token_type;
    const char* string_val;
    unsigned long long num_val;
};

#ifndef __cplusplus
typedef struct Token Token;
#endif

#ifdef __cplusplus
extern "C" {
#endif
uint8_t open_compilation(const char* file_name);

void print_token(Token* token);
void print_error();

Token* tokenize();

size_t get_token_count();
#ifdef __cplusplus
}
#endif


