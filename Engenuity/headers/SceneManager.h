#pragma once

#include<vector>
#include<string>
#include<queue>
#include<map>
#include<stdio.h>
#include<wctype.h>
#include<sstream>
#include"FirstPersonController.h"
#include"Scene.h"

#define DIRECTIONALLIGHT 0
#define POINTLIGHT 1
#define SPOTLIGHT 2


//Because of the structure of this project,
//each of our "scenes" will likely be modular, small, manageable environments
//for this reason, the management system is written mostly to allow for complete
//modularity and less for reducing redundant resource creation (for example, it is not
//unlikely that we will waste space and time loading the same walls and wall textures for every
//scene, but it would either add more complexity or limit the freedom of the system to try to share
//walls or assume there will always be walls, etc.

//In a project of this scope I do not expect to run into memory wastefulness/non-optimization problems
//and if I was going to go that far it would require essentially pseudo-caches of common resources,
//etc..

class SceneManager
{
public:
	//singleton functionality
	static SceneManager* getInstance()
	{
		if (m_instance == nullptr) m_instance = new SceneManager(); 
		return m_instance;
	}
	~SceneManager();

	//actual functions
	FirstPersonController* getFPC() { return m_FPC; }
	unsigned int LoadScene(char* p_filename);
	void ReleaseScene(unsigned int p_index);
	Scene* GetScene(unsigned int p_index) { return m_sceneList[p_index]; }
	
	void SetDevice(ID3D11Device* device) { m_device = device; }
	void SetContext(ID3D11DeviceContext* context) { m_context = context; }
	//toggle lights and such will come later, right now we just always draw all the lights
	//and all the meshes until I come up with an intelligent way to architect this

private:
	
	static SceneManager* m_instance;
	FirstPersonController* m_FPC;
	std::vector<Scene*> m_sceneList;
	std::vector<std::vector<ID3D11ShaderResourceView*>> m_srvList; //we're responsible for releasing these

	std::queue<unsigned int> m_freedIndices; //allows us to recycle indices after the
											//scenes get freed without resizing the vector
											//and ruining indexing
	ID3D11SamplerState* m_samplerState;
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_context;
	SceneManager();

	int skipWSandComments(char* chars, std::ifstream* file); //for file processing
	

};