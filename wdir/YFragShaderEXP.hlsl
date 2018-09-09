bool al_use_tex;
texture al_tex;
sampler2D s = sampler_state {
	texture = <al_tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MIPFilter = LINEAR;
};

float PixelOffsets[13] = {-6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6};
float BlurWeights[13] = {0.002216, 0.008764, 0.026995, 0.064759, 0.1209850, 0.176033, 0.199471, 0.176033, 0.120985, 0.064759,0.026995, 0.008764, 0.002216};

float4 ps_main(VS_OUTPUT Input) : COLOR0
{
	if (al_use_tex)
	{
		float4 TextureCol = tex2D(s, float2(Input.TexCoord.x, Input.TexCoord.y));
		if (TextureCol.r == 1.0f)
		{
			return TextureCol;
		}
		float4 TextureCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
		for (int i = 0; i < 13; i++)
		{
			TextureCol += tex2D(s, float2(Input.TexCoord.x, (PixelOffsets[i]/BmpHeight) + Input.TexCoord.y)) * BlurWeights[i];
		}
		return TextureCol;
	}
	else
	{
		return Input.Color ;
	}
}