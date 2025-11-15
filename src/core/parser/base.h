#ifndef __CORE__PARSER_BASE_H__
#define __CORE__PARSER_BASE_H__

#include <util/string.h>
#include <util/list.h>

enum NodeType {
    NodeType_Root,
    NodeType_Function,
    NodeType_FunctionParams,
    NodeType_FunctionBody,
    NodeType_CallExpression,
    NodeType_VariableDeclaration,
    NodeType_VariableAssignment,
    NodeType_Identifier,
};
typedef struct {
    enum NodeType type;
    String value;
    List children;
} Node;

#endif