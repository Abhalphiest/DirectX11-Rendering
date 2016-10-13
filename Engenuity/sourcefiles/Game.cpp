#include "../headers/Game.h"
#include "../headers/Vertex.h"

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
	mTextureSRV->Release();
	specTextureSRV->Release();
	defaultSRV->Release();
	nTextureSRV->Release();
	defaultNSRV->Release();
	earthTextureSRV->Release();
	earthspecTextureSRV->Release();
	circuitNormalSRV->Release();
	crystalNormalSRV->Release();
	crystalSRV->Release();
	//delete all the stuff we allocated
	//if (e1) delete e1;
	//if (e2) delete e2;
	//if (e3) delete e3;
	//if (e4) delete e4;
	//if (camera) delete camera;
	if (scene) delete scene;
	if (fpc) delete fpc;
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
	fpc = new FirstPersonController(1280 / (float)720);
	dlight = DirectionalLight {DirectX::XMFLOAT4(0.1,0.1,0.1,1.0),DirectX::XMFLOAT4(0,0,.5,1), DirectX::XMFLOAT3(1,-1,0)};
	dlight2 = DirectionalLight { DirectX::XMFLOAT4(0.0,0.0,0.0,1.0),DirectX::XMFLOAT4(.3,0,0,1), DirectX::XMFLOAT3(-1,1,0) };
	plight = PointLight { DirectX::XMFLOAT4(.1,.1,.1,1),DirectX::XMFLOAT4(1,1,1,1), DirectX::XMFLOAT3(0,-1,0) };
	scene = new Scene(fpc);
	light1 = scene->AddDirectionalLight(dlight);
	light2 = scene->AddDirectionalLight(dlight2);
	light3 = scene->AddPointLight(plight);
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
	mesh1 = Mesh::LoadObj("Assets/Models/cube.obj", device);
	mesh2 = Mesh::LoadObj("Assets/Models/sphere.obj", device);
	mesh3 = Mesh::LoadObj("Assets/Models/helix.obj", device);
	mesh4 = Mesh::LoadObj("Assets/Models/fireplace.obj", device);

	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"../Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"../VertexShader.cso");
	pixelShader = new SimplePixelShader(device, context);
	if (!pixelShader->LoadShaderFile(L"../Debug/PixelShader.cso"))
		pixelShader->LoadShaderFile(L"../PixelShader.cso");

	NpixelShader = new SimplePixelShader(device, context);
	if (!NpixelShader->LoadShaderFile(L"../Debug/nPixelShader.cso"))
		NpixelShader->LoadShaderFile(L"../nPixelShader.cso");

	//texture loading
	CreateWICTextureFromFile(
		device,
		context, // If I provide the context, it will auto-generate Mipmaps
		L"Assets/Textures/metal.jpg",
		0, // We don't actually need the texture reference
		&metalTextureSRV);
	CreateWICTextureFromFile(
		device,
		context, 
		L"Assets/Textures/wood.jpg",
		0, 
		&woodTextureSRV);
	CreateWICTextureFromFile(
		device,
		context,
		L"Assets/Textures/earthdiffuse.jpg",
		0,
		&earthTextureSRV);
	CreateWICTextureFromFile(
		device,
		context,
		L"Assets/Textures/metalspec.jpg",
		0,
		&specTextureSRV);
	CreateWICTextureFromFile(
		device,
		context,
		L"Assets/Textures/earthspec.png",
		0,
		&earthspecTextureSRV);
	CreateWICTextureFromFile(
		device,
		context,
		L"Assets/Textures/multiply.jpg",
		0,
		&mTextureSRV);
	CreateWICTextureFromFile(
		device,
		context,
		L"Assets/Textures/default.jpg",
		0,
		&defaultSRV);
	CreateWICTextureFromFile(
		device,
		context,
		L"Assets/Textures/flatnormal.jpg",
		0,
		&defaultNSRV);
	CreateWICTextureFromFile(
		device,
		context,
		L"Assets/Textures/earthnormal.jpg",
		0,
		&nTextureSRV);
	CreateWICTextureFromFile(
		device,
		context,
		L"Assets/Textures/circuitnormal.jpg",
		0,
		&circuitNormalSRV);
	CreateWICTextureFromFile(
		device,
		context,
		L"Assets/Textures/crystalnormal.jpg",
		0,
		&crystalNormalSRV);
	CreateWICTextureFromFile(
		device,
		context,
		L"Assets/Textures/crystaldiffuse.jpg",
		0,
		&crystalSRV);
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &sampler);

	Material* material1 = new Material(vertexShader, pixelShader,metalTextureSRV,defaultSRV,
		specTextureSRV,defaultNSRV,sampler);
	Material* material2 = new Material(vertexShader, NpixelShader, earthTextureSRV,defaultSRV,
		earthspecTextureSRV,nTextureSRV,sampler);
	Material* material3 = new Material(vertexShader, NpixelShader, metalTextureSRV, defaultSRV,
		specTextureSRV, circuitNormalSRV, sampler);
	Material* material4 = new Material(vertexShader, NpixelShader, crystalSRV, defaultSRV,
		defaultSRV, crystalNormalSRV, sampler);

	object1 = scene->CreateObject(mesh1, material3);
	scene->SetObjectPosition(object1,XMFLOAT3(-2.5, 1.5, 0));
	object2 = scene->CreateObject(mesh2, material2);
	scene->SetObjectPosition(object2,XMFLOAT3(0, 0, 2));

	object3 = scene->CreateObject(mesh3, material1);
	scene->SetObjectPosition(object3,XMFLOAT3(0, -1.0, 0));
	object4 = scene->CreateObject(mesh4, material4);
	scene->SetObjectPosition(object4, XMFLOAT3(-5.0, 0, -2));
	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
	//e1 = new Entity(mesh1, material3);
	//e1->SetPosition(XMFLOAT3(-2.5, 1.5, 0));
	//e2 = new Entity(mesh2, material2);
	//e2->SetPosition(XMFLOAT3(0, 0, 2));
	//e3 = new Entity(mesh3, material1);
	//e3->SetPosition(XMFLOAT3(0, -1.0, 0));
	//e4 = new Entity(mesh4, material4);
	//e4->SetPosition(XMFLOAT3(-5.0, 0, -2));
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
	//e1->Rotate(XMFLOAT3(0, 1, 0), 2 * deltaTime);
	//e2->Rotate(XMFLOAT3(0, 1, 0), deltaTime);
	//e3->Move(XMFLOAT3(-deltaTime*XMScalarCos( totalTime), 0, 0));

	//handle camera movement here until I can move this logic to an input manager
	if (GetAsyncKeyState('W') & 0x8000) { fpc->camera->Move(XMFLOAT3(0, 0, MOVE_SCALE*deltaTime)); }
	if (GetAsyncKeyState('S') & 0x8000) { fpc->camera->Move(XMFLOAT3(0, 0, -deltaTime*MOVE_SCALE)); }
	if (GetAsyncKeyState('A') & 0x8000) { fpc->camera->Move(XMFLOAT3(-deltaTime*MOVE_SCALE, 0, 0)); }
	if (GetAsyncKeyState('D') & 0x8000) { fpc->camera->Move(XMFLOAT3(deltaTime*MOVE_SCALE, 0, 0)); }
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) { fpc->camera->Move(XMFLOAT3(0, deltaTime*MOVE_SCALE, 0)); }
	if (GetAsyncKeyState('X') & 0x8000) { fpc->camera->Move(XMFLOAT3(0, -deltaTime*MOVE_SCALE, 0)); }

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

/*	
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
	pixelShader->SetData(
		"cameraPos", // The name of the variable in the shader
		&camera->GetPosition(), // The address of the data to copy
		sizeof(XMFLOAT3)); // The size of the data to copy


	NpixelShader->SetData(
		"light", // The name of the variable in the shader
		&dlight, // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy
	NpixelShader->SetData(
		"light2", // The name of the variable in the shader
		&dlight2, // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy
	NpixelShader->SetData(
		"light3", // The name of the variable in the shader
		&plight, // The address of the data to copy
		sizeof(PointLight)); // The size of the data to copy
	NpixelShader->SetData(
		"cameraPos", // The name of the variable in the shader
		&camera->GetPosition(), // The address of the data to copy
		sizeof(XMFLOAT3)); // The size of the data to copy
	e1->Draw(context, camera->GetView(), camera->GetProjection());
	e2->Draw(context, camera->GetView(), camera->GetProjection());
	e3->Draw(context, camera->GetView(), camera->GetProjection());
	e4->Draw(context, camera->GetView(), camera->GetProjection());
*/
	std::vector<uint> objects = { object1,object2,object3,object4 };
	std::vector<uint> dlights = { light1,light2};
	std::vector<uint> plights = { light3 };
	std::vector<uint> slights = {};
	scene->Render(context, objects, dlights, plights, slights);
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
		fpc->camera->Rotate(XMFLOAT3(CAMERA_DELTA*(y- prevMousePos.y), CAMERA_DELTA*(x - prevMousePos.x), 0));
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