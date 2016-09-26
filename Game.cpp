#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"Margaret Dorsey GGP",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{


#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	
	sampler->Release();
	woodTextureSRV->Release();
	metalTextureSRV->Release();
	//delete all the stuff we allocated
	if (e1) delete e1;
	if (e2) delete e2;
	if (e3) delete e3;
	if (camera) delete camera;
	if (pixelShader) delete pixelShader;
	if (vertexShader) delete vertexShader;
	
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	camera = new Camera(1280 /(float) 720);
	dlight = {DirectX::XMFLOAT4(0.1,0.1,0.1,1.0),DirectX::XMFLOAT4(0,0,.5,1), DirectX::XMFLOAT3(1,-1,0)};
	dlight2 = { DirectX::XMFLOAT4(0.0,0.0,0.0,1.0),DirectX::XMFLOAT4(.3,0,0,1), DirectX::XMFLOAT3(-1,1,0) };
	plight = { DirectX::XMFLOAT4(0,0,0,0),DirectX::XMFLOAT4(1,1,1,1), DirectX::XMFLOAT3(0,-1,0) };
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}




// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	//generate our three meshes
	//the mesh constructor will also work, but it's easier to just
	// let the computer generate our vertices and indices for us
	mesh1 = Mesh::LoadObj("../Assets/Models/cube.obj", device);
	mesh2 = Mesh::LoadObj("../Assets/Models/torus.obj", device);
	mesh3 = Mesh::LoadObj("../Assets/Models/helix.obj", device);

	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"../Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"../VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	if (!pixelShader->LoadShaderFile(L"../Debug/PixelShader.cso"))
		pixelShader->LoadShaderFile(L"../PixelShader.cso");

	//texture loading
	CreateWICTextureFromFile(
		device,
		context, // If I provide the context, it will auto-generate Mipmaps
		L"../Assets/Textures/metal.jpg",
		0, // We don't actually need the texture reference
		&metalTextureSRV);
	CreateWICTextureFromFile(
		device,
		context, 
		L"../Assets/Textures/wood.jpg",
		0, 
		&woodTextureSRV);
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &sampler);

	Material* material1 = new Material(vertexShader, pixelShader,metalTextureSRV,sampler);
	Material* material2 = new Material(vertexShader, pixelShader, woodTextureSRV, sampler);
	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
	e1 = new Entity(mesh1, material1);
	e1->SetPosition(XMFLOAT3(-2.5, 1.5, 0));
	e2 = new Entity(mesh2, material1);
	e2->SetPosition(XMFLOAT3(0, 1.5, 4));
	e3 = new Entity(mesh3, material2);
	e3->SetPosition(XMFLOAT3(0, -1.0, 0));
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	camera->SetProjection((float)width / height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	e1->Rotate(XMFLOAT3(0, 1, 0), 2 * deltaTime);
	e2->Rotate(XMFLOAT3(0, 1, 0), 2 * deltaTime);
	e2->Move(XMFLOAT3(deltaTime*XMScalarSin(totalTime), 0, 0));
	e3->Move(XMFLOAT3(-deltaTime*XMScalarCos( totalTime), 0, 0));

	//handle camera movement here until I can move this logic to an input manager
	if (GetAsyncKeyState('W') & 0x8000) { camera->Move(XMFLOAT3(0, 0, MOVE_SCALE*deltaTime)); }
	if (GetAsyncKeyState('S') & 0x8000) { camera->Move(XMFLOAT3(0, 0, -deltaTime*MOVE_SCALE)); }
	if (GetAsyncKeyState('A') & 0x8000) { camera->Move(XMFLOAT3(-deltaTime*MOVE_SCALE, 0, 0)); }
	if (GetAsyncKeyState('D') & 0x8000) { camera->Move(XMFLOAT3(deltaTime*MOVE_SCALE, 0, 0)); }
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) { camera->Move(XMFLOAT3(0, deltaTime*MOVE_SCALE, 0)); }
	if (GetAsyncKeyState('X') & 0x8000) { camera->Move(XMFLOAT3(0, -deltaTime*MOVE_SCALE, 0)); }

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.

	

	
	
	XMFLOAT4X4 world;
	//mesh1 
	
	pixelShader->SetData(
	"light", // The name of the variable in the shader
	&dlight, // The address of the data to copy
	sizeof(DirectionalLight)); // The size of the data to copy
	pixelShader->SetData(
	"light2", // The name of the variable in the shader
	&dlight2, // The address of the data to copy
	sizeof(DirectionalLight)); // The size of the data to copy
	pixelShader->SetData(
		"light3", // The name of the variable in the shader
		&plight, // The address of the data to copy
		sizeof(PointLight)); // The size of the data to copy


	e1->Draw(context, camera->GetView(), camera->GetProjection());

	
	//mesh 2
	pixelShader->SetData(
		"light", // The name of the variable in the shader
		&dlight, // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy
	pixelShader->SetData(
		"light2", // The name of the variable in the shader
		&dlight2, // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy
	pixelShader->SetData(
		"light3", // The name of the variable in the shader
		&plight, // The address of the data to copy
		sizeof(PointLight)); // The size of the data to copy
	
	

	e2->Draw(context, camera->GetView(), camera->GetProjection());
	
	//mesh 3
	pixelShader->SetData(
		"light", // The name of the variable in the shader
		&dlight, // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy
	pixelShader->SetData(
		"light2", // The name of the variable in the shader
		&dlight2, // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy
	pixelShader->SetData(
		"light3", // The name of the variable in the shader
		&plight, // The address of the data to copy
		sizeof(PointLight)); // The size of the data to copy

	

	e3->Draw(context, camera->GetView(), camera->GetProjection());
	

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	if(buttonState&0x0001) //left button down
		camera->Rotate(XMFLOAT3(CAMERA_DELTA*(y- prevMousePos.y), CAMERA_DELTA*(x - prevMousePos.x), 0));
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion