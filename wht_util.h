#ifndef __WHT_UTIL__
#define __WHT_UTIL__

//================================================
// @title        wht_util.h
// @author       Jonathan Hadida
// @contact      Jonathan.hadida [at] dtc.ox.ac.uk
//================================================

#include <cmath>
#include <vector>
#include <cstdio>
#include <iostream>



        /********************     **********     ********************/
        /********************     **********     ********************/



/**
 * Count the number of bits set.
 */
template <typename IntType>
unsigned bit_count( IntType n )
{
	unsigned c;
	for (c = 0; n; ++c)
		n &= n - 1;
	return c;
}

// ------------------------------------------------------------------------

/**
 * Reverse the bits (left-right) of an integer.
 */
template <typename IntType>
IntType bit_reverse( IntType n, unsigned nbits )
{
	IntType r = n;
	IntType l, m;

	// Compute shift and bitmask
	l = (IntType) (nbits - 1);
	m = (1 << nbits) - 1;

	// Permute
	while ( n >>= 1 )
	{
		r <<= 1;
		r |= n & 1;
		--l;
	}

	// Final shift and masking
	r <<= l;
	return r & m;
}

template <typename IntType>
IntType bit_reverse( IntType n )
{
	IntType r = n;
	IntType l = 8*sizeof(n)-1;

	// Permute
	while ( n >>= 1 )
	{
		r <<= 1;
		r |= n & 1;
		--l;
	}

	// Final shift
	return r <<= l;
}

// ------------------------------------------------------------------------

/**
 * Print the bits of an integer.
 */
template <typename IntType>
void bit_print( IntType n )
{
	static const unsigned b = 8*sizeof(IntType);
	static char buf[b+1]; buf[b] = '\0';

	std::cout<< n;
	for (unsigned m = b; m; n >>= 1, --m)
		buf[m-1] = (n & 1) + '0';
	printf(" = %s\n",buf);
}

// ------------------------------------------------------------------------

/**
 * Gray codes transforms.
 */
template <typename IntType>
IntType bin2gray( IntType n )
{
	return (n >> 1) ^ n;
}

template <typename IntType>
IntType gray2bin( IntType n )
{
	for ( IntType m = n; m >>= 1; n = n ^ m );
	return n;
}

// ------------------------------------------------------------------------

/**
 * Integer logarithm base 2.
 */
template <typename IntType>
unsigned ilog2( IntType n )
{
	unsigned l;
	for ( l = 0; n; n >>= 1, ++l );
	return l;
}

// ------------------------------------------------------------------------

/**
 * Rotate qbit
 */
template <typename T>
void rotate( T& a, T& b )
{
	static T A;
	A = a;
	a = A + b;
	b = A - b;
}

// ------------------------------------------------------------------------

template <typename IntType>
void fwht_sequency_permutation( std::vector<IntType>& perm, unsigned order )
{
	if ( perm.size() == (size_t)(1 << order) ) return;

	perm.resize(1 << order); IntType k = 0;
	for ( auto& p: perm ) p = bit_reverse(bin2gray(k++),order);
}

// ------------------------------------------------------------------------

/**
 * Fast Walsh-Hadamard transforms
 */
template <typename T>
void fwht( std::vector<T>& data, bool sequency_ordered = false )
{
	unsigned l2 = ilog2(data.size());
	if ( data.size() == (size_t)(1 << (l2-1)) ) --l2;
	unsigned p2 = (1 << l2);

	// 0-padding to the next power of 2
	data.resize(p2,T(0));

	// Compute the WHT
	for ( unsigned i = 0; i < l2; ++i )
	{
		for ( unsigned j = 0; j < p2; j += (1 << (i+1)) )
		for ( unsigned k = 0; k < (unsigned)(1 << i); ++k )
			rotate( data[j+k], data[j+ k + (unsigned)(1<<i)] );
	}

	static std::vector<unsigned> perm;
	if ( sequency_ordered )
	{
		// Compute permutation
		fwht_sequency_permutation( perm, l2 );

		// Copy transform
		std::vector<T> tmp = data;		
		for ( unsigned i = 0; i < p2; ++i )
			data[i] = tmp[perm[i]] / p2;
	}
	else for ( auto& d: data ) d /= p2;
}

#endif