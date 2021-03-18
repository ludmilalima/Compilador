/****************************************************/
/* File: symtab.h                                   */
/* Alunos:                                          */
/* Bruno Sampaio Leite                              */
/* Talita Ludmila de Lima                           */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

#define SIZE 211

static int hash ( char * key );

typedef struct LineListRec{
  int lineno;
  struct LineListRec * next;
} * LineList;

typedef struct BucketListRec {
  char * name;
  TreeNode * treeNode;
  LineList lines;
  int memloc; /* memory location for variable */
  struct BucketListRec * next;
  ExpType type;
} * BucketList;

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(char * scopeName, char * name, ExpType type, TreeNode * streeNode, int loc);

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
BucketList st_lookup(char * name);

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */

void printSymTab(FILE * listing);

/*Function st_lookup_memPos returns the memory 
 * location of a variable or NULL if not found
 */

int st_lookup_memPos(char *varName, char *scopeName);

int st_isGlobal(char *varName, char *scopeName);

/*
 *Scope definitions
*/

//Scope List
typedef struct ScopeListRec 
{
  char * name; // function name
  int nestedLevel;
  int qtdeElementos;
  int posMem;
  struct ScopeListRec *parent;
  BucketList hashTable[SIZE]; /* the hash table */
  ExpType type;
} * Scope;

// global scope to cover function definitions
Scope globalScope;

//Scope List to output
static Scope scopeList[SIZE];
static int sizeOfList = 0;

//Stack to deal with scope
static Scope scopeStack[SIZE];
static int topScope = 0;

Scope newScope(char * scopeName, ExpType type);
void popScope(void);
void pushScope(Scope scope);
void insertScopeToList(Scope scope);
Scope currScope();
Scope st_lookup_scope(char * scopeName);
BucketList st_lookup_all_scope(char * name);
void insertLines(char* name, int lineno);

#endif