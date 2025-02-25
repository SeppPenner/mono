/**
 * \file
 */

#ifndef __INTERPRETER_MINTOPS_H
#define __INTERPRETER_MINTOPS_H

#include <glib.h>

typedef enum
{
	MintOpNoArgs,
	MintOpShortInt,
	MintOpUShortInt,
	MintOpInt,
	MintOpLongInt,
	MintOpFloat,
	MintOpDouble,
	MintOpBranch,
	MintOpShortBranch,
	MintOpSwitch,
	MintOpMethodToken,
	MintOpFieldToken,
	MintOpClassToken,
	MintOpTwoShorts,
	MintOpShortAndInt
} MintOpArgType;

#define OPDEF(a,b,c,d,e,f) a,
enum {
#include "mintops.def"
	MINT_LASTOP
};
#undef OPDEF

#if NO_UNALIGNED_ACCESS
#  if G_BYTE_ORDER == G_LITTLE_ENDIAN
#define READ32(x) (((guint16 *)(x)) [0] | ((guint16 *)(x)) [1] << 16)
#define READ64(x) ((guint64)((guint16 *)(x)) [0] | \
                   (guint64)((guint16 *)(x)) [1] << 16 | \
                   (guint64)((guint16 *)(x)) [2] << 32 | \
                   (guint64)((guint16 *)(x)) [3] << 48)
#  else
#define READ32(x) (((guint16 *)(x)) [0] << 16 | ((guint16 *)(x)) [1])
#define READ64(x) ((guint64)((guint16 *)(x)) [0] << 48 | \
                   (guint64)((guint16 *)(x)) [1] << 32 | \
                   (guint64)((guint16 *)(x)) [2] << 16 | \
                   (guint64)((guint16 *)(x)) [3])
#  endif
#else /* unaligned access OK */
#define READ32(x) (*(guint32 *)(x))
#define READ64(x) (*(guint64 *)(x))
#endif

#define MINT_SWITCH_LEN(n) (3 + (n) * 2)

#define MINT_IS_LDLOC(op) ((op) >= MINT_LDLOC_I1 && (op) <= MINT_LDLOC_VT)
#define MINT_IS_STLOC(op) ((op) >= MINT_STLOC_I1 && (op) <= MINT_STLOC_VT)
#define MINT_IS_MOVLOC(op) ((op) >= MINT_MOVLOC_1 && (op) <= MINT_MOVLOC_VT)
#define MINT_IS_STLOC_NP(op) ((op) >= MINT_STLOC_NP_I4 && (op) <= MINT_STLOC_NP_O)

#define MINT_POP_ALL	-2
#define MINT_VAR_PUSH	-1
#define MINT_VAR_POP	-1

extern unsigned char const mono_interp_oplen[];
extern int const mono_interp_oppop[];
extern int const mono_interp_oppush[];
extern MintOpArgType const mono_interp_opargtype[];
extern char* mono_interp_dis_mintop (const unsigned short *base, const guint16 *ip);
extern const guint16* mono_interp_dis_mintop_len (const guint16 *ip);

// This, instead of an array of pointers, to optimize away a pointer and a relocation per string.
extern const guint16 mono_interp_opname_offsets [ ];
typedef struct MonoInterpOpnameCharacters MonoInterpOpnameCharacters;
extern const MonoInterpOpnameCharacters mono_interp_opname_characters;

const char*
mono_interp_opname (int op);

#endif
