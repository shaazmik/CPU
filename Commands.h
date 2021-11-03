
DEF_CMD_(0x11, PUSH, 1, 
{   
    type_array argument = 0;

    argument = *(type_array*)( &(CPPU->code[ip + 1]) ); 

    stack_pushka(&(CPPU->stk), argument);

    ip += 1 + 1 * sizeof(type_array);
}
)

DEF_CMD_(0x12, PUSH_X, 1, 
{   
    int mem_cell_num = 0;

    mem_cell_num = CPPU->code[ip + 1];

    stack_pushka( &(CPPU->stk), CPPU->register_cpu[mem_cell_num]  );

    ip += 1 + 1 * sizeof(char);
} 
)

DEF_CMD_(0x13, PUSH_MEM, 1,
{   
    int mem_cell_num = 0;

    mem_cell_num = *(int*)( &(CPPU->code[ip + 1]) );

    stack_pushka( &(CPPU->stk), CPPU->RAM[mem_cell_num] );

    ip += 1 + 1 * sizeof(int);
}
)


DEF_CMD_(0x21, POP, 1, 
{
    stack_popka(&(CPPU->stk));
    ip += 1 + 1 * sizeof(type_array);
}
)


DEF_CMD_(0x22, POP_X, 1, 
{
    int mem_cell_num = 0;

    mem_cell_num = CPPU->code[ip + 1];

    CPPU->register_cpu[mem_cell_num] = stack_popka( &(CPPU->stk) );

    ip += 1 + 1 * sizeof(char);
}
)

DEF_CMD_(0x23, POP_MEM, 1, 
{
    int mem_cell_num = 0;

    mem_cell_num = *(int*)( &(CPPU->code[ip + 1]) ); 

    CPPU->RAM[mem_cell_num] = stack_popka( &(CPPU->stk) );

    ip += 1 + 1 * sizeof(int);

}
)

DEF_CMD_(0x06, ADD, 0,
{  
    stack_pushka( &(CPPU->stk), stack_popka( &(CPPU->stk) ) + stack_popka( &(CPPU->stk) ) );
    ip += 1;
}
)


DEF_CMD_(0x07, SUB, 0,
{   
    type_array arg1 = stack_popka( &(CPPU->stk));
    type_array arg2 = stack_popka( &(CPPU->stk));

    stack_pushka( &(CPPU->stk),  arg2 - arg1);
    ip += 1;
}
)


DEF_CMD_(0x08, MUL, 0,
{
    stack_pushka( &(CPPU->stk), stack_popka( &(CPPU->stk) ) * stack_popka( &(CPPU->stk) ) );
    ip += 1;
}
)


DEF_CMD_(0x09, DIV, 0, 
{
    type_array arg1 = stack_popka(&(CPPU->stk));
    type_array arg2 = stack_popka(&(CPPU->stk));

    stack_pushka (&(CPPU->stk), ( arg2 / arg1));
    ip += 1;
}
)


DEF_CMD_(0x16, SHOW, 0, 
{
    printf(type_array_format, stack_popka( &(CPPU->stk) ) ); 
    printf("\n");
    ip += 1;
}
)

DEF_CMD_(0x17, IN, 0,
{

}
)

DEF_CMD_(0x0A, JMP, 1, 
{
    int jmp_ip = *(int*)( &(CPPU->code[ip + 1]) );
    ip = jmp_ip;
}
)

DEF_CMD_(0x0B, JMP_X, 1, 
{
    int jmp_ip = CPPU->code[ip + 1];
    ip = CPPU->register_cpu[jmp_ip];
}
)

DEF_CMD_(0x0C, JMP_MEM, 1, 
{
    int jmp_ip = *(int*)( &(CPPU->code[ip + 1]) );
    ip = CPPU->RAM[jmp_ip];
}
)

DEF_CMD_(0x0D, JMP_POINTER, 1, 
{
    int jmp_ip = *(int*)( &(CPPU->code[ip + 1]) );
    ip = jmp_ip;
}
)


DEF_CMD_(0xDA, CALL, 1,
{
    printf("KEKKKK");
}
)

DEF_CMD_(0xAD, RET, 0,
{
    printf("KKKKK");
}
)

DEF_CMD_(0x1F, HLT, 0, 
{
    printf("The program is completed\n\n.");
}
)