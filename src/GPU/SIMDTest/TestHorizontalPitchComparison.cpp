#include "stdafx.h"
#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <cstdint>
#include <emmintrin.h>
#include "StopWatch.hpp"

using namespace std;

BOOST_AUTO_TEST_SUITE( TestHorizontalPitchComparison )



uint8_t PlainC_HorizontalPitch( 
	uint8_t center,
	uint8_t leftEast, uint8_t leftRight, uint8_t rightEast, uint8_t rightWest, 
	double pitch )
{
	uint16_t toFloor = (uint16_t)( ((pitch - (int)pitch) * 0x40)+0.5);
	uint16_t toCeiling = 0x40 - toFloor;

	uint16_t left = toFloor * leftEast + toCeiling * leftRight;
	uint16_t right = toCeiling * rightEast +  toFloor * rightWest;

	uint16_t c = center * 0x40;
	uint16_t diff1 = std::max( 0, c-left ) + std::max( 0, left-c );
	uint16_t diff2 = std::max( 0, c-right ) + std::max( 0, right-c );
	uint8_t diff = std::min( 255, ( diff1 + diff2 ) >> 6 );
	return diff ;
}	


BOOST_AUTO_TEST_CASE( Test_PlainC_HorizontalPitch )
{  
	uint8_t maxError = 1;

	BOOST_CHECK( PlainC_HorizontalPitch( 80, 100, 0, 0, 100, 10.8 ) <= maxError );
	BOOST_CHECK( ( PlainC_HorizontalPitch( 40, 100, 0, 0, 100, 10.8 ) - 80 ) <= maxError );

	BOOST_CHECK( PlainC_HorizontalPitch( 100, 90, 110, 110, 90, 10.5 ) <= maxError );
	BOOST_CHECK( PlainC_HorizontalPitch( 100, 50, 150, 150, 50, 10.5 ) <= maxError );
	BOOST_CHECK( PlainC_HorizontalPitch( 100, 0, 200, 200, 0, 10.5 ) <= maxError );
	BOOST_CHECK( PlainC_HorizontalPitch( 127, 0, 255, 255, 0, 10.5 ) <= maxError );
	BOOST_CHECK( PlainC_HorizontalPitch( 128, 128, 0, 0, 128, 10.999f ) <= maxError );
	BOOST_CHECK( PlainC_HorizontalPitch( 128, 0, 128, 128, 0, 10.0f ) <= maxError );
	BOOST_CHECK( PlainC_HorizontalPitch( 0, 0, 0, 0, 0, 10.0f ) <= maxError );
	BOOST_CHECK( PlainC_HorizontalPitch( 255, 255, 255, 255, 255, 10.5f ) <= maxError );

	BOOST_CHECK( abs( PlainC_HorizontalPitch( 50, 50, 150, 150, 50, 10.5f ) - 100 ) <= maxError );
	BOOST_CHECK( abs( PlainC_HorizontalPitch( 150, 50, 150, 150, 50, 10.5f ) - 100 ) <= maxError );
	BOOST_CHECK( abs( PlainC_HorizontalPitch( 50, 0, 0, 0, 0, 10.5f ) - 100 ) <= maxError );
	BOOST_CHECK( abs( PlainC_HorizontalPitch( 205, 255, 255, 255, 255, 10.5f ) - 100 ) <= maxError );
	BOOST_CHECK( abs( PlainC_HorizontalPitch( 0, 255, 255, 255, 255, 10.5f ) - 255 ) <= maxError );
	BOOST_CHECK( abs( PlainC_HorizontalPitch( 255, 0, 0, 0, 0, 10.5f ) - 255 ) <= maxError );
}


class SSE
{
public:
	static const int SizeOfDataType;
};

const int SSE::SizeOfDataType = 16;


#ifdef _WIN64 
#else
/*
	SSE4.1 instructions use
		PMOVSXBW
*/

void SSE_Pitch0( 
	const uint8_t *h_src, uint8_t *h_dst, 
	int width, int height, 
	int roiLeft, int roiTop, int roiRight, int roiBottom,
	double horPitch )
{
	int sizeOfDataTypeInWords = SSE::SizeOfDataType / 2;
	int integerPitch = (int)horPitch;

	uint16_t toFloor = (uint16_t)(((horPitch - (int)horPitch) * 0x40) + 0.5 );
	uint16_t toCeiling = 0x40 - toFloor;

	std::vector<uint16_t> toFloors( sizeOfDataTypeInWords, toFloor );
	std::vector<uint16_t> toCeilings( sizeOfDataTypeInWords, toCeiling );
	const uint16_t* toFloorsPtr = &toFloors[0];
	const uint16_t* toCeilingsPtr = &toCeilings[0];

	__asm {
		MOV			ECX, toFloorsPtr	
		MOVDQU		XMM6, [ECX]
		MOV			EDX, toCeilingsPtr
		MOVDQU		XMM7, [EDX]
	}

	for ( int y=roiTop; y<=roiBottom; y++ ) 
	{
		for ( int x=roiLeft; x<=roiRight; x+=sizeOfDataTypeInWords ) 
		{
			const uint8_t *source = h_src + width*y+x;
			uint8_t *target = h_dst + width*y+x;
			
			__asm {

				MOV			ECX, source	
				SUB			ECX, integerPitch	// left pitch pixel
				PMOVZXBW	XMM1, [ECX-1]	// east
				PMOVZXBW	XMM2, [ECX]		// west

				PMULLW		XMM1, XMM6	
				PMULLW		XMM2, XMM7
				PADDUSW		XMM1, XMM2	

				MOV			ECX, source	
				ADD			ECX, integerPitch	// right pitch pixel

				PMOVZXBW	XMM2, [ECX]		// east
				PMOVZXBW	XMM3, [ECX+1]	// west

				PMULLW		XMM2, XMM7	
				PMULLW		XMM3, XMM6
				PADDUSW		XMM2, XMM3

				MOV			ECX, source;	
				PMOVZXBW	XMM0, [ECX]		// XMM0 = I(n) | ... | I(n+7)
				PSLLW		XMM0, 6
				MOVDQU		XMM4, XMM0
				MOVDQU		XMM5, XMM0

				PSUBUSW		XMM4, XMM1		
				PSUBUSW		XMM1, XMM0		
				PADDUSW		XMM4, XMM1		// XMM4 = |C - L| 

				PSUBUSW		XMM5, XMM2		
				PSUBUSW		XMM2, XMM0		
				PADDUSW		XMM5, XMM2		// XMM5 = |C - R| 

				PADDUSW		XMM4, XMM5		// XMM4 = |C - L| + |C - R|

				PSRLW		XMM4, 6
				PXOR		XMM1, XMM1
				PACKUSWB	XMM1, XMM4

				MOV			EDX, target
				MOVHPS		[EDX], XMM1
			}
		}
	}
}


BOOST_AUTO_TEST_CASE( Test_SSE_Pitch0 )
{  
	int width = 32, height = 2;

	vector<uint8_t> src( width*height, 80 );
	vector<uint8_t> dst( width*height, 255 );

	src[ 0 ] = 100; 	src[ 1 ] = 0;
	src[ 11 ] = 80;
	src[ 21 ] = 0; src[ 22 ] = 100;

	SSE_Pitch0( 
		&src[0], &dst[0], 
		width, height, 
		11, 0, 20, 1,
		10.8f );

	BOOST_CHECK( dst[11] <= 1 );

	src[ 0 ] = 100; 	src[ 1 ] = 0;
	src[ 11 ] = 40;
	src[ 21 ] = 0; src[ 22 ] = 100;

	SSE_Pitch0( 
		&src[0], &dst[0], 
		width, height, 
		11, 0, 20, 1,
		10.8f );

	BOOST_CHECK( (dst[11] - 80)  <= 1 );
}

BOOST_AUTO_TEST_CASE( Test_Speed_SSE_Pitch0 )
{  
	int width = 12*1024, height = 1024;
	float pitch = 10.8f;
	int loopCount = 10;

	vector<uint8_t> src( width*height, 80 );
	vector<uint8_t> dst( width*height, 255 );

	ProgrammingStudy::StopWatch w;

	for( int i=0; i<loopCount; ++i )
	{
		SSE_Pitch0( 
			&src[0], &dst[0], 
			width, height, 
			(int)(pitch+1), 0, width-(int)(pitch+1), height-1,
			pitch );
	}

	double duration = w.Elapsed();

	printf( "\nTest SSE_Pitch0 : %.1f MB/s", (double)(width*height*loopCount / duration) / (1024*1024) );
	printf( "\n using image size %dx%d with %d loop", width, height, loopCount );
	printf( "\n");
}


#endif


void SSE_Intrinsics_Pitch0( 
	const uint8_t *h_src, uint8_t *h_dst, 
	int width, int height, 
	int roiLeft, int roiTop, int roiRight, int roiBottom,
	float horPitch )
{
	int sizeOfXMMInWords = SSE::SizeOfDataType / 2;
	int integerPitch = (int)horPitch;

	uint16_t toFloor = (uint16_t)( (horPitch - (int)horPitch) * 0x40);
	uint16_t toCeiling = 0x40 - toFloor;

	std::vector<uint16_t> toFloors( sizeOfXMMInWords, toFloor );
	std::vector<uint16_t> toCeilings( sizeOfXMMInWords, toCeiling );

	__m128i xfloor, xceil;
	xfloor = _mm_loadu_si128( reinterpret_cast<const __m128i *>(&toFloors[0]) );
	xceil = _mm_loadu_si128( reinterpret_cast<const __m128i *>(&toCeilings[0]) );

	for ( int y=roiTop; y<=roiBottom; y++ ) 
	{
		int x = roiLeft;
		const uint8_t *source = h_src + width*y+x;
		uint8_t *target = h_dst + width*y+x;

		for ( ; x<=roiRight; 
			x+=sizeOfXMMInWords, source+=sizeOfXMMInWords, target+=sizeOfXMMInWords ) 
		{
			__m128i xtemp, xleft, xright, xeast, xwest, xcenter, xdiff1, xdiff2;
			xtemp = _mm_loadu_si128( (const __m128i*)(source-integerPitch-1) );
			xeast = _mm_cvtepu8_epi16( xtemp );
			xtemp = _mm_srli_si128( xtemp, 1 );
			xwest = _mm_cvtepu8_epi16( xtemp );

			xeast = _mm_mullo_epi16( xeast, xfloor );
			xwest = _mm_mullo_epi16( xwest, xceil );
			xleft = _mm_adds_epu16( xeast, xwest );

			xtemp = _mm_loadu_si128( (const __m128i*)(source+integerPitch) );
			xeast = _mm_cvtepu8_epi16( xtemp );
			xtemp = _mm_srli_si128( xtemp, 1 );
			xwest = _mm_cvtepu8_epi16( xtemp );

			xeast = _mm_mullo_epi16( xeast, xceil );
			xwest = _mm_mullo_epi16( xwest, xfloor );
			xright = _mm_adds_epu16( xeast, xwest );

			xtemp = _mm_loadu_si128( (const __m128i*)(source) );
			xcenter = _mm_cvtepu8_epi16( xtemp );
			xcenter = _mm_slli_epi16( xcenter, 6 );
			xdiff1 = xcenter;
			xdiff2 = xcenter;

			xdiff1 = _mm_subs_epu16( xdiff1, xleft );
			xleft = _mm_subs_epu16( xleft, xcenter );
			xdiff1 = _mm_adds_epu16( xdiff1, xleft );

			xdiff2 = _mm_subs_epu16( xdiff2, xright );
			xright = _mm_subs_epu16( xright, xcenter );
			xdiff2 = _mm_adds_epu16( xdiff2, xright );

			xdiff1 = _mm_adds_epu16( xdiff1, xdiff2 );
			xdiff1 = _mm_srli_epi16( xdiff1, 6 );
			xtemp = _mm_xor_si128( xtemp, xtemp);
			xtemp = _mm_packus_epi16( xdiff1, xtemp );

			_mm_storel_epi64( (__m128i*)(target), xtemp );
		}
	}
}

BOOST_AUTO_TEST_CASE( Test_SSE_Intrinsics_Pitch0 )
{  
	int width = 32, height = 2;

	vector<uint8_t> src( width*height, 80 );
	vector<uint8_t> dst( width*height, 255 );

	src[ 0 ] = 100; 	src[ 1 ] = 0;
	src[ 11 ] = 80;
	src[ 21 ] = 0; src[ 22 ] = 100;

	SSE_Intrinsics_Pitch0( 
		&src[0], &dst[0], 
		width, height, 
		11, 0, 20, 1,
		10.8f );

	BOOST_CHECK( dst[11] <= 1 );


	src[ 0 ] = 100; 	src[ 1 ] = 0;
	src[ 11 ] = 40;
	src[ 21 ] = 0; src[ 22 ] = 100;

	SSE_Intrinsics_Pitch0( 
		&src[0], &dst[0], 
		width, height, 
		11, 0, 20, 1,
		10.8f );

	BOOST_CHECK( (dst[11] - 80)  <= 1 );

}


BOOST_AUTO_TEST_CASE( Test_Speed_SSE_Intrinsics_Pitch0 )
{  
	int width = 12*1024, height = 1024;
	float pitch = 10.8f;
	int loopCount = 10;

	vector<uint8_t> src( width*height, 80 );
	vector<uint8_t> dst( width*height, 255 );

	ProgrammingStudy::StopWatch w;

	for( int i=0; i<loopCount; ++i )
	{
		SSE_Intrinsics_Pitch0( 
			&src[0], &dst[0], 
			width, height, 
			(int)(pitch+1), 0, width-(int)(pitch+1), height-1,
			pitch );
	}

	double duration = w.Elapsed();

	printf( "\nTest SSE_Intrinsic_Pitch00 : %.1f MB/s", (double)(width*height*loopCount / duration) / (1024*1024) );
	printf( "\n using image size %dx%d with %d loop", width, height, loopCount );
	printf( "\n");
}


BOOST_AUTO_TEST_SUITE_END()