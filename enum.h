
        

enum commands
{
    #define DEF_CMD_(number, name, arg, code) CMD_##name = number,

    #include "Commands.h"

    #undef DEF_CMD_
};

