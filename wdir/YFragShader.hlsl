bool al_use_tex;
texture al_tex;
sampler2D s = sampler_state {
	texture = <al_tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MIPFilter = LINEAR;
};

static float PixelOffsets[11] = {0, 1.49537, 3.4892, 5.48303, 7.47687, 9.47071, 11.4646, 13.4584, 15.4523, 17.4462, 19.4401};
static float BlurWeights[11] = {0.0453551, 0.089325, 0.0839938, 0.0751867, 0.06407, 0.0519742, 0.0401365, 0.029506, 0.0206491, 0.0137566, 0.00872447};

float4 ps_main(VS_OUTPUT Input) : COLOR0
{
	float4 TextureCol = tex2D(s, Input.TexCoord) * BlurWeights[0];
	for (int i = 1; i < 11; i++)
	{
		TextureCol += tex2D(s, float2(0.0f, PixelOffsets[i]/BmpHeight) + Input.TexCoord) * BlurWeights[i];
		TextureCol += tex2D(s, float2(0.0f, -PixelOffsets[i]/BmpHeight) + Input.TexCoord) * BlurWeights[i];
	}
	if (TextureCol[3] > .7)
	{
		TextureCol[3] = .7;
	}
	return TextureCol;
}