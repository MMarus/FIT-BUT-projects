// bit-array.h
// Řešení IJC-DU1, příklad a), 20.3.2014
// Autor: Marek Marusic, FIT
// Přeloženo: gcc 4.8.2
// Hlavickovy subor pre vytvorenie a pracu s bitovymi polami
// Podla definicie USE_INLINE su pouzite funkcie makra alebo inline funkcie

#include"error.h"

#define Bits 8
typedef unsigned long BitArray_t;

void Eratosthenes(BitArray_t cisla[]);

#define BitArray(name,size)	BitArray_t name[size/(sizeof(BitArray_t)*Bits)+2] = {0};\
	name[0] = size;

/////////////-------------------NOT use inline-----------//////////
#ifndef USE_INLINE

#define BitArraySize(name) name[0]
            
#define GetBit(name,index) ((index < name[0]) ? DU1__GET_BIT(name,index)  : (FatalError("Index %ld mimo rozsah 0..%ld\n", (long)index, (long)name[0]),0) ) 

#define SetBit(name,index,b)\
	if(index < name[0])\
	{\
		if(b==0)\
			name[index/(sizeof(BitArray_t)*Bits)+1] &= ~(1UL << (index%(sizeof(BitArray_t)*Bits)));\
		else\
			name[index/(sizeof(BitArray_t)*Bits)+1] |= 1UL << (index%(sizeof(BitArray_t)*Bits));\
	}\
	else\
		FatalError("Index %ld mimo rozsah 0..%ld\n", (long)index, (long)name[0]);
		
#define DU1__GET_BIT(p,index)\
( (p[index/(sizeof(p[0])*Bits)+1] & (1UL << (index%(sizeof(p[0])*Bits)))) != 0 )

#define DU1__SET_BIT(p,index,b) \
 if(b==0)\
	p[index/(sizeof(p[0])*Bits)] &= ~(1UL << (index%(sizeof(p[0])*Bits)));\
	else\
	p[index/(sizeof(p[0])*Bits)] |= 1UL << (index%(sizeof(p[0])*Bits));\


//////////////////------------USE_INLINE----------//////////////
#else
inline void SetBit(BitArray_t pole[],int index,int b)
{
	if(index < pole[0])
	{
		if(b==0)
			pole[index/(sizeof(BitArray_t)*Bits)+1] &= ~(1UL << (index%(sizeof(BitArray_t)*Bits)));
		else
			pole[index/(sizeof(BitArray_t)*Bits)+1] |= 1UL << (index%(sizeof(BitArray_t)*Bits));
	}
	else
		FatalError("Index %ld mimo rozsah 0..%ld\n", (long)index, (long)pole[0]);
}

inline int GetBit(BitArray_t pole[],int index)
{
	return 	(index < pole[0]) ? ((pole[index/(sizeof(BitArray_t)*Bits)+1] & (1UL << (index%(sizeof(BitArray_t)*Bits)))) != 0 ) : (FatalError("Index %ld mimo rozsah 0..%ld\n", (long)index, (long)pole[0]),0);
 

}

inline BitArray_t BitArraySize(BitArray_t name[])
{
	return name[0];
}

#endif
