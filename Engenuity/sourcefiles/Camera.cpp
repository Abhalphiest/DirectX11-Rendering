#include"../headers/Camera.h"


Camera::Camera(float p_aspratio)
{
	//initialize everything to default values
    // TODO: Hacky fixed values for demo. Should really have these be set in scene file, no?
	m_pos = DirectX::XMFLOAT3(-0.5, 1.3, -1);
	m_rot = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMStoreFloat4x4(&m_view, DirectX::XMMatrixIdentity());
	SetProjection(p_aspratio);
}

void Camera::Update()
{
	//get a quaternion based on our rotation values
	DirectX::XMVECTOR rot_quaternion = DirectX::XMQuaternionRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z);

	//set up all our DirectX structures
	DirectX::XMFLOAT3 forward = DirectX::XMFLOAT3(0,0,1);
	DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0, 1, 0);
	DirectX::XMVECTOR f_vec = DirectX::XMLoadFloat3(&forward);
	DirectX::XMVECTOR u_vec = DirectX::XMLoadFloat3(&up);

	//rotate our forward and up vector
	f_vec = DirectX::XMVector3Transform(f_vec, DirectX::XMMatrixRotationQuaternion(rot_quaternion));
	u_vec = DirectX::XMVector3Transform(u_vec, DirectX::XMMatrixRotationQuaternion(rot_quaternion));

	//store our view matrix
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&m_pos), f_vec, u_vec);
	DirectX::XMStoreFloat4x4(&m_view, DirectX::XMMatrixTranspose(view)); //transpose for HLSL
	return;
}

void Camera::Move(DirectX::XMFLOAT3 p_movevector)
{
	DirectX::XMVECTOR rot_quaternion = DirectX::XMQuaternionRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z);

	//set up all our DirectX structures
	DirectX::XMVECTOR move_vec = DirectX::XMLoadFloat3(&p_movevector);
	DirectX::XMVECTOR pos_vec = DirectX::XMLoadFloat3(&m_pos);

	//rotate our movement vector to match our rotation
	move_vec = DirectX::XMVector3Transform(move_vec, DirectX::XMMatrixRotationQuaternion(rot_quaternion));

	//add our adjusted movement vector and update our position
	pos_vec = DirectX::XMVectorAdd(pos_vec, move_vec);
	DirectX::XMStoreFloat3(&m_pos, pos_vec);
	return;

	
}

void Camera::Rotate(DirectX::XMFLOAT3 p_rotvector)
{
	//set up vectors
	DirectX::XMVECTOR rot_vec = DirectX::XMLoadFloat3(&p_rotvector);
	DirectX::XMVECTOR orientation = DirectX::XMLoadFloat3(&m_rot);

	//update our orientation
	orientation = DirectX::XMVectorAdd(orientation, rot_vec);
	//store our new member rotation vector
	DirectX::XMStoreFloat3(&m_rot, orientation);
	return;
}

void Camera::SetProjection(float p_aspratio)
{
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		p_aspratio,					// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&m_projection, DirectX::XMMatrixTranspose(P)); // Transpose for HLSL!
}