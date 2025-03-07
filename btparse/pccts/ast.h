/* Abstract syntax tree
 *
 * Macros, definitions
 *
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to the Purdue Compiler Construction Tool
 * Set (PCCTS) -- PCCTS is in the public domain.  An individual or
 * company may do whatever they wish with source code distributed with
 * PCCTS or the code generated by PCCTS, including the incorporation of
 * PCCTS, or its output, into commerical software.
 * 
 * We encourage users to develop software with PCCTS.  However, we do ask
 * that credit is given to us for developing PCCTS.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like PCCTS and have developed a nice tool with the
 * output, please mention that you developed it using PCCTS.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * ANTLR 1.33
 * Terence Parr
 * Parr Research Corporation
 * with Purdue University and AHPCRC, University of Minnesota
 * 1989-1995
 */

#ifndef ZZAST_H
#define ZZAST_H

#define zzastOvfChk														\
			if ( zzast_sp <= 0 )                                        \
            {                                                           \
                fprintf(stderr, zzStackOvfMsg, __FILE__, __LINE__);    	\
                exit(PCCTS_EXIT_FAILURE);                                               \
            }

#ifndef USER_DEFINED_AST
#ifndef AST_FIELDS
#define AST_FIELDS
#endif

typedef struct _ast {
            struct _ast *right, *down;
#ifdef zzAST_DOUBLE
            struct _ast *left, *up;
#endif
            AST_FIELDS
} AST;

#else

#ifdef zzAST_DOUBLE
#define AST_REQUIRED_FIELDS   struct _ast *right, *down, *left, *up;
#else
#define AST_REQUIRED_FIELDS   struct _ast *right, *down;
#endif

#endif


/* N o d e  a c c e s s  m a c r o s */
#define zzchild(t)		(((t)==NULL)?NULL:(t->down))
#define zzsibling(t)	(((t)==NULL)?NULL:(t->right))


/* define global variables needed by #i stack */
#define zzASTgvars												\
	AST *zzastStack[ZZAST_STACKSIZE];							\
	int zzast_sp = ZZAST_STACKSIZE;

#define zzASTVars	AST *_ast = NULL, *_sibling = NULL, *_tail = NULL
#define zzSTR		( (_tail==NULL)?(&_sibling):(&(_tail->right)) )
#define zzastCur	(zzastStack[zzast_sp])
#define zzastArg(i)	(zzastStack[zztsp-i])
#define zzastPush(p) zzastOvfChk; zzastStack[--zzast_sp] = p;
#define zzastDPush	--zzast_sp
#define zzastMARK	zztsp=zzast_sp;		/* Save state of stack */
#define zzastREL	zzast_sp=zztsp;		/* Return state of stack */
#define zzrm_ast	{zzfree_ast(*_root); _tail = _sibling = (*_root)=NULL;}

extern int zzast_sp;
extern AST *zzastStack[];

#ifdef __STDC__
void zzlink(AST **, AST **, AST **);
void zzsubchild(AST **, AST **, AST **);
void zzsubroot(AST **, AST **, AST **);
void zzpre_ast(AST *, void (*)(AST *), void (*)(AST *), void (*)(AST *));
void zzfree_ast(AST *);
AST *zztmake(AST *, ...);
AST *zzdup_ast(AST *);
void zztfree(AST *);
void zzdouble_link(AST *, AST *, AST *);
AST *zzastnew(void);

#else

void zzlink();
AST *zzastnew();
void zzsubchild();
void zzsubroot();
void zzpre_ast();
void zzfree_ast();
AST *zztmake();
AST *zzdup_ast();
void zztfree();
void zzdouble_link();
#endif

#endif
