#include "..\libr\Stack.h"
#include "..\enum.h"

#ifdef Int_t

#define DEF_CMD_(number, name, arg, code_func)            \
        case CMD_##name:                                  \
             code                                         \
             ip += 1 + arg * sizeof(type_array);          \
             break;                                       \

//исправить переход т.к. не все аргументы sizeof(type_array); прописать их в commands h
#endif

struct CPU
{
   type_array register_cpu[4] = {};

   type_array RAM[2000] = {};

   int len_of_code = 0;

   struct pstack_info stk = {};

   char* code;

};

int check_asm_file(FILE* ass)
{
   char sign[5] = {}; 

   fscanf(ass, "%c%c%c%c", &sign[0], &sign[1], &sign[2], &sign[3]);

   sign[4] = '\0';

   if (strcmp(sign, "SHZ!") != 0) 
   {
      printf("Wrong signature of file!\n");
      return 0;
   }   

   char version = '\0';

   fscanf(ass, "%c", &version);

   if (version != Version_of_programm) 
   {
      printf("You have the new version, swap assembler file\n");
      return 0;
   }

   return 1;
}

void input_code_from_file(FILE* ass, struct CPU* CPPU)
{
   fscanf(ass,"%d", &(CPPU->len_of_code));

   CPPU->code = (char*)calloc(CPPU->len_of_code, sizeof(char));

   fread(CPPU->code, sizeof(char), CPPU->len_of_code, ass);
}


int just_do_it(struct CPU* CPPU)
{
   int ip = 0;

   type_array argument = 0;

   while (CPPU->code[ip] != CMD_HLT)
   {
      switch(CPPU->code[ip])
      {
         #include "../Commands.h"
      }
   }
   printf("Program is completed.\n\n");
   return 0;
}

int main()
{
   struct CPU CPPU = {};

   stack_constructor(&(CPPU.stk), Size_of_stack);

   FILE* ass = fopen("../assembler.bin", "rb");

   if  ( !check_asm_file(ass) ) 
   {
      return 0;
   }

   input_code_from_file(ass, &CPPU);

   fclose(ass);

   just_do_it(&CPPU);

}