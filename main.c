/****************************************************/
/* File: main.c                                     */
/* Main program for TINY compiler                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symboltable.h"
#include "util.h"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code */
#define NO_CODE FALSE

/* set NO_ASSEMBLY to TRUE to get a compiler that does not
generate assembly-code */
#define NO_ASSEMBLY FALSE

/* set NO_BINARY to TRUE to get a compiler that does not
generate assembly-code */
#define NO_BINARY FALSE

#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#endif
#if !NO_ANALYZE
#include "analyze.h"
#endif
#if !NO_CODE
#include "codegenerate.h"
#endif
#if !NO_ASSEMBLY
#include "assemblygenerate.h"
#endif
#if !NO_BINARY
#include "bincode.h"
#endif

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * intermed;
FILE * errorfile;
FILE * assembly;
FILE * bincode;
FILE * binario;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = FALSE;

int Error = FALSE;

Scope globalScope;

int main( int argc, char * argv[] )
{ TreeNode * syntaxTree;
  globalScope = (Scope) malloc(sizeof(Scope));
  globalScope = newScope("global", Void);
  
  char pgm[120]; /* source code file name */
  if (argc != 2)
    { fprintf(stderr,"usage: %s <filename>\n",argv[0]);
      exit(1);
    }
  strcpy(pgm,argv[1]) ;
  if (strchr (pgm, '.') == NULL)
     strcat(pgm,".cm");
  source = fopen(pgm,"r");
  if (source==NULL)
  { fprintf(stderr,"File %s not found\n",pgm);
    exit(1);
  }
  
  errorfile = fopen("errorfile.txt", "w");
  if(errorfile == NULL){
    fprintf(stderr, "Could not open file to print errors.\n");
  }
  fprintf(errorfile, "\\----------------------------------\\\n");
  fprintf(errorfile, "ERROS SINTATICOS DO ARQUIVO %s:\n\n", pgm);
  
  listing = fopen("output.txt", "w"); //Print syntax tree and symbol table to file output.txt
  if(listing == NULL){
    fprintf(stderr, "Could not open output file to print.\n");
  }
  fprintf(listing,"\nCompilacao C-: %s\n",pgm);

  intermed = fopen("intermediario.txt", "w");
  if(intermed == NULL){
    fprintf(stderr, "Could not open intermediate code file");
  }

  assembly = fopen("assembly.txt", "w");
  if(assembly == NULL){
    fprintf(stderr, "Could not open intermediate code file");
  }

  bincode = fopen("programa.txt", "w");
  if(bincode == NULL){
    fprintf(stderr, "Could not open intermediate code file");
  }

  binario = fopen("binario.txt", "w");

#if NO_PARSE
  while (getToken()!=ENDFILE);
#else
  syntaxTree = parse();
  if (TraceParse) {
    fprintf(listing,"\nArvore Sintatica:\n");
    printTree(syntaxTree);
  }
#if !NO_ANALYZE
  if (! Error)
  { 
    fprintf(errorfile, "  ERROS SEMANTICOS DO ARQUIVO %s:\n", pgm);
    fprintf(errorfile, "\\---------------------------------------------\\\n");
    if (TraceAnalyze) fprintf(listing,"\nConstruindo a tabela de simbolos...\n");
    buildSymtab(syntaxTree);
  }
#if !NO_CODE
  #if !NO_ASSEMBLY
   /*  if (! Error)
    { 
      fprintf(assembly , "  CODIGO ASSEMBLY DO ARQUIVO %s:\n", pgm);
      fprintf(assembly , "\\----------------------------------------------\\\n");
    } */
    #endif
    #if !NO_BINARY
    /* if (! Error)
    { 
      fprintf(bincode , "  CODIGO EXECUTAVEL DO ARQUIVO %s:\n", pgm);
      fprintf(bincode , "\\----------------------------------------------\\\n");
    } */
  #endif
  if (! Error)
  { 
    /* fprintf(intermed , "  CODIGO INTERMEDIARO DO ARQUIVO %s:\n", pgm);
    fprintf(intermed , "\\----------------------------------------------\\\n"); */
    printCode(syntaxTree);
    quadModel *ptrBegin = beginQuadList;
    getBin(ptrBegin);
    fprintf(bincode,"00110000000000000000000000000000");
  }
#endif
#endif
#endif
  fclose(source);
  fclose(listing);
  fclose(intermed);
  fclose(errorfile);
  fclose(assembly);
  fclose(bincode);
  fclose(binario);
  return 0;
}
