struct VS_INPUT
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

cbuffer ConstantBuffer : register(b0)
{
	float4 colorMultiplier;
};

cbuffer WvpConstantBuffer : register(b1)
{
	float4x4 wvp;
};

// simple vertex shader
VS_OUTPUT vsMain(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = float4(input.pos, 1.0f);
	output.color = input.color * colorMultiplier;
	output.pos = mul(output.pos, wvp);

	return output;
}

// simple pixel shader
float4 psMain(VS_OUTPUT input) : SV_TARGET
{
	return input.color;
}