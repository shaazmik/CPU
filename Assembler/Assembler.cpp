#include "..\libr\Hamlet.h"
#include "..\enum.h"
#include "..\libr\Stack.h"


enum errors_of_assembler
{
    WRONG_ARGUMENT = 0xF1FA1228,
    EXIT = 0x111111,
};



#define DEF_CMD_(number, name, argc, code_func)                                                                     \
        if (strcmp(cmd, #name) == 0)                                                                                \
        {                                                                                                           \
            machine->code[ip++] = CMD_##name;                                                                       \
                                                                                                                    \
            for (int i = 0; i < argc; i++)                                                                          \
            {                                                                                                       \
                arg_var = check_RAM(line_cmd);                                                                      \
                                                                                                                    \
                if (arg_var != WRONG_ARGUMENT)                                                                      \
                {                                                                                                   \
                    if (i == 0)                                                                                     \
                    {                                                                                               \
                        machine->code[ip - 1] = CMD_##name + 0x02;                                                  \
                    }                                                                                               \
                    *(type_array*)(machine->code + ip) = arg_var;                                                   \
                    ip += sizeof(type_array);                                                                       \
                    line_cmd = next_argument(line_cmd);                                                             \
                }                                                                                                   \
                else                                                                                                \
                {                                                                                                   \
                    arg_var = check_register(line_cmd);                                                             \
                                                                                                                    \
                    if (arg_var != WRONG_ARGUMENT)                                                                  \
                    {                                                                                               \
                        if (i == 0)                                                                                 \
                        {                                                                                           \
                            machine->code[ip - 1] = CMD_##name + 0x01;                                              \
                        }                                                                                           \
                        machine->code[ip++] = arg_var;                                                              \
                        line_cmd = next_argument(line_cmd);                                                         \
                    }                                                                                               \
                    else                                                                                            \
                    {                                                                                               \
                        if (sscanf(line_cmd, type_array_format, &arg_var) != 0)                                     \
                        {                                                                                           \
                            *(type_array*)(machine->code + ip) = arg_var;                                           \
                            ip += sizeof(type_array);                                                               \
                            line_cmd = next_argument(line_cmd);                                                     \
                        }                                                                                           \
                        else                                                                                        \
                        {                                                                                           \
                            arg_var = hash_jmp_pointer(line_cmd);                                                   \
                            if (arg_var != WRONG_ARGUMENT)                                                          \
                            {                                                                                       \
                                if (i == 0)                                                                         \
                                {                                                                                   \
                                    machine->code[ip - 1] = CMD_##name + 0x03;                                      \
                                }                                                                                   \
                                hash_ip = find_hash_name(&(machine->cmd_pointer), arg_var);                         \
                                *(int*)(machine->code + ip) = hash_ip;                                              \
                                ip += sizeof(int);                                                                  \
                                line_cmd = next_argument(line_cmd);                                                 \
                            }                                                                                       \
                            else                                                                                    \
                            {                                                                                       \
                                printf("Wrong argument of command!\n\n");                                           \
                                return WRONG_ARGUMENT;                                                              \
                            }                                                                                       \
                        }                                                                                           \
                    }                                                                                               \
                }                                                                                                   \
            }                                                                                                       \
        }                                                                                                           \
        else                                                                                                        \

 

 struct pointer_struct
 {
    long long hash = 0;
    int ip_cmd = -1;
 };


 struct hash_name
 {
    int count_of_pointer = 0;
    struct pointer_struct pointer_info[15] = {};
 };
                                                                  


struct asm_file
{
    char version;

    const char sign[4] = {'S', 'H', 'Z', '!'};

    int len_of_code = 0;

    char* code;

    struct hash_name cmd_pointer = {};
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

int check_RAM(char* start_of_line)
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

        sscanf(arg_str, "%d", &arg);

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

uint32_t dl_new_hash (const char* str, int len)
{
    uint32_t h = 5381;
    int i = 0;

    for (unsigned char c = *str; len > i; c = *++str) 
    {
        h = h * 33 + c;
        i++;
    }
    return h;
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

    assert(len_command <= 100);

    strncpy(str, start_line_of_command, len_command);

    str[len_command] = '\0';
    
}

int hash_jmp_pointer(char* cmd)
{
    assert(cmd != nullptr);
    
    char* jmp_name = nullptr; 
    
    if (cmd[0] == ':')
    {
        jmp_name = cmd + 1;                       //  :rar
        char* jmp_name_end = strchr(cmd, ' ');

        if (jmp_name_end == nullptr)
        {
            jmp_name_end = strchr(cmd, '\0');
        }

        int len_jmp_name = jmp_name_end - jmp_name;



       // char* tmp = (char*)calloc(len_jmp_name, sizeof(char));

       // strncpy(tmp, jmp_name, len_jmp_name);

       // uint32_t hash_word = dl_new_hash(tmp);

       // free(tmp);

        return dl_new_hash(jmp_name, len_jmp_name);
    }

    return WRONG_ARGUMENT;
}


int insert_hash_name(struct hash_name* cmd_pointer, uint32_t arg_var, int ip)
{
    assert(cmd_pointer != nullptr);

    cmd_pointer->pointer_info[cmd_pointer->count_of_pointer].hash = arg_var;
    cmd_pointer->pointer_info[cmd_pointer->count_of_pointer].ip_cmd = ip;
    cmd_pointer->count_of_pointer++;

    return OK;
}


int find_hash_name(struct hash_name* cmd_pointer, uint32_t arg_var)
{
    assert(cmd_pointer != nullptr);

    for (int i = 0; i < cmd_pointer->count_of_pointer; i++)
    {
        if (cmd_pointer->pointer_info[i].hash == arg_var)
        {
            return cmd_pointer->pointer_info[i].ip_cmd;
        }
    }
    return -1;
}


int compilation(struct Text* text_struct, struct asm_file* machine)
{
    assert(text_struct != nullptr);
    assert(machine != nullptr);
    
    char cmd[100] = "";
    char* line_cmd = nullptr;

    size_t count_of_space = 0;
    int len_cmd = 0;
    uint32_t arg_var = 0;
    int hash_ip = 0;

    int ip = 0;

    for (int j = 0; j < text_struct->quantity_lines; j++)
    {
        line_cmd = text_struct->linii[j].start_line;

        get_line_cmd(cmd, line_cmd);

        arg_var = hash_jmp_pointer(cmd);   //7c96fd0e

        if (arg_var != WRONG_ARGUMENT)
        {
            insert_hash_name( &(machine->cmd_pointer), arg_var, ip);
        }
        else
        {                        
            len_cmd = strlen(cmd);

            sscanf(line_cmd, " %n", &count_of_space);

            line_cmd += count_of_space + len_cmd;

            line_cmd = next_argument(line_cmd);

            //#define DEF_CMD_...

            #include "..\Commands.h" 

            #undef DEF_CMD_ 

            /*else*/ printf( "Error! Wrong command on %d line\n", j + 1 );
        }

    }

    machine->len_of_code = ip;

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

    compilation(&text_struct, &machine_code);

    compilation(&text_struct, &machine_code);

    FILE* ass = fopen("../assembler.bin", "wb");

    write_machine_code(&machine_code, ass); 

    fclose(ass);

    free_memory(&text_struct);

    //free структуру асм файла

    return EXIT;    
}
