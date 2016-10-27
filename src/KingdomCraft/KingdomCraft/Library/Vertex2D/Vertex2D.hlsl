//�O���[�o��

Texture2D g_Texture: register(t0);
SamplerState g_Sampler : register(s0);

cbuffer global
{
	matrix g_W; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_ViewPort;//�r���[�|�[�g�i�X�N���[���j���T�C�Y
};
//�\����
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS(float4 Pos : POSITION, float2 UV : TEXCOORD)
{
	PS_INPUT Out;

	Out.Pos = mul(Pos, g_W);

	Out.Pos.x = (Out.Pos.x / g_ViewPort.x) * 2 - 1;
	Out.Pos.y = 1 - (Out.Pos.y / g_ViewPort.y) * 2;
	Out.UV = UV;

	return Out;
}
//
//
//�s�N�Z���V�F�[�_�[
float4 PS(PS_INPUT Input) : SV_Target
{
	return g_Texture.Sample(g_Sampler, Input.UV);
}