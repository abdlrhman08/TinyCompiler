#include <stdint.h>

enum TokenType {
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

struct Token {
    TokenType token_type;
    char* string_val;
    uint64_t num_val;
};

#ifndef __cplusplus
typedef struct Token Token;
#endif


#ifdef __cplusplus
extern "C" {
#endif
void* open_compilation(const char* file_name);

Token next_token();
#ifdef __cplusplus
}
#endif


