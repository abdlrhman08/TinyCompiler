#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <cstddef>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TokenType {
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
    ELSE,
} TokenType;

typedef struct Token {
    TokenType token_type;
    const char* string_val;
    unsigned long long num_val;
} Token;


uint8_t open_compilation(const char* file_name);
size_t get_token_count();

void print_token(Token* token);
void print_error();

const char* get_error_string();

Token* tokenize();

#ifdef __cplusplus
}
#endif
#endif
