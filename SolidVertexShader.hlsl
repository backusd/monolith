// #include "VertexShaderConstantBuffer.hlsl"
/*
struct VSOut
{
	float3 color : Color;
	float4 pos : SV_Position;
};

VSOut main(float2 pos : POSITION, float3 color : COLOR)
{
	VSOut vso;
	vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
	vso.color = color;
	return vso;
}
*/


cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix modelViewProjection;
	matrix inverseTransposeModel;
};


struct VertexShaderInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};


// Simple shader to do vertex processing on the GPU.
//float4 main(float3 position : POSITION) : SV_Position
float4 main(VertexShaderInput input) : SV_Position
{
	/*
	PixelShaderInput output;
	float4 position = float4(input.position, 1.0f);

	output.position = mul(modelViewProjection, position);                 // Screen position
	output.positionWS = mul(model, position);                               // World space position
	output.normalWS = mul((float3x3)inverseTransposeModel, input.normal); // compute the world space normal
	*/
	return mul(modelViewProjection, float4(input.position, 1.0f));
}