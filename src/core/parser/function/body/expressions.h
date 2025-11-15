#ifndef __CORE__PARSER__FUNCTION__BODY__EXPRESSIONS_H__
#define __CORE__PARSER__FUNCTION__BODY__EXPRESSIONS_H__

#include <core/tokenizer.h>
#include <core/parser/base.h>
#include <core/parser/util.h>

void Parser_parseFunctionCall(Node *node, const List *tokens, size_t *tokenId) {
    // TODO: Parse parameters | For now we ignore them
    Token *token;
    while ((token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected ')' for function call parameters"))->type != TokenType_CloseParen);

    token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected ';' after function call");
    if (token->type != TokenType_Semicolon) {
        fprintf(stderr, "Parser Error (line %zu, column %zu): Expected ';' after function call, got '%s'\n", token->line, token->column, token->value.data == NULL ? TokenType_Keys[token->type] : token->value.data);
        exit(EXIT_FAILURE);
    }
}
void Parser_parseUnknownExpression(Node *node, const List *tokens, size_t *tokenId) {
    Token *token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected expression");
    if (token->type == TokenType_OpenParen) { // It's probably a function call
        node->type = NodeType_CallExpression;
        Parser_parseFunctionCall(node, tokens, tokenId);
        return;
    }
    
    fprintf(stderr, "Parser Error (line %zu, column %zu): Unknown expression starting with '%s'\n", token->line, token->column, token->value.data == NULL ? TokenType_Keys[token->type] : token->value.data);
    exit(EXIT_FAILURE);
}

void Parser_parseVariableDeclaration(Node *node, const List *tokens, size_t *tokenId) {
    Node *varDecl = (Node *)malloc(sizeof(Node));
    varDecl->type = NodeType_VariableDeclaration;
    varDecl->value = (String){0};
    varDecl->children = (List){0};
    Parser_parseIdentifier(varDecl, tokens, tokenId, "Expected variable name after 'var'");

    Token *token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected ':' after variable declaration");
    if (token->type != TokenType_Colon) {
        fprintf(stderr, "Parser Error (line %zu, column %zu): Expected ':' after variable declaration, got '%s'\n", token->line, token->column, token->value.data == NULL ? TokenType_Keys[token->type] : token->value.data);
        exit(EXIT_FAILURE);
    }

    Parser_parseIdentifier(varDecl, tokens, tokenId, "Expected variable type after ':'");
    
    // For now we ignore the rest of the variable declaration (like type and initial value)
    while ((token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected ';' after variable declaration"))->type != TokenType_Semicolon);

    List_push(&node->children, varDecl);
}

#endif