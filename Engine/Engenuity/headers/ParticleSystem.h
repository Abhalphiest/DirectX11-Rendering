#pragma once
#include<DirectXMath.h>
#include"DXCore.h"
#include<vector>
#include"SimpleShader.h"
#include "Camera.h"

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

    int front;                     //indices for actives in particle vector
    std::vector<Particle> particles;    //cyclic vector for particles

public:
    ParticleSystem() {}

    ParticleSystem(int p_maxEmission, int p_emissionRate, bool p_continuous, bool p_emittance)
    {
        m_maxEmission = p_maxEmission;
        m_emissionRate = p_emissionRate;
        m_continuous = p_continuous;
        m_emittance = p_emittance;
    }

    // TODO: Move to contructor, has to happen before we can do anything interesting anyway
    void SetShaders(SimplePixelShader* p_ps, SimpleVertexShader* p_vs)
    {
        m_ps = p_ps;
        m_vs = p_vs;
    }

    void Update(float dt)
    {
        // Using max_emission gets around need for "end" index
        int particlesToUpdate = m_maxEmission;
        int currIndex = front;
        while (particlesToUpdate > 0)
        {
            Particle currParticle = particles[currIndex];
            // Make sure particle *can* be alive
            if (currParticle.data.lifetime >= m_minlifetime)
            {
                currParticle.data.lifetime += dt;
                if (currParticle.data.lifetime >= m_maxlifetime)
                {
                    // Particle died, update cycle
                    currParticle.active = false;
                    front++;
                    if (front >= particles.size())
                    {
                        front = 0;  // Wrap
                    }
                }
                else
                {
                    // Use particle!

                    // For now, same interpolation as in example
                    float agePercent = currParticle.data.lifetime / m_maxlifetime;

                    currParticle.data.size = m_minsize + agePercent * (m_maxsize - m_minsize);

                    // Wasn't sure if we wanted movement to be on CPU or GPU side. . .
                    //  But this is where we'd take currParticle.data.Position and
                    //  adjust it by some amount, if we were to do it on CPU

                    // currParticle.data.Position += some sinusoidal curve function, passing agePercent as input? Maybe?
                }
            }

            currIndex++;
            if (currIndex >= particles.size())
            {
                currIndex = 0;  // Wrap around list
            }
            particlesToUpdate--;
        }

        // TODO: Next spawn new particles, if appropriate (based on dt)

    }

    void Render(ID3D11DeviceContext* context, Camera* camera)
    {
        // TODO: First Copy to GPU to make sure our updates take affect

        // Second, set up buffers
        UINT stride = sizeof(Particle);
        UINT offset = 0;

        // Grab the buffers (once we make them. . .)
        ID3D11Buffer* particleVB;
        ID3D11Buffer* particleIB;
        context->IASetVertexBuffers(0, 1, &particleVB, &stride, &offset);
        context->IASetIndexBuffer(particleIB, DXGI_FORMAT_R32_UINT, 0);

        // Set up shaders
        m_vs->SetMatrix4x4("view", camera->GetView());
        m_vs->SetMatrix4x4("projection", camera->GetProjection());
        m_vs->CopyAllBufferData();
        m_vs->SetShader();

        m_ps->SetShaderResourceView("particle", m_textureSRV);
        m_ps->CopyAllBufferData();
        m_ps->SetShader();

        // Actually draw
        int particlesToUpdate = m_maxEmission;
        int currIndex = front;
        while (particlesToUpdate > 0)
        {
            // * 6 because particles rendered as quad, so 2 tris, so 6 verts
            // Made need a different index count than m_maxEmission, not sure
            context->DrawIndexed(m_maxEmission * 6, front * 6, 0);

            currIndex++;
            if (currIndex >= particles.size())
            {
                currIndex = 0;  // Wrap
            }
            particlesToUpdate--;
        }
    }
};