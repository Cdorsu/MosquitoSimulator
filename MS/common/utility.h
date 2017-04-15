#pragma once

namespace utility
{
	struct SColor
	{
		float r;
		float g;
		float b;
		float a;
		SColor( )
		{ } // Do nothing
		SColor( float r, float g = 0.0f, float b = 0.0f, float a = 0.0f )
			:r( r ), g( g ), b( b ), a( a ) {};
		inline operator float*( )
		{
			return &r; // return the address of first variable
		}
	};
	inline SColor hexToRGB( long long int color )
	{
		// Shift bits, use a mask to make sure that there are only bits for current channel
		// Divide the color
		float red = ( ( color >> 8 ) & 0xFF ) / 255.0f;
		float green = ( ( color >> 16 ) & 0xFF ) / 255.0f;
		float blue = ( ( color >> 24 ) & 0xFF ) / 255.0f;
		return SColor( red, green, blue, 1.0f );
	}
}