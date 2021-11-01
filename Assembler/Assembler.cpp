#include "..\libr\Hamlet.h"
#include "..\enum.h"
#include "..\libr\Stack.h"


enum errors_of_assembler
{
    WRONG_ARGUMENT = 0xF1FA1228,
    EXIT = 0x111111,
};


#ifdef Int_t            

#define type_array_format "%d"

#endif

#ifdef Double_t

#define type_array_format "%.14lf"

#endif

#define DEF_CMD_(number, name, arg, code_func)                                                                  \
        if (strcmp(cmd, #name) == 0)                                                                            \
        {                                                                                                       \
            code[ip++] = CMD_##name;                                                                            \
                                                                                                                \
            for (int i = 0; i < arg; i++)                                                                       \
            {                                                                                                   \
                arg_var = check_RAM(text_struct->linii[j].start_line);                                          \
                                                                                                                \
                if (arg_var != WRONG_ARGUMENT)                                                                  \
                {                                                                                               \
                    if (i == 0)                                                                                 \
                    {                                                                                           \
                        code[ip - 1] = CMD_##name + 0x02;                                                       \
                    }                                                                                           \
                    *(type_array*)(code + ip) = arg_var;                                                        \
                     ip += sizeof(type_array);                                                                  \
                    text_struct->linii[j].start_line = next_argument(text_struct->linii[j].start_line);         \
                                                                                                                \
                }                                                                                               \
                else                                                                                            \
                {                                                                                               \
                    arg_var = check_register(text_struct->linii[j].start_line);                                 \
                                                                                                                \
                    if (arg_var != WRONG_ARGUMENT)                                                              \
                    {                                                                                           \
                    if (i == 0)                                                                                 \
                    {                                                                                           \
                        code[ip - 1] = CMD_##name + 0x01;                                                       \
                    }                                                                                           \
                        code[ip++] = arg_var;                                                                   \
                        text_struct->linii[j].start_line = next_argument(text_struct->linii[j].start_line);     \
                    }                                                                                           \
                    else                                                                                        \
                    {                                                                                           \
                        if (sscanf(text_struct->linii[j].start_line, type_array_format, &arg_var) != 0)         \
                        {                                                                                       \
                            *(type_array*)(code + ip) = arg_var;                                                \
                            ip += sizeof(type_array);                                                           \
                            text_struct->linii[j].start_line = next_argument(text_struct->linii[j].start_line); \
                        }                                                                                       \
                        else                                                                                    \
                        {                                                                                       \
                                printf("Wrong argument of command!\n\n");                                       \
                                return WRONG_ARGUMENT;                                                          \
                        }                                                                                       \
                    }                                                                                           \
                }                                                                                               \
            }                                                                                                   \
        }                                                                                                       \
        else                                                                                                    \

 
                                                                  


struct asm_file
{
    char version;

    const char sign[4] = {'S', 'H', 'Z', '!'};

    int len_of_code = 0;

    char* code;
};


const int Max_argc_cmd = 3 * 8;



int len_name_of_command(char* str)  
{
    assert(str != nullptr);

    char* pointer_end = strchr(str, ' ');

    size_t len_of_command = 0;

    if (pointer_end != nullptr) 
    {
        len_of_command =  (pointer_end - str);
    }
    else
    {
        pointer_end = strchr(str, '\0');
        len_of_command =  (pointer_end - str);
    }

    return len_of_command;
}

type_array check_RAM(char* start_of_line)
{
    assert(start_of_line != nullptr);

    char skobka = '0';
    type_array arg = 0;

    sscanf(start_of_line, "%c", &skobka);

    if (skobka != '[')
    {
        return WRONG_ARGUMENT;
    }
    else
    {
        char* end_of_line = strchr(start_of_line, ']');
        if (end_of_line == nullptr)
        {
            printf("Wrong argument! \n");
            return WRONG_ARGUMENT;
        }

        int len_of_arg = 0;

        len_of_arg =  end_of_line - start_of_line - 1;   //  [143] 
        char* arg_str = (char*)calloc(len_of_arg, sizeof(char));

        strncpy(arg_str, start_of_line + 1, len_of_arg );

        sscanf(arg_str, type_array_format, &arg);

        free(arg_str);
    }

    return arg;

}

int check_register(char* start_of_line)
{
    assert(start_of_line != nullptr);

    char* end_of_line = strchr(start_of_line, ' ');

    if (end_of_line == nullptr)
    {
        end_of_line = strchr(start_of_line, '\0');
    }

    int len_of_arg = end_of_line - start_of_line + 1;
    char* arg_line = (char*)calloc(len_of_arg, sizeof(char));
    int arg = 0; 

    strncpy(arg_line, start_of_line, len_of_arg);  // ax_   ax\0 

    arg_line[len_of_arg - 1] = '\0';

    if (strcmp(arg_line, "ax") == 0)
    {
        arg = 0x00;
    }
    else
    {
        if (strcmp(arg_line, "bx") == 0)
        {
            arg = 0x01;
        }
        else
        {
            if (strcmp(arg_line, "cx") == 0)
            {
                arg = 0x02;
            }
            else 
            {
                if (strcmp(arg_line, "dx") == 0)
                {
                    arg = 0x03;
                }
                else
                {
                    arg = WRONG_ARGUMENT;
                }
            }
        }
    }

    return arg;
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

char* next_argument(char* line_command)
{
    assert(line_command != nullptr);

    char* str = strchr(line_command, ' ');
    if (str == nullptr)
    {
        str = strchr(line_command, '\0');
        return(str);
    }

    int count_of_space = 0;

    sscanf(str, " %n", &count_of_space);

    return (str + count_of_space);
}

void get_line_cmd(char* str, char* start_line_of_command)
{       
    assert(str != nullptr);
    assert(start_line_of_command != nullptr);


    int len_command = 0;
    int count_of_space = 0;

    sscanf(start_line_of_command, " %n", &count_of_space);

    start_line_of_command += count_of_space;

    len_command = len_name_of_command(start_line_of_command);

    strncpy(str, start_line_of_command, len_command);

    str[len_command] = '\0';
    
}

int compilation(struct Text* text_struct, char* code)
{
    assert(text_struct != nullptr);
    assert(code != nullptr);
    
    char cmd[100] = "";
    size_t ip = 0;
    size_t count_of_space = 0;
    int len_cmd = 0;
    type_array arg_var = 0;

    for (int j = 0; j < text_struct->quantity_lines; j++)
    {
        get_line_cmd(cmd, text_struct->linii[j].start_line);

        len_cmd = strlen(cmd);

        sscanf(text_struct->linii[j].start_line, " %n", &count_of_space);

        text_struct->linii[j].start_line += count_of_space + len_cmd;

        text_struct->linii[j].start_line = next_argument(text_struct->linii[j].start_line);

        #include "..\Commands.h"

        /*else*/ printf("Error!\n");

    }
    return EXIT;
}

void write_machine_code(struct asm_file* machine_code, FILE* ass)
{
    assert(machine_code != nullptr);
    assert(ass != nullptr);

    for (int i = 0; i < 4; i++)
    {
        fprintf(ass, "%c", machine_code->sign[i]);
    }

    fprintf(ass, "%c", machine_code->version);

    fprintf(ass, "%d", machine_code->len_of_code);

    fwrite(machine_code->code, sizeof(char), machine_code->len_of_code, ass);
}

void input_machine_struct(struct asm_file* machine_code, struct Text* text_struct)
{
    machine_code->version = Version_of_programm;

    machine_code->len_of_code = text_struct->quantity_lines * Max_argc_cmd;

    machine_code->code = (char*)calloc(machine_code->len_of_code, sizeof(char));
}


int main()
{
    FILE* cmd = fopen("../commands.txt", "rb");

    if (cmd == nullptr) 
    {
        printf("Wrong name of file\n");
        return EXIT;
    }

    struct Text text_struct = {};

    input_struct(cmd, &text_struct);

    fclose(cmd);

    navigation_for_comment(&text_struct);

    struct asm_file machine_code = {};

    input_machine_struct(&machine_code, &text_struct);

    compilation(&text_struct, machine_code.code);

    FILE* ass = fopen("../assembler.bin", "wb");

    write_machine_code(&machine_code, ass); 

    fclose(ass);

    free_memory(&text_struct);

    return EXIT;    
}


#undef DEF_CMD_