#pragma once
#include<DirectXMath.h>
#include"DXCore.h"
#include<vector>
#include"SimpleShader.h"
struct ParticleData
{
	DirectX::XMFLOAT3 Position; // The position of the vertex
	int initialtime; //for letting the vertex shader calculate positions
	int lifetime;
	float size;
};
struct Particle
{
	ParticleData data;
	bool active;
};

enum PARTICLE_TYPE //to facilitate correct sorting with depth
{

};
class ParticleSystem
{
	int m_maxEmission; //maximum number of particles active at any one time
	int m_emissionRate; //particles per second emitted
	bool m_continuous; //go continually forever, or just one burst?
	
	bool m_emittance; //do we emit light?
	DirectX::XMFLOAT4 m_lightcolor; //color of emitted light if emittance is true
	DirectX::XMFLOAT4 m_color; //color if no texture provided
	int m_minlifetime, m_maxlifetime; //minimum and maximum lifetimes of emitted particles
	float m_minsize, m_maxsize; //minimum and maximum sizes of emitted particles
	ID3D11ShaderResourceView* m_textureSRV; //texture for particles
	PARTICLE_TYPE m_particletype; //determines how we set up alpha

	SimplePixelShader* m_ps;
	SimpleVertexShader* m_vs;

	int front; //index for particle vector
	std::vector<Particle> particles; //cyclic vector for particles



};