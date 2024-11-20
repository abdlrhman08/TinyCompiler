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
    NUMBER
};

#ifndef __cplusplus
typedef enum TokenType TokenType;
#endif

#include <stddef.h> 

typedef struct Token {
    int token_type;
    const char* string_val;
    unsigned long long num_val;
} Token;



#ifndef __cplusplus
typedef struct Token Token;
#endif


#ifdef __cplusplus
extern "C" {
#endif
void open_compilation(const char* file_name);
Token* tokenize();
size_t get_token_count();

#ifdef __cplusplus
}
#endif


