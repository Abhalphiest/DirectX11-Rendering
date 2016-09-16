#pragma once
#include <DirectXMath.h>
class Camera
{
public:
	Camera(float p_aspratio);

	void Update();
	void Move(DirectX::XMFLOAT3 p_movevector);
	void Camera::Rotate(DirectX::XMFLOAT3 p_rotvector);

	DirectX::XMFLOAT3 GetRotation(void) { return m_rot; }
	void SetRotation(DirectX::XMFLOAT3 p_rot) { m_rot = p_rot; }
	DirectX::XMFLOAT3 GetPosition(void) { return m_pos; }
	void SetPosition(DirectX::XMFLOAT3 p_pos) { m_pos = p_pos; }
	DirectX::XMFLOAT4X4 GetView(void) { Update(); return m_view; }
	DirectX::XMFLOAT4X4 GetProjection(void) { return m_projection; }
	void SetProjection(float p_aspratio);
private:
	Camera();
	DirectX::XMFLOAT4X4 m_view, m_projection;
	DirectX::XMFLOAT3 m_rot,m_pos; //also have Z coordinate because hey, more freedom
												//is never a bad thing

};