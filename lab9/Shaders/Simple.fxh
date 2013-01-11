// EXTERNAL
//----------------------------------------
//	-	Everything below comes from C++ / CPU
//	-	All variables below are filled with valid data and cannot be modified from HLSL / GPU

// Matrices
float4x4 view;			// VIEW
float4x4 proj;			// PROJECTION
float4x4 world;			// TRANSLATION * ROTATION * SCALE

// Floats
float4 center;			// object center
float3 lightPosition;	// position of a light source
float3 eyePosition;		// position of the observer/camera
float4 color;			// basic object color
float loop;				// this just gets incremented every frame

// Resources
Texture2D modelTexture;	// a texture for this model

// And an array of vertex information, stored as :
struct InputData
{
	float4 position : POSITION;				// Position X Y Z
	float3 normal	: NORMAL;
	float2 texcoord : TEXCOORD;				// Texture Coordinates U V
};


// INTERNAL
//----------------------------------------
//	-	For local usage on the GPU

float PI = 3.1415926f;

// This is needed to know how sample the texture, don't worry about it for now, the fields below work in almost all cases
SamplerState modelTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;			
    AddressU = Wrap;
    AddressV = Wrap;
};

// Main structure that holds vertex information, we will use this to pass around vertex information
struct VertexData
{
	float4 position 		: SV_POSITION;	// Position X Y Z
	float3 normal 			: NORMAL;		// Normal X Y Z
	float2 texcoord 		: TEXCOORD;		// Texture Coordinates U V
	float3 absolutePosition : TEXCOORD2;	// Position X Y Z in Object Space
};

