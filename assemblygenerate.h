/****************************************************/
/* File: assemblygenerate.c                         */
/* Assembly Code Generator implementation           */
/*  Aluna:                                          */
/*  Talita Ludmila de Lima    120895                */
/****************************************************/

#ifndef _ASSEMBLYGENERATE_H_
#define _ASSEMBLYGENERATE_H_

//define os nomes das operacoes em assembly
typedef enum tipoR {in, out, add, sub, mul, divi, slt, sgt, set,
            jr, and, or, mod, xor, not, move, sll, srl, sdt}operation;

typedef enum tipoIeJ{r, nop, j, jal, load, store, addi, subi, beq,
                    bne, loadi, lui, lab, fun_b, fun_f}operation2;

#define ra 31
#define fp 30
#define sp 29 
#define hp 28
#define rv 27
#define zero 0

//define os tipos de instrucoes
typedef enum{R, I, J}instructType;

//define estrutura da instrucão tipo R
typedef struct
{
    operation op;
    int rs;
    int rt;
    int rd;
    int shamt;
    int func;
}rType;

//define estrutura da instrucão tipo I
typedef struct
{
    operation2 op;
    int rs;
    int rt;
    int immediate;
}iType;

//define estrutura da instrucão tipo J
typedef struct
{
    operation2 op;
    int target;
    char *name;
}jType;

//define estrutura da quadrupla
typedef struct quadModel
{
    instructType instrType;
    union
    {
        rType quadR; 
        iType quadI;
        jType quadJ;
    }quadType;
    struct quadModel *proxQuad;
    int lineNo;
}quadModel;

//define estrutura do no da lista de labels
typedef struct labelNode
{
    int labelName;
    int lineNo;
    struct labelNode *proxLabel;
}labelNode;

//define estrutura do no da lista de funcoes
typedef struct funNode
{
    char* funName;
    int lineNo;
    int endlineNo;
    struct funNode *proxFun;
}funNode;

//armazena inicio e fim da lista de quadruplas
quadModel *beginQuadList;
quadModel *endQuadList;

//armazena inicio e fim da lista de linhas de labels
labelNode *beginLabelList;
labelNode *endLabelList;

//armazena inicio e fim da lista de linhas de funcoes
funNode *beginFunList;
funNode *endFunList;

void createQuadR(int op, int rs, int rt, int rd, int shamt, int funct);
void createQuadI(int op, int rs, int rt, int immediate);
void createQuadJ(int op, int value, char*name);

void releaseLabel(int label);
int searchLabel(int label);

void releaseFun(char* fun);
void releaseFunEnd(char* fun);
int searchFun(char* fun);
int searchFunEnd(char* fun);

void releaseQuadList(int op, int t1, int t2, int t3, char *name);

void printQuadR(char* op, int rs, int rt, int rd, int shamt, int funct);
void printQuadI(char* op, int rs, int rt, int immediate);
void printQuadJ(char* op, int target);

#endif // ASSEMBLYGENERATE_H