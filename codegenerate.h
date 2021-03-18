/****************************************************/
/* File: codegenerate.c                             */
/* Linear Intermediary Code Generator implementation*/
/*  Aluna:                                          */
/*  Talita Ludmila de Lima    120895                */
/****************************************************/

#ifndef _CODEGENERATE_H_
#define _CODEGENERATE_H_

#include "globals.h"

//lista dos mnemonicos das operacoes assembly
typedef enum ausente_LEX {GOTO, LAB, ASSIGN, STORE, LOADI, LOAD, FUN, VAR,
             VAR_VET, SOMi, PAR_VET, PAR, CALL, FUN_END, SUBi,
             AND, OR, MOD, XOR, NOT, SLL, SRL, CLKADJ, BEQ,
             BNE, LUI, SET} op;

void printNode(TreeNode *tree);

char * printType(TreeNode *tree);

void printCode(TreeNode *tree);

#endif