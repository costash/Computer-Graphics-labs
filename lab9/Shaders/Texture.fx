// VARIABLES
#include "Simple.fx"

// SHADERS
//----------------------------------------

// TEXTURE PIXEL SHADER

// This needs to sample a texture
float4 TexturePixelShader(VertexData In) : SV_Target
{
	float4 output = (float4)0.7;
	
	//-- TODO: Sample modelTexture
	output = modelTexture.Sample(modelTextureSampler, In.texcoord);
	//--
	
	return output;
}
//----------------------------------------

// TECHNIQUES
//----------------------------------------

// JUST ONE TECHNIQUE
// -	This HAS to have the same name as this file, because we include Simple.fx and there's another technique there. The compiler needs to know which one to use.
technique10 Texture
{

	// JUST ONE PASS
	pass P0
	{
		// Set states
		SetRasterizerState( RState );
		SetDepthStencilState( DState, 0);
		
		// Set all shaders
		SetVertexShader( 	CompileShader( vs_4_0, SimpleVertexShader() ) );		// Using the one from Simple.fx
		SetGeometryShader( 	CompileShader( gs_4_0, SimpleGeometryShader() ) );		// Using the one from Simple.fx
		SetPixelShader( 	CompileShader( ps_4_0, TexturePixelShader() ) );		// This is the only thing that's changed 
	}
}