#ifndef __CORE__PARSER_UTIL_H__
#define __CORE__PARSER_UTIL_H__

#include <core/parser/base.h>
#include <util/list.h>
#include <core/tokenizer.h>

Token *Parser_getTokenAndAdvance(const List *tokens, size_t *tokenId, const char *errorMessage) {
    if (*tokenId >= tokens->length) {
        Token *token = tokens->length > 0 ? (Token *)List_get(tokens, *tokenId - 1) : NULL;
        fprintf(stderr, "Parser Error (line %zu, column %zu): %s\n", token != NULL ? token->line : 1, token != NULL ? token->column : 1, errorMessage != NULL ? errorMessage : "Unexpected end of tokens");
        exit(EXIT_FAILURE);
    }
    Token *token = (Token *)List_get(tokens, *tokenId);
    (*tokenId)++;
    return token;
}
void Parser_quoteIdentifier(const Token *token, size_t *start, size_t *end, const char *purpose) {
    while (token->value.data[*start] == ' ' || token->value.data[*start] == '\n' || token->value.data[*start] == '\t' || token->value.data[*start] == '\r') {
        (*start)++;
        if (*start >= token->value.length) {
            fprintf(stderr, "Parser Error (line %zu, column %zu): %s identifier cannot be empty or only whitespace\n", token->line, token->column, purpose == NULL ? "" : purpose);
            exit(EXIT_FAILURE);
        }
    }
    while (token->value.data[*end] == ' ' || token->value.data[*end] == '\n' || token->value.data[*end] == '\t' || token->value.data[*end] == '\r') {
        (*end)--;
        if (*end < *start) {
            fprintf(stderr, "Parser Error (line %zu, column %zu): %s identifier cannot be empty or only whitespace\n", token->line, token->column, purpose == NULL ? "" : purpose);
            exit(EXIT_FAILURE);
        }
    }
    for (size_t i = *start; i <= *end; i++) {
        if (!((token->value.data[i] >= 'a' && token->value.data[i] <= 'z') ||
              (token->value.data[i] >= 'A' && token->value.data[i] <= 'Z') ||
              (token->value.data[i] >= '0' && token->value.data[i] <= '9') ||
              (token->value.data[i] == '_'))) {
            fprintf(stderr, "Parser Error (line %zu, column %zu): Invalid character '%c' in %s identifier\n", token->line, token->column, token->value.data[i], purpose == NULL ? "" : purpose);
            exit(EXIT_FAILURE);
        }
    }
}
void Parser_parseIdentifier(Node *node, const List *tokens, size_t *tokenId, const char *purpose) {
    Token *token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected identifier");
    if (token->type != TokenType_Identifier) {
        fprintf(stderr, "Parser Error (line %zu, column %zu): Expected %s identifier, got '%s'\n", token->line, token->column, purpose == NULL ? "" : purpose, token->value.data == NULL ? TokenType_Keys[token->type] : token->value.data);
        exit(EXIT_FAILURE);
    }

    size_t identifierStart = 0;
    size_t identifierEnd = token->value.length - 1;
    Parser_quoteIdentifier(token, &identifierStart, &identifierEnd, purpose);

    Node *identifier = (Node *)malloc(sizeof(Node));
    identifier->type = NodeType_Identifier;
    String_fromN(&identifier->value, &token->value.data[identifierStart], identifierEnd - identifierStart + 1);
    identifier->children = (List){0};
    List_push(&node->children, identifier);
}

#endif