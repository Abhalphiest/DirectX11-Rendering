#include "Material.h"



Material::Material(SimpleVertexShader* p_vShader, SimplePixelShader* p_pShader)
{
	m_vertexShader = p_vShader;
	m_pixelShader = p_pShader;
}


