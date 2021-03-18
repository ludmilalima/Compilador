/****************************************************/
/* File: bincode.c                                  */
/* Binary Code Generator implementation             */
/*  Aluna:                                          */
/*  Talita Ludmila de Lima    120895                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "util.h"
#include "codegenerate.h"
#include "assemblygenerate.h"
#include "bincode.h"


FILE *bincode; //arquivo com codigo de maquina
FILE *binario; //arquivo auxiliar com codigo em decimal para verificacao

//funcao que converte decimal em binario
int convertBin(int n, int bits){
    int c, k;
    
    for (c = bits-1; c >= 0; c--)
  {
    k = n >> c;

    if (k & 1)
      fprintf(bincode,"1");
    else
      fprintf(bincode,"0");
  }
  fprintf(binario,"%d\t",n);
}

//funcao que itera sobre a lista de quadruplas em assembly gerando o codigo de maquina
void getBin(quadModel *quad){

    quadModel *ptrQuad = quad;

    while (ptrQuad!=NULL)
    {
        if(ptrQuad->instrType == J)
        {
            
            if((ptrQuad->quadType.quadJ.op == j)||(ptrQuad->quadType.quadJ.op == jal))
                {
                    if(strcmp(ptrQuad->quadType.quadJ.name,"lab")==0)
                         ptrQuad->quadType.quadJ.target = searchLabel(ptrQuad->quadType.quadJ.target);
                    else if(ptrQuad->quadType.quadJ.target == -1)
                        ptrQuad->quadType.quadJ.target = searchFunEnd(ptrQuad->quadType.quadJ.name);
                    else 
                        ptrQuad->quadType.quadJ.target = searchFun(ptrQuad->quadType.quadJ.name);

                  //  fprintf(bincode,"op: %d name:%s target: %d \n", ptrQuad->quadType.quadJ.op, ptrQuad->quadType.quadJ.name, ptrQuad->quadType.quadJ.target);

                }
        }else if(ptrQuad->instrType == I)
        {
            if((ptrQuad->quadType.quadI.op == bne) || (ptrQuad->quadType.quadI.op == bne))
                ptrQuad->quadType.quadI.immediate = searchLabel(ptrQuad->quadType.quadI.immediate);
        }
        ptrQuad = ptrQuad->proxQuad;
    }

    ptrQuad = quad;    

    while(ptrQuad != NULL){

        switch (ptrQuad->instrType)
        {
        case R:
            convertBin(0,6);
            convertBin(ptrQuad->quadType.quadR.rs,5);
            convertBin(ptrQuad->quadType.quadR.rt,5);
            convertBin(ptrQuad->quadType.quadR.rd,5);
            convertBin(ptrQuad->quadType.quadR.shamt,5);
            convertBin(ptrQuad->quadType.quadR.op,6);
            fprintf(bincode, "\n");
            fprintf(binario,"\n");
        //    fprintf(bincode,"R    %d %d %d %d %d %d\n", 0, ptrQuad->quadType.quadR.rs, ptrQuad->quadType.quadR.rt, ptrQuad->quadType.quadR.rd, 0, ptrQuad->quadType.quadR.op);
            break;
        case J:
            if((ptrQuad->quadType.quadJ.op != fun_b)
                &&(ptrQuad->quadType.quadJ.op!=lab)
                &&(ptrQuad->quadType.quadJ.op!=fun_f)){
                    convertBin(ptrQuad->quadType.quadJ.op,6);
                    convertBin(ptrQuad->quadType.quadJ.target,26);
                    fprintf(bincode, "\n");
                    fprintf(binario,"\n");
                }
                 if((ptrQuad->quadType.quadJ.op != fun_b)
                    &&(ptrQuad->quadType.quadJ.op!=lab)
                    &&(ptrQuad->quadType.quadJ.op!=fun_f))
                   //     fprintf(bincode, "J    %d %d %s\n",ptrQuad->quadType.quadJ.op,ptrQuad->quadType.quadJ.target, ptrQuad->quadType.quadJ.name);
            break;

        case I:
            convertBin(ptrQuad->quadType.quadI.op,6);
            convertBin(ptrQuad->quadType.quadI.rs,5);
            convertBin(ptrQuad->quadType.quadI.rt,5);
            convertBin(ptrQuad->quadType.quadI.immediate,16);
            fprintf(bincode, "\n");
            fprintf(binario,"\n");
        //    fprintf(bincode, "I    %d %d %d %d\n", ptrQuad->quadType.quadI.op,ptrQuad->quadType.quadI.rs,ptrQuad->quadType.quadI.rt, ptrQuad->quadType.quadI.immediate);
            break;
        default:
            break;
        }
        ptrQuad = ptrQuad->proxQuad;
    }
}
