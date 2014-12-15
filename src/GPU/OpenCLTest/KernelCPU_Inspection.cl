__kernel 
__attribute__((reqd_work_group_size(1,1,1)))
void pitch_comparion_0( 
  __global const uchar *src, __global uchar *dst, 
  __const int width, __const int height,
  __const int roiLeft, __const int roiTop, __const int roiRight, __const int roiBottom,
  __const int blockWidth,
  __const int nPitch, 
  __const short toCeiling, __const short toFloor )
{
	int i = get_global_id(0);

	for( int y = roiTop; y <= roiBottom; ++y )
	{
		int left = roiLeft + blockWidth*i;
		int right = left + blockWidth;
		int idx = y * width + left;

		for( int x = left; x < right; x+=8, idx += 8 )
		{
			short8 eastLeft = convert_short8( vload8( 0, src + idx + nPitch ) );
			short8 eastRight = convert_short8( vload8( 0, src + idx + nPitch + 1 ) );
			short8 westLeft = convert_short8( vload8( 0, src + idx - nPitch - 1 ) );
			short8 westRight = convert_short8( vload8( 0, src + idx - nPitch ) );
			short8 center = convert_short8( vload8( 0, src + idx ) );

			short8 east = toFloor * eastLeft + toCeiling * eastRight;
			short8 west = toFloor * westRight + toCeiling * westLeft;
  
			east = east >> 1;
			west = west >> 1;
			center = center << 7;

			ushort8 diff = abs_diff( center, east+west );

			diff = diff >> 7;
		
			if ( x <= roiRight ) 
			{
				vstore8( convert_uchar8( diff ), 0, dst + idx );
			}
		}

	}
}