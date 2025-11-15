#ifndef __CORE__PARSER__FUNCTION__RETURN_TYPE_H__
#define __CORE__PARSER__FUNCTION__RETURN_TYPE_H__

#include <core/parser/base.h>
#include <core/parser/util.h>
#include <core/tokenizer.h>

void Parser_parseFunctionReturnType(Node *node, const List *tokens, size_t *tokenId) {
    // Expect Colon
    Token *token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected ':' for function return type");
    if (token->type != TokenType_Colon) {
        fprintf(stderr, "Parser Error (line %zu, column %zu): Expected ':' for function return type, got '%s'\n", token->line, token->column, token->value.data == NULL ? TokenType_Keys[token->type] : token->value.data);
        exit(EXIT_FAILURE);
    }

    // Expect type name (Identifier)
    Parser_parseIdentifier(node, tokens, tokenId, "function return type");
}

#endif