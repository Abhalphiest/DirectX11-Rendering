#pragma once

#include"../SimpleShader.h"
class Material
{
public:
	SimpleVertexShader* m_vertexShader;
	SimplePixelShader* m_pixelShader;

	void GetInstance() { m_references++; }
	void RemoveInstance() { m_references--; if (m_references == 0) delete this; }

	Material(SimpleVertexShader* p_vShader, SimplePixelShader* p_pShader);
	SimpleVertexShader* GetVertexShader() { return m_vertexShader; }
	SimplePixelShader* GetPixelShader() { return m_pixelShader; }
private:
	//will be changing simple shader to have a reference system when I have time, until then
	//it's more convenient to handle the shader freeing in Game.cpp
	~Material() { //m_vertexShader->RemoveInstance(); m_pixelShader->RemoveInstance();
	}
	int m_references;
	DirectX::XMFLOAT4 m_surfaceColor;
};

