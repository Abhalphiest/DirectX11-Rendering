#pragma once
#include"../headers/Camera.h"
#include"../headers/DataStructs.h"
#include"../headers/Lights.h"
#include <DirectXMath.h>
#define uint unsigned int

class FirstPersonController
{
public:
    Camera* camera;

    FirstPersonController(float p_aspratio)
    {
        camera = new Camera(p_aspratio);
        fpcCollider = Collider(camera->GetPosition());
        fpcLight = SpotLight();
		fpcLight.Position = DirectX::XMFLOAT3(0.0, 0.0, 0.0);
    }

    FirstPersonController(float p_aspratio, Collider* p_collider)
    {
        camera = new Camera(p_aspratio);
    }

	~FirstPersonController() { if (camera) delete camera; }

    /**
     * Move
     * 
     * @param sceneColliders    - 
     * @param p_movevector      - 
     */
    void Move(std::vector<Collider> sceneColliders, DirectX::XMFLOAT3 p_movevector)
    {
	
        // First adjust our movement vector to account for rotation
        DirectX::XMFLOAT3 camRot = camera->GetRotation();
        DirectX::XMVECTOR rot_quaternion = DirectX::XMQuaternionRotationRollPitchYaw(camRot.x, camRot.y, camRot.z);

        DirectX::XMVECTOR move_vec = DirectX::XMLoadFloat3(&p_movevector);
        DirectX::XMVECTOR pos_vec = DirectX::XMLoadFloat3(&(camera->GetPosition()));

        // Rotate our movement vector to match our rotation
        move_vec = DirectX::XMVector3Transform(move_vec, DirectX::XMMatrixRotationQuaternion(rot_quaternion));

        move_vec = DirectX::XMVectorSetIntY(move_vec, 0);   // Prevent FPC from floating
        pos_vec = DirectX::XMVectorAdd(pos_vec, move_vec);

        DirectX::XMFLOAT3 cldr_pos;
        DirectX::XMStoreFloat3(&fpcCollider.m_position, pos_vec);
        fpcCollider.m_position =cldr_pos;

        // Check for collisions
        // TODO: Add culling algorithm of some sort to reduce number of collision checks
        // TODO: Implement "nudging" for when objects are barely colliding
        bool canMove = true;
        for (std::vector<uint>::size_type i = 0; i != sceneColliders.size() && canMove; ++i)
        {
            if (fpcCollider.IsColliding((sceneColliders[i])))
            {
                canMove = false;
            }
        }

        // If moving would not collide, update positions, else reset collider
        if (canMove)
        {
            camera->SetPosition(cldr_pos);
            fpcLight.Position = cldr_pos;
        }
        else
        {
            // Revert collider position to former (valid) location
            fpcCollider.m_position = camera->GetPosition();
        }
    }

    // Will turn fpcLight on or off
    void SetLightState(bool on)
    {
        lightOn = on;
    }

    // Returns whether fpcLight is on or off (without changing it)
    bool GetLightState()
    {
        return lightOn;
    }

private:
    FirstPersonController();
    SpotLight fpcLight;
    Collider fpcCollider;
    bool lightOn;
};