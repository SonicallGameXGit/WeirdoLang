#ifndef __CORE__PARSER__FUNCTION__BODY__BODY_H__
#define __CORE__PARSER__FUNCTION__BODY__BODY_H__

#include <core/parser/base.h>
#include <core/tokenizer.h>
#include <core/parser/function/body/expressions.h>

void Parser_parseBodyToken(Node *node, Token *token, const List *tokens, size_t *tokenId) {
    if (token->type == TokenType_Function) {
        fprintf(stderr, "Parser Error (line %zu, column %zu): Unexpected function declaration inside function body\n", token->line, token->column);
        exit(EXIT_FAILURE);
    }
    if (token->type == TokenType_Identifier) {
        // TODO: Delete this comment if there's no case where there's NodeType other than expression
        size_t identifierStart = 0, identifierEnd = token->value.length - 1;
        Parser_quoteIdentifier(token, &identifierStart, &identifierEnd, "expression");

        Node *expression = (Node *)malloc(sizeof(Node));
        expression->children = (List){0};
        Node *identifier = (Node *)malloc(sizeof(Node));
        identifier->type = NodeType_Identifier;
        identifier->value = (String){0};
        identifier->children = (List){0};
        String_fromN(&identifier->value, &token->value.data[identifierStart], identifierEnd - identifierStart + 1);
        List_push(&expression->children, identifier);
        expression->value = (String){0};

        Parser_parseUnknownExpression(expression, tokens, tokenId);
        List_push(&node->children, expression);
        return;
    }
    if (token->type == TokenType_Variable) { // TODO: Add type declaration and (optional) initial value parsing
        Parser_parseVariableDeclaration(node, tokens, tokenId);
        return;
    }
}
void Parser_parseFunctionBody(Node *node, const List *tokens, size_t *tokenId) {
    // Expect OpenBrace
    Token *token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected '{' for function body");
    if (token->type != TokenType_OpenBrace) {
        fprintf(stderr, "Parser Error (line %zu, column %zu): Expected '{' for function body, got '%s'\n", token->line, token->column, token->value.data == NULL ? TokenType_Keys[token->type] : token->value.data);
        exit(EXIT_FAILURE);
    }

    Node *functionBody = (Node *)malloc(sizeof(Node));
    functionBody->type = NodeType_FunctionBody;
    functionBody->value = (String){0};
    functionBody->children = (List){0};

    while ((token = Parser_getTokenAndAdvance(tokens, tokenId, "Expected '}' for function body"))->type != TokenType_CloseBrace) {
        Parser_parseBodyToken(functionBody, token, tokens, tokenId);
    }
    List_push(&node->children, functionBody);
}

#endif