#pragma once
#include <DirectXMath.h>
class Camera
{
public:
	Camera(float p_aspratio);

	/**
	* Update re-applies all camera information to ensure correct facing.
	*/
	void Update();

	/**
	* Move takes the provided displacement vector and adds it to the Camera's
	* current position. Note that visible change will not occur until
	* call to Update().
	*
	* @param p_movevector   - the displacement vector
	*/
	void Move(DirectX::XMFLOAT3 p_movevector);

	/**
	* Rotate takes the provided rotation vector and adds it to the Camera's
	* current orientation. Note that visible change will not occur until
	* call to Update().
	*
	* @param p_rotvector    - the rotation vector
	*/
	void Rotate(DirectX::XMFLOAT3 p_rotvector);

	void SetRotation(DirectX::XMFLOAT3 p_rot) { m_rot = p_rot; }
	void SetPosition(DirectX::XMFLOAT3 p_pos) { m_pos = p_pos; }
	void SetProjection(float p_aspratio);

	DirectX::XMFLOAT3 GetRotation(void) { return m_rot; }
	DirectX::XMFLOAT3 GetPosition(void) { return m_pos; }
	DirectX::XMFLOAT4X4 GetView(void) { Update(); return m_view; }
	DirectX::XMFLOAT4X4 GetProjection(void) { return m_projection; }

private:
	Camera();   // Why the private default constructor?
	DirectX::XMFLOAT4X4 m_view, m_projection;
	DirectX::XMFLOAT3 m_rot, m_pos; // Also have Z coordinate because hey, more freedom is never a bad thing
};