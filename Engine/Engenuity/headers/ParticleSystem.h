#pragma once
#include<DirectXMath.h>
#include"DXCore.h"
#include<vector>
#include"SimpleShader.h"
#include "Camera.h"

using DirectX::XMFLOAT4X4;

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
    int m_maxEmission;  //maximum number of particles active at any one time
    int m_emissionRate; //particles per second emitted
    bool m_continuous;  //go continually forever, or just one burst?

    bool m_emittance;                       //do we emit light?
    DirectX::XMFLOAT4 m_lightcolor;         //color of emitted light if emittance is true
    DirectX::XMFLOAT4 m_color;              //color if no texture provided
    int m_minlifetime, m_maxlifetime;       //minimum and maximum lifetimes of emitted particles
    float m_minsize, m_maxsize;             //minimum and maximum sizes of emitted particles
    ID3D11ShaderResourceView* m_textureSRV; //texture for particles
    PARTICLE_TYPE m_particletype;           //determines how we set up alpha

    SimplePixelShader* m_ps;
    SimpleVertexShader* m_vs;
    SimpleGeometryShader* m_gs;
    SimpleVertexShader* m_spawnvs;
    SimpleGeometryShader* m_spawngs;
    
    ID3D11SamplerState* m_sampler;
    ID3D11BlendState* m_blendstate;
    ID3D11DepthStencilState* m_depthstate;

    // Need at least 2 buffers - one for our initial data we pass to the Geometry shader that spawns particles, and one buffer for the GS itself

    int front;                          //indices for actives in particle vector
    std::vector<Particle> particles;    //cyclic vector for particles

public:
    ParticleSystem() {}

    ParticleSystem(int p_maxEmission, int p_emissionRate, bool p_continuous, bool p_emittance, SimplePixelShader* p_ps, SimpleVertexShader* p_vs, SimpleGeometryShader* p_gs, SimpleVertexShader* p_spawnvs, SimpleGeometryShader* p_spawngs, ID3D11SamplerState* p_sampler, ID3D11BlendState* p_blendstate, ID3D11DepthStencilState* p_depthstate)
    {
        m_maxEmission = p_maxEmission;
        m_emissionRate = p_emissionRate;
        m_continuous = p_continuous;
        m_emittance = p_emittance;

        m_ps = p_ps;
        m_vs = p_vs;
        m_gs = p_gs;
        m_spawnvs = p_spawnvs;
        m_spawngs = p_spawngs;

        m_sampler = p_sampler;
        m_blendstate = p_blendstate;
        m_depthstate = p_depthstate;


        // Setup particle start position, velocity, etc.

        // Setup the data we want to put into the vertex shader (everything in VSinput)
        // Create the initial vertex buffer (need this to send the right stuff to spawngs)

        // Example used 1D texture w/ random data to get randomized values in the shader - if we want to do that, we'd set it up here

        // Setup texture

        // Setup texture SRV

        // Create Stream Out buffer - example had a 2 buffer swap system going on, which is cool, but I'm not sure I want to get into that unless I need to
        // either way, for the spawngs buffer, will use something like m_spawngs->CreateCompatibleStreamOutBuffer(&OurSOBuffer, 1000000);
        // Big number is the vertex count for the buffer
        // Also, if we go the way of the swap buffers, will need a helper swap function
    }

    // Effectively does double duty as both Draw and Update
    void Render(ID3D11DeviceContext * context, Camera* camera, float deltaTime, float totalTime)
    {
        // Spawn particles
        Spawn(context, deltaTime, totalTime);

        // Draw particles ----------------------------------------------------

        m_gs->SetMatrix4x4("world", XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)); // Identity
        m_gs->SetMatrix4x4("view", camera->GetView());
        m_gs->SetMatrix4x4("projection", camera->GetProjection());
        m_gs->CopyAllBufferData();

        // Let vertex shader figure out acceleration on its own
        m_vs->SetFloat("maxLifetime", m_maxlifetime);
        m_vs->CopyAllBufferData();

        m_ps->SetSamplerState("trilinear", m_sampler);
        m_ps->SetShaderResourceView("particleTexture", m_textureSRV);
        m_ps->CopyAllBufferData();

        m_gs->SetShader();
        m_vs->SetShader();
        m_ps->SetShader();

        // Set up states
        float factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        context->OMSetBlendState(m_blendstate, factor, 0xffffffff);
        context->OMSetDepthStencilState(m_depthstate, 0);

        // Set buffers
        UINT particleStride = sizeof(ParticleData);
        UINT particleOffset = 0;
        // Need to pass this our main stream output buffer that we're reading from
        //context->IASetVertexBuffers(0, 1, &soBufferRead, &particleStride, &particleOffset);

        // Draw auto - draws based on current stream out buffer
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
        context->DrawAuto();

        // Unset Geometry Shader for next frame and reset states
        context->GSSetShader(0, 0, 0);
        context->OMSetBlendState(0, factor, 0xffffffff);
        context->OMSetDepthStencilState(0, 0);

        // Reset topology
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void Spawn(ID3D11DeviceContext * context, float deltaTime, float totalTime)
    {
        // Set topology for pointlist (since we're spawning from points)
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
        UINT stride = sizeof(ParticleData);
        UINT offset = 0;

        // setup spawnGS and all its "stuff"
        // Set all of our external data (delta time, at the very least
        // Set sampler/srv
        m_spawngs->SetShader();
        m_spawngs->CopyAllBufferData();

        // setup spawnVS
        m_spawnvs->SetShader();
        m_spawnvs->CopyAllBufferData();

        // Cancel out PS shader (since we don't want spawn to go to the screen) & unbind vertex buffer
        context->PSSetShader(0, 0, 0);

        ID3D11Buffer* unset = 0;
        context->IASetVertexBuffers(0, 1, &unset, &stride, &offset);

        // "Draw" (spawn) using buffer
        
        // Unbind SO targets and shader, since we're done with spawnGS for the moment
        SimpleGeometryShader::UnbindStreamOutStage(context);
        context->GSSetShader(0, 0, 0);
    }
};