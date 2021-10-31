DEF_CMD_(0x11, PUSH, 1, 
{

    #ifdef Int_t

    stack_pushka(&(CPPU->stk), *(type_array*)(&(CPPU->code[ip + 1]) ) );

    #endif

    #ifdef Double_t

    #endif
}
)

DEF_CMD_(0x12, PUSH_X, 1, 
{
    printf("de\n");
} 
)

DEF_CMD_(0x13, PUSH_MEM, 1,
{
    printf("mem\n");
})


DEF_CMD_(0x21, POP, 1, 
{
    stack_popka(&(CPPU->stk));
}
)


DEF_CMD_(0x22, POP_X, 1, 
{
    stack_popka(&(CPPU->stk));
}
)

DEF_CMD_(0x23, POP_MEM, 1, 
{
    stack_popka(&(CPPU->stk));
}
)

DEF_CMD_(0x07, SHOW, 0, 
{
    #ifdef Int_t
    printf("%d\n",stack_popka( &(CPPU->stk) ) );
    #endif

    #ifdef Double_t
    printf("%.14lf\n", stack_popka( &(CPPU->stk) ) );
    #endif
}
)


DEF_CMD_(0x0F, HLT, 0, 
{
    printf("The program is completed\n\n.");
    
}
)