// -*- C++ -*-

#include "waitpost.h"

void mvs_post(uint32_t* ecb) 
{
  __asm(" l 2,0(%0)            get contents of ECB \n "
        " n 2,=X'3fffffff'     clear wait/post bits \n"
        " l 3,=x'40000000'     load post bit (and post code 0) \n"
        " cs 2,3,0(%0)         try to fast post \n"
        " jz postdone          successful? great \n"
        " ltr 2,2              anyone waiting? \n"
        " jm dopost            if so, post them \n"
        " n 2,=x'40000000'     did someone post it? \n"
        " jnz postdone         if so, we don't have to \n"
        "dopost ds 0h          \n"
        " post (%0),0          post the ECB \n"
        "postdone ds 0h        \n"
        "  lr 0,0              need a noop\n"
        : 
        : "a"(ecb)
        : "r0 r1 r2 r3");
}

void mvs_wait(uint32_t* ecb)
{
  __asm(" tm 0(%0),x'40'        already posted?"
        " jo nowait \n"
        " wait ecb=(%0)         wait for event \n"
        "nowait ds 0h \n"
        " lr 0,0                nop \n"
        : 
        : "a"(ecb)
        : "r0 r1");
}

void mvs_direct_post(uint32_t* ecb) 
{
  __asm(" post (%0),0          post the ECB \n"
        : 
        : "a"(ecb)
        : "r0 r1 r2 r3");
}

void mvs_direct_wait(uint32_t* ecb)
{
  __asm(" wait ecb=(%0)         wait for event \n"
        : 
        : "a"(ecb)
        : "r0 r1");
}

