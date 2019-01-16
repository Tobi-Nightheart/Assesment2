#pragma once
#include "scene_node.h"
#include "camera.h"
#include "Model.h"
#include "Skybox.h"
#include "Reflective.h"
#include "Text2D.h"
#include "RainController.h"
#include "Input.h"

class SceneManager
{
private:
	ID3D11Device* sc_pDevice;
	ID3D11DeviceContext* sc_pContext;
	//plane
	Model* sc_pPlane;


	//Nodes
	scene_node* sc_pRoot_node;
	scene_node* sc_pNode_enemy[5];
	scene_node* sc_pNode_obj[5];
	scene_node* sc_pReflective;

	//Lighting
	XMVECTOR sc_directional_light_shines_from;
	XMFLOAT4 sc_directional_light_colour;
	XMFLOAT4 sc_ambient_light_colour;
	//Point light
	XMVECTOR sc_point_light_postition;
	XMFLOAT4 sc_point_light_colour;
	//spot light
	XMVECTOR sc_SpotPos;
	XMVECTOR sc_SpotDir;
	XMFLOAT4 sc_SpotC;

	//Model
	Model* sc_pModel[10];

	//Skybox
	Skybox* sc_pSkybox;

	//reflective
	Reflective* sc_pReflect;

	//rain
	RainController* sc_pRainController;
	ID3D11ShaderResourceView* sc_pRainTex;

	//2d text
	Text2D* sc_pText2D;
	ID3D11BlendState* sc_pBlendAlphaEnable;
	ID3D11BlendState* sc_pBlendAlphaDisable;

	//Texture pointer
	ID3D11ShaderResourceView* sc_pTexture0; 
	ID3D11ShaderResourceView* sc_pTexture1;
	ID3D11ShaderResourceView* sc_pTexturePlane;
	ID3D11SamplerState* sc_pSampler0; //sampler state for texture

	//Input
	Input* sc_pInput;
	
	//camera
	camera* sc_pCamera;
	camera* sc_pCameraBirdsEye;
	

public:
	SceneManager();
	~SceneManager();
	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context, Input* input); 
	void Render(GameTimer* gameTimer, string fps);
};

