/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */


#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "genksyms.h"

static int is_typedef;
static int is_extern;
static char *current_name;
static struct string_list *decl_spec;

static void yyerror(const char *);

static inline void
remove_node(struct string_list **p)
{
  struct string_list *node = *p;
  *p = node->next;
  free_node(node);
}

static inline void
remove_list(struct string_list **pb, struct string_list **pe)
{
  struct string_list *b = *pb, *e = *pe;
  *pb = e;
  free_list(b, e);
}

/* Record definition of a struct/union/enum */
static void record_compound(struct string_list **keyw,
		       struct string_list **ident,
		       struct string_list **body,
		       enum symbol_type type)
{
	struct string_list *b = *body, *i = *ident, *r;

	if (i->in_source_file) {
		remove_node(keyw);
		(*ident)->tag = type;
		remove_list(body, ident);
		return;
	}
	r = copy_node(i); r->tag = type;
	r->next = (*keyw)->next; *body = r; (*keyw)->next = NULL;
	add_symbol(i->string, type, b, is_extern);
}



# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parse.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ASM_KEYW = 3,                   /* ASM_KEYW  */
  YYSYMBOL_ATTRIBUTE_KEYW = 4,             /* ATTRIBUTE_KEYW  */
  YYSYMBOL_AUTO_KEYW = 5,                  /* AUTO_KEYW  */
  YYSYMBOL_BOOL_KEYW = 6,                  /* BOOL_KEYW  */
  YYSYMBOL_BUILTIN_INT_KEYW = 7,           /* BUILTIN_INT_KEYW  */
  YYSYMBOL_CHAR_KEYW = 8,                  /* CHAR_KEYW  */
  YYSYMBOL_CONST_KEYW = 9,                 /* CONST_KEYW  */
  YYSYMBOL_DOUBLE_KEYW = 10,               /* DOUBLE_KEYW  */
  YYSYMBOL_ENUM_KEYW = 11,                 /* ENUM_KEYW  */
  YYSYMBOL_EXTERN_KEYW = 12,               /* EXTERN_KEYW  */
  YYSYMBOL_EXTENSION_KEYW = 13,            /* EXTENSION_KEYW  */
  YYSYMBOL_FLOAT_KEYW = 14,                /* FLOAT_KEYW  */
  YYSYMBOL_INLINE_KEYW = 15,               /* INLINE_KEYW  */
  YYSYMBOL_INT_KEYW = 16,                  /* INT_KEYW  */
  YYSYMBOL_LONG_KEYW = 17,                 /* LONG_KEYW  */
  YYSYMBOL_REGISTER_KEYW = 18,             /* REGISTER_KEYW  */
  YYSYMBOL_RESTRICT_KEYW = 19,             /* RESTRICT_KEYW  */
  YYSYMBOL_SHORT_KEYW = 20,                /* SHORT_KEYW  */
  YYSYMBOL_SIGNED_KEYW = 21,               /* SIGNED_KEYW  */
  YYSYMBOL_STATIC_KEYW = 22,               /* STATIC_KEYW  */
  YYSYMBOL_STATIC_ASSERT_KEYW = 23,        /* STATIC_ASSERT_KEYW  */
  YYSYMBOL_STRUCT_KEYW = 24,               /* STRUCT_KEYW  */
  YYSYMBOL_TYPEDEF_KEYW = 25,              /* TYPEDEF_KEYW  */
  YYSYMBOL_UNION_KEYW = 26,                /* UNION_KEYW  */
  YYSYMBOL_UNSIGNED_KEYW = 27,             /* UNSIGNED_KEYW  */
  YYSYMBOL_VOID_KEYW = 28,                 /* VOID_KEYW  */
  YYSYMBOL_VOLATILE_KEYW = 29,             /* VOLATILE_KEYW  */
  YYSYMBOL_TYPEOF_KEYW = 30,               /* TYPEOF_KEYW  */
  YYSYMBOL_VA_LIST_KEYW = 31,              /* VA_LIST_KEYW  */
  YYSYMBOL_EXPORT_SYMBOL_KEYW = 32,        /* EXPORT_SYMBOL_KEYW  */
  YYSYMBOL_ASM_PHRASE = 33,                /* ASM_PHRASE  */
  YYSYMBOL_ATTRIBUTE_PHRASE = 34,          /* ATTRIBUTE_PHRASE  */
  YYSYMBOL_TYPEOF_PHRASE = 35,             /* TYPEOF_PHRASE  */
  YYSYMBOL_BRACE_PHRASE = 36,              /* BRACE_PHRASE  */
  YYSYMBOL_BRACKET_PHRASE = 37,            /* BRACKET_PHRASE  */
  YYSYMBOL_EXPRESSION_PHRASE = 38,         /* EXPRESSION_PHRASE  */
  YYSYMBOL_STATIC_ASSERT_PHRASE = 39,      /* STATIC_ASSERT_PHRASE  */
  YYSYMBOL_CHAR = 40,                      /* CHAR  */
  YYSYMBOL_DOTS = 41,                      /* DOTS  */
  YYSYMBOL_IDENT = 42,                     /* IDENT  */
  YYSYMBOL_INT = 43,                       /* INT  */
  YYSYMBOL_REAL = 44,                      /* REAL  */
  YYSYMBOL_STRING = 45,                    /* STRING  */
  YYSYMBOL_TYPE = 46,                      /* TYPE  */
  YYSYMBOL_OTHER = 47,                     /* OTHER  */
  YYSYMBOL_FILENAME = 48,                  /* FILENAME  */
  YYSYMBOL_49_ = 49,                       /* ';'  */
  YYSYMBOL_50_ = 50,                       /* '}'  */
  YYSYMBOL_51_ = 51,                       /* ','  */
  YYSYMBOL_52_ = 52,                       /* '('  */
  YYSYMBOL_53_ = 53,                       /* ')'  */
  YYSYMBOL_54_ = 54,                       /* '*'  */
  YYSYMBOL_55_ = 55,                       /* '='  */
  YYSYMBOL_56_ = 56,                       /* '{'  */
  YYSYMBOL_57_ = 57,                       /* ':'  */
  YYSYMBOL_YYACCEPT = 58,                  /* $accept  */
  YYSYMBOL_declaration_seq = 59,           /* declaration_seq  */
  YYSYMBOL_declaration = 60,               /* declaration  */
  YYSYMBOL_61_1 = 61,                      /* $@1  */
  YYSYMBOL_declaration1 = 62,              /* declaration1  */
  YYSYMBOL_63_2 = 63,                      /* $@2  */
  YYSYMBOL_64_3 = 64,                      /* $@3  */
  YYSYMBOL_simple_declaration = 65,        /* simple_declaration  */
  YYSYMBOL_init_declarator_list_opt = 66,  /* init_declarator_list_opt  */
  YYSYMBOL_init_declarator_list = 67,      /* init_declarator_list  */
  YYSYMBOL_init_declarator = 68,           /* init_declarator  */
  YYSYMBOL_decl_specifier_seq_opt = 69,    /* decl_specifier_seq_opt  */
  YYSYMBOL_decl_specifier_seq = 70,        /* decl_specifier_seq  */
  YYSYMBOL_decl_specifier = 71,            /* decl_specifier  */
  YYSYMBOL_storage_class_specifier = 72,   /* storage_class_specifier  */
  YYSYMBOL_type_specifier = 73,            /* type_specifier  */
  YYSYMBOL_simple_type_specifier = 74,     /* simple_type_specifier  */
  YYSYMBOL_ptr_operator = 75,              /* ptr_operator  */
  YYSYMBOL_cvar_qualifier_seq_opt = 76,    /* cvar_qualifier_seq_opt  */
  YYSYMBOL_cvar_qualifier_seq = 77,        /* cvar_qualifier_seq  */
  YYSYMBOL_cvar_qualifier = 78,            /* cvar_qualifier  */
  YYSYMBOL_declarator = 79,                /* declarator  */
  YYSYMBOL_direct_declarator = 80,         /* direct_declarator  */
  YYSYMBOL_nested_declarator = 81,         /* nested_declarator  */
  YYSYMBOL_direct_nested_declarator = 82,  /* direct_nested_declarator  */
  YYSYMBOL_parameter_declaration_clause = 83, /* parameter_declaration_clause  */
  YYSYMBOL_parameter_declaration_list_opt = 84, /* parameter_declaration_list_opt  */
  YYSYMBOL_parameter_declaration_list = 85, /* parameter_declaration_list  */
  YYSYMBOL_parameter_declaration = 86,     /* parameter_declaration  */
  YYSYMBOL_m_abstract_declarator = 87,     /* m_abstract_declarator  */
  YYSYMBOL_direct_m_abstract_declarator = 88, /* direct_m_abstract_declarator  */
  YYSYMBOL_function_definition = 89,       /* function_definition  */
  YYSYMBOL_initializer_opt = 90,           /* initializer_opt  */
  YYSYMBOL_initializer = 91,               /* initializer  */
  YYSYMBOL_class_body = 92,                /* class_body  */
  YYSYMBOL_member_specification_opt = 93,  /* member_specification_opt  */
  YYSYMBOL_member_specification = 94,      /* member_specification  */
  YYSYMBOL_member_declaration = 95,        /* member_declaration  */
  YYSYMBOL_member_declarator_list_opt = 96, /* member_declarator_list_opt  */
  YYSYMBOL_member_declarator_list = 97,    /* member_declarator_list  */
  YYSYMBOL_member_declarator = 98,         /* member_declarator  */
  YYSYMBOL_member_bitfield_declarator = 99, /* member_bitfield_declarator  */
  YYSYMBOL_attribute_opt = 100,            /* attribute_opt  */
  YYSYMBOL_enum_body = 101,                /* enum_body  */
  YYSYMBOL_enumerator_list = 102,          /* enumerator_list  */
  YYSYMBOL_enumerator = 103,               /* enumerator  */
  YYSYMBOL_asm_definition = 104,           /* asm_definition  */
  YYSYMBOL_asm_phrase_opt = 105,           /* asm_phrase_opt  */
  YYSYMBOL_export_definition = 106,        /* export_definition  */
  YYSYMBOL_static_assert = 107             /* static_assert  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   550

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  136
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  191

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      52,    53,    54,     2,    51,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    57,    49,
       2,    55,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    56,     2,    50,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   116,   116,   117,   121,   121,   127,   127,   129,   129,
     131,   132,   133,   134,   135,   136,   137,   141,   155,   156,
     160,   168,   181,   187,   188,   192,   193,   197,   203,   207,
     208,   209,   210,   211,   215,   216,   217,   218,   222,   224,
     226,   230,   232,   234,   239,   242,   243,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     263,   268,   269,   273,   274,   278,   278,   278,   279,   287,
     288,   292,   301,   310,   312,   314,   316,   323,   324,   328,
     329,   330,   332,   334,   336,   338,   343,   344,   345,   349,
     350,   354,   355,   360,   365,   367,   371,   372,   380,   384,
     386,   388,   390,   392,   397,   406,   407,   412,   417,   418,
     422,   423,   427,   428,   432,   434,   439,   440,   444,   445,
     449,   450,   451,   455,   459,   460,   464,   465,   469,   470,
     473,   478,   486,   490,   491,   495,   501
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ASM_KEYW",
  "ATTRIBUTE_KEYW", "AUTO_KEYW", "BOOL_KEYW", "BUILTIN_INT_KEYW",
  "CHAR_KEYW", "CONST_KEYW", "DOUBLE_KEYW", "ENUM_KEYW", "EXTERN_KEYW",
  "EXTENSION_KEYW", "FLOAT_KEYW", "INLINE_KEYW", "INT_KEYW", "LONG_KEYW",
  "REGISTER_KEYW", "RESTRICT_KEYW", "SHORT_KEYW", "SIGNED_KEYW",
  "STATIC_KEYW", "STATIC_ASSERT_KEYW", "STRUCT_KEYW", "TYPEDEF_KEYW",
  "UNION_KEYW", "UNSIGNED_KEYW", "VOID_KEYW", "VOLATILE_KEYW",
  "TYPEOF_KEYW", "VA_LIST_KEYW", "EXPORT_SYMBOL_KEYW", "ASM_PHRASE",
  "ATTRIBUTE_PHRASE", "TYPEOF_PHRASE", "BRACE_PHRASE", "BRACKET_PHRASE",
  "EXPRESSION_PHRASE", "STATIC_ASSERT_PHRASE", "CHAR", "DOTS", "IDENT",
  "INT", "REAL", "STRING", "TYPE", "OTHER", "FILENAME", "';'", "'}'",
  "','", "'('", "')'", "'*'", "'='", "'{'", "':'", "$accept",
  "declaration_seq", "declaration", "$@1", "declaration1", "$@2", "$@3",
  "simple_declaration", "init_declarator_list_opt", "init_declarator_list",
  "init_declarator", "decl_specifier_seq_opt", "decl_specifier_seq",
  "decl_specifier", "storage_class_specifier", "type_specifier",
  "simple_type_specifier", "ptr_operator", "cvar_qualifier_seq_opt",
  "cvar_qualifier_seq", "cvar_qualifier", "declarator",
  "direct_declarator", "nested_declarator", "direct_nested_declarator",
  "parameter_declaration_clause", "parameter_declaration_list_opt",
  "parameter_declaration_list", "parameter_declaration",
  "m_abstract_declarator", "direct_m_abstract_declarator",
  "function_definition", "initializer_opt", "initializer", "class_body",
  "member_specification_opt", "member_specification", "member_declaration",
  "member_declarator_list_opt", "member_declarator_list",
  "member_declarator", "member_bitfield_declarator", "attribute_opt",
  "enum_body", "enumerator_list", "enumerator", "asm_definition",
  "asm_phrase_opt", "export_definition", "static_assert", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-145)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-112)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -145,    26,  -145,   215,  -145,  -145,    38,  -145,  -145,  -145,
    -145,  -145,  -145,    -5,  -145,     7,  -145,  -145,  -145,  -145,
    -145,  -145,  -145,  -145,  -145,    -3,  -145,    12,  -145,  -145,
    -145,    -8,  -145,     9,    23,  -145,  -145,    28,  -145,  -145,
    -145,    27,   504,  -145,  -145,  -145,  -145,  -145,  -145,  -145,
    -145,  -145,  -145,  -145,    14,    33,  -145,  -145,    24,   107,
    -145,   504,    24,  -145,   504,    41,  -145,  -145,  -145,  -145,
      27,     0,    45,    51,  -145,    27,   -16,   -29,  -145,  -145,
      37,    40,  -145,   504,  -145,    49,   -30,    46,   161,  -145,
    -145,    27,  -145,   412,    47,    52,    53,  -145,     0,  -145,
    -145,    27,  -145,  -145,  -145,  -145,  -145,   265,    69,  -145,
      21,  -145,  -145,  -145,    63,  -145,    13,    71,    32,  -145,
     -12,    81,    88,  -145,  -145,  -145,    83,  -145,   110,  -145,
    -145,     4,    43,  -145,    -7,  -145,    91,  -145,  -145,  -145,
     -19,    92,    93,   103,    96,  -145,  -145,  -145,  -145,  -145,
      95,  -145,    97,  -145,  -145,   117,  -145,   314,  -145,   -30,
      99,  -145,   101,  -145,  -145,   363,  -145,  -145,   120,  -145,
    -145,  -145,  -145,  -145,   462,  -145,  -145,   102,   112,  -145,
    -145,  -145,   121,   131,  -145,  -145,  -145,  -145,  -145,  -145,
    -145
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     4,     2,     0,     1,     3,     0,    29,    56,    58,
      47,    65,    54,     0,    32,     0,    53,    33,    49,    50,
      30,    68,    48,    51,    31,     0,     8,     0,    52,    55,
      66,     0,    57,     0,     0,    67,    37,     0,    59,     5,
      10,    18,    24,    25,    27,    28,    34,    35,    11,    12,
      13,    14,    15,    16,    40,     0,    44,     6,    38,     0,
      45,    23,    39,    46,     0,     0,   132,   136,    71,    72,
       0,    61,     0,    19,    20,     0,   133,    70,    26,    43,
     130,     0,   128,    23,    41,     0,   116,     0,     0,   112,
       9,    18,    42,    96,     0,     0,     0,    60,    62,    63,
      17,     0,    69,   134,   104,   124,    75,     0,     0,   126,
       0,     7,   115,   109,    79,    80,     0,     0,     0,   124,
      78,     0,   117,   118,   122,   108,     0,   113,   133,    97,
      59,     0,    96,    93,    95,    36,     0,    76,    64,    21,
     105,     0,     0,    87,    90,    91,   131,   127,   129,   121,
       0,    79,     0,   123,    77,   120,    83,     0,   114,     0,
       0,    98,     0,    94,   101,     0,   135,   125,     0,    22,
     106,    74,    73,    86,     0,    85,    84,     0,     0,   119,
     103,   102,     0,     0,   107,    88,    92,    82,    81,   100,
      99
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -145,  -145,   159,  -145,  -145,  -145,  -145,   -55,  -145,  -145,
      62,    -1,   -64,   -31,  -145,  -145,  -145,   -83,  -145,  -145,
     -67,   -23,  -145,   -52,  -145,  -144,  -145,  -145,   -63,   -28,
    -145,  -145,  -145,  -145,   -20,  -145,  -145,    98,  -145,  -145,
      34,    80,    78,   144,  -145,    89,  -145,  -145,  -145,  -145
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     3,    39,    83,    61,    40,    72,    73,
      74,    86,    42,    43,    44,    45,    46,    75,    97,    98,
      47,   128,    77,   119,   120,   142,   143,   144,   145,   133,
     134,    48,   169,   170,    60,    87,    88,    89,   121,   122,
     123,   124,   140,    56,    81,    82,    49,   105,    50,    51
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      93,    94,    41,   118,    99,   160,    90,    63,   106,    11,
     132,    78,   114,   178,   150,   167,   115,   103,    76,    21,
     104,   183,   116,   107,    71,   156,     4,   117,   111,    30,
     164,   138,    57,   118,    35,   118,   168,    54,    84,    58,
     157,   -96,    92,    93,    64,   165,   129,    96,   132,   132,
     161,    55,   102,    59,    62,   151,   131,   -96,    71,   115,
      91,    65,    78,    80,   152,   116,   154,    71,    59,    68,
      55,   147,    66,    69,   151,    80,   118,    67,   115,    70,
      59,    71,    91,    95,   116,   129,    71,    52,    53,   161,
     109,   110,   108,    93,   100,   131,   125,    71,   112,   113,
     135,    93,   101,   162,   163,   136,   137,   146,    85,   153,
      93,   186,     7,     8,     9,    10,    11,    12,    13,    14,
     117,    16,    17,    18,    19,    20,    21,    22,    23,    24,
     158,    25,   112,    27,    28,    29,    30,    31,    32,   159,
     166,    35,    36,   103,   173,   171,   172,   174,   175,   -23,
     176,   167,   180,    38,   181,   187,   -23,  -110,   184,   -23,
       5,   -23,   126,   139,   -23,   188,     7,     8,     9,    10,
      11,    12,    13,    14,   189,    16,    17,    18,    19,    20,
      21,    22,    23,    24,   190,    25,   127,    27,    28,    29,
      30,    31,    32,   179,   149,    35,    36,   155,    79,   148,
       0,     0,     0,   -23,     0,     0,     0,    38,     0,     0,
     -23,  -111,     0,   -23,     0,   -23,     6,     0,   -23,     0,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,     0,     0,     0,    37,     0,     0,   -23,     0,     0,
       0,    38,     0,     0,   -23,     0,   141,   -23,     0,   -23,
       7,     8,     9,    10,    11,    12,    13,    14,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
       0,    27,    28,    29,    30,    31,    32,     0,     0,    35,
      36,     0,     0,     0,     0,     0,   -89,     0,     0,     0,
       0,    38,     0,     0,     0,   177,     0,     0,   -89,     7,
       8,     9,    10,    11,    12,    13,    14,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,     0,
      27,    28,    29,    30,    31,    32,     0,     0,    35,    36,
       0,     0,     0,     0,     0,   -89,     0,     0,     0,     0,
      38,     0,     0,     0,   182,     0,     0,   -89,     7,     8,
       9,    10,    11,    12,    13,    14,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,     0,    27,
      28,    29,    30,    31,    32,     0,     0,    35,    36,     0,
       0,     0,     0,     0,   -89,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,   -89,     7,     8,     9,
      10,    11,    12,    13,    14,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,     0,    27,    28,
      29,    30,    31,    32,     0,     0,    35,    36,     0,     0,
       0,     0,     0,     0,   129,     0,     0,     0,   130,     0,
       0,     0,     0,     0,   131,     0,    71,     7,     8,     9,
      10,    11,    12,    13,    14,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,    25,     0,    27,    28,
      29,    30,    31,    32,     0,     0,    35,    36,     0,     0,
       0,     0,     0,   185,     0,     0,     0,     0,    38,     7,
       8,     9,    10,    11,    12,    13,    14,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,    25,     0,
      27,    28,    29,    30,    31,    32,     0,     0,    35,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      38
};

static const yytype_int16 yycheck[] =
{
      64,    64,     3,    86,    71,     1,    61,    27,    37,     9,
      93,    42,    42,   157,     1,    34,    46,    33,    41,    19,
      36,   165,    52,    52,    54,    37,     0,    57,    83,    29,
      37,    98,    25,   116,    34,   118,    55,    42,    58,    42,
      52,    37,    62,   107,    52,    52,    42,    70,   131,   132,
      46,    56,    75,    56,    42,    42,    52,    53,    54,    46,
      61,    52,    93,    42,   116,    52,   118,    54,    56,    42,
      56,    50,    49,    46,    42,    42,   159,    49,    46,    52,
      56,    54,    83,    42,    52,    42,    54,    49,    50,    46,
      50,    51,    55,   157,    49,    52,    50,    54,    49,    50,
      53,   165,    51,   131,   132,    53,    53,    38,     1,    38,
     174,   174,     5,     6,     7,     8,     9,    10,    11,    12,
      57,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      49,    24,    49,    26,    27,    28,    29,    30,    31,    51,
      49,    34,    35,    33,    41,    53,    53,    51,    53,    42,
      53,    34,    53,    46,    53,    53,    49,    50,    38,    52,
       1,    54,     1,   101,    57,    53,     5,     6,     7,     8,
       9,    10,    11,    12,    53,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    53,    24,    88,    26,    27,    28,
      29,    30,    31,   159,   114,    34,    35,   119,    54,   110,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    -1,    54,     1,    -1,    57,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    39,    -1,    -1,    42,    -1,    -1,
      -1,    46,    -1,    -1,    49,    -1,     1,    52,    -1,    54,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      -1,    26,    27,    28,    29,    30,    31,    -1,    -1,    34,
      35,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,     1,    -1,    -1,    53,     5,
       6,     7,     8,     9,    10,    11,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    -1,    34,    35,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,     1,    -1,    -1,    53,     5,     6,
       7,     8,     9,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    -1,    26,
      27,    28,    29,    30,    31,    -1,    -1,    34,    35,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    -1,    26,    27,
      28,    29,    30,    31,    -1,    -1,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    54,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    -1,    26,    27,
      28,    29,    30,    31,    -1,    -1,    34,    35,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    46,     5,
       6,     7,     8,     9,    10,    11,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    -1,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    59,    60,    61,     0,    60,     1,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    39,    46,    62,
      65,    69,    70,    71,    72,    73,    74,    78,    89,   104,
     106,   107,    49,    50,    42,    56,   101,    25,    42,    56,
      92,    64,    42,    92,    52,    52,    49,    49,    42,    46,
      52,    54,    66,    67,    68,    75,    79,    80,    71,   101,
      42,   102,   103,    63,    92,     1,    69,    93,    94,    95,
      65,    69,    92,    70,    86,    42,    79,    76,    77,    78,
      49,    51,    79,    33,    36,   105,    37,    52,    55,    50,
      51,    65,    49,    50,    42,    46,    52,    57,    75,    81,
      82,    96,    97,    98,    99,    50,     1,    95,    79,    42,
      46,    52,    75,    87,    88,    53,    53,    53,    78,    68,
     100,     1,    83,    84,    85,    86,    38,    50,   103,    99,
       1,    42,    81,    38,    81,   100,    37,    52,    49,    51,
       1,    46,    87,    87,    37,    52,    49,    34,    55,    90,
      91,    53,    53,    41,    51,    53,    53,     1,    83,    98,
      53,    53,     1,    83,    38,    41,    86,    53,    53,    53,
      53
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    58,    59,    59,    61,    60,    63,    62,    64,    62,
      62,    62,    62,    62,    62,    62,    62,    65,    66,    66,
      67,    67,    68,    69,    69,    70,    70,    71,    71,    72,
      72,    72,    72,    72,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      75,    76,    76,    77,    77,    78,    78,    78,    78,    79,
      79,    80,    80,    80,    80,    80,    80,    81,    81,    82,
      82,    82,    82,    82,    82,    82,    83,    83,    83,    84,
      84,    85,    85,    86,    87,    87,    88,    88,    88,    88,
      88,    88,    88,    88,    89,    90,    90,    91,    92,    92,
      93,    93,    94,    94,    95,    95,    96,    96,    97,    97,
      98,    98,    98,    99,   100,   100,   101,   101,   102,   102,
     103,   103,   104,   105,   105,   106,   107
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     0,     4,     0,     3,
       1,     1,     1,     1,     1,     2,     2,     3,     0,     1,
       1,     3,     4,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     2,     2,
       2,     3,     3,     3,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     0,     1,     1,     2,     1,     1,     1,     1,     2,
       1,     1,     1,     4,     4,     2,     3,     2,     1,     1,
       1,     4,     4,     2,     3,     3,     2,     1,     3,     0,
       1,     1,     3,     2,     2,     1,     0,     1,     1,     4,
       4,     2,     3,     3,     3,     0,     1,     2,     3,     3,
       0,     1,     1,     2,     3,     2,     0,     1,     1,     3,
       2,     2,     1,     2,     0,     2,     3,     4,     1,     3,
       1,     3,     2,     0,     1,     5,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* $@1: %empty  */
        { is_typedef = 0; is_extern = 0; current_name = NULL; decl_spec = NULL; }
    break;

  case 5: /* declaration: $@1 declaration1  */
        { free_list(*yyvsp[0], NULL); *yyvsp[0] = NULL; }
    break;

  case 6: /* $@2: %empty  */
                                    { is_typedef = 1; }
    break;

  case 7: /* declaration1: EXTENSION_KEYW TYPEDEF_KEYW $@2 simple_declaration  */
                { yyval = yyvsp[0]; }
    break;

  case 8: /* $@3: %empty  */
                       { is_typedef = 1; }
    break;

  case 9: /* declaration1: TYPEDEF_KEYW $@3 simple_declaration  */
                { yyval = yyvsp[0]; }
    break;

  case 15: /* declaration1: error ';'  */
                                                { yyval = yyvsp[0]; }
    break;

  case 16: /* declaration1: error '}'  */
                                                { yyval = yyvsp[0]; }
    break;

  case 17: /* simple_declaration: decl_specifier_seq_opt init_declarator_list_opt ';'  */
                { if (current_name) {
		    struct string_list *decl = (*yyvsp[0])->next;
		    (*yyvsp[0])->next = NULL;
		    add_symbol(current_name,
			       is_typedef ? SYM_TYPEDEF : SYM_NORMAL,
			       decl, is_extern);
		    current_name = NULL;
		  }
		  yyval = yyvsp[0];
		}
    break;

  case 18: /* init_declarator_list_opt: %empty  */
                                                { yyval = NULL; }
    break;

  case 20: /* init_declarator_list: init_declarator  */
                { struct string_list *decl = *yyvsp[0];
		  *yyvsp[0] = NULL;
		  add_symbol(current_name,
			     is_typedef ? SYM_TYPEDEF : SYM_NORMAL, decl, is_extern);
		  current_name = NULL;
		  yyval = yyvsp[0];
		}
    break;

  case 21: /* init_declarator_list: init_declarator_list ',' init_declarator  */
                { struct string_list *decl = *yyvsp[0];
		  *yyvsp[0] = NULL;
		  free_list(*yyvsp[-1], NULL);
		  *yyvsp[-1] = decl_spec;
		  add_symbol(current_name,
			     is_typedef ? SYM_TYPEDEF : SYM_NORMAL, decl, is_extern);
		  current_name = NULL;
		  yyval = yyvsp[0];
		}
    break;

  case 22: /* init_declarator: declarator asm_phrase_opt attribute_opt initializer_opt  */
                { yyval = yyvsp[0] ? yyvsp[0] : yyvsp[-1] ? yyvsp[-1] : yyvsp[-2] ? yyvsp[-2] : yyvsp[-3]; }
    break;

  case 23: /* decl_specifier_seq_opt: %empty  */
                                                { decl_spec = NULL; }
    break;

  case 25: /* decl_specifier_seq: decl_specifier  */
                                                { decl_spec = *yyvsp[0]; }
    break;

  case 26: /* decl_specifier_seq: decl_specifier_seq decl_specifier  */
                                                { decl_spec = *yyvsp[0]; }
    break;

  case 27: /* decl_specifier: storage_class_specifier  */
                { /* Version 2 checksumming ignores storage class, as that
		     is really irrelevant to the linkage.  */
		  remove_node(yyvsp[0]);
		  yyval = yyvsp[0];
		}
    break;

  case 32: /* storage_class_specifier: EXTERN_KEYW  */
                        { is_extern = 1; yyval = yyvsp[0]; }
    break;

  case 33: /* storage_class_specifier: INLINE_KEYW  */
                        { is_extern = 0; yyval = yyvsp[0]; }
    break;

  case 38: /* type_specifier: STRUCT_KEYW IDENT  */
                { remove_node(yyvsp[-1]); (*yyvsp[0])->tag = SYM_STRUCT; yyval = yyvsp[0]; }
    break;

  case 39: /* type_specifier: UNION_KEYW IDENT  */
                { remove_node(yyvsp[-1]); (*yyvsp[0])->tag = SYM_UNION; yyval = yyvsp[0]; }
    break;

  case 40: /* type_specifier: ENUM_KEYW IDENT  */
                { remove_node(yyvsp[-1]); (*yyvsp[0])->tag = SYM_ENUM; yyval = yyvsp[0]; }
    break;

  case 41: /* type_specifier: STRUCT_KEYW IDENT class_body  */
                { record_compound(yyvsp[-2], yyvsp[-1], yyvsp[0], SYM_STRUCT); yyval = yyvsp[0]; }
    break;

  case 42: /* type_specifier: UNION_KEYW IDENT class_body  */
                { record_compound(yyvsp[-2], yyvsp[-1], yyvsp[0], SYM_UNION); yyval = yyvsp[0]; }
    break;

  case 43: /* type_specifier: ENUM_KEYW IDENT enum_body  */
                { record_compound(yyvsp[-2], yyvsp[-1], yyvsp[0], SYM_ENUM); yyval = yyvsp[0]; }
    break;

  case 44: /* type_specifier: ENUM_KEYW enum_body  */
                { add_symbol(NULL, SYM_ENUM, NULL, 0); yyval = yyvsp[0]; }
    break;

  case 45: /* type_specifier: STRUCT_KEYW class_body  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 46: /* type_specifier: UNION_KEYW class_body  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 59: /* simple_type_specifier: TYPE  */
                                { (*yyvsp[0])->tag = SYM_TYPEDEF; yyval = yyvsp[0]; }
    break;

  case 60: /* ptr_operator: '*' cvar_qualifier_seq_opt  */
                { yyval = yyvsp[0] ? yyvsp[0] : yyvsp[-1]; }
    break;

  case 61: /* cvar_qualifier_seq_opt: %empty  */
                                                        { yyval = NULL; }
    break;

  case 64: /* cvar_qualifier_seq: cvar_qualifier_seq cvar_qualifier  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 68: /* cvar_qualifier: RESTRICT_KEYW  */
                { /* restrict has no effect in prototypes so ignore it */
		  remove_node(yyvsp[0]);
		  yyval = yyvsp[0];
		}
    break;

  case 69: /* declarator: ptr_operator declarator  */
                                                { yyval = yyvsp[0]; }
    break;

  case 71: /* direct_declarator: IDENT  */
                { if (current_name != NULL) {
		    error_with_pos("unexpected second declaration name");
		    YYERROR;
		  } else {
		    current_name = (*yyvsp[0])->string;
		    yyval = yyvsp[0];
		  }
		}
    break;

  case 72: /* direct_declarator: TYPE  */
                { if (current_name != NULL) {
		    error_with_pos("unexpected second declaration name");
		    YYERROR;
		  } else {
		    current_name = (*yyvsp[0])->string;
		    yyval = yyvsp[0];
		  }
		}
    break;

  case 73: /* direct_declarator: direct_declarator '(' parameter_declaration_clause ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 74: /* direct_declarator: direct_declarator '(' error ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 75: /* direct_declarator: direct_declarator BRACKET_PHRASE  */
                { yyval = yyvsp[0]; }
    break;

  case 76: /* direct_declarator: '(' declarator ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 77: /* nested_declarator: ptr_operator nested_declarator  */
                                                { yyval = yyvsp[0]; }
    break;

  case 81: /* direct_nested_declarator: direct_nested_declarator '(' parameter_declaration_clause ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 82: /* direct_nested_declarator: direct_nested_declarator '(' error ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 83: /* direct_nested_declarator: direct_nested_declarator BRACKET_PHRASE  */
                { yyval = yyvsp[0]; }
    break;

  case 84: /* direct_nested_declarator: '(' nested_declarator ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 85: /* direct_nested_declarator: '(' error ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 86: /* parameter_declaration_clause: parameter_declaration_list_opt DOTS  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 88: /* parameter_declaration_clause: parameter_declaration_list ',' DOTS  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 89: /* parameter_declaration_list_opt: %empty  */
                                                        { yyval = NULL; }
    break;

  case 92: /* parameter_declaration_list: parameter_declaration_list ',' parameter_declaration  */
                { yyval = yyvsp[0]; }
    break;

  case 93: /* parameter_declaration: decl_specifier_seq m_abstract_declarator  */
                { yyval = yyvsp[0] ? yyvsp[0] : yyvsp[-1]; }
    break;

  case 94: /* m_abstract_declarator: ptr_operator m_abstract_declarator  */
                { yyval = yyvsp[0] ? yyvsp[0] : yyvsp[-1]; }
    break;

  case 96: /* direct_m_abstract_declarator: %empty  */
                                                        { yyval = NULL; }
    break;

  case 97: /* direct_m_abstract_declarator: IDENT  */
                { /* For version 2 checksums, we don't want to remember
		     private parameter names.  */
		  remove_node(yyvsp[0]);
		  yyval = yyvsp[0];
		}
    break;

  case 98: /* direct_m_abstract_declarator: TYPE  */
                { remove_node(yyvsp[0]);
		  yyval = yyvsp[0];
		}
    break;

  case 99: /* direct_m_abstract_declarator: direct_m_abstract_declarator '(' parameter_declaration_clause ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 100: /* direct_m_abstract_declarator: direct_m_abstract_declarator '(' error ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 101: /* direct_m_abstract_declarator: direct_m_abstract_declarator BRACKET_PHRASE  */
                { yyval = yyvsp[0]; }
    break;

  case 102: /* direct_m_abstract_declarator: '(' m_abstract_declarator ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 103: /* direct_m_abstract_declarator: '(' error ')'  */
                { yyval = yyvsp[0]; }
    break;

  case 104: /* function_definition: decl_specifier_seq_opt declarator BRACE_PHRASE  */
                { struct string_list *decl = *yyvsp[-1];
		  *yyvsp[-1] = NULL;
		  add_symbol(current_name, SYM_NORMAL, decl, is_extern);
		  yyval = yyvsp[0];
		}
    break;

  case 105: /* initializer_opt: %empty  */
                                                        { yyval = NULL; }
    break;

  case 107: /* initializer: '=' EXPRESSION_PHRASE  */
                { remove_list(yyvsp[0], &(*yyvsp[-1])->next); yyval = yyvsp[0]; }
    break;

  case 108: /* class_body: '{' member_specification_opt '}'  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 109: /* class_body: '{' error '}'  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 110: /* member_specification_opt: %empty  */
                                                        { yyval = NULL; }
    break;

  case 113: /* member_specification: member_specification member_declaration  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 114: /* member_declaration: decl_specifier_seq_opt member_declarator_list_opt ';'  */
                { yyval = yyvsp[0]; }
    break;

  case 115: /* member_declaration: error ';'  */
                { yyval = yyvsp[0]; }
    break;

  case 116: /* member_declarator_list_opt: %empty  */
                                                        { yyval = NULL; }
    break;

  case 119: /* member_declarator_list: member_declarator_list ',' member_declarator  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 120: /* member_declarator: nested_declarator attribute_opt  */
                                                        { yyval = yyvsp[0] ? yyvsp[0] : yyvsp[-1]; }
    break;

  case 121: /* member_declarator: IDENT member_bitfield_declarator  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 123: /* member_bitfield_declarator: ':' EXPRESSION_PHRASE  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 124: /* attribute_opt: %empty  */
                                                        { yyval = NULL; }
    break;

  case 126: /* enum_body: '{' enumerator_list '}'  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 127: /* enum_body: '{' enumerator_list ',' '}'  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 130: /* enumerator: IDENT  */
                {
			const char *name = strdup((*yyvsp[0])->string);
			add_symbol(name, SYM_ENUM_CONST, NULL, 0);
		}
    break;

  case 131: /* enumerator: IDENT '=' EXPRESSION_PHRASE  */
                {
			const char *name = strdup((*yyvsp[-2])->string);
			struct string_list *expr = copy_list_range(*yyvsp[0], *yyvsp[-1]);
			add_symbol(name, SYM_ENUM_CONST, expr, 0);
		}
    break;

  case 132: /* asm_definition: ASM_PHRASE ';'  */
                                                        { yyval = yyvsp[0]; }
    break;

  case 133: /* asm_phrase_opt: %empty  */
                                                        { yyval = NULL; }
    break;

  case 135: /* export_definition: EXPORT_SYMBOL_KEYW '(' IDENT ')' ';'  */
                { export_symbol((*yyvsp[-2])->string); yyval = yyvsp[0]; }
    break;

  case 136: /* static_assert: STATIC_ASSERT_PHRASE ';'  */
                                                        { yyval = yyvsp[0]; }
    break;



      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}



static void
yyerror(const char *e)
{
  error_with_pos("%s", e);
}
