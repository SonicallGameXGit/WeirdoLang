#ifndef __CORE__PARSER_H__
#define __CORE__PARSER_H__

#include <core/parser/base.h>
#include <core/tokenizer.h>
#include <core/parser/function/function.h>
#include <core/parser/util.h>

void Parser_parse(Node *root, const List *tokens) {
    size_t tokenId = 0;
    while (tokenId < tokens->length) {
        Token *token = (Token *)List_get(tokens, tokenId);
        tokenId++;

        if (token->type == TokenType_Function) {
            Node *node = (Node *)malloc(sizeof(Node));
            node->type = NodeType_Function;
            node->value = (String){0};
            node->children = (List){0};

            Parser_parseFunction(node, tokens, &tokenId);
            List_push(&root->children, node);
            continue;
        }

        fprintf(stderr, "Parser Error (line %zu, column %zu): Unexpected token '%s'\n", token->line, token->column, token->value.length == 0 ? TokenType_Keys[token->type] : token->value.data);
        exit(EXIT_FAILURE);
    }
}
void Parser_free(Node *root) {
    for (size_t i = 0; i < root->children.length; i++) {
        Node *child = (Node *)List_get(&root->children, i);
        Parser_free(child);
    }
    String_free(&root->value);
    List_free(&root->children);
    free(root);
}

#endif