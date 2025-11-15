#include <util/string.h>
#include <util/file.h>
#include <core/tokenizer.h>
#include <core/compiler.h>
#include <core/parser/parser.h>

void Node_print(Node *node, int depth, bool isFirst, bool isLast) {
    for (int i = 0; i < depth; i++) {
        printf("   ");
    }

    if (isLast) {
        printf("┗");
    } else {
        printf("┣");
    }

    if (node->children.length > 0) {
        printf("━━┳━■ ");
    } else {
        printf("━━━━■ ");
    }

    switch (node->type) {
        case NodeType_Root: {
            printf("Root\n");
            break;
        }
        case NodeType_Function: {
            printf("Function\n");
            break;
        }
        case NodeType_FunctionParams: {
            printf("FunctionParams\n");
            break;
        }
        case NodeType_FunctionBody: {
            printf("FunctionBody\n");
            break;
        }
        case NodeType_Identifier: {
            printf("Identifier, Value: \"%s\"\n", node->value.data);
            break;
        }
        case NodeType_CallExpression: {
            printf("CallExpression\n");
            break;
        }
        case NodeType_VariableDeclaration: {
            printf("VariableDeclaration\n");
            break;
        }
        default: {
            printf("Unknown\n");
            break;
        }
    }
    for (size_t i = 0; i < node->children.length; i++) {
        Node *child = (Node *)node->children.items[i];
        Node_print(child, depth + 1, i == 0, i == node->children.length - 1);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    String filename;
    String_from(&filename, argv[1]);

    String source;
    File_read(filename.data, &source);
    printf("\nSource:\n%s\n", source.data);

    List tokens = {0};
    Tokenizer_tokenize(&tokens, &source);
    printf("\nTokens (%zu):\n", tokens.length);
    for (size_t i = 0; i < tokens.length; i++) {
        Token *token = (Token *)tokens.items[i];
        printf("  %zu: \"%s\"\n", i, token->type == TokenType_Identifier ? token->value.data : TokenType_Keys[token->type]);
    }

    Node *root = (Node *)malloc(sizeof(Node));
    root->type = NodeType_Root;
    root->value = (String){0};
    root->children = (List){0};

    Parser_parse(root, &tokens);
    printf("\nParse Tree:\n");
    Node_print(root, 0, true, true);

    String output = {0};
    Compiler_compile(root, &output);
    File_write("./dist/output.asm", &output);

    system("nasm -felf64 ./dist/output.asm -o ./dist/output.o");
    system("ld ./dist/output.o -o ./dist/output");
    system("rm ./dist/output.o");
    system("rm ./dist/output.asm");

    printf("\nCompiled successfully assembly to ./dist/output. Source:\n%s\n", output.data);

    String_free(&output);
    Parser_free(root);
    Tokenizer_free(&tokens);
    String_free(&source);
    String_free(&filename);
    return 0;
}