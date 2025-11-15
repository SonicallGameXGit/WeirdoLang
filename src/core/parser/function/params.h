#ifndef __CORE__PARSER__FUNCTION__PARAMS_H__
#define __CORE__PARSER__FUNCTION__PARAMS_H__

#include <core/tokenizer.h>
#include <core/parser/base.h>
#include <core/parser/util.h>

void Parser_parseFunctionParams(Node *node, const List *tokens, size_t *tokenId) {
    // Expect OpenParen
    Token *token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected '(' for function parameters");
    if (token->type != TokenType_OpenParen) {
        fprintf(stderr, "Parser Error (line %zu, column %zu): Expected '(' for function parameters, got '%s'\n", token->line, token->column, token->value.data == NULL ? TokenType_Keys[token->type] : token->value.data);
        exit(EXIT_FAILURE);
    }

    // TODO: Parse parameters | For now we ignore them
    while ((token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected ')' for function parameters"))->type != TokenType_CloseParen);

    Node *functionParams = (Node *)malloc(sizeof(Node));
    functionParams->type = NodeType_FunctionParams;
    functionParams->value = (String){0};
    functionParams->children = (List){0};
    List_push(&node->children, functionParams);
}

#endif