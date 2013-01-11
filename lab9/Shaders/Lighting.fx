// VARIABLES
#include "Simple.fx"

float3 ambientLight = float3(0.1, 0.0, 0.1);
float lightPower = 0.2;

// SHADERS
//----------------------------------------

// VERTEX SHADER
// -	Input : InputData for one vertex from the stream
// -	Output: Transformed VertexData for that vertex

// This also has an absolutePosition that does not get multiplied by view/proj for light computations
VertexData LightVertexShader(InputData In)
{
	VertexData Out = (VertexData)0;
	
	// In.position contains the vertex position in OBJECT space, from C++
	float4 transformedPosition = In.position;
	
	transformedPosition = mul(transformedPosition, world);
	transformedPosition = mul(transformedPosition, view);
	transformedPosition = mul(transformedPosition, proj);
	
    Out.position = transformedPosition;
	Out.normal = mul( In.normal, (float3x3) world);
	Out.texcoord = In.texcoord;
	
	// this is new :
	Out.absolutePosition = mul( (float3)In.position, (float3x3) world);
	
	return Out;
}

// PER-PIXEL LIGHTING PIXEL SHADER
// -	Input : VertexData for one interpolated vertex on the surface
// -	Output: float4 colour for one pixel on the screen

// This has to compute diffuse light values for each pixel
float4 LightPixelShader(VertexData In) : SV_Target
{
	// Object color from C++
	float4 output = color;
	
	// Light computation variabls
	float3 lightDirection;
	float lightDistance = 0;
	float lightIntensity = 0;
	
	//-- TODO: Compute direction, distance and color
	//--
	lightDistance = length(In.absolutePosition- lightPosition);
	lightDirection = normalize(In.absolutePosition - lightPosition);
	lightIntensity = saturate(dot(In.normal, -lightDirection));


	output.rgb *= lightDistance * lightPower * lightIntensity;
	output.rgb += ambientLight;
	
	return output;
}

//----------------------------------------

// TECHNIQUES
//----------------------------------------

// JUST ONE TECHNIQUE
// -	This HAS to have the same name as this file, because we include Simple.fx and there's another technique there. The compiler needs to know which one to use.
technique10 Lighting
{

	// JUST ONE PASS
	pass P0
	{
		// Set states
		SetRasterizerState( RState );
		SetDepthStencilState( DState, 0);
		
		// Set all shaders
		SetVertexShader( 	CompileShader( vs_4_0, LightVertexShader() ) );			// Using a new one	
		SetGeometryShader( 	CompileShader( gs_4_0, SimpleGeometryShader() ) );		// Using the one from Simple.fx
		SetPixelShader( 	CompileShader( ps_4_0, LightPixelShader() ) );			// Using a new one
	}
}