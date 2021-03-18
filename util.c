/****************************************************/
/* File: util.h                                     */
/* Utils functions and procedures implementation    */
/* Alunos:                                          */
/* Bruno Sampaio Leite                              */
/* Talita Ludmila de                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "codegenerate.h"
#include "assemblygenerate.h"

FILE * errorfile;
FILE * intermed;

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
char* printToken(int arq, TokenType token, const char* tokenString )
{ 
  if(arq == 1){
    switch (token)
    { case ELSE:
      case IF:
      case INT:
      case RETURN:
      case VOID:
      case WHILE:
      fprintf(errorfile, "reserved word: %s ",tokenString);
      break;
      case SOM:     fprintf(errorfile, "+ ");  break;
      case SUB:    fprintf(errorfile, "- ");  break;
      case MUL:    fprintf(errorfile, "* ");  break;
      case DIV:     fprintf(errorfile, "/ ");  break;
      case MEN:       fprintf(errorfile, "< ");  break;
      case IME:       fprintf(errorfile, "<= "); break;
      case MAI:       fprintf(errorfile, "> ");  break;
      case IMA:       fprintf(errorfile, ">= "); break;
      case IGL:       fprintf(errorfile, "== "); break;
      case DIF:       fprintf(errorfile, "!= "); break;
      case ATR:   fprintf(errorfile, "= ");  break;
      case PEV:     fprintf(errorfile, "; ");  break;
      case VIR:    fprintf(errorfile, ", ");  break;
      case APR:   fprintf(errorfile, "( ");  break;
      case FPR:   fprintf(errorfile, ") ");  break;
      case ACL: fprintf(errorfile, "[ ");  break;
      case FCL: fprintf(errorfile, "] ");  break;
      case ACH:   fprintf(errorfile, "{ ");  break;
      case FCH:   fprintf(errorfile, "} ");  break;
      case ENDFILE:  fprintf(errorfile,"%s %s ",  "ENDFILE", "EOF"); break;
      case NUM: fprintf(errorfile, "NUM, val = %s ",tokenString); break;
      case ID: fprintf(errorfile, "ID, name = %s ",tokenString); break;
      case ERR: fprintf(errorfile, "ERROR: %s ",tokenString); break;
      default: /* should never happen */
        fprintf(errorfile,"Unknown token: %d ",token);
    }
  }else if(arq == 0){
    switch (token)
    { case ELSE:
      case IF:
      case INT:
      case RETURN:
      case VOID:
      case WHILE:
      fprintf(listing, "reserved word: %s\n",tokenString);
      break;
      case SOM:     fprintf(listing, "+\n");  break;
      case SUB:    fprintf(listing, "-\n");  break;
      case MUL:    fprintf(listing, "*\n");  break;
      case DIV:     fprintf(listing, "/\n");  break;
      case MEN:       fprintf(listing, "<\n");  break;
      case IME:       fprintf(listing, "<=\n"); break;
      case MAI:       fprintf(listing, ">\n");  break;
      case IMA:       fprintf(listing, ">=\n"); break;
      case IGL:       fprintf(listing, "==\n"); break;
      case DIF:       fprintf(listing, "!=\n"); break;
      case ATR:   fprintf(listing, "=\n");  break;
      case PEV:     fprintf(listing, ";\n");  break;
      case VIR:    fprintf(listing, ",\n");  break;
      case APR:   fprintf(listing, "(\n");  break;
      case FPR:   fprintf(listing, ")\n");  break;
      case ACL: fprintf(listing, "[\n");  break;
      case FCL: fprintf(listing, "]\n");  break;
      case ACH:   fprintf(listing, "{\n");  break;
      case FCH:   fprintf(listing, "}\n");  break;
      case ENDFILE:  fprintf(listing,"%s %s\n",  "ENDFILE", "EOF"); break;
      case NUM: fprintf(listing, "NUM, val = %s\n",tokenString); break;
      case ID: fprintf(listing, "ID, name = %s\n",tokenString); break;
      case ERR: fprintf(listing, "ERROR: %s\n",tokenString); break;
      default: /* should never happen */
        fprintf(listing,"Unknown token: %d\n",token);
    }
  }else{
    switch(token)
    {
      case ELSE:    return "ELSE"; break;
      case IF:      return "IF"; break;
      case INT:     return "INT"; break;
      case RETURN:  return "RETURN"; break;
      case VOID:    return "VOID"; break;
      case WHILE:   return "WHILE"; break;
      case SOM:     return "SOM"; break;
      case SUB:     return "SUB";  break;
      case MUL:     return "MUL";  break;
      case DIV:     return "DIV";  break;
      case MEN:     return "MEN";  break;
      case IME:     return "IME";  break;
      case MAI:     return "MAI";  break;
      case IMA:     return "IMA";  break;
      case IGL:     return "IGL";  break;
      case DIF:     return "DIF";  break;
      case ATR:     return "ATR";  break;
      case PEV:     return "PEV";  break;
      case VIR:     return "VIR";  break;
      case APR:     return "APR";  break;
      case FPR:     return "FPR";  break;
      case ACL:     return "ACL";  break;
      case FCL:     return "FCL";  break;
      case ACH:     return "ACH";  break;
      case FCH:     return "FCH";  break;
      case GOTO:    return "GOTO"; break;
      case LAB:     return "LAB";  break;
      case ASSIGN:  return "ASSIGN"; break;
      case STORE:   return "STORE"; break;
      case LOADI:   return "LOADI"; break;
      case LOAD:    return "LOAD"; break;
      case FUN:     return "FUN"; break;
      case VAR:     return "VAR"; break;
      case VAR_VET: return "VAR_VET"; break;
      case SOMi:    return "SOMi"; break;
      case PAR_VET: return "PAR_VET"; break;
      case PAR:     return "PAR"; break;
      default: /* should never happen */
        return "INDEFINIDO";
        break;
    }
  }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
    t->scopeName = NULL;
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
    t->scopeName = NULL;
    //t->temporary = 0;
  }
  return t;
}

/* Function newDeclNode creates a new declaration
 * node for syntax tree construction
 */
TreeNode * newDeclNode(DeclKind kind) 
{
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DeclK;
    t->kind.decl = kind;
    t->lineno = lineno;
    t->type = Void;
    t->scopeName = NULL;
  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* printType print types of functions and variables */
void printTypes(TreeNode* tree) {
  if (tree->child[0] != NULL) {
    switch (tree->child[0]->type) {
      case Integer:
        fprintf(listing,"int");
        break;
      case Void:
        fprintf(listing,"void");
        break;
      case IntegerArray:
        fprintf(listing,"int array");
        break;
      default: return;
    }
  } else {
    switch (tree->type) {
      case Integer:
        fprintf(listing,"int");
        break;
      case Void:
        fprintf(listing,"void");
        break;
      case IntegerArray:
        fprintf(listing,"int array");
        break;
      default: return;
    }
  }
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  // IfK, WhileK, CompoundK, ReturnK
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK) {
      printSpaces();
      switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case WhileK:
          fprintf(listing, "While\n");
          break;
        case CompoundK:
          fprintf(listing, "Declaracao composta\n");
          break;
        case ReturnK:
          fprintf(listing, "Return\n");
          break;
        case AssignK:
          fprintf(listing, "Atribuicao\n");
          break;
        default:
          fprintf(listing, "Unknown stmtNode kind\n");
          break;
        }
    }
    // OpK, ConstK, AssignK, IdK, TypeK, ArrIdK, CallK, CalK
    else if (tree->nodekind==ExpK)
    { if (tree->kind.exp != TypeK) printSpaces();
      switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op: ");
          printToken(0, tree->attr.op, "\0");
          break;
        case ConstK:
          fprintf(listing,"Const: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        case TypeK: break;
        case ArrIdK:
          fprintf(listing,"ArrId \n");
          break;
        case CallK:
          fprintf(listing, "Chamada de funcao: %s\n", tree->attr.name);
          break;
        case CalcK:
          fprintf(listing, "Operador : ");
          printToken(0, tree->child[1]->attr.op, "\0");
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
  }
  // VarK, FunK, ArrVarK, ArrParamK, ParamK
    else if (tree->nodekind==DeclK) {
      printSpaces();
      switch (tree->kind.decl) {
        case FunK :
          fprintf(listing, "Declaracao de funcao:  ");
          printTypes(tree);
          fprintf(listing, " %s()\n", tree->attr.name);
          break;
        case VarK :
          fprintf(listing, "Declaracao de variavel:  ");
          printTypes(tree);
          fprintf(listing, " %s;\n", tree->attr.name);
          break;
        case ArrVarK :
          fprintf(listing, "Declaracao de Vetor:  ");
          printTypes(tree);
          fprintf(listing," %s[%d];\n", tree->attr.arr.name, tree->attr.arr.size);
          break;
        case ArrParamK :
          fprintf(listing, "Parametro Vetor: %s\n", tree->attr.name);
          break;
        case ParamK :
          fprintf(listing, "Parametro: ");
          printTypes(tree);
          if (tree->attr.name != NULL) {
            fprintf(listing, " %s\n", tree->attr.name);
          } else {
            fprintf(listing, " void\n");
          }
          break;
        default:
          fprintf(listing, "Unknown Declaration\n");
          break;
        }
      }else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++) 
      if (tree->child[i] != NULL) 
        printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}