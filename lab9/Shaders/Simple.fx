// VARIABLES
#include "Simple.fxh"

// SHADERS
//----------------------------------------

// VERTEX SHADER
// -	Input : InputData for one vertex from the stream
// -	Output: Transformed VertexData for that vertex
VertexData SimpleVertexShader(InputData In)
{
	VertexData Out = (VertexData)0;
	
	// In.position contains the vertex position in OBJECT space
	float4 transformedPosition = In.position;
	
	// Apply the WORLD matrix - applying all object transformations (translations, rotations, scales)
	transformedPosition = mul(transformedPosition, world);
	// Apply the VIEW matrix - moving it in EYE space
	transformedPosition = mul(transformedPosition, view);
	// Apply the PROJECTION matrix - moving it in SCREEN space
	transformedPosition = mul(transformedPosition, proj);
	
	Out.normal = In.normal;
    Out.position = transformedPosition;
	Out.texcoord = In.texcoord;
	
	return Out;
}

// GEOMETRY SHADER
// -	Input : VertexData for three vertices from the stream (one triangle)
// -	Output: Transformed VertexData for 3 or more vertices, as specified in maxvertexcount() - has to be a multiple of 3 because we're using TriangleStream
[maxvertexcount(3)]
void SimpleGeometryShader(triangle VertexData In[3], inout TriangleStream<VertexData> OutputStream)
{		
	// For each vertex
	for( int i = 0; i < 3; i++)
	{
		// Create a copy
		VertexData vertex = (VertexData)0;
	
		// Set values
		vertex.position = In[i].position;
		vertex.texcoord = In[i].texcoord;
		vertex.normal = In[i].normal;
		vertex.absolutePosition = In[i].absolutePosition;
		// here we would normally have some more processing, not just a deep copy
		
		// Write to triangle stream
		OutputStream.Append(vertex);
	}
	
	// Reset stream
	OutputStream.RestartStrip();
}

// PIXEL SHADER
// -	Input : VertexData for one vertex on the surface
// -	Output: float4 colour for one pixel on the screen
float4 SimplePixelShader(VertexData In) : SV_Target
{
	// a simple colour
	float4 output;
	
	// this works:
	output = float4(0,0,0,0); // all black + transparent
	
	// we can also do this:
	output = (float4)1;	// all white + opaque
	
	// or this :
	output.rgb = float3(In.texcoord.x,In.texcoord.y, 0.5); 
	
	// or this :
	output.b = 0;
	
	return output;
}
//----------------------------------------


// STATES
//----------------------------------------

// DEPTH STATE
DepthStencilState DState
{
	DepthEnable					= TRUE;
	DepthFunc					= LESS;
};

// RASTER STATE
RasterizerState RState
{
	FillMode 					= SOLID;	// SOLID | WIREFRAME
	CullMode 					= NONE;		// BACK | FRONT | NONE
};


// TECHNIQUES
//----------------------------------------

// JUST ONE TECHNIQUE
technique10 Simple
{

	// JUST ONE PASS
	pass P0
	{
		// Set states
		SetRasterizerState( RState );
		SetDepthStencilState( DState, 0);
		
		// Set all shaders
		SetVertexShader( 	CompileShader( vs_4_0, SimpleVertexShader() ) );	// Vertex
		SetGeometryShader( 	CompileShader( gs_4_0, SimpleGeometryShader() ) );	// Geometry
		SetPixelShader( 	CompileShader( ps_4_0, SimplePixelShader() ) );		// Pixel
	}
}