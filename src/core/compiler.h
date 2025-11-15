#ifndef __CORE__COMPILER_H__
#define __CORE__COMPILER_H__

#include <core/parser/base.h>

typedef struct {
    String name;
    size_t stackOffset;
} VariableStackPair;

void Compiler_declareFunctions(const Node *root, List *functionIdentifiers, String *output) {
    for (size_t i = 0; i < root->children.length; i++) {
        Node *child = (Node *)root->children.items[i];
        if (child->type == NodeType_Function) {
            // Assuming it's always: "fn <identifier>(<params>): <return_type> { <body> }"
            Node *identifier = (Node *)child->children.items[0];
            List_push(functionIdentifiers, &identifier->value);
            Node *returnType = (Node *)child->children.items[2];
            String_append(output, "global ");
            if (strcmp(identifier->value.data, "main") == 0) {
                if (strcmp(returnType->value.data, "void") != 0) {
                    fprintf(stderr, "Compiler Error: 'main' function must have return type 'void'\n");
                    exit(EXIT_FAILURE);
                }
                
                String_append(output, "_start\n");
            } else {
                String_append(output, identifier->value.data);
                String_append(output, "\n");
            }
        }
    }
}
void Compiler_handleVariableDeclarations(const List *variables, String *output) {
    for (size_t i = 0; i < variables->length; i++) {
        VariableStackPair *varPair = (VariableStackPair *)variables->items[i];
        char buffer[32 + 14];
        sprintf(buffer, "mov dword[rsp+%zu],0\n", varPair->stackOffset);
        String_append(output, buffer);
    }
}
void Compiler_defineFunctionBody(const Node *node, const List *functionIdentifiers, String *output) {
    size_t stackOffset = 0;
    List variables = {0}; // TODO: Currently variables are only current-stack scoped, no nested scopes (e.g. ifs, loops, etc.)

    for (size_t i = 0; i < node->children.length; i++) {
        Node *child = (Node *)List_get(&node->children, i);
        if (child->type == NodeType_CallExpression) {
            Node *identifier = (Node *)child->children.items[0];
            bool found = false;
            for (size_t j = 0; j < functionIdentifiers->length; j++) {
                String *funcId = (String *)functionIdentifiers->items[j];
                if (strcmp(identifier->value.data, funcId->data) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                fprintf(stderr, "Compiler Error: Undefined function '%s'\n", identifier->value.data);
                exit(EXIT_FAILURE);
            }
            String_append(output, "call ");
            if (strcmp(identifier->value.data, "main") == 0) {
                String_append(output, "_start\n");
            } else {
                String_append(output, identifier->value.data);
                String_append(output, "\n");
            }
            continue;
        }
        if (child->type == NodeType_VariableDeclaration) {
            // TODO: Handle variable declarations
            Node *identifier = (Node *)child->children.items[0];
            VariableStackPair *varPair = (VariableStackPair *)malloc(sizeof(VariableStackPair));
            String_copy(&varPair->name, &identifier->value);
            varPair->stackOffset = stackOffset;
            stackOffset += 4; // TODO: Assuming int32 variables for now, need to handle all other types later
            List_push(&variables, varPair);

            continue;
        }

        fprintf(stderr, "Compiler Error: Unsupported expression in function body\n");
        exit(EXIT_FAILURE);
    }

    // Preserve stack space for everything needed
    char buffer[32 + 9];
    sprintf(buffer, "sub rsp,%zu\n", stackOffset);
    String_append(output, buffer);

    for (size_t i = 0; i < variables.length; i++) {
        for (size_t j = 0; j < variables.length; j++) {
            VariableStackPair *varPairA = (VariableStackPair *)variables.items[i];
            VariableStackPair *varPairB = (VariableStackPair *)variables.items[j];
            if (i != j && strcmp(varPairA->name.data, varPairB->name.data) == 0) {
                fprintf(stderr, "Compiler Error: Duplicate variable declaration '%s'\n", varPairA->name.data);
                exit(EXIT_FAILURE);
            }
        }
    }

    Compiler_handleVariableDeclarations(&variables, output);
    for (size_t i = 0; i < variables.length; i++) {
        VariableStackPair *varPair = (VariableStackPair *)variables.items[i];
        String_free(&varPair->name);
        free(varPair);
    }
    List_free(&variables);
}
void Compiler_defineFunctions(const Node *root, const List *functionIdentifiers, String *output) {
    for (size_t i = 0; i < root->children.length; i++) {
        Node *child = (Node *)root->children.items[i];
        if (child->type == NodeType_Function) {
            Node *identifier = (Node *)child->children.items[0];
            Node *functionBody = (Node *)child->children.items[3];

            bool isEntryPoint = strcmp(identifier->value.data, "main") == 0;
            if (isEntryPoint) {
                String_append(output, "_start:\n");
            } else {
                String_append(output, identifier->value.data);
                String_append(output, ":\n");
            }

            String_append(output, "push rbp\nmov rbp,rsp\n");
            Compiler_defineFunctionBody(functionBody, functionIdentifiers, output);
            String_append(output, "mov rsp,rbp\npop rbp\n");
            if (isEntryPoint) {
                String_append(output, "mov rax,60\nxor rdi,rdi\nsyscall\n");
            } else {
                String_append(output, "ret\n");
            }
        }
    }
}
void Compiler_compile(const Node *root, String *output) {
    String_append(output, "section .text\n");
    List functionIdentifiers = {0};
    Compiler_declareFunctions(root, &functionIdentifiers, output);
    Compiler_defineFunctions(root, &functionIdentifiers, output);
    List_free(&functionIdentifiers);
}

#endif