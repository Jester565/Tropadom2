struct VS_INPUT
{
	float4 Position	: POSITION0;
	float2 TexCoord	:TEXCOORD0;
	float4 Color	:TEXCOORD1;
};
struct VS_OUTPUT
{
	float4 Position	: POSITION0;
	float4 Color	: COLOR0;
	float2 TexCoord	: TEXCOORD0;
};

float4x4 al_projview_matrix;
bool al_use_tex_matrix;
float4x4 al_tex_matrix;
float BmpWidth;
float BmpHeight;

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;
	Output.Color = Input.Color;
	if (al_use_tex_matrix) 
	{
		Output.TexCoord = mul(float4(Input.TexCoord, 1.0f, 0.0f), al_tex_matrix).xy;
	}
	else
	{
		Output.TexCoord = Input.TexCoord;
	}
	Output.Position = mul(Input.Position, al_projview_matrix);
	return Output;
}