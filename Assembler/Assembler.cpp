#include "..\libr\Hamlet.h"
#include "..\enum.h"
#include "..\libr\Stack.h"


#define DEF_CMD_(number, name, agr, code)                                        \
        if (strcmp(str, #name) == 0)                                             \
        {                                                                        \
            code[ip++] = CMD_##name;                                             \
                                                                                 \
            for (int i = 0; i < argc; i++)                                       \
            {                                                                    \
                #ifdef Int_t                                                     \
                    sscanf(text_struct->linii[i].start_line, "%d", &code[ip]);   \
                #endif                                                           \
                                                                                 \
                #ifdef Double_t                                                  \
                                                                                 \
                    sscanf(text_struct->linii[i].start_line, "%lg", &double_var);\
                    *(double*)(code + ip) = double_var;                          \
                #endif                                                           \
                                                                                 \
                ip += sizeof(type_array);                                        \
            }                                                                    \
        }                                                                        \
        else                                                                     \                                                                        \

        


struct asm_file
{
    const char Version = 1;

    const char sign[4] = {'S', 'H', 'Z', '!'};

    char* code;
};


const int Max_argc_cmd = 3 * 8;


int len_name_of_command(char* str)
{
    assert(str != nullptr);

    char* pointer_end = strchr(str, ' ');

    size_t len_name_of_command = 0;

    if (pointer_end != nullptr) 
    {
        len_name_of_command =  (pointer_end - str);
    }
    else
    {
        pointer_end = strchr(str, '\0');
        len_name_of_command =  (pointer_end - str);
    }
     
}

void navigation_for_comment(struct Text* text_struct)
{
    assert(text_struct != nullptr);

    char* pointer_ne_start = text_struct->linii[0].start_line;
    

    for (int i = 0; i < text_struct->quantity_lines; i++)
    {
        if (strchr(text_struct->linii[i].start_line, ';') != nullptr)
        {
            pointer_ne_start = strchr(text_struct->linii[i].start_line, ';');
            *pointer_ne_start = '\0';
        }
    }
}

int compilation(struct Text* text_struct, char* code)
{
    assert(text_struct != nullptr);
    assert(code != nullptr);
    
    char str[100] = "";
    int len_command = 0;
    int argc = 1;
    size_t ip = 0;
    double double_var = 0;

    for (int i = 0; i < text_struct->quantity_lines; i++)
    {
        len_command = len_name_of_command(text_struct->linii[i].start_line);

        strncpy(str, text_struct->linii[i].start_line, len_command);

        str[len_command] = '\0';

        text_struct->linii[i].start_line += len_command + 1;

        #include "Commands.h"

        /*else*/ printf("Error!");

    }
    return 0;
}



int main()
{
    FILE* cmd = fopen("../commands.txt", "rb");

    if (cmd == nullptr) 
    {
        printf("Wrong name of file\n");
        return 1;
    }

    struct Text text_struct = {};

    input_struct(cmd, &text_struct);

    fclose(cmd);

    navigation_for_comment(&text_struct);

    //printf("%d", CMD_OUT);

    struct asm_file machine_code = {};

    int len_of_machine_code = text_struct.quantity_lines * Max_argc_cmd;

    machine_code.code = (char*)calloc(len_of_machine_code, sizeof(char));

    compilation(&text_struct, machine_code.code);

    FILE* ass = fopen("../assembler.bin", "wb");

    output_machine_code(machine_code.code, ass);

    return 0;    
}