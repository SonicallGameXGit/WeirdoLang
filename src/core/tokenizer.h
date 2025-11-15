#ifndef __CORE__TOKENIZER_H__
#define __CORE__TOKENIZER_H__

#include <util/string.h>
#include <util/list.h>

enum TokenType {
    TokenType_Function,
    TokenType_OpenParen, TokenType_CloseParen,
    TokenType_OpenBrace, TokenType_CloseBrace,
    TokenType_Colon,
    TokenType_Semicolon,
    TokenType_Variable,
    TokenType_Identifier,
};
const char *TokenType_Keys[] = {
    [TokenType_Function] = "fn ",
    [TokenType_OpenParen] = "(",
    [TokenType_CloseParen] = ")",
    [TokenType_OpenBrace] = "{",
    [TokenType_CloseBrace] = "}",
    [TokenType_Colon] = ":",
    [TokenType_Semicolon] = ";",
    [TokenType_Variable] = "var ",
};

typedef struct {
    enum TokenType type;
    String value;
    size_t line, column;
} Token;

void Tokenizer_tokenize(List *tokens, const String *source) {
    size_t ptr = 0;
    size_t identifierStart = 0;
    size_t line = 1, column = 1;
    bool parsingIdentifier = false;
    while (ptr < source->length) {
        // Skip whitespaces
        if (source->data[ptr] == ' ' || source->data[ptr] == '\n' || source->data[ptr] == '\t' || source->data[ptr] == '\r') {
            if (source->data[ptr] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            
            ptr++;
            continue;
        }

        // Try to match any token by it's key
        bool matched = false;
        for (size_t i = 0; i < sizeof(TokenType_Keys) / sizeof(char *); i++) {
            if (strncmp(&source->data[ptr], TokenType_Keys[i], strlen(TokenType_Keys[i])) != 0) {
                continue;
            }

            // If we were parsing an identifier, finalize it
            if (parsingIdentifier) {
                parsingIdentifier = false;
                
                Token *token = (Token *)malloc(sizeof(Token));
                token->type = TokenType_Identifier;
                token->line = line;
                token->column = column;
                String_fromN(&token->value, &source->data[identifierStart], ptr - identifierStart);
                List_push(tokens, token);
            }

            Token *token = (Token *)malloc(sizeof(Token));
            token->type = (enum TokenType)i;
            token->line = line;
            token->column = column;
            token->value = (String){0};
            List_push(tokens, token);

            size_t tokenLength = strlen(TokenType_Keys[i]);
            ptr += tokenLength;
            column += tokenLength;
            goto continue_;
        }
        
        // Else, append the char to the identifier
        if (!parsingIdentifier) {
            // Maybe, I don't really need that?
            // if (ptr > 0) {
            //     if (source->data[ptr - 1] != ' ' && source->data[ptr - 1] != '\n' && source->data[ptr - 1] != '\t' && source->data[ptr - 1] != '\r') {
            //         // If there's no space before an identifier, it's an error
            //         fprintf(stderr, "Tokenizer Error: Expected whitespace before identifier at line %zu, column %zu\n", line, column);
            //         exit(EXIT_FAILURE);
            //     }
            // }

            identifierStart = ptr;
            parsingIdentifier = true;
        }

        ptr++;
        column++;

        continue_:;
    }

    if (parsingIdentifier) {
        parsingIdentifier = false;
        
        Token *token = (Token *)malloc(sizeof(Token));
        token->type = TokenType_Identifier;
        token->line = line;
        token->column = column;
        String_fromN(&token->value, &source->data[identifierStart], ptr - identifierStart);
        List_push(tokens, token);
    }
}
void Tokenizer_free(List *tokens) {
    for (size_t i = 0; i < tokens->length; i++) {
        Token *token = (Token *)tokens->items[i];
        String_free(&token->value);
        free(token);
    }
    List_free(tokens);
}

#endif