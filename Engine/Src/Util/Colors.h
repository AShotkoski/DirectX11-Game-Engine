#pragma once

class Color
{
	typedef unsigned short ushort;
	typedef unsigned long dword;
public:
	static inline constexpr Color MakeRgb( float r, float g, float b )
	{
		return Color{ r, g, b };
	}
	static inline constexpr Color MakeRgbS( ushort r, ushort g, ushort b )
	{
		return MakeRgb( (float)r / 255.f, (float)g / 255.f, (float)b / 255.f );
	}
	// RGBA, ignores A
	static inline constexpr Color MakeRgb( dword c )
	{
		return MakeRgbS(
			(ushort)( ( c & 0xFF000000 ) >> 24 ),
			(ushort)( ( c & 0x00FF0000 ) >> 16 ),
			(ushort)( ( c & 0x0000FF00 ) >> 8 ) );
	}
	operator DirectX::XMFLOAT3() const
	{
		return DirectX::XMFLOAT3( el );
	}

public:
	float el[3];
};

namespace Colors
{
	// Reds
	static constexpr const Color IndianRed   = Color::MakeRgbS( 205, 92, 92 ); 
	static constexpr const Color LightCoral  = Color::MakeRgbS( 240, 128, 128 );
	static constexpr const Color Salmon      = Color::MakeRgbS( 250, 128, 114 );
	static constexpr const Color DarkSalmon  = Color::MakeRgbS( 233, 150, 122 );
	static constexpr const Color LightSalmon = Color::MakeRgbS( 255, 160, 122 );
	static constexpr const Color Crimson     = Color::MakeRgbS( 220, 20, 60 );
	static constexpr const Color Red         = Color::MakeRgbS( 255, 0, 0 );
	static constexpr const Color FireBrick   = Color::MakeRgbS( 178, 34, 34 );
	static constexpr const Color DarkRed     = Color::MakeRgbS( 139, 0, 0 );

	// Pinks
	static constexpr const Color Pink            = Color::MakeRgbS( 255, 192, 203 ); 
	static constexpr const Color LightPink       = Color::MakeRgbS( 255, 182, 193 );
	static constexpr const Color HotPink         = Color::MakeRgbS( 255, 105, 180 );
	static constexpr const Color DeepPink        = Color::MakeRgbS( 255, 20, 147 );
	static constexpr const Color MediumVioletRed = Color::MakeRgbS( 199, 21, 133 );
	static constexpr const Color PaleVioletRed   = Color::MakeRgbS( 219, 112, 147 );

	// Oranges
	static constexpr const Color Coral      = Color::MakeRgbS( 255, 127, 80 ); 
	static constexpr const Color Tomato     = Color::MakeRgbS( 255, 99, 71 );
	static constexpr const Color OrangeRed  = Color::MakeRgbS( 255, 69, 0 );
	static constexpr const Color DarkOrange = Color::MakeRgbS( 255, 140, 0 );
	static constexpr const Color Orange     = Color::MakeRgbS( 255, 165, 0 );

	// Yellows
	static constexpr const Color Gold                 = Color::MakeRgbS( 255, 215, 0 ); 
	static constexpr const Color Yellow               = Color::MakeRgbS( 255, 255, 0 );
	static constexpr const Color LightYellow          = Color::MakeRgbS( 255, 255, 224 );
	static constexpr const Color LemonChiffon         = Color::MakeRgbS( 255, 250, 205 );
	static constexpr const Color LightGoldenrodYellow = Color::MakeRgbS( 250, 250, 210 );
	static constexpr const Color PapayaWhip           = Color::MakeRgbS( 255, 239, 213 );
	static constexpr const Color Moccasin             = Color::MakeRgbS( 255, 228, 181 );
	static constexpr const Color PeachPuff            = Color::MakeRgbS( 255, 218, 185 );
	static constexpr const Color PaleGoldenrod        = Color::MakeRgbS( 238, 232, 170 );
	static constexpr const Color Khaki                = Color::MakeRgbS( 240, 230, 140 );
	static constexpr const Color DarkKhaki            = Color::MakeRgbS( 189, 183, 107 );

	// Purples
	static constexpr const Color Lavender        = Color::MakeRgbS( 230, 230, 250 ); 
	static constexpr const Color Thistle         = Color::MakeRgbS( 216, 191, 216 );
	static constexpr const Color Plum            = Color::MakeRgbS( 221, 160, 221 );
	static constexpr const Color Violet          = Color::MakeRgbS( 238, 130, 238 );
	static constexpr const Color Orchid          = Color::MakeRgbS( 218, 112, 214 );
	static constexpr const Color Fuchsia         = Color::MakeRgbS( 255, 0, 255 );
	static constexpr const Color Magenta         = Color::MakeRgbS( 255, 0, 255 );
	static constexpr const Color MediumOrchid    = Color::MakeRgbS( 186, 85, 211 );
	static constexpr const Color MediumPurple    = Color::MakeRgbS( 147, 112, 219 );
	static constexpr const Color BlueViolet      = Color::MakeRgbS( 138, 43, 226 );
	static constexpr const Color DarkViolet      = Color::MakeRgbS( 148, 0, 211 );
	static constexpr const Color DarkOrchid      = Color::MakeRgbS( 153, 50, 204 );
	static constexpr const Color DarkMagenta     = Color::MakeRgbS( 139, 0, 139 );
	static constexpr const Color Purple          = Color::MakeRgbS( 128, 0, 128 );
	static constexpr const Color RebeccaPurple   = Color::MakeRgbS( 102, 51, 153 );
	static constexpr const Color Indigo          = Color::MakeRgbS( 75, 0, 130 );
	static constexpr const Color MediumSlateBlue = Color::MakeRgbS( 123, 104, 238 );
	static constexpr const Color SlateBlue       = Color::MakeRgbS( 106, 90, 205 );
	static constexpr const Color DarkSlateBlue   = Color::MakeRgbS( 72, 61, 139 );

	// Greens
	static constexpr const Color GreenYellow       = Color::MakeRgbS( 173, 255, 47 ); 
	static constexpr const Color Chartreuse        = Color::MakeRgbS( 127, 255, 0 );
	static constexpr const Color LawnGreen         = Color::MakeRgbS( 124, 252, 0 );
	static constexpr const Color Lime              = Color::MakeRgbS( 0, 255, 0 );
	static constexpr const Color LimeGreen         = Color::MakeRgbS( 50, 205, 50 );
	static constexpr const Color PaleGreen         = Color::MakeRgbS( 152, 251, 152 );
	static constexpr const Color LightGreen        = Color::MakeRgbS( 144, 238, 144 );
	static constexpr const Color MediumSpringGreen = Color::MakeRgbS( 0, 250, 154 );
	static constexpr const Color SpringGreen       = Color::MakeRgbS( 0, 255, 127 );
	static constexpr const Color MediumSeaGreen    = Color::MakeRgbS( 60, 179, 113 );
	static constexpr const Color SeaGreen          = Color::MakeRgbS( 46, 139, 87 );
	static constexpr const Color ForestGreen       = Color::MakeRgbS( 34, 139, 34 );
	static constexpr const Color Green             = Color::MakeRgbS( 0, 128, 0 );
	static constexpr const Color DarkGreen         = Color::MakeRgbS( 0, 100, 0 );
	static constexpr const Color YellowGreen       = Color::MakeRgbS( 154, 205, 50 );
	static constexpr const Color OliveDrab         = Color::MakeRgbS( 107, 142, 35 );
	static constexpr const Color Olive             = Color::MakeRgbS( 128, 128, 0 );
	static constexpr const Color DarkOliveGreen    = Color::MakeRgbS( 85, 107, 47 );
	static constexpr const Color MediumAquamarine  = Color::MakeRgbS( 102, 205, 170 );
	static constexpr const Color DarkSeaGreen      = Color::MakeRgbS( 143, 188, 143 );
	static constexpr const Color LightSeaGreen     = Color::MakeRgbS( 32, 178, 170 );
	static constexpr const Color DarkCyan          = Color::MakeRgbS( 0, 139, 139 );
	static constexpr const Color Teal              = Color::MakeRgbS( 0, 128, 128 );

	// Blues / Cyans
	static constexpr const Color Aqua          = Color::MakeRgbS( 0, 255, 255 );
	static constexpr const Color Cyan          = Color::MakeRgbS( 0, 255, 255 );
	static constexpr const Color LightCyan     = Color::MakeRgbS( 224, 255, 255 );
	static constexpr const Color PaleTurquoise = Color::MakeRgbS( 175, 238, 238 );
	static constexpr const Color Aquamarine    = Color::MakeRgbS( 127, 255, 212 );
	static constexpr const Color Turquoise     = Color::MakeRgbS( 64, 224, 208 );
	static constexpr const Color MediumTurquoise = Color::MakeRgbS( 72, 209, 204 );
	static constexpr const Color DarkTurquoise   = Color::MakeRgbS( 0, 206, 209 );
	static constexpr const Color CadetBlue       = Color::MakeRgbS( 95, 158, 160 );
	static constexpr const Color SteelBlue       = Color::MakeRgbS( 70, 130, 180 );
	static constexpr const Color LightSteelBlue  = Color::MakeRgbS( 176, 196, 222 );
	static constexpr const Color PowderBlue      = Color::MakeRgbS( 176, 224, 230 );
	static constexpr const Color LightBlue       = Color::MakeRgbS( 173, 216, 230 );
	static constexpr const Color SkyBlue         = Color::MakeRgbS( 135, 206, 235 );
	static constexpr const Color LightSkyBlue    = Color::MakeRgbS( 135, 206, 250 );
	static constexpr const Color DeepSkyBlue     = Color::MakeRgbS( 0, 191, 255 );
	static constexpr const Color DodgerBlue      = Color::MakeRgbS( 30, 144, 255 );
	static constexpr const Color CornflowerBlue  = Color::MakeRgbS( 100, 149, 237 );
	static constexpr const Color RoyalBlue       = Color::MakeRgbS( 65, 105, 225 );
	static constexpr const Color Blue            = Color::MakeRgbS( 0, 0, 255 );
	static constexpr const Color MediumBlue      = Color::MakeRgbS( 0, 0, 205 );
	static constexpr const Color DarkBlue        = Color::MakeRgbS( 0, 0, 139 );
	static constexpr const Color Navy            = Color::MakeRgbS( 0, 0, 128 );
	static constexpr const Color MidnightBlue    = Color::MakeRgbS( 25, 25, 112 );

	// Browns
	static constexpr const Color Cornsilk       = Color::MakeRgbS( 255, 248, 220 ); 
	static constexpr const Color BlanchedAlmond = Color::MakeRgbS( 255, 235, 205 );
	static constexpr const Color Bisque         = Color::MakeRgbS( 255, 228, 196 );
	static constexpr const Color NavajoWhite    = Color::MakeRgbS( 255, 222, 173 );
	static constexpr const Color Wheat          = Color::MakeRgbS( 245, 222, 179 );
	static constexpr const Color BurlyWood      = Color::MakeRgbS( 222, 184, 135 );
	static constexpr const Color Tan            = Color::MakeRgbS( 210, 180, 140 );
	static constexpr const Color RosyBrown      = Color::MakeRgbS( 188, 143, 143 );
	static constexpr const Color SandyBrown     = Color::MakeRgbS( 244, 164, 96 );
	static constexpr const Color Goldenrod      = Color::MakeRgbS( 218, 165, 32 );
	static constexpr const Color DarkGoldenrod  = Color::MakeRgbS( 184, 134, 11 );
	static constexpr const Color Peru           = Color::MakeRgbS( 205, 133, 63 );
	static constexpr const Color Chocolate      = Color::MakeRgbS( 210, 105, 30 );
	static constexpr const Color SaddleBrown    = Color::MakeRgbS( 139, 69, 19 );
	static constexpr const Color Sienna         = Color::MakeRgbS( 160, 82, 45 );
	static constexpr const Color Brown          = Color::MakeRgbS( 165, 42, 42 );
	static constexpr const Color Maroon         = Color::MakeRgbS( 128, 0, 0 );

	// Whites
	static constexpr const Color White         = Color::MakeRgbS( 255, 255, 255 ); 
	static constexpr const Color Snow          = Color::MakeRgbS( 255, 250, 250 );
	static constexpr const Color Honeydew      = Color::MakeRgbS( 240, 255, 240 );
	static constexpr const Color MintCream     = Color::MakeRgbS( 245, 255, 250 );
	static constexpr const Color Azure         = Color::MakeRgbS( 240, 255, 255 );
	static constexpr const Color AliceBlue     = Color::MakeRgbS( 240, 248, 255 );
	static constexpr const Color GhostWhite    = Color::MakeRgbS( 248, 248, 255 );
	static constexpr const Color WhiteSmoke    = Color::MakeRgbS( 245, 245, 245 );
	static constexpr const Color Seashell      = Color::MakeRgbS( 255, 245, 238 );
	static constexpr const Color Beige         = Color::MakeRgbS( 245, 245, 220 );
	static constexpr const Color OldLace       = Color::MakeRgbS( 253, 245, 230 );
	static constexpr const Color FloralWhite   = Color::MakeRgbS( 255, 250, 240 );
	static constexpr const Color Ivory         = Color::MakeRgbS( 255, 255, 240 );
	static constexpr const Color AntiqueWhite  = Color::MakeRgbS( 250, 235, 215 );
	static constexpr const Color Linen         = Color::MakeRgbS( 250, 240, 230 );
	static constexpr const Color LavenderBlush = Color::MakeRgbS( 255, 240, 245 );
	static constexpr const Color MistyRose     = Color::MakeRgbS( 255, 228, 225 );

	// Greys
	static constexpr const Color Gainsboro      = Color::MakeRgbS( 220, 220, 220 );
	static constexpr const Color LightGray      = Color::MakeRgbS( 211, 211, 211 );
	static constexpr const Color LightGrey      = Color::MakeRgbS( 211, 211, 211 );
	static constexpr const Color Silver         = Color::MakeRgbS( 192, 192, 192 );
	static constexpr const Color DarkGray       = Color::MakeRgbS( 169, 169, 169 );
	static constexpr const Color DarkGrey       = Color::MakeRgbS( 169, 169, 169 );
	static constexpr const Color Gray           = Color::MakeRgbS( 128, 128, 128 );
	static constexpr const Color Grey           = Color::MakeRgbS( 128, 128, 128 );
	static constexpr const Color DimGray        = Color::MakeRgbS( 105, 105, 105 );
	static constexpr const Color DimGrey        = Color::MakeRgbS( 105, 105, 105 );
	static constexpr const Color LightSlateGray = Color::MakeRgbS( 119, 136, 153 );
	static constexpr const Color LightSlateGrey = Color::MakeRgbS( 119, 136, 153 );
	static constexpr const Color SlateGray      = Color::MakeRgbS( 112, 128, 144 );
	static constexpr const Color SlateGrey      = Color::MakeRgbS( 112, 128, 144 );
	static constexpr const Color DarkSlateGray  = Color::MakeRgbS( 47, 79, 79 );
	static constexpr const Color DarkSlateGrey  = Color::MakeRgbS( 47, 79, 79 );
	static constexpr const Color Black		    = Color::MakeRgbS( 0, 0, 0 );
};
