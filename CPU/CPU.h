#ifndef CPU_H
#define CPU_H

#include "..\libr\Stack.h"
#include "..\enum.h"

const int Max_memory = 4000;

struct CPU
{
   type_array register_cpu[4] = {};

   type_array RAM[Max_memory] = {};

   int len_of_code = 0;

   struct pstack_info stk = {};

   struct pstack_info stk_back = {};

   char* code = nullptr;

};


int check_asm_file(FILE* ass);


void input_code_from_file(FILE* ass, struct CPU* CPPU);


int srav(type_array arg1, type_array arg2);


#endif