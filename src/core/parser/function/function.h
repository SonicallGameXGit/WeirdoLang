#ifndef __CORE__PARSER__FUNCTION__PARSER_H__
#define __CORE__PARSER__FUNCTION__PARSER_H__

#include <core/parser/function/params.h>
#include <core/parser/function/return_type.h>
#include <core/parser/function/body/body.h>

void Parser_parseFunction(Node *node, const List *tokens, size_t *tokenId) {
    Parser_parseIdentifier(node, tokens, tokenId, "function name");
    Parser_parseFunctionParams(node, tokens, tokenId);
    Parser_parseFunctionReturnType(node, tokens, tokenId);
    Parser_parseFunctionBody(node, tokens, tokenId);
}

#endif