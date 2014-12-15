__kernel
void pitch_comprison_0(
	__global uchar *d_src, 
	__global uchar *d_dst,
	int width, int height, 
	int left, int top, int right, int bottom,
	int pitch )
{
	int x = left + get_global_id(0);
	int y = top + get_global_id(1);
	int idx = x + y * width;

	bool isInBound = x <= right && y <= bottom;

	short center = d_src[idx]; 
	short diff = ( center - d_src[idx-pitch] )  + ( center - d_src[idx+pitch] );
	diff = diff >= 0 ? diff : -diff;

	if( isInBound )
	{
		d_dst[ idx ] = (uchar)( diff  );
	}
}

float PitchedSubPixel( __global uchar* src, int idx, float weightLeft, float weightRight )
{
	return (float)src[ idx ] * weightLeft + (float)src[ idx+1 ] * weightRight;
}

float PixelDifference( float center, float east, float west )
{
	float diff = ( center * 2.0f  - ( east + west ) ) / 2.0f;
	return diff >= 0 ? diff : -diff;
}

__kernel
void pitch_threshold_0(
	__global uchar *d_src, 
	__global uchar *d_dst,
	int width, int height,
	int roiLeft, int roiTop, int roiRight, int roiBottom,
	int integerPitch, float toCeiling, float toFloor,
	__local uchar *localDst,
	uchar threshold )
{ 
	int x = (roiLeft & (~0x07)) + get_global_id(0);
	int y = roiTop + get_global_id(1);
	int idx = x + y * width;

	bool isInBound = x >= roiLeft && x <= roiRight && y <= roiBottom;

	float east = 0.0f, west = 0.0f, diff = 0.0f;
	if( isInBound ) 
	{
		east = PitchedSubPixel( d_src, idx-integerPitch-1, toCeiling, toFloor );
		west = PitchedSubPixel( d_src, idx+integerPitch, toFloor, toCeiling );
		diff = PixelDifference( (float)d_src[idx], east, west );
	}

	uchar binary = ((uchar)( diff ) <  threshold) ? 0 : 1;

	int localX = get_local_id(0);
	int localIdx = get_local_id(1) * get_local_size(0) + localX;
	binary = binary << (7-(localX & 0x07));
	localDst[localIdx] = binary;  

	isInBound = x <= roiRight && y <= roiBottom;
	
	barrier( CLK_LOCAL_MEM_FENCE );

	if( isInBound && (localX & 0x07) == 0 )
	{
		uchar packed = 
			localDst[ localIdx   ] | localDst[ localIdx+1 ] | localDst[ localIdx+2 ] | localDst[ localIdx+3 ] | 
			localDst[ localIdx+4 ] | localDst[ localIdx+5 ] | localDst[ localIdx+6 ] | localDst[ localIdx+7 ] ; 

		d_dst[ idx >> 3 ] = packed;
	}   
} 
    