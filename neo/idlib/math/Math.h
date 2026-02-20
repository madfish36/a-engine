/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __MATH_MATH_H__
#define __MATH_MATH_H__

/*
===============================================================================

  Math

===============================================================================
*/

#ifdef INFINITY
#undef INFINITY
#endif

#ifdef FLT_EPSILON
#undef FLT_EPSILON
#endif

#define DEG2RAD(a)				( (a) * idMath::M_DEG2RAD )
#define RAD2DEG(a)				( (a) * idMath::M_RAD2DEG )

#define SEC2MS(t)				( idMath::Ftoi( (t) * idMath::M_SEC2MS ) )
#define MS2SEC(t)				( (t) * idMath::M_MS2SEC )

#define	ANGLE2SHORT(x)			( idMath::Ftoi( (x) * 65536.0f / 360.0f ) & 65535 )
#define	SHORT2ANGLE(x)			( (x) * ( 360.0f / 65536.0f ) )

#define	ANGLE2BYTE(x)			( idMath::Ftoi( (x) * 256.0f / 360.0f ) & 255 )
#define	BYTE2ANGLE(x)			( (x) * ( 360.0f / 256.0f ) )

#define C_FLOAT_TO_INT( x )		(int)(x)

/*
================================================================================================

	two-complements integer bit layouts

================================================================================================
*/

#define INT8_SIGN_BIT		7
#define INT16_SIGN_BIT		15
#define INT32_SIGN_BIT		31
#define INT64_SIGN_BIT		63

#define INT8_SIGN_MASK		( 1 << INT8_SIGN_BIT )
#define INT16_SIGN_MASK		( 1 << INT16_SIGN_BIT )
#define INT32_SIGN_MASK		( 1UL << INT32_SIGN_BIT )
#define INT64_SIGN_MASK		( 1ULL << INT64_SIGN_BIT )

/*
================================================================================================

	integer sign bit tests

================================================================================================
*/

// If this was ever compiled on a system that had 64 bit unsigned ints,
// it would fail.
compile_time_assert( sizeof( unsigned int ) == 4 );

#define OLD_INT32_SIGNBITSET(i)		(static_cast<const unsigned int>(i) >> INT32_SIGN_BIT)
#define OLD_INT32_SIGNBITNOTSET(i)	((~static_cast<const unsigned int>(i)) >> INT32_SIGN_BIT)

// Unfortunately, /analyze can't figure out that these always return
// either 0 or 1, so this extra wrapper is needed to avoid the static
// alaysis warning.

ID_INLINE_EXTERN int INT32_SIGNBITSET( int i ) {
	int	r = OLD_INT32_SIGNBITSET( i );
	assert( r == 0 || r == 1 );
	return r;
}

ID_INLINE_EXTERN int INT32_SIGNBITNOTSET( int i ) {
	int	r = OLD_INT32_SIGNBITNOTSET( i );
	assert( r == 0 || r == 1 );
	return r;
}

/*
================================================================================================

	floating point bit layouts according to the IEEE 754-1985 and 754-2008 standard

================================================================================================
*/

#define IEEE_FLT16_MANTISSA_BITS	10
#define IEEE_FLT16_EXPONENT_BITS	5
#define IEEE_FLT16_EXPONENT_BIAS	15
#define IEEE_FLT16_SIGN_BIT			15
#define IEEE_FLT16_SIGN_MASK		( 1U << IEEE_FLT16_SIGN_BIT )

#define IEEE_FLT_MANTISSA_BITS		23
#define IEEE_FLT_EXPONENT_BITS		8
#define IEEE_FLT_EXPONENT_BIAS		127
#define IEEE_FLT_SIGN_BIT			31
#define IEEE_FLT_SIGN_MASK			( 1UL << IEEE_FLT_SIGN_BIT )

#define IEEE_DBL_MANTISSA_BITS		52
#define IEEE_DBL_EXPONENT_BITS		11
#define IEEE_DBL_EXPONENT_BIAS		1023
#define IEEE_DBL_SIGN_BIT			63
#define IEEE_DBL_SIGN_MASK			( 1ULL << IEEE_DBL_SIGN_BIT )

#define IEEE_DBLE_MANTISSA_BITS		63
#define IEEE_DBLE_EXPONENT_BITS		15
#define IEEE_DBLE_EXPONENT_BIAS		0
#define IEEE_DBLE_SIGN_BIT			79

/*
================================================================================================

	floating point sign bit tests

================================================================================================
*/

#define IEEE_FLT_SIGNBITSET( a )	(reinterpret_cast<const unsigned int &>(a) >> IEEE_FLT_SIGN_BIT)
#define IEEE_FLT_SIGNBITNOTSET( a )	((~reinterpret_cast<const unsigned int &>(a)) >> IEEE_FLT_SIGN_BIT)
#define IEEE_FLT_ISNOTZERO( a )		(reinterpret_cast<const unsigned int &>(a) & ~(1u<<IEEE_FLT_SIGN_BIT))

/*
================================================================================================

	floating point special value tests

================================================================================================
*/

/*
========================
IEEE_FLT_IS_NAN
========================
*/
ID_INLINE_EXTERN bool IEEE_FLT_IS_NAN( float x ) {
	return x != x;
}

/*
========================
IEEE_FLT_IS_INF
========================
*/
ID_INLINE_EXTERN bool IEEE_FLT_IS_INF( float x ) {
	return x == x && x * 0 != x * 0;
}

/*
========================
IEEE_FLT_IS_INF_NAN
========================
*/
ID_INLINE_EXTERN bool IEEE_FLT_IS_INF_NAN( float x ) {
	return x * 0 != x * 0;
}

/*
========================
IEEE_FLT_IS_IND
========================
*/
ID_INLINE_EXTERN bool IEEE_FLT_IS_IND( float x ) {
	return	(reinterpret_cast<const unsigned int &>(x) == 0xffc00000); 
}

/*
========================
IEEE_FLT_IS_DENORMAL
========================
*/
ID_INLINE_EXTERN bool IEEE_FLT_IS_DENORMAL( float x ) {
	return ((reinterpret_cast<const unsigned int &>(x) & 0x7f800000) == 0x00000000 &&
			(reinterpret_cast<const unsigned int &>(x) & 0x007fffff) != 0x00000000 ); 
}


/*
========================
IsNAN
========================
*/template<class type>
ID_INLINE_EXTERN bool IsNAN( const type &v ) {
	for ( int i = 0; i < v.GetDimension(); i++ ) {
		const float f = v.ToFloatPtr()[i];
		if ( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) ) {
			return true;
		}
	}
	return false;
}

/*
========================
IsValid
========================
*/
template<class type>
ID_INLINE_EXTERN bool IsValid( const type &v ) {
	for ( int i = 0; i < v.GetDimension(); i++ ) {
		const float f = v.ToFloatPtr()[i];
		if ( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) || IEEE_FLT_IS_DENORMAL( f ) ) {
			return false;
		}
	}
	return true;
}

/*
========================
IsValid
========================
*/
template<>
ID_INLINE_EXTERN bool IsValid( const float & f ) {	// these parameter must be a reference for the function to be considered a specialization
	return !( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) || IEEE_FLT_IS_DENORMAL( f ) );
}

/*
========================
IsNAN
========================
*/
template<>
ID_INLINE_EXTERN bool IsNAN( const float & f ) {	// these parameter must be a reference for the function to be considered a specialization
	if ( IEEE_FLT_IS_NAN( f ) || IEEE_FLT_IS_INF( f ) || IEEE_FLT_IS_IND( f ) ) {
		return true;
	}
	return false;
}

/*
========================
IsInRange

Returns true if any scalar is greater than the range or less than the negative range.
========================
*/
template<class type>
ID_INLINE_EXTERN bool IsInRange( const type &v, const float range ) {
	for ( int i = 0; i < v.GetDimension(); i++ ) {
		const float f = v.ToFloatPtr()[i];
		if ( f > range || f < -range ) {
			return false;
		}
	}
	return true;
}


/*
================================================================================================

	MinIndex/MaxIndex

================================================================================================
*/
template<class T> ID_INLINE int	MaxIndex( T x, T y ) { return  ( x > y ) ? 0 : 1; }
template<class T> ID_INLINE int	MinIndex( T x, T y ) { return ( x < y ) ? 0 : 1; }

template<class T> ID_INLINE T	Max3( T x, T y, T z ) { return ( x > y ) ? ( ( x > z ) ? x : z ) : ( ( y > z ) ? y : z ); }
template<class T> ID_INLINE T	Min3( T x, T y, T z ) { return ( x < y ) ? ( ( x < z ) ? x : z ) : ( ( y < z ) ? y : z ); }
template<class T> ID_INLINE int	Max3Index( T x, T y, T z ) { return ( x > y ) ? ( ( x > z ) ? 0 : 2 ) : ( ( y > z ) ? 1 : 2 ); }
template<class T> ID_INLINE int	Min3Index( T x, T y, T z ) { return ( x < y ) ? ( ( x < z ) ? 0 : 2 ) : ( ( y < z ) ? 1 : 2 ); }

/*
================================================================================================

	Sign/Square/Cube

================================================================================================
*/
template<class T> ID_INLINE T	Sign( T f ) { return ( f > 0 ) ? 1 : ( ( f < 0 ) ? -1 : 0 ); }
template<class T> ID_INLINE T	Square( T x ) { return x * x; }
template<class T> ID_INLINE T	Cube( T x ) { return x * x * x; }

class idMath {
public:

	static void					Init();

	static float				InvSqrt( float x );			// inverse square root with 32 bits precision, returns huge number when x == 0.0

	static float				Sqrt( float x );			// square root with 32 bits precision

	static float				Sin( float a );				// sine with 32 bits precision

	static float				Cos( float a );				// cosine with 32 bits precision

	static void					SinCos( float a, float &s, float &c );		// sine and cosine with 32 bits precision

	static float				Tan( float a );				// tangent with 32 bits precision

	static float				ASin( float a );			// arc sine with 32 bits precision, input is clamped to [-1, 1] to avoid a silent NaN

	static float				ACos( float a );			// arc cosine with 32 bits precision, input is clamped to [-1, 1] to avoid a silent NaN

	static float				ATan( float a );			// arc tangent with 32 bits precision

	static float				ATan( float y, float x );	// arc tangent with 32 bits precision

	static float				Pow( float x, float y );	// x raised to the power y with 32 bits precision

	static float				Exp( float f );				// e raised to the power f with 32 bits precision

	static float				Log( float f );				// natural logarithm with 32 bits precision

	static int					IPow( int x, int y );		// integral x raised to the power y
	static int					ILog2( float f );			// integral base-2 logarithm of the floating point value
	static int					ILog2( int i );				// integral base-2 logarithm of the integer value

	static int					BitsForFloat( float f );	// minumum number of bits required to represent ceil( f )
	static int					BitsForInteger( int i );	// minumum number of bits required to represent i
	static int					MaskForFloatSign( float f );// returns 0x00000000 if x >= 0.0f and returns 0xFFFFFFFF if x <= -0.0f
	static int					MaskForIntegerSign( int i );// returns 0x00000000 if x >= 0 and returns 0xFFFFFFFF if x < 0
	static int					FloorPowerOfTwo( int x );	// round x down to the nearest power of 2
	static int					CeilPowerOfTwo( int x );	// round x up to the nearest power of 2
	static bool					IsPowerOfTwo( int x );		// returns true if x is a power of 2
	static int					BitCount( int x );			// returns the number of 1 bits in x
	static int					BitReverse( int x );		// returns the bit reverse of x

	static int					Abs( int x );				// returns the absolute value of the integer value (for reference only)
	static float				Fabs( float f );			// returns the absolute value of the floating point value
	static float				Floor( float f );			// returns the largest integer that is less than or equal to the given value
	static float				Ceil( float f );			// returns the smallest integer that is greater than or equal to the given value
	static float				Rint( float f );			// returns the nearest integer

	static float				Frac( float f );			// f - Floor( f )

	static int					Ftoi( float f );			// float to int conversion
	static char					Ftoi8( float f );			// float to char conversion
	static short				Ftoi16( float f );			// float to short conversion
	static unsigned short		Ftoui16( float f );			// float to unsigned short conversion
	static byte					Ftob( float f );			// float to byte conversion, the result is clamped to the range [0-255]

	static signed char			ClampChar( int i );
	static signed short			ClampShort( int i );
	static int					ClampInt( int min, int max, int value );
	static float				ClampFloat( float min, float max, float value );

	static float				AngleNormalize360( float angle );
	static float				AngleNormalize180( float angle );
	static float				AngleDelta( float angle1, float angle2 );

	static int					FloatToBits( float f, int exponentBits, int mantissaBits );
	static float				BitsToFloat( int i, int exponentBits, int mantissaBits );

	static int					FloatHash( const float *array, const int numFloats );

	static float				LerpToWithScale( const float cur, const float dest, const float scale );

	static const float			PI;							// pi
	static const float			TWO_PI;						// pi * 2
	static const float			HALF_PI;					// pi / 2
	static const float			ONEFOURTH_PI;				// pi / 4
	static const float			ONEOVER_PI;					// 1 / pi
	static const float			ONEOVER_TWOPI;				// 1 / pi * 2
	static const float			E;							// e
	static const float			SQRT_TWO;					// sqrt( 2 )
	static const float			SQRT_THREE;					// sqrt( 3 )
	static const float			SQRT_1OVER2;				// sqrt( 1 / 2 )
	static const float			SQRT_1OVER3;				// sqrt( 1 / 3 )
	static const float			M_DEG2RAD;					// degrees to radians multiplier
	static const float			M_RAD2DEG;					// radians to degrees multiplier
	static const float			M_SEC2MS;					// seconds to milliseconds multiplier
	static const float			M_MS2SEC;					// milliseconds to seconds multiplier
	static const float			INFINITY;					// huge number which should be larger than any valid number used
	static const float			FLT_EPSILON;				// smallest positive number such that 1.0+FLT_EPSILON != 1.0
	static const float			FLT_SMALLEST_NON_DENORMAL;	// smallest non-denormal 32-bit floating point value

	static const __m128			SIMD_SP_zero;
	static const __m128			SIMD_SP_255;
	static const __m128			SIMD_SP_min_char;
	static const __m128			SIMD_SP_max_char;
	static const __m128			SIMD_SP_min_short;
	static const __m128			SIMD_SP_max_short;
	static const __m128			SIMD_SP_smallestNonDenorm;
	static const __m128			SIMD_SP_tiny;
	static const __m128			SIMD_SP_rsqrt_c0;
	static const __m128			SIMD_SP_rsqrt_c1;

private:
	enum {
		LOOKUP_BITS				= 8,							
		EXP_POS					= 23,							
		EXP_BIAS				= 127,							
		LOOKUP_POS				= (EXP_POS-LOOKUP_BITS),
		SEED_POS				= (EXP_POS-8),
		SQRT_TABLE_SIZE			= (2<<LOOKUP_BITS),
		LOOKUP_MASK				= (SQRT_TABLE_SIZE-1)
	};

	union _flint {
		dword					i;
		float					f;
	};

	static dword				iSqrt[SQRT_TABLE_SIZE];
	static bool					initialized;
};

ID_INLINE byte CLAMP_BYTE( int x )	{ 
	return ( (x) < 0 ? (0) : ( (x) > 255 ? 255 : (byte)(x) ) ); 
}

/*
========================
idMath::InvSqrt
========================
*/
ID_INLINE float idMath::InvSqrt( float x ) {
	return ( x > FLT_SMALLEST_NON_DENORMAL ) ? sqrtf( 1.0f / x ) : INFINITY;
}


/*
========================
idMath::Sqrt
========================
*/
ID_INLINE float idMath::Sqrt( float x ) {
	return ( x >= 0.0f ) ?  x * InvSqrt( x ) : 0.0f;
}


/*
========================
idMath::Frac
========================
*/
ID_INLINE float idMath::Frac( float f ) {
	return f - floorf( f );
}

/*
========================
idMath::Sin
========================
*/
ID_INLINE float idMath::Sin( float a ) {
	return sinf( a );
}


/*
========================
idMath::Cos
========================
*/
ID_INLINE float idMath::Cos( float a ) {
	return cosf( a );
}

/*
========================
idMath::SinCos
========================
*/
ID_INLINE void idMath::SinCos( float a, float &s, float &c ) {
	s = sinf( a );
	c = cosf( a );
}

/*
========================
idMath::Tan
========================
*/
ID_INLINE float idMath::Tan( float a ) {
	return tanf( a );
}

/*
========================
idMath::ASin
========================
*/
ID_INLINE float idMath::ASin( float a ) {
	if ( a <= -1.0f ) {
		return -HALF_PI;
	}
	if ( a >= 1.0f ) {
		return HALF_PI;
	}
	return asinf( a );
}

/*
========================
idMath::ACos
========================
*/
ID_INLINE float idMath::ACos( float a ) {
	if ( a <= -1.0f ) {
		return PI;
	}
	if ( a >= 1.0f ) {
		return 0.0f;
	}
	return acosf( a );
}

/*
========================
idMath::ATan
========================
*/
ID_INLINE float idMath::ATan( float a ) {
	return atanf( a );
}


/*
========================
idMath::ATan
========================
*/
ID_INLINE float idMath::ATan( float y, float x ) {
	assert( fabs( y ) > idMath::FLT_SMALLEST_NON_DENORMAL || fabs( x ) > idMath::FLT_SMALLEST_NON_DENORMAL );
	return atan2f( y, x );
}

/*
========================
idMath::Pow
========================
*/
ID_INLINE float idMath::Pow( float x, float y ) {
	return powf( x, y );
}

/*
========================
idMath::Exp
========================
*/
ID_INLINE float idMath::Exp( float f ) {
	return expf( f );
}

/*
========================
idMath::Log
========================
*/
ID_INLINE float idMath::Log( float f ) {
	return logf( f );
}

/*
========================
idMath::IPow
========================
*/
ID_INLINE int idMath::IPow( int x, int y ) {
	int r; for( r = x; y > 1; y-- ) { r *= x; } return r;
}

/*
========================
idMath::ILog2
========================
*/
ID_INLINE int idMath::ILog2( float f ) {
	return ( ( (*reinterpret_cast<int *>(&f)) >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
}

/*
========================
idMath::ILog2
========================
*/
ID_INLINE int idMath::ILog2( int i ) {
	return ILog2( (float)i );
}

/*
========================
idMath::BitsForFloat
========================
*/
ID_INLINE int idMath::BitsForFloat( float f ) {
	return ILog2( f ) + 1;
}

/*
========================
idMath::BitsForInteger
========================
*/
ID_INLINE int idMath::BitsForInteger( int i ) {
	return ILog2( (float)i ) + 1;
}

/*
========================
idMath::MaskForFloatSign
========================
*/
ID_INLINE int idMath::MaskForFloatSign( float f ) {
	return ( (*reinterpret_cast<int *>(&f)) >> IEEE_FLT_SIGN_BIT );
}

/*
========================
idMath::MaskForIntegerSign
========================
*/
ID_INLINE int idMath::MaskForIntegerSign( int i ) {
	return ( i >> INT32_SIGN_BIT );
}

/*
========================
idMath::FloorPowerOfTwo
========================
*/
ID_INLINE int idMath::FloorPowerOfTwo( int x ) {
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return x >> 1;
}

/*
========================
idMath::CeilPowerOfTwo
========================
*/
ID_INLINE int idMath::CeilPowerOfTwo( int x ) {
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return x;
}

/*
========================
idMath::IsPowerOfTwo
========================
*/
ID_INLINE bool idMath::IsPowerOfTwo( int x ) {
	return ( x & ( x - 1 ) ) == 0 && x > 0;
}

/*
========================
idMath::BitCount
========================
*/
ID_INLINE int idMath::BitCount( int x ) {
	x -= ( ( x >> 1 ) & 0x55555555 );
	x = ( ( ( x >> 2 ) & 0x33333333 ) + ( x & 0x33333333 ) );
	x = ( ( ( x >> 4 ) + x ) & 0x0f0f0f0f );
	x += ( x >> 8 );
	return ( ( x + ( x >> 16 ) ) & 0x0000003f );
}

/*
========================
idMath::BitReverse
========================
*/
ID_INLINE int idMath::BitReverse( int x ) {
	x = ( ( ( x >> 1 ) & 0x55555555 ) | ( ( x & 0x55555555 ) << 1 ) );
	x = ( ( ( x >> 2 ) & 0x33333333 ) | ( ( x & 0x33333333 ) << 2 ) );
	x = ( ( ( x >> 4 ) & 0x0f0f0f0f ) | ( ( x & 0x0f0f0f0f ) << 4 ) );
	x = ( ( ( x >> 8 ) & 0x00ff00ff ) | ( ( x & 0x00ff00ff ) << 8 ) );
	return ( ( x >> 16 ) | ( x << 16 ) );
}

/*
========================
idMath::Abs
========================
*/
ID_INLINE int idMath::Abs( int x ) {
	return abs( x );
}

/*
========================
idMath::Fabs
========================
*/
ID_INLINE float idMath::Fabs( float f ) {
	return fabsf( f );
}

/*
========================
idMath::Floor
========================
*/
ID_INLINE float idMath::Floor( float f ) {
	return floorf( f );
}

/*
========================
idMath::Ceil
========================
*/
ID_INLINE float idMath::Ceil( float f ) {
	return ceilf( f );
}

/*
========================
idMath::Rint
========================
*/
ID_INLINE float idMath::Rint( float f ) {
	return floorf( f + 0.5f );
}


/*
========================
idMath::Ftoi
========================
*/
ID_INLINE int idMath::Ftoi( float f ) {
	// If a converted result is larger than the maximum signed doubleword integer,
	// the floating-point invalid exception is raised, and if this exception is masked,
	// the indefinite integer value (80000000H) is returned.
	__m128 x = _mm_load_ss( &f );
	return _mm_cvttss_si32( x );
}

/*
========================
idMath::Ftoi8
========================
*/
ID_INLINE char idMath::Ftoi8( float f ) {
	__m128 x = _mm_load_ss( &f );
	x = _mm_max_ss( x, SIMD_SP_min_char );
	x = _mm_min_ss( x, SIMD_SP_max_char );
	return static_cast<char>( _mm_cvttss_si32( x ) );
}

/*
========================
idMath::Ftoi16
========================
*/
ID_INLINE short idMath::Ftoi16( float f ) {
	__m128 x = _mm_load_ss( &f );
	x = _mm_max_ss( x, SIMD_SP_min_short );
	x = _mm_min_ss( x, SIMD_SP_max_short );
	return static_cast<short>( _mm_cvttss_si32( x ) );
}

/*
========================
idMath::Ftoui16
========================
*/
ID_INLINE unsigned short idMath::Ftoui16( float f ) {
	// TO DO - SSE ??

	// The converted result is clamped to the range [-32768,32767].
	int i = C_FLOAT_TO_INT( f );
	if ( i < 0 ) {
		return 0;
	} else if ( i > 65535 ) {
		return 65535;
	}
	return static_cast<unsigned short>( i );
}

/*
========================
idMath::Ftob
========================
*/
ID_INLINE byte idMath::Ftob( float f ) {
	// If a converted result is negative the value (0) is returned and if the
	// converted result is larger than the maximum byte the value (255) is returned.
	__m128 x = _mm_load_ss( &f );
	x = _mm_max_ss( x, SIMD_SP_zero );
	x = _mm_min_ss( x, SIMD_SP_255 );
	return static_cast<byte>( _mm_cvttss_si32( x ) );
}

/*
========================
idMath::ClampChar
========================
*/
ID_INLINE signed char idMath::ClampChar( int i ) {
	if ( i < -128 ) {
		return -128;
	}
	if ( i > 127 ) {
		return 127;
	}
	return static_cast<signed char>( i );
}

/*
========================
idMath::ClampShort
========================
*/
ID_INLINE signed short idMath::ClampShort( int i ) {
	if ( i < -32768 ) {
		return -32768;
	}
	if ( i > 32767 ) {
		return 32767;
	}
	return static_cast<signed short>( i );
}

/*
========================
idMath::ClampInt
========================
*/
ID_INLINE int idMath::ClampInt( int min, int max, int value ) {
	if ( value < min ) {
		return min;
	}
	if ( value > max ) {
		return max;
	}
	return value;
}

/*
========================
idMath::ClampFloat
========================
*/
ID_INLINE float idMath::ClampFloat( float min, float max, float value ) {
	return Max( min, Min( max, value ) );
}

/*
========================
idMath::AngleNormalize360
========================
*/
ID_INLINE float idMath::AngleNormalize360( float angle ) {
	if ( ( angle >= 360.0f ) || ( angle < 0.0f ) ) {
		angle -= floorf( angle * ( 1.0f / 360.0f ) ) * 360.0f;
	}
	return angle;
}

/*
========================
idMath::AngleNormalize180
========================
*/
ID_INLINE float idMath::AngleNormalize180( float angle ) {
	angle = AngleNormalize360( angle );
	if ( angle > 180.0f ) {
		angle -= 360.0f;
	}
	return angle;
}

/*
========================
idMath::AngleDelta
========================
*/
ID_INLINE float idMath::AngleDelta( float angle1, float angle2 ) {
	return AngleNormalize180( angle1 - angle2 );
}

/*
========================
idMath::FloatHash
========================
*/
ID_INLINE int idMath::FloatHash( const float *array, const int numFloats ) {
	int i, hash = 0;
	const int *ptr;

	ptr = reinterpret_cast<const int *>( array );
	for ( i = 0; i < numFloats; i++ ) {
		hash ^= ptr[i];
	}
	return hash;
}

template< typename T >
ID_INLINE_EXTERN T Lerp( const T from, const T to, float f ) { 
	return from + ( ( to - from ) * f );
}

template<>
ID_INLINE_EXTERN int Lerp( const int from, const int to, float f ) { 
	return idMath::Ftoi( (float) from + ( ( (float) to - (float) from ) * f ) );
}


/*
========================
LerpToWithScale

Lerps from "cur" to "dest", scaling the delta to change by "scale"
If the delta between "cur" and "dest" is very small, dest is returned to prevent denormals.
========================
*/
inline float idMath::LerpToWithScale( const float cur, const float dest, const float scale ) {
	float delta = dest - cur;
	if ( delta > -1.0e-6f && delta < 1.0e-6f ) {
		return dest;
	}
	return cur + ( dest - cur ) * scale;
}


#endif /* !__MATH_MATH_H__ */
