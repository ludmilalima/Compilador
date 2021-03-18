/****************************************************/
/* File: codegenerate.c                             */
/* Linear Intermediary Code Generator implementation*/
/*  Aluna:                                          */
/*  Talita Ludmila de Lima    120895                */
/****************************************************/

#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "symboltable.h"
#include "util.h"
#include "codegenerate.h"
#include "assemblygenerate.h"

FILE *intermed; //arquivo contendo codigo linearizado intermediario

//verifica se trata-se da leitura do primeiro nó da arvore
int isFirst = 0;
//gerencia os saltos em operacoes de desvio
int label=0;
//gerenciam os parametros quando funcoes sao chamadas
int parametro_on=0;
int parametros=0;
//gerencia a utilizacao dos registradores do processador
int reg_status[MAX_REG-1];

//funcao verifica qual o proximo registrador livre no processador
int proxRegLivre(){
    int i;
    for(i=1;i<=MAX_REG;i++){
        if(reg_status[i]==0){
            reg_status[i]=1;
            return i;
        }
    }
}

/* procedimento que:
    * insere o escopo da funcao main na lista de escopos
    * inicializa os registradores especiais de controle heap, stack e frame-pointer
 */
void insereMain(){
    Scope globalScope = st_lookup_scope("global");

    releaseQuadList(SOMi, 0, sp, globalScope->qtdeElementos, NULL);
    releaseQuadList(SOMi, 0, hp, MAX_MEM, NULL);
    releaseQuadList(SOMi, sp, fp, 0, NULL);p
    releaseQuadList(SOMi, 0, sp, 1, NULL);
    Scope mainScope = st_lookup_scope("main");
    releaseQuadList(SOMi, 0, sp, mainScope->qtdeElementos, NULL);

}

//funcao que implime o tipo do nó passado como argumento
char * printType(TreeNode* tree) {
  if (tree->child[0] != NULL) {
    switch (tree->child[0]->type) {
      case Integer:
        return "int";
        break;
      case Void:
        return "void";
        break;
      case IntegerArray:
        return "int array";
        break;
      default: 
        break;
    }
  } else {
    switch (tree->type) {
      case Integer:
        return "int";
        break;
      case Void:
        return "void";
        break;
      case IntegerArray:
        return "int array";
        break;
      default: 
        break;
    }
  }
}

/* procedimento que:
    * identifica a operação do nó;
    * imprime o código intermediario da operacao;
    * cria quadrupla relacionada
 */
void printNode(TreeNode * t){
    int loc_label;
    TreeNode *aux;

    switch(t->nodekind){
        case StmtK:
            switch(t->kind.stmt)
            {
                case IfK:
                    label=label+2;
                    loc_label=label;
                    if(t->child[0]!=NULL){
                        printCode(t->child[0]);
                        //verificar se trata-se de um vetor
                        if((t->child[0]->nodekind==ExpK)&&(t->child[0]->kind.exp==ArrIdK))
                        {
                            fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[0]->attr.name, t->child[0]->reg);
                            releaseQuadList(LOAD,t->child[0]->reg,t->child[0]->reg,0,NULL);
                        }
                    }

                    char *token = printToken(2, t->attr.type,0);
                    

                    fprintf(intermed,"(IF, _t%d, 0, L%d)\n", t->child[0]->reg,loc_label-2);
                    releaseQuadList(IF,t->child[0]->reg, 0, loc_label-2, NULL);

                    if(t->child[2]!=NULL) {   
                        printCode(t->child[2]);
                        //verificar se trata-se de um vetor
                        if((t->child[2]->nodekind==ExpK)&&(t->child[2]->kind.exp==ArrIdK))
                        {
                            fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[2]->attr.name, t->child[2]->reg);
                            releaseQuadList(LOAD,t->child[2]->reg,t->child[2]->reg,0,NULL);
                        }
                    }

                    

                    fprintf(intermed,"(GOTO, -, -, L%d)\n",loc_label-1);
                    releaseQuadList(GOTO,0,0,loc_label-1, "lab");

                    fprintf(intermed,"(LAB, -, -, %d)\n",loc_label-2);
                    releaseQuadList(LAB,0,0,loc_label-2, "lab");

                    if(t->child[1]!=NULL){
                        printCode(t->child[1]);
                        //verificar se trata-se de um vetor
                        if((t->child[1]->nodekind==ExpK)&&(t->child[1]->kind.exp==ArrIdK))
                        {
                            fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[1]->attr.name, t->child[1]->reg);
                            releaseQuadList(LOAD,t->child[1]->reg,t->child[1]->reg,0,NULL);
                        }
                    }

                    fprintf(intermed,"(GOTO, -, -, L%d)\n",loc_label-1);
                    releaseQuadList(GOTO,0,0,loc_label-1, "lab");

                    fprintf(intermed,"(LAB, -, -, %d)\n",loc_label-1);
                    releaseQuadList(LAB,0,0,loc_label-1, "lab");

                    //liberar registradores utilizados
                    if(t->child[0]!=NULL)
                        reg_status[t->child[0]->reg]=0;
                    if(t->child[1]!=NULL)
                        reg_status[t->child[0]->reg]=0;
                    if(t->child[2]!=NULL)
                        reg_status[t->child[0]->reg]=0;


                break;
                case WhileK:
                                       
                    label=label+3;
                    loc_label=label;

                    fprintf(intermed,"(LAB, -, -, L%d)\n",loc_label-3);
                    releaseQuadList(LAB,0,0,loc_label-3, "lab");

                    if(t->child[0]!=NULL){
                        printCode(t->child[0]);
                        //verificar se trata-se de um vetor
                        if((t->child[0]->nodekind==ExpK)&&(t->child[0]->kind.exp==ArrIdK))
                        {
                            fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[0]->attr.name, t->child[0]->reg);
                            releaseQuadList(LOAD,t->child[0]->reg,t->child[0]->reg,0,NULL);
                        }

                    }
                    fprintf(intermed,"(WHILE, _t%d, zero -, L%d)\n",t->child[0]->reg,loc_label-2);
                    releaseQuadList(WHILE,t->child[0]->reg, 0, loc_label-2, NULL);

                    fprintf(intermed,"(GOTO, -, -, L%d)\n",loc_label-1);
                    releaseQuadList(GOTO,0,0,loc_label-1, "lab");

                    fprintf(intermed,"(LAB, -, -, %d)\n",loc_label-2);
                    releaseQuadList(LAB,0,0,loc_label-2, "lab");
                    
                    if(t->child[1]!=NULL){
                        printCode(t->child[1]);
                        //verificar se trata-se de um vetor
                        if((t->child[1]->nodekind==ExpK)&&(t->child[1]->kind.exp==ArrIdK))
                        {
                            fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[1]->attr.name, t->child[1]->reg);
                            releaseQuadList(LOAD,t->child[1]->reg,t->child[1]->reg,0,NULL);
                        }
                    
                    }

                    
                    fprintf(intermed,"(GOTO, -, -, L%d)\n",loc_label-3);
                    releaseQuadList(GOTO,0,0,loc_label-3, "lab");

                    fprintf(intermed,"(LAB, -, -, %d)\n",loc_label-1);
                    releaseQuadList(LAB,0,0,loc_label-1, "lab");
                    
                    //liberar os registradores utilizados
                    if(t->child[0]!=NULL)
                        reg_status[t->child[0]->reg]=0;
                    if(t->child[1]!=NULL)
                        reg_status[t->child[0]->reg]=0;
                    
                        
                break;
                case AssignK:
                    if(t->child[0]!=NULL)
                        printCode(t->child[0]);
                    if(t->child[1]!=NULL)
                        printCode(t->child[1]);

                    if((t->child[1]->nodekind) == ExpK)
                        //verificar se trata-se de uma chamada de funcao/procedimento
                        if(t->child[1]->kind.exp == CallK){
                            fprintf(intermed,"(ASSIGN, rv, -, _t%d)\n", t->child[0]->reg);
                            releaseQuadList(ASSIGN, rv, 0, t->child[0]->reg, NULL);
                        //verificar se trata-se de um vetor
                        }else if(t->child[1]->kind.exp == ArrIdK){
                            //verificar se trata-se de um vetor
                            if((t->child[0]->nodekind == ExpK) && (t->child[0]->kind.exp == ArrIdK)){
                                fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[1]->attr.name, t->child[1]->reg);
                                releaseQuadList(LOAD,t->child[1]->reg,t->child[1]->reg,0,NULL);

                            }else{
                                fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[1]->attr.name, t->child[0]->reg);
                                releaseQuadList(LOAD,t->child[1]->reg,t->child[0]->reg,0,NULL);
                                reg_status[t->child[1]->reg]=0;
                            }
                        }else{
                            //verificar se trata-se de um vetor
                            if((t->child[0]->nodekind == ExpK) && (t->child[0]->kind.exp == ArrIdK)){
                                
                            }else{
                                fprintf(intermed,"(ASSIGN, _t%d, -, _t%d)\n", t->child[1]->reg, t->child[0]->reg);
                                releaseQuadList(ASSIGN, t->child[1]->reg, 0, t->child[0]->reg, NULL);
                                reg_status[t->child[1]->reg]=0;
                            }
                        }
                        // verificar se o escopo e global
                        if(st_isGlobal(t->child[0]->attr.name,t->scopeName)==1){
                            //verificar se trata-se de um vetor
                            if((t->child[0]->nodekind == ExpK) && (t->child[0]->kind.exp == ArrIdK)){
                              
                                fprintf(intermed,"(STORE, %s[i], _t%d, 0)\n", t->child[0]->attr.name, t->child[0]->reg);
                                releaseQuadList(STORE,t->child[0]->reg,t->child[1]->reg,0,NULL);
                                reg_status[t->child[1]->reg]=0;

                            }else{
                                fprintf(intermed,"(STORE, _t%d, gp, %d)\n", t->child[0]->reg, st_lookup_memPos(t->child[0]->attr.name,t->child[0]->scopeName));
                                releaseQuadList(STORE, 0, t->child[0]->reg, st_lookup_memPos(t->child[0]->attr.name,t->child[0]->scopeName), NULL);
                                reg_status[t->child[0]->reg]=0;
                            }
                        }else{
                            //verificar se trata-se de um vetor
                            if((t->child[0]->nodekind == ExpK) && (t->child[0]->kind.exp == ArrIdK)){
    
                                fprintf(intermed,"(STORE, %s[i], _t%d, 0)\n", t->child[0]->attr.name, t->child[0]->reg);
                                releaseQuadList(STORE,t->child[0]->reg,t->child[1]->reg,0,NULL);
                                reg_status[t->child[1]->reg]=0;

                            }else{
                                fprintf(intermed,"(STORE, _t%d, fp, %d)\n", t->child[0]->reg, st_lookup_memPos(t->child[0]->attr.name,t->child[0]->scopeName));
                                releaseQuadList(STORE,fp , t->child[0]->reg, st_lookup_memPos(t->child[0]->attr.name,t->child[0]->scopeName), NULL);
                                reg_status[t->child[0]->reg]=0;
                            }
                        }
                    //liberar os registradores utilizados
                    if(t->child[0]!=NULL)
                        reg_status[t->child[0]->reg]=0;
                    if(t->child[1]!=NULL)
                        reg_status[t->child[1]->reg]=0;
                    
                        
                break;
                case CompoundK:
                    if(t->child[0]!=NULL)
                        printCode(t->child[0]);
                    if(t->child[1]!=NULL)
                        printCode(t->child[1]);


                break;
                case ReturnK:
                    if(t->child[0]==NULL)
                    {
                    }else
                    {
                        printCode(t->child[0]);
                        //verificar se trata-se de um vetor
                        if((t->child[0]->nodekind==ExpK)&&(t->child[0]->kind.exp==ArrIdK))
                    {
                        fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[0]->attr.name, t->child[0]->reg);
                        releaseQuadList(LOAD,t->child[0]->reg,t->child[0]->reg,0,NULL);
                    }
                    
                        fprintf(intermed,"(RETURN, _t%d, -, rv)\n", t->child[0]->reg);
                        releaseQuadList(RETURN, t->child[0]->reg,0, rv, NULL);
                        //desviar para a instrucao de chamada
                        releaseQuadList(GOTO,0,0,-1,t->scopeName);
                        //liberar registrador utilizado
                        reg_status[t->child[0]->reg]=0;
                    }
                    
                        
                break;
                default:
                break;
            }
        break;
        case ExpK:
            switch(t->kind.exp)
            {
                case OpK:
                
                break;
                case ConstK:
                    t->reg=proxRegLivre();

                    fprintf(intermed,"(LOADI, - , _t%d, %d)\n",t->reg, t->attr.val);
                    releaseQuadList(LOADI,0,t->reg,t->attr.val, NULL);
                        
                break;
                case IdK:
                    t->reg=proxRegLivre();
                    //verificar se o escopo é global
                    if(st_isGlobal(t->attr.name,t->scopeName) == 1){
                        fprintf(intermed,"(LOAD, %s, _t%d, %d)\n",t->attr.name, t->reg, (st_lookup_memPos(t->attr.name,"global")-1));
                        releaseQuadList(LOAD, 0, t->reg, st_lookup_memPos(t->attr.name,t->scopeName), NULL);
                    }
                    else{
                        fprintf(intermed,"(LOAD, %s, _t%d, %d)\n",t->attr.name, t->reg, st_lookup_memPos(t->attr.name,t->scopeName));
                        releaseQuadList(LOAD, fp, t->reg, st_lookup_memPos(t->attr.name,t->scopeName), NULL);
                    }
                        
                    
                        
                break;
                case TypeK:

                break;
                case ArrIdK:
                    if(t->child[0]!=NULL)
                        printCode(t->child[0]);

                    // refazer o calculo de indice considerando heap
                    int heapExactPos = proxRegLivre();
                    int heapBeginPos = proxRegLivre();
                    t->reg=heapExactPos;

                    if(st_isGlobal(t->attr.name,t->scopeName)==1){
                       
                        fprintf(intermed,"(LOAD, gp, _t%d, %d)\n", heapBeginPos, (st_lookup_memPos(t->attr.name, "global")-1));
                        releaseQuadList(LOAD,0,heapBeginPos,(st_lookup_memPos(t->attr.name, "global")-1),NULL);

                        fprintf(intermed,"(SOM, _t%d, _t%d, _t%d)\n", heapBeginPos, t->child[0]->reg, heapExactPos);
                        releaseQuadList(SOM,heapBeginPos, t->child[0]->reg, heapExactPos, NULL);
                        //liberar registradores utilizados
                        reg_status[heapBeginPos]=0;
                        reg_status[ t->child[0]->reg]=0;

                    }else{
                        fprintf(intermed,"(LOAD, fp, _t%d, %d)\n", heapBeginPos, st_lookup_memPos(t->attr.name, t->scopeName));
                        releaseQuadList(LOAD,fp,heapBeginPos,st_lookup_memPos(t->attr.name, t->scopeName),NULL);

                        fprintf(intermed,"(SOM, _t%d, _t%d, _t%d)\n", heapBeginPos, t->child[0]->reg, heapExactPos);
                        releaseQuadList(SOM,heapBeginPos, t->child[0]->reg, heapExactPos, NULL);
                        //liberar registradores utilizados
                        reg_status[heapBeginPos]=0;
                        reg_status[ t->child[0]->reg]=0;

                    }
   
                break;
                case CallK:
                    //imprimir os parametros antes
                    if(t->child[0]!=NULL)
                    {
                        parametro_on=1;
                        printCode(t->child[0]);
                        parametros++;
                    }
                    //retomar o parametro inicial
                    aux=t->child[0];
                    //verificar se e funcao especial input ou output
                    if((strcmp(t->attr.name,"output")!=0)&&(strcmp(t->attr.name,"input")!=0)){

                 //armazena posicão fp em fp_velho 
                    releaseQuadList(STORE,fp,sp,0, NULL);

                 //incrementa sp
                    releaseQuadList(SOMi, 0,sp, 1, NULL);

                 //armazena a linha da instrucão de retorno

                 // atualiza fp com o valor de st 
                    releaseQuadList(SOMi, sp, fp, 0, NULL);

                 //incrementa sp
                    releaseQuadList(SOMi, 0, sp, 1, NULL);

                    Scope newScope = st_lookup_scope(t->scopeName);

                //incrementa sp com parametros e variaveis do registro de ativacão
                    releaseQuadList(SOMi, 0, sp, newScope->qtdeElementos, NULL);
                    
                    int i = 1;
                    //imprimir e carregar os parametros nos registradores
                    while(aux!=NULL){
                        fprintf(intermed,"(PAR, -, -, _t%d)\n", aux->reg);
                        //verificar se trata-se de um vetor
                        if((aux->nodekind==ExpK)&&(aux->kind.exp==ArrIdK))
                        {
                            fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", aux->attr.name, aux->reg);
                            releaseQuadList(LOAD,aux->reg,aux->reg,0,NULL);
                        }
                        releaseQuadList(STORE,fp, aux->reg,i, NULL);
                        aux=aux->sibling;
                        i++;
                    }
                    //liberar registrador utilizado
                    t->reg=proxRegLivre();

                    fprintf(intermed,"(CALL, %s, %d, _t%d)\n",t->attr.name, parametros, t->reg);
                    releaseQuadList(CALL,t->reg,t->reg,t->reg,t->attr.name);
                    //caso de chamada de output
                    }else if((strcmp(t->attr.name,"output")) == 0){ //definir output

                        if((t->child[0]->nodekind==ExpK)&&(t->child[0]->kind.exp==ArrIdK))
                        {
                            fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[0]->attr.name, t->child[0]->reg);
                            releaseQuadList(LOAD,t->child[0]->reg,t->child[0]->reg,0,NULL);
                        }

                        fprintf(intermed,"(ASSIGN, _t%d, -, %d)\n",t->child[0]->reg,rv);
                        releaseQuadList(ASSIGN, t->child[0]->reg,0,rv, NULL);

                        fprintf(intermed,"(CALL, %s, %d, -, -)\n",t->attr.name, rv);
                        releaseQuadList(CALL,rv,0,0,t->attr.name);
                    //caso de chamada de input
                    }else if((strcmp(t->attr.name,"input")) == 0){ //definir input
                        
                        t->reg=rv;

                        fprintf(intermed,"(CALL, %s, %d, -)\n",t->attr.name, t->reg);
                        releaseQuadList(CALL,0,rv,0,t->attr.name);
                    }

                    parametros = 0;
                    //liberar registradores utilizados
                    aux=t->child[0];
                    while(aux!=NULL){
                        reg_status[aux->reg]=0;
                        aux=aux->sibling;
                    }
                      
                break;
                case CalcK:
                   //imprimir nós filhos anteriormente
                    if(t->child[0]!=NULL){
                        printCode(t->child[0]);
                        //verificar se trata-se de um vetor
                        if((t->child[0]->nodekind==ExpK)&&(t->child[0]->kind.exp==ArrIdK))
                        {
                            fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[0]->attr.name, t->child[0]->reg);
                            releaseQuadList(LOAD,t->child[0]->reg,t->child[0]->reg,0,NULL);
                        }
                    }
                    
                    if(t->child[2]!=NULL){
                        printCode(t->child[2]);
                    //verificar se trata-se de um vetor
                    if((t->child[2]->nodekind==ExpK)&&(t->child[2]->kind.exp==ArrIdK))
                    {
                        fprintf(intermed,"(LOAD, %s[i], _t%d, 0)\n", t->child[2]->attr.name, t->child[2]->reg);
                        releaseQuadList(LOAD,t->child[2]->reg,t->child[2]->reg,0,NULL);
                    }
                    }
                    t->reg=proxRegLivre();
                    char *token = printToken(2,t->child[1]->attr.op, NULL);
                    if(t->child[1]->attr.op == IME){
                        int a = proxRegLivre();
                        int b = proxRegLivre();
                        
                        fprintf(intermed,"(%s, _t%d, _t%d, _t%d)\n", "MEN" ,t->child[0]->reg,t->child[2]->reg, a);
                        releaseQuadList(MEN, t->child[0]->reg,t->child[2]->reg, a, NULL); 

                        fprintf(intermed,"(%s, _t%d, _t%d, _t%d)\n", "IGL" ,t->child[0]->reg,t->child[2]->reg, b);
                        releaseQuadList(IGL, t->child[0]->reg,t->child[2]->reg, b, NULL); 

                        fprintf(intermed,"(%s, _t%d, _t%d, _t%d)\n", "OR" ,a,b, t->reg);
                        releaseQuadList(OR, a,b, t->reg, NULL); 
                        //liberar registradores utilizados
                        reg_status[a]=0;
                        reg_status[b]=0;

                    }else if(t->child[1]->attr.op == IMA){
                        int a = proxRegLivre();
                        int b = proxRegLivre();
                        
                        fprintf(intermed,"(%s, _t%d, _t%d, _t%d)\n", "MAI" ,t->child[0]->reg,t->child[2]->reg, a);
                        releaseQuadList(MAI, t->child[0]->reg,t->child[2]->reg, a, NULL); 

                        fprintf(intermed,"(%s, _t%d, _t%d, _t%d)\n", "IGL" ,t->child[0]->reg,t->child[2]->reg, b);
                        releaseQuadList(IGL, t->child[0]->reg,t->child[2]->reg, b, NULL); 

                        fprintf(intermed,"(%s, _t%d, _t%d, _t%d)\n", "OR" ,a,b, t->reg);
                        releaseQuadList(OR, a,b, t->reg, NULL);
                        //liberar registradores utilizados
                        reg_status[a]=0;
                        reg_status[b]=0;

                    }else{
                    fprintf(intermed,"(%s, _t%d, _t%d, _t%d)\n", token ,t->child[0]->reg,t->child[2]->reg, t->reg);
                    releaseQuadList(t->child[1]->attr.op, t->child[0]->reg,t->child[2]->reg, t->reg, NULL);
                    }
                    //liberar registradores utilizados
                    if(t->child[0]!=NULL)
                        reg_status[t->child[0]->reg]=0;
                    if(t->child[2]!=NULL)
                        reg_status[t->child[2]->reg]=0;

                break;
                default:
                break;
            }
        break;
        case DeclK:
            switch(t->kind.stmt)
            {
                case VarK:
                    fprintf(intermed,"(VAR, %s, -, %s)\n",t->attr.name, printType(t));
                            
                break;
                case FunK:
                    fprintf(intermed,"(FUN, %s, -, %s)\n",t->attr.name, printType(t));
                    releaseQuadList(FUN, 0, 0, 0, t->attr.name);

                //armazena o endereco de retorno em fp
                    releaseQuadList(STORE, fp, 31, 0, NULL);

                    if(t->child[1]!=NULL)    
                        printCode(t->child[1]);
                    if(t->child[2]!=NULL)    
                        printCode(t->child[2]);

                //jump register para o endereco de retorno (antigo ra)
                    releaseQuadList(FUN_END, 0,(st_lookup_scope(t->scopeName)->qtdeElementos+2),ra, t->attr.name);

                        
                break;
                case ArrVarK: // alocar espaco no heap e salvar a posicão do heap no registro de ativacão
                    fprintf(intermed,"(VAR_VET, *%s, %d, %s) \n", t->attr.arr.name,t->attr.arr.size, printType(t));
                    if(strcmp(t->scopeName,"global")==0){
                        releaseQuadList(SUBi, hp,hp,t->attr.arr.size, NULL);
                        releaseQuadList(STORE, 0, hp, (st_lookup_memPos(t->attr.arr.name, "global")-1),NULL);

                    }else{
                        releaseQuadList(SUBi, hp,hp,t->attr.arr.size, NULL);
                        releaseQuadList(STORE, fp, hp, st_lookup_memPos(t->attr.arr.name, t->scopeName),NULL);
                    }
                        
                break;
                case ArrParamK:
                    fprintf(intermed,"(PAR_VET, * %s, -, %s)\n", t->attr.name, printType(t));
                    
                        
                break;
                case ParamK:
                    //verificar a ecistencia de argumentos
                    if(t->attr.name!=NULL)
                    fprintf(intermed,"(PAR, %s, -, %s)\n", t->attr.name, printType(t));
                    
                        
                break;
                default:
                break;
            }
        break;
        default:
        break;
    }
}

//procedimento que itera sobre os nós e irmaos dos nós da arvore sintatica
void printCode(TreeNode *t){
    TreeNode *irmaos;
    switch(t->nodekind){
        case StmtK:
            switch(t->kind.stmt)
            {
                case IfK:
                    printNode(t);

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;

                break;
                case WhileK:
                    printNode(t);
                    
                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;

                break;
                case AssignK:
                    printNode(t);
                        
                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;

                break;
                case CompoundK:
                    printNode(t);

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;

                break;
                case ReturnK:
                    printNode(t);                    
                        
                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;

                break;
                default:
                break;
            }
        break;
        case ExpK:
            switch(t->kind.exp)
            {
                case OpK:
                    //printNode(t);

                break;
                case ConstK:                 
                    printNode(t);   

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;

                break;
                case IdK:            
                    printNode(t);    

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;

                break;
                case TypeK:
                    //printNode(t);

                break;
                case ArrIdK:
                    printNode(t);   

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;               
                        
                break;
                case CallK:
                    printNode(t);

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0; 

                break;
                case CalcK:
                    printNode(t);  

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;               
                        
                break;
                default:
                break;
            }
        break;
        case DeclK:
            switch(t->kind.stmt)
            {
                case VarK:
                    if(isFirst == 0)
                        insereMain();
                    printNode(t);

                    if(isFirst == 0){
                    TreeNode *new;

                    for(new = t; new->sibling != NULL; new = new->sibling){
                         while((new->sibling!=NULL)&&((new->sibling->kind.stmt == VarK)||(new->sibling->kind.stmt == ArrVarK))){
                            printNode(new->sibling);
                            new->sibling = new->sibling->sibling;
                        }
                    }
                      //  fprintf(intermed,"(GOTO, -, -, main)\n");
                        releaseQuadList(GOTO, 0, 0, 0, "main");
                        isFirst = 1;
                    }

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;          
                            
                break;
                case FunK:

                    if(isFirst == 0){
                    TreeNode *new;

                    for(new = t; new->sibling != NULL; new = new->sibling){
                         while((new->sibling!=NULL)&&((new->sibling->kind.stmt == VarK)||(new->sibling->kind.stmt == ArrVarK))){
                            printNode(new->sibling);
                            new->sibling = new->sibling->sibling;
                        }
                    }
                       if(isFirst == 0) 
                            insereMain();
                    //    fprintf(intermed,"(GOTO, -, -, main)\n");
                        releaseQuadList(GOTO, 0, 0, 0, "main");
                        isFirst = 1;
                    }
                    
                    printNode(t);

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;          
                         
                break;
                case ArrVarK:
                    if(isFirst == 0)
                        insereMain();
                    printNode(t);

                    if(isFirst == 0){
                    TreeNode *new;

                    for(new = t; new->sibling != NULL; new = new->sibling){
                        while((new->sibling!=NULL)&&((new->sibling->kind.stmt == VarK)||(new->sibling->kind.stmt == ArrVarK))){
                            printNode(new->sibling);
                            new->sibling = new->sibling->sibling;
                        }
                    }
            
                     //   fprintf(intermed,"(GOTO, -, -, main)\n");
                        releaseQuadList(GOTO, 0, 0, 0, "main");
                        isFirst = 1;
                    }

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;          
                        
                break;
                case ArrParamK:
                    printNode(t);

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;          
                        
                break;
                case ParamK:
                    printNode(t);

                    irmaos=t->sibling;
                    while(irmaos!=NULL){
                        if(parametro_on)
                            parametros++;
                        printNode(irmaos);
                        irmaos=irmaos->sibling;
                    }
                    parametro_on=0;          
                        
                break;
                default:
                break;
            }
        break;
        default:
        break;
    }
}