/****************************************************/
/* File: assemblygenerate.c                         */
/* Assembly Code Generator implementation           */
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

FILE *assembly;
int currLine = 0;

//funcão de criacão de quadrupla do tipo R
void createQuadR(int op, int rs, int rt, int rd, int shamt, int funct){
    quadModel *newQuad = (quadModel*)calloc(1, sizeof(quadModel));
    newQuad->instrType = R;
    newQuad->lineNo = currLine;
    newQuad->quadType.quadR.op = op;
    newQuad->quadType.quadR.rs = rs;
    newQuad->quadType.quadR.rt = rt;
    newQuad->quadType.quadR.rd = rd;
    newQuad->quadType.quadR.shamt = shamt;
    newQuad->quadType.quadR.func = funct;

    if(beginQuadList == NULL)
    {
        beginQuadList = newQuad;
        endQuadList = newQuad;
        endQuadList->proxQuad = NULL;
    }
    else
    {
        endQuadList->proxQuad = newQuad;
        endQuadList = newQuad;
    }
    currLine++;
}

//funcão de criacão de quadrupla do tipo I
void createQuadI(int op, int rs, int rt, int immediate){
    quadModel *newQuad = (quadModel*)calloc(1, sizeof(quadModel));
    newQuad->instrType = I;
    newQuad->lineNo = currLine;
    newQuad->quadType.quadI.op = op;
    newQuad->quadType.quadI.rs = rs;
    newQuad->quadType.quadI.rt = rt;
    newQuad->quadType.quadI.immediate = immediate;

    if(beginQuadList == NULL)
    {
        beginQuadList = newQuad;
        endQuadList = newQuad;
        endQuadList->proxQuad = NULL;
    }
    else
    {
        endQuadList->proxQuad = newQuad;
        endQuadList = newQuad;
    }
    currLine++;
}

//funcão de criacão de quadrupla do tipo J
void createQuadJ(int op, int value, char* name){
    quadModel *newQuad = (quadModel*)calloc(1, sizeof(quadModel));
    newQuad->instrType = J;
    newQuad->lineNo = currLine;
    newQuad->quadType.quadJ.op = op;
    newQuad->quadType.quadJ.target = value;
    if(name!=NULL)
        newQuad->quadType.quadJ.name = name;
    
    if(beginQuadList == NULL)
    {
        beginQuadList = newQuad;
        endQuadList = newQuad;
        endQuadList->proxQuad = NULL;
    }
    else
    {
        endQuadList->proxQuad = newQuad;
        endQuadList = newQuad;
    }
    currLine++;
}

//funcão que insere novo label
void releaseLabel(int label){
    labelNode *newLabel = (labelNode*)calloc(1,sizeof(labelNode));
    
    newLabel->labelName = label;
    newLabel->lineNo = currLine;

    if(beginLabelList == NULL)
    {
        beginLabelList = newLabel;
        endLabelList = newLabel;
        endLabelList->proxLabel = NULL;
    }else
    {
        endLabelList->proxLabel = newLabel;
        endLabelList = newLabel;
    }    
}

//funcão que retorna a linha da label
int searchLabel(int label){
    labelNode *searchLabel = beginLabelList;
    while ((searchLabel!=NULL)&&(searchLabel->labelName!=label))
    {
        searchLabel = searchLabel->proxLabel;
    }
    if(searchLabel!=NULL)
        return searchLabel->lineNo;
    else
        return (-1);
}

//funcão que insere um novo no de funcão
void releaseFun(char* fun){
    funNode *newFun = (funNode*)calloc(1,sizeof(funNode));
    
    newFun->funName = fun;
    newFun->lineNo = currLine;
    newFun->endlineNo = -1;

    if(beginFunList == NULL)
    {
        beginFunList = newFun;
        endFunList = newFun;
        endFunList->proxFun = NULL;
    }else
    {
        endFunList->proxFun = newFun;
        endFunList = newFun;
    }
          
}

void releaseFunEnd(char* fun){
    funNode *searchFun = beginFunList;
    while ((searchFun!=NULL)&&(strcmp(fun, searchFun->funName)!=0))
    {
        searchFun = searchFun->proxFun;
    }
    if(searchFun !=NULL)
        searchFun->endlineNo = currLine;
}

//funcão que retorna a linha da funcão
int searchFun(char* fun){
    funNode *searchFun = beginFunList;
    while ((searchFun!=NULL)&&(strcmp(fun, searchFun->funName)!=0))
    {
        searchFun = searchFun->proxFun;
    }
    if(searchFun!=NULL)
        return searchFun->lineNo;
    else
        return (-1);
}

//funcão que retorna a linha final da funcão
int searchFunEnd(char* fun){
    funNode *searchFun = beginFunList;
    while ((searchFun!=NULL)&&(strcmp(fun, searchFun->funName)!=0))
    {
        searchFun = searchFun->proxFun;
    }
    if(searchFun!=NULL)
        return searchFun->endlineNo;
    else
        return (-1);
}

//funcão que imprime quadrupla tipo R
void printQuadR(char* op, int rs, int rt, int rd, int shamt, int funct)
{
    fprintf(assembly,"(%s, %d, %d, %d, %d, %d)\n", op, rs, rt, rd, shamt, funct);
}

//funcão que imprime quadrupla tipo I
void printQuadI(char* op, int rs, int rt, int immediate)
{
    fprintf(assembly,"(%s, %d, %d, %d)\n", op, rs, rt, immediate);
}

//funcão que imprime quadrupla tipo J
void printQuadJ(char* op, int target)
{
    fprintf(assembly,"(%s, %d)\n", op, target);
}

//funcão que cria quadruplas de acordo com o operador op
void releaseQuadList(int op, int t1, int t2, int t3, char *name)
{
    switch (op)
    {
    case IF:
        createQuadI(bne,t1,t2,t3);
        printQuadI("bne", t1,t2,t3);
        
        break;

    case GOTO:
        if(name != NULL)
        {   
            createQuadJ(j, t3, name);
            printQuadJ("j",t3);
        }else
        {
            createQuadJ(j, t3, NULL);
            printQuadJ("j",t3);
        }
        
        break;

    case LAB:
        releaseLabel(t3);
        
        break;

    case WHILE:
        createQuadI(bne,t1,t2,t3);
        printQuadI("bne", t1,t2,t3);
        
        break;

    case ASSIGN:
        createQuadR(move,t1,0,t3,0,15);
        printQuadR("move",t1,0,t3,0,15);
        
        break;

    case STORE:
        createQuadI(store, t1,t2,t3);
        printQuadI("store", t1,t2,t3);
        
        break;

    case RETURN:
        createQuadR(move,t1,t2,t3,0,15);
        printQuadR("move",t1,t2,t3,0,15);
        
        break;
    case LOADI:
        createQuadI(loadi,0,t2,t3);
        printQuadI("loadi",0,t2,t3);
        
        break;

    case LOAD:
        createQuadI(load,t1,t2,t3);
        printQuadI("load",t1,t2,t3);
        
        break;

    case SOM:
        createQuadR(add,t1,t2,t3,0,2);
        printQuadR("add",t1,t2,t3,0,2);
        
        break;

    case CALL:
        if(strcmp(name, "input")==0){
            createQuadR(in,t1,t2,t3,0,0);
            printQuadR("in",t1,t2,t3,0,0);
            
            createQuadJ(nop,t3,name);
            printQuadJ("nop",t3);
        }else if(strcmp(name, "output")==0){
            createQuadR(out,t1,t2,t3,0,1);
            printQuadR("out",t1,t2,t3,0,1);

            createQuadJ(nop,0,name);
            printQuadJ("nop",0);
        }else{
            createQuadJ(jal,t3, name);
            printQuadJ("jal",t3);
        }
        
        break;

    case MUL:
        createQuadR(mul,t1,t2,t3,0,4);
        printQuadR("mul",t1,t2,t3,0,4);
        
        break;

    case DIV:
        createQuadR(divi,t1,t2,t3,0,5);
        printQuadR("div",t1,t2,t3,0,5);
        
        break;

    case MEN:
        createQuadR(slt,t1,t2,t3,0,6);
        printQuadR("slt",t1,t2,t3,0,6);
        
        break;

    case MAI:
        createQuadR(sgt,t1,t2,t3,0,7);
        printQuadR("sgt",t1,t2,t3,0,7);
        
        break;
 
    case IGL:
        createQuadR(set,t1,t2,t3,0,8);
        printQuadR("set",t1,t2,t3,0,8);
        
        break;

    case FUN:
        releaseFun(name);

        break;

    case FUN_END:
        releaseFunEnd(name);
        if(strcmp(name,"main")){
            releaseQuadList(LOAD,fp, ra,0, NULL);
            releaseQuadList(LOAD, fp, fp, -1, NULL);
            releaseQuadList(SUBi, sp, sp, t2, NULL);
            createQuadR(jr,t3,0,0,0,9);
            printQuadR("jr", t3,0,0,0,9);
        }
        break;

    case SUBi:
        createQuadI(subi,t1,t2,t3);
        printQuadI("subi",t1,t2,t3);
        
        break;

    case AND:
        createQuadR(and,t1,t2,t3,0,10);
        printQuadR("and",t1,t2,t3,0,10);
        
        break;  

    case OR:
        createQuadR(or,t1,t2,t3,0,11);
        printQuadR("or",t1,t2,t3,0,11);
        
        break;

    case MOD:
        createQuadR(mod,t1,t2,t3,0,12);
        printQuadR("mod",t1,t2,t3,0,12);
        
        break;

    case XOR:
        createQuadR(xor,t1,t2,t3,0,13);
        printQuadR("xor",t1,t2,t3,0,13);
        
        break;

    case NOT:
        createQuadR(not,t1,t2,t3,0,14);
        printQuadR("not",t1,t2,t3,0,14);
        
        break;

    case SLL:
        createQuadR(sll,t1,t2,t3,0,16);
        printQuadR("sll",t1,t2,t3,0,16);
        
        break;

    case SRL:
        createQuadR(srl,t1,t2,t3,0,17);
        printQuadR("srl",t1,t2,t3,0,17);
        
        break;

    case CLKADJ:
        createQuadJ(nop,0, name);
        printQuadJ("nop", 0);  
              
        break;

    case BEQ:
        createQuadI(beq,t1,t2,t3);
        printQuadI("beq", t1,t2,t3);
        

        break;
    case BNE:
        createQuadI(bne,t1,t2,t3);
        printQuadI("bne", t1,t2,t3);
        break;

    case LUI:
        createQuadI(lui,t1,t2,t3);
        printQuadI("lui", t1,t2,t3);
        break;

    case SUB:
        createQuadR(sub,t1,t2,t3,0,3);
        printQuadR("sub",t1,t2,t3,0,3);  
        break;

    case SOMi:
        createQuadI(addi,t1,t2,t3);
        printQuadI("addi",t1,t2,t3);  
        break;

    case DIF:
        createQuadR(sdt,t1,t2,t3,0,18);
        printQuadR("sdt",t1,t2,t3,0,18);  
        break;

    default:
        break;
    }
}
