// VARIABLES
#include "Simple.fx"

float3 ambientLight = float3(0.1, 0.1, 0.1);
float lightIntensity = 0.1;

// SHADERS
//----------------------------------------

// PER-PIXEL LIGHTING PIXEL SHADER
// -	Input : VertexData for one interpolated vertex on the surface
// -	Output: float4 colour for one pixel on the screen
float4 DemoPixelShader(VertexData In) : SV_Target
{
	float4 output = (float4)1;

	// Texture
	//-- TODO: Texture sampling
	//--
	output = modelTexture.Sample(modelTextureSampler, In.texcoord);
	
	// Lighting
	//-- TODO: Light computation
	float3 lightDirection = float3(0,1,0);
	float lightDistance = 0.2f;
	float lightColor = 0.3;
	lightDistance = length(In.absolutePosition- lightPosition);
	lightDirection = normalize(In.absolutePosition - lightPosition);
	float3 lightIntensity2 = saturate(dot(In.normal, -lightDirection));
	//--
	
	output.rgb *= lightDistance * lightColor * lightIntensity2;
	output.rgb += ambientLight;
	
	// Colour pulsing
	output.rgb *= abs(cos(loop / 20));
	
	// Circular blob
	output.rgb += abs(cos(1 + In.texcoord.x * 4)) * abs(sin(-0.5 + In.texcoord.y * 4));
	
	// Transparency
	output.a = 0.7;
	
	//
	// Brown border from 0% to 11%
	if( In.texcoord.x < 0.11 || In.texcoord.x > 0.89 || In.texcoord.y < 0.11 || In.texcoord.y > 0.89 )
	{
		output.rgba = float4(0,0,0,1);
		
		// Black border from 10% to 11%
		if( In.texcoord.x < 0.1 || In.texcoord.x > 0.9 || In.texcoord.y < 0.1 || In.texcoord.y > 0.9 )
		{
			output.rgb = ambientLight + float3(0.6,0.2,0.1) * lightDistance * lightColor * lightIntensity;
			output.a = 1;
		}
	}
	
	
	return output;
}

//----------------------------------------

BlendState BState
{
	BlendEnable[0] = TRUE;
	RenderTargetWriteMask[0] = 0xF;

	BlendOpAlpha = SUBTRACT;
	SrcBlend[0] = SRC_ALPHA;
	DestBlend[0] = INV_SRC_ALPHA;
};

// DEPTH STATE
DepthStencilState DStateDisabled
{
	DepthEnable					= FALSE;
};

// TECHNIQUES
//----------------------------------------

// JUST ONE TECHNIQUE
// -	This HAS to have the same name as this file, because we include Simple.fx and there's another technique there. The compiler needs to know which one to use.
technique10 Demo
{

	// JUST ONE PASS
	pass P0
	{
		// Set states
		SetRasterizerState( RState );
		SetDepthStencilState( DStateDisabled, 0);
		SetBlendState( BState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xF);
		
		// Set all shaders
		SetVertexShader( 	CompileShader( vs_4_0, SimpleVertexShader() ) );			// Using a new one	
		SetGeometryShader( 	CompileShader( gs_4_0, SimpleGeometryShader() ) );		// Using the one from Simple.hlsl
		SetPixelShader( 	CompileShader( ps_4_0, DemoPixelShader() ) );			// Using a new one
	}
}