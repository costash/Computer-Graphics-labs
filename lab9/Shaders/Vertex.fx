// VARIABLES
#include "Simple.fx"

// SHADERS
//----------------------------------------

// VERTEX SHADER
// -	Input : InputData for one vertex from the stream
// -	Output: Transformed VertexData for that vertex

// This needs to have a grow/shrink pulsing effect for all vertices
VertexData PulsingVertexShader(InputData In)
{
	VertexData Out = (VertexData)0;
	
	Out.absolutePosition = mul( (float3)In.position, (float3x3) world);
	Out.normal = normalize( mul( In.normal, (float3x3) world) );
	
	// In.position contains the vertex position in OBJECT space
	float4 transformedPosition = In.position;
	transformedPosition.xyz += In.normal*(cos(loop/11.0f)/11.0f);	
	//-- TODO: Offset the position by a pulsating variable offset
	//--
	
	
	transformedPosition = mul(transformedPosition, world);
	transformedPosition = mul(transformedPosition, view);
	transformedPosition = mul(transformedPosition, proj);
	
    Out.position = transformedPosition;
	Out.texcoord = In.texcoord;
	
	return Out;
}

// TECHNIQUES
//----------------------------------------

// JUST ONE TECHNIQUE
technique10 Vertex
{

	// JUST ONE PASS
	pass P0
	{
		// Set states
		SetRasterizerState( RState );
		SetDepthStencilState( DState, 0);
		
		// Set all shaders
		SetVertexShader( 	CompileShader( vs_4_0, PulsingVertexShader() ) );	// Vertex
		SetGeometryShader( 	CompileShader( gs_4_0, SimpleGeometryShader() ) );	// Using the one from Simple.fx
		SetPixelShader( 	CompileShader( ps_4_0, SimplePixelShader() ) );		// Using the one from Simple.fx
	}
}