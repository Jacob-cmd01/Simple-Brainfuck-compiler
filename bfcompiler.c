#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 30000

void execute(const char *code) {
    char memory[MEMORY_SIZE] = {0};
    char *ptr = memory;
    char loop_stack[MEMORY_SIZE] = {0};
    int loop_stack_index = -1;

    for (const char *ptr_code = code; *ptr_code != '\0'; ++ptr_code) {
        switch (*ptr_code) {
            case '>':
                ++ptr;
                break;
            case '<':
                --ptr;
                break;
            case '+':
                ++(*ptr);
                break;
            case '-':
                --(*ptr);
                break;
            case '.':
                putchar(*ptr);
                break;
            case ',':
                *ptr = getchar();
                break;
            case '[':
                loop_stack[++loop_stack_index] = ptr_code - code;
                break;
            case ']':
                if (*ptr) {
                    ptr_code = code + loop_stack[loop_stack_index];
                } else {
                    --loop_stack_index;
                }
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: bfcompiler <filename>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *code = (char *)malloc(file_size + 1);
    if (!code) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }

    fread(code, 1, file_size, file);
    code[file_size] = '\0';

    fclose(file);

    execute(code);

    free(code);

    return 0;
}
