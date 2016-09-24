#include "Material.h"



Material::Material(SimpleVertexShader* p_vShader, SimplePixelShader* p_pShader)
{
	m_vertexShader = p_vShader;
	m_pixelShader = p_pShader;
}

Material::Material(SimpleVertexShader* p_vShader, SimplePixelShader* p_pShader,
	ID3D11ShaderResourceView* p_srv, ID3D11SamplerState* p_s)
{
	m_vertexShader = p_vShader;
	m_pixelShader = p_pShader;
	m_srv = p_srv;
	m_sState = p_s;
}


