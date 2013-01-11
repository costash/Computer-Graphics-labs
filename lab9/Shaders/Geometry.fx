// VARIABLES
#include "Simple.fx"

// SHADERS
//----------------------------------------

// VERTEX SHADER
// -	Input : InputData for one vertex from the stream
// -	Output: Transformed VertexData for that vertex

// This is just a deep copy, nothing more
VertexData CopyVertexShader(InputData In)
{
	VertexData Out = (VertexData)0;
	
	// No further computation here, we'll do it all in the Geometry Shader
	Out.normal = In.normal;
    Out.position = In.position;
	Out.texcoord = In.texcoord;
	
	return Out;
}

VertexData copyOffset(VertexData In, float offset)
{
	// Create a copy
	VertexData vertex = (VertexData)0;
	
	// In.position contains the vertex position in OBJECT space
	float4 transformedPosition = In.position;

	//-- TODO : Offset transformed position based on the normal
	//-- 
	transformedPosition.xyz+=In.normal*offset;
	
	transformedPosition = mul(transformedPosition, world);
	transformedPosition = mul(transformedPosition, view);
	transformedPosition = mul(transformedPosition, proj);

	// Set values
	vertex.position = transformedPosition;
	vertex.texcoord = In.texcoord;
	vertex.normal = In.normal;
	vertex.absolutePosition = In.absolutePosition;
	
	return vertex;
}

// GEOMETRY SHADER
// -	Input : VertexData for three vertices from the stream (one triangle)
// -	Output: Transformed VertexData for 3 or more vertices, as specified in maxvertexcount() - has to be a multiple of 3 because we're using TriangleStream

// This needs to multiply all vertices by world/view/proj and create a duplicate of each triangle, offset by a value
[maxvertexcount(6)]
void ExtendedGeometryShader(triangle VertexData In[3], inout TriangleStream<VertexData> OutputStream)
{		
	// For each vertex
	for( int i = 0; i < 3; i++)
	{
		// Create a copy
		VertexData vertex = copyOffset(In[i], 0);
		
		// Write to triangle stream
		OutputStream.Append(vertex);
	}
	
	OutputStream.RestartStrip();
	
	//-- TODO: Create a new triangle, above the original one
	//--
	for( int i = 0; i < 3; i++)
	{
		// Create a copy
		VertexData vertex = copyOffset(In[i], 5);
		
		// Write to triangle stream
		OutputStream.Append(vertex);
	}
	OutputStream.RestartStrip();
}


// TECHNIQUES
//----------------------------------------

// JUST ONE TECHNIQUE
technique10 Geometry
{

	// JUST ONE PASS
	pass P0
	{
		// Set states
		SetRasterizerState( RState );
		SetDepthStencilState( DState, 0);
		
		// Set all shaders
		SetVertexShader( 	CompileShader( vs_4_0, CopyVertexShader() ) );	// Vertex
		SetGeometryShader( 	CompileShader( gs_4_0, ExtendedGeometryShader() ) );	// Geometry
		SetPixelShader( 	CompileShader( ps_4_0, SimplePixelShader() ) );		// Pixel
	}
}