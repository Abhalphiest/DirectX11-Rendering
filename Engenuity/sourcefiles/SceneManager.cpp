#include"../headers/SceneManager.h"

SceneManager* SceneManager::m_instance = nullptr;

SceneManager::~SceneManager()
{
	for (int i = 0; i < m_sceneList.size(); i++)
	{
		if (m_sceneList[i])
			ReleaseScene(i);
	}
	delete m_FPC;
	if (m_samplerState)
		m_samplerState->Release();
}
unsigned int SceneManager::LoadScene(char* p_filename)
{
	//create new scene
	Scene* newScene = new Scene(m_FPC);
	SceneData newData = SceneData();
	//for loading all textures, meshes, etc and making our materials
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	std::vector<SimplePixelShader*> pixelShaders;
	std::vector<SimpleVertexShader*> vertexShaders;
	std::vector<ID3D11ShaderResourceView*> dsrvList; //diffuse srvs
	std::vector<ID3D11ShaderResourceView*> nsrvList; //normal srvs
	std::vector<ID3D11ShaderResourceView*> ssrvList; //spec srvs
	std::vector<ID3D11ShaderResourceView*> msrvList; //multiply srvs

	// File input object
	std::ifstream file(p_filename);

	// Check for successful open
	if (!file.is_open())
		return -1;

	char chars[100];                     // String for line reading

	int i = 0; //iterator
	std::string indices[6];	//indices for building the materials
	std::string s; //temporary string
	ID3D11ShaderResourceView* d_srv, *m_srv, *n_srv, *s_srv;
	size_t pos, pos2;
	float x, y, z, w;
										
	while (file.getline(chars, 100) && chars[0]!='%') //while in resources section of file
	{
		i = 0;
		while (chars[i] != '\n' && iswspace(chars[i])) i++;
		if (chars[i] == '\n' || chars[i] == '\0' || chars[i] == '#')
			continue; //go to next line if this one is over or a comment

		switch (chars[i]) //first not whitespace character tells us how to interpret
		{
		case 'o': //object file
		{
			Mesh* mesh = Mesh::LoadObj(chars + i + 2, m_device); //add on the number of characters
															 //of the data label
			if (!mesh)
				printf("Failed to load mesh %s\n",chars+i+2);
			meshes.push_back(mesh);
			break;
		}
		case 't': //diffuse texture
		{
			s = std::string(chars + i + 2);
			std::wstring wstr(s.begin(), s.end()); //wide strings... ugh

			DirectX::CreateWICTextureFromFile(
				m_device,
				m_context, // If I provide the context, it will auto-generate Mipmaps
				wstr.c_str(),
				0, // We don't actually need the texture reference
				&d_srv);
			if (!d_srv)
				printf("Failed to load diffuse texture %s\n", wstr.c_str());
			dsrvList.push_back(d_srv);
			break;
		}
		case 'n': //normal map
		{
			s = std::string(chars +i+ 2);
			std::wstring wstr(s.begin(), s.end()); //wide strings... ugh

			DirectX::CreateWICTextureFromFile(
				m_device,
				m_context, // If I provide the context, it will auto-generate Mipmaps
				wstr.c_str(),
				0, // We don't actually need the texture reference
				&n_srv);
			if (!n_srv)
				printf("Failed to load normal texture %s\n", wstr.c_str());
			nsrvList.push_back(n_srv);
			break;
		}
		case 's': //spec map
		{
			s = std::string(chars + i+ 2); 
			std::wstring wstr(s.begin(), s.end()); //wide strings... ugh

			DirectX::CreateWICTextureFromFile(
				m_device,
				m_context, // If I provide the context, it will auto-generate Mipmaps
				wstr.c_str(),
				0, // We don't actually need the texture reference
				&s_srv);
			if (!s_srv)
				printf("Failed to load specular texture %s\n", wstr.c_str());
			ssrvList.push_back(s_srv);
			break;
		}
		case 'm': //multiply map
		{
			s = std::string(chars +i+ 2);
			std::wstring wstr(s.begin(), s.end()); //wide strings... ugh

			DirectX::CreateWICTextureFromFile(
				m_device,
				m_context, // If I provide the context, it will auto-generate Mipmaps
				wstr.c_str(),
				0, // We don't actually need the texture reference
				&m_srv);
			if (!m_srv)
				printf("Failed to load multiply texture %s", wstr.c_str());
			msrvList.push_back(m_srv);
			break;
		}
		case 'p': //pixel shader
		{
			SimplePixelShader* pixelShader = new SimplePixelShader(m_device, m_context);
			s = std::string(chars +i+ 3);
			std::wstring wstr(s.begin(), s.end()); //wide strings... ugh
			std::wstring fwstr = L"../x64/Debug/";
			if (!pixelShader->LoadShaderFile(fwstr.append(wstr).c_str()))
			{
				fwstr = L"../x64/";
				if(!pixelShader->LoadShaderFile(fwstr.append(wstr).c_str()))
					printf("Failed to load pixel shader %s\n", wstr.c_str());
			}
			pixelShaders.push_back(pixelShader);
			break;
		}
		case 'v': //vertex shader
		{
			SimpleVertexShader* vertexShader = new SimpleVertexShader(m_device, m_context);
			s = std::string(chars + i + 3);
			std::wstring wstr(s.begin(), s.end()); //wide strings... ugh
			std::wstring fwstr = L"../x64/Debug/";
			if (!vertexShader->LoadShaderFile(fwstr.append(wstr).c_str()))
			{
				fwstr = L"../x64/";

				if(!vertexShader->LoadShaderFile(fwstr.append(wstr).c_str()))
					printf("Failed to load vertex shader %s\n", wstr.c_str());
			}
			vertexShaders.push_back(vertexShader);
			break;
		}
		}
		
	}


	//moving on to constructing materials
	while (file.getline(chars, 100) && chars[0] != '$')
	{
		i = 0;
		while (chars[i] != '\n' && iswspace(chars[i])) i++;
		if (chars[i] == '\n' || chars[i] == '\0' || chars[i] == '#')
			continue; //go to next line if this one is over or a comment

		s = std::string(chars+i);
		pos = s.find('/');
		pos2 = 0;
		i = 0;
		//get our indices
		while (pos != std::string::npos)
		{
			indices[i] = s.substr(pos2, pos - pos2);
			pos2 = ++pos;
			pos = s.find('/', pos2);
			i++;
			if (pos == std::string::npos)
				indices[i] = (s.substr(pos2, s.length())); //get the last one too
		}
		//create material
		
			d_srv = dsrvList[(uint)std::stoul(indices[0], NULL, 10)];
			m_srv = msrvList[(uint)std::stoul(indices[3], NULL, 10)];
			s_srv = ssrvList[(uint)std::stoul(indices[2], NULL, 10)];
			n_srv = nsrvList[(uint)std::stoul(indices[1], NULL, 10)];
		
		
		Material* material = new Material(vertexShaders[(uint)std::stoul(indices[4], NULL, 10)],
			pixelShaders[(uint)std::stoul(indices[5], NULL, 10)],
			d_srv, m_srv, s_srv, n_srv,m_samplerState);
		materials.push_back(material);

	}
	//now we make objects
	while (file.getline(chars, 100) && chars[0] != '&')
	{
		i = 0;
		while (chars[i] != '\n' && iswspace(chars[i])) i++;
		if (chars[i] == '\n' || chars[i] == '\0' || chars[i] == '#')
			continue; //go to next line if this one is over or a comment

		s = std::string(chars+i);
		pos = s.find('/');
		
		Mesh* mesh = meshes[std::stoul(s.substr(0, pos), NULL, 10)];
		Material* material = materials[std::stoul(s.substr(pos+1, s.length()), NULL, 10)];
		uint object = newScene->CreateObject(mesh, material);
		newData.m_indices.push_back(object);
		if (!file.getline(chars, 100))
		{
			//if we want to do error handling in the future
		}

		i = skipWSandComments(chars, &file);
			
		s = std::string(chars+i);
		pos = s.find(' ');
		pos2 = s.find(' ', ++pos);
		x = std::stof(s.substr(pos, pos2), NULL);
		pos = pos2;
		pos2 = s.find(' ', ++pos);
		y = std::stof(s.substr(pos, pos2), NULL);
		z = std::stof(s.substr(pos, s.length()), NULL);
		//set up position, orientation, scale vectors
		newScene->SetObjectPosition(object,DirectX::XMFLOAT3(x, y, z));

		//orientation
		if (!file.getline(chars, 100))
		{
			//if we want to do error handling in the future
		}

		i = skipWSandComments(chars, &file);

		s = std::string(chars + i);
		pos = s.find(' ');
		pos2 = s.find(' ', ++pos);
		x = std::stof(s.substr(pos, pos2), NULL);
		pos = pos2;
		pos2 = s.find(' ', ++pos);
		y = std::stof(s.substr(pos, pos2), NULL);
		z = std::stof(s.substr(pos, s.length()), NULL);
		newScene->SetObjectOrientation(object, DirectX::XMFLOAT3(x, y, z));

		//scale
		if (!file.getline(chars, 100))
		{
			//if we want to do error handling in the future
		}

		i = skipWSandComments(chars, &file);

		s = std::string(chars + i);
		pos = s.find(' ');
		pos2 = s.find(' ', ++pos);
		x = std::stof(s.substr(pos, pos2), NULL);
		newScene->SetObjectScale(object, x);

		//done with this object
	}

	//now we make the lights
	while (file.getline(chars, 100))
	{
		i = 0;
		while (chars[i] != '\n' && iswspace(chars[i])) i++;
		if (chars[i] == '\n' || chars[i] == '\0' ||chars[i] == '#')
			continue; //go to next line if this one is over or a comment

		switch (chars[i])
		{
		case 'd': //directional light
		{	
			file.getline(chars, 100);
			i = skipWSandComments(chars, &file);
			s = std::string(chars + i);
			pos = s.find(' ');
			x = std::stof(s.substr(0, pos), NULL);
			pos2 = s.find(' ', ++pos);
			y = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			z = std::stof(s.substr(pos, pos2), NULL);
			w = std::stof(s.substr(pos2, s.length()), NULL);
			DirectX::XMFLOAT4 ambient(x, y, z, w);

			file.getline(chars, 100);
			i = skipWSandComments(chars, &file);
			s = std::string(chars + i);
			pos = s.find(' ');
			x = std::stof(s.substr(0, pos), NULL);
			pos2 = s.find(' ', ++pos);
			y = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			z = std::stof(s.substr(pos, pos2), NULL);
			w = std::stof(s.substr(pos2, s.length()), NULL);
			DirectX::XMFLOAT4 diffuse(x, y, z, w);

			file.getline(chars, 100);
			i = skipWSandComments(chars, &file);
			s = std::string(chars + i);
			pos = s.find(' ');
			x = std::stof(s.substr(0, pos), NULL);
			pos2 = s.find(' ', ++pos);
			y = std::stof(s.substr(pos, pos2), NULL);			
			z = std::stof(s.substr(pos2, s.length()), NULL);
			DirectX::XMFLOAT3 direction(x, y, z);

			uint index = newScene->AddDirectionalLight({ ambient,diffuse,direction });
			newData.m_dlights.push_back(index);
			break;
		}
		case 'p': //point light
		{
			file.getline(chars, 100);
			i = skipWSandComments(chars, &file);
			s = std::string(chars + i);
			pos = s.find(' ');
			pos2 = s.find(' ', ++pos);
			x = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			y = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			z = std::stof(s.substr(pos, pos2), NULL);
			w = std::stof(s.substr(pos2, s.length()), NULL);
			DirectX::XMFLOAT4 ambient(x, y, z, w);

			file.getline(chars, 100);
			i = skipWSandComments(chars, &file);
			s = std::string(chars + i);
			pos = s.find(' ');
			pos2 = s.find(' ', ++pos);
			x = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			y = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			z = std::stof(s.substr(pos, pos2), NULL);
			w = std::stof(s.substr(pos2, s.length()), NULL);
			DirectX::XMFLOAT4 diffuse(x, y, z, w);

			file.getline(chars, 100);
			i = skipWSandComments(chars, &file);
			s = std::string(chars + i);
			pos = s.find(' ');
			pos2 = s.find(' ', ++pos);
			x = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			y = std::stof(s.substr(pos, pos2), NULL);
			z = std::stof(s.substr(pos2, s.length()), NULL);
			DirectX::XMFLOAT3 position(x, y, z);

			uint index = newScene->AddPointLight({ ambient,diffuse,position});
			newData.m_plights.push_back(index);
			break;
		}
		case 's': //spot light
		{
			file.getline(chars, 100);
			i = skipWSandComments(chars, &file);
			s = std::string(chars + i);
			pos = s.find(' ');
			pos2 = s.find(' ', ++pos);
			x = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			y = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			z = std::stof(s.substr(pos, pos2), NULL);
			w = std::stof(s.substr(pos2, s.length()), NULL);
			DirectX::XMFLOAT4 ambient(x, y, z, w);

			file.getline(chars, 100);
			i = skipWSandComments(chars, &file);
			s = std::string(chars + i);
			pos = s.find(' ');
			pos2 = s.find(' ', ++pos);
			x = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			y = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			z = std::stof(s.substr(pos, pos2), NULL);
			w = std::stof(s.substr(pos2, s.length()), NULL);
			DirectX::XMFLOAT4 diffuse(x, y, z, w);

			file.getline(chars, 100);
			i = skipWSandComments(chars, &file);
			s = std::string(chars + i);
			pos = s.find(' ');
			pos2 = s.find(' ', ++pos);
			x = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			y = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			z = std::stof(s.substr(pos, pos2), NULL);
			w = std::stof(s.substr(pos2, s.length()), NULL);
			DirectX::XMFLOAT4 dirangle (x, y, z, w);

			file.getline(chars, 100);
			i = skipWSandComments(chars, &file);
			s = std::string(chars + i);
			pos = s.find(' ');
			pos2 = s.find(' ', ++pos);
			x = std::stof(s.substr(pos, pos2), NULL);
			pos = pos2;
			pos2 = s.find(' ', ++pos);
			y = std::stof(s.substr(pos, pos2), NULL);
			z = std::stof(s.substr(pos2, s.length()), NULL);
			DirectX::XMFLOAT3 position(x, y, z);
			uint index = newScene->AddSpotLight({ ambient,diffuse,dirangle,position});
			newData.m_slights.push_back(index);
			break;
		}
		}
	}

	//close the file
	file.close();

	//get index of
	uint index;
	//build our big srv list
	std::vector<ID3D11ShaderResourceView*> srv;
	srv.insert(srv.end(), dsrvList.begin(), dsrvList.end());
	srv.insert(srv.end(), nsrvList.begin(), nsrvList.end());
	srv.insert(srv.end(), msrvList.begin(), msrvList.end());
	srv.insert(srv.end(), ssrvList.begin(), ssrvList.end());

	if (m_freedIndices.empty() == true)
	{
		index = m_sceneList.size();
		m_sceneList.push_back(newScene);
		m_dataList.push_back(newData);
		m_srvList.push_back(srv);
	}
	else
	{
		index = m_freedIndices.front();
		m_sceneList[index] = newScene;
		m_dataList[index] = newData;
		m_srvList[index]=srv;
		m_freedIndices.pop();
	}
	
	//return our index
	return index;
}
void SceneManager::ReleaseScene(unsigned int p_index)
{
	//release all the SRV's first
	for (int i = 0; i < m_srvList[p_index].size(); i++)
	{
		if(m_srvList[p_index][i])
			m_srvList[p_index][i]->Release();
	}
	m_srvList[p_index].clear();
	delete m_sceneList[p_index];
	m_sceneList[p_index] = nullptr;
	m_freedIndices.push(p_index); //we can use this index again now
}


SceneManager::SceneManager()
{
	//make first person controller
	m_FPC = new FirstPersonController(1280 / (float)720);
	
}
void SceneManager::SetSamplerState(D3D11_SAMPLER_DESC samplerDesc)
{
	if (m_samplerState)
		m_samplerState->Release();
	m_device->CreateSamplerState(&samplerDesc, &m_samplerState);
}
void SceneManager::SetSamplerState()
{
	if(m_samplerState)
		m_samplerState->Release();
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_device->CreateSamplerState(&samplerDesc, &m_samplerState);
}
int SceneManager::skipWSandComments(char* chars, std::ifstream* file)
{
	int i = 0;
	while (chars[i] != '\n' && iswspace(chars[i])) i++;
	while ((chars[i] == '\n' || chars[i] == '\0' || chars[i] == '#') && ((*file).getline(chars, 100)))
	{
		i = 0;
		while (chars[i] != '\n' && iswspace(chars[i])) i++;
	}
	return i;
}

void SceneManager::RenderCurrentScene()
{
	m_sceneList[m_currScene]->Render(m_context,
		m_dataList[m_currScene].m_indices,
		m_dataList[m_currScene].m_dlights,
		m_dataList[m_currScene].m_plights,
		m_dataList[m_currScene].m_slights);
	int i = 0; //just here to set a breakpoint
}