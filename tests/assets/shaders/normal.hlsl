#include <types.hlsl>

SamplerState SampleType {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

Texture2D t_diffuse;
Texture2D t_alpha;
Texture2D t_normal;
Texture2D t_specular;
Texture2D t_shininess;

TextureCube envmap;

// PixelShader
//------------------------------------------------------------------------------------
PixelOutType mainPixel(PixelInputType input)
{
	PixelOutType output;
	output.normal = input.normal;
	output.view = input.view;
	output.diff = .5 * input.normal - .5;
	return output;
}

// maps envmap to output from the view
PixelOutType envmapNormal(PixelInputType input)
{
	PixelOutType output;
	output.normal = input.normal;
	output.view = input.view;
	//output.diff = envmap.Sample(SampleType, input.normal);

	//nincsilyen;

	output.diff = float4(1, 0, 0, 1);
	return output;
} 
