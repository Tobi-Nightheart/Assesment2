#include "pch.h"
#include "SceneManager.h"
#include <minwinbase.h>
#include <minwinbase.h>


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	
	if (sc_pPlane)
	{
		delete sc_pPlane;
		sc_pPlane = nullptr;
	}
	if (sc_pSampler0) sc_pSampler0->Release();
	if (sc_pCamera)
	{
		delete sc_pCamera;
		sc_pCamera = nullptr;
	}
	if(sc_pCameraBirdsEye)
	{
		delete sc_pCameraBirdsEye;
		sc_pCameraBirdsEye = nullptr;
	}
	delete[] sc_pModel;		
	sc_pInput = nullptr;
	if (sc_pSkybox)
	{
		delete sc_pSkybox;
		sc_pSkybox = nullptr;
	}
	if (sc_pText2D)
	{
		delete sc_pText2D;
		sc_pText2D = nullptr;
	}
	if(sc_pBlendAlphaEnable)
	{
		sc_pBlendAlphaEnable->Release();
	}
	if (sc_pBlendAlphaDisable)
	{
		sc_pBlendAlphaEnable->Release();
	}
	if (sc_pRainController)
	{
		delete sc_pRainController;
		sc_pRainController = nullptr;
	}

	if (sc_pRainTex) sc_pRainTex->Release();
	if(sc_pReflect)
	{
		delete sc_pReflect;
		sc_pReflect = nullptr;
	}
	if(sc_pRoot_node)
	{
		delete sc_pRoot_node;
		sc_pRoot_node = nullptr;
	}

	delete[] sc_pNode_enemy;
	delete[] sc_pNode_obj;
	
	if(sc_pReflective)
	{
		delete sc_pReflective;
		sc_pReflective = nullptr;
	}
	if (sc_pTexture1) sc_pTexture1->Release();
	if (sc_pTexture0) sc_pTexture0->Release();
	if (sc_pDevice) sc_pDevice = nullptr;
	if (sc_pContext) sc_pContext = nullptr;
}

HRESULT SceneManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, Input* input)
{
	HRESULT hr = S_OK;
	sc_pDevice = device;
	sc_pContext = context;
	sc_pInput = input;

	sc_pText2D = new Text2D("assets/font1Alpha.png", sc_pDevice, sc_pContext);
	//initialize scene nodes
	sc_pRoot_node = new scene_node();
	for(int i = 0; i<10; i++)
	{
		sc_pNode_obj[i] = new scene_node();
		sc_pNode_enemy[i] = new scene_node();

	}
	//initialize textures
	string a = "assets/texture1.jpg";
	string b = "assets/texture2.jpg";
	string d = "assets/texture3.jpg";
	hr = D3DX11CreateShaderResourceViewFromFile(sc_pDevice, a.c_str(), NULL, NULL, &sc_pTexture0, NULL);
	if (FAILED(hr)) return hr;
	hr = D3DX11CreateShaderResourceViewFromFile(sc_pDevice, b.c_str(), NULL, NULL, &sc_pTexture1, NULL);
	if (FAILED(hr)) return hr;
	hr = D3DX11CreateShaderResourceViewFromFile(sc_pDevice, d.c_str(), NULL, NULL, &sc_pTexturePlane, NULL);
	if (FAILED(hr)) return hr;
	string c = "assets/raindrop.dds";
	hr = D3DX11CreateShaderResourceViewFromFile(sc_pDevice, c.c_str(), nullptr, nullptr, &sc_pRainTex, nullptr);
	if (FAILED(hr)) return hr;
	
	//initialize skybox
	sc_pSkybox = new Skybox(sc_pDevice, sc_pContext);
	sc_pSkybox->Initialize();
	//init plane
	sc_pPlane=new Model(sc_pDevice, sc_pContext);
	sc_pPlane->LoadObjModel((char*)"Resources/plane.obj");
	sc_pPlane->SetTexture(sc_pTexturePlane);

	//sc_pModel 1-4 are objects and 5-10 are enemies
	for(int i = 0; i<5; i++)
	{
		sc_pModel[i] = new Model(sc_pDevice, sc_pContext);
		sc_pModel[i]->LoadObjModel((char*)"Resources/cube.obj");
		sc_pModel[i]->SetTexture(sc_pTexture0);
		sc_pNode_obj[i]->SetModel(sc_pModel[i]);
	}
	
	for(int i = 5; i<10; i++)
	{
		sc_pModel[i] = new Model(sc_pDevice, sc_pContext);
		sc_pModel[i]->LoadObjModel((char*)"Resources/cat.obj");
		sc_pModel[i]->SetTexture(sc_pTexture1);
		sc_pNode_enemy[i-5]->SetModel(sc_pModel[i]);
	}
	//placing the enemies
	sc_pNode_enemy[0]->SetX(  1.0f, sc_pRoot_node);
	sc_pNode_enemy[0]->SetZ(  5.0f, sc_pRoot_node);
	sc_pNode_enemy[1]->SetX( 12.0f, sc_pRoot_node);
	sc_pNode_enemy[1]->SetZ( -6.0f, sc_pRoot_node);
	sc_pNode_enemy[2]->SetX( -6.0, sc_pRoot_node);
	sc_pNode_enemy[2]->SetZ(  0.0f, sc_pRoot_node);
	sc_pNode_enemy[3]->SetX(  9.0f, sc_pRoot_node);
	sc_pNode_enemy[3]->SetZ( -2.0f, sc_pRoot_node);
	sc_pNode_enemy[4]->SetX( -6.0f, sc_pRoot_node);
	sc_pNode_enemy[4]->SetZ( -7.0f, sc_pRoot_node);
	//placing the objects
	sc_pNode_obj[0]->SetX(1.0f, sc_pRoot_node);
	sc_pNode_obj[0]->SetZ(3.0f, sc_pRoot_node);
	sc_pNode_obj[1]->SetX(10.0f, sc_pRoot_node);
	sc_pNode_obj[1]->SetZ(-6.0f, sc_pRoot_node);
	sc_pNode_obj[2]->SetX(-4.0, sc_pRoot_node);
	sc_pNode_obj[2]->SetZ(0.0f, sc_pRoot_node);
	sc_pNode_obj[3]->SetX(7.0f, sc_pRoot_node);
	sc_pNode_obj[3]->SetZ(-2.0f, sc_pRoot_node);
	sc_pNode_obj[4]->SetX(-4.0f, sc_pRoot_node);
	sc_pNode_obj[4]->SetZ(-7.0f, sc_pRoot_node);
	//add them too child nodes
	for(int i = 0; i<5; i++)
	{
		if (i == 0)
		{
			sc_pRoot_node->addChildNode(sc_pNode_obj[0]);
			sc_pRoot_node->addChildNode(sc_pNode_enemy[0]);
		}
		else 
		{
			sc_pNode_obj[i-1]->addChildNode(sc_pNode_obj[i]);
			sc_pRoot_node->addChildNode(sc_pNode_enemy[i]);

		}
	}
	
	
	//init blend state
	D3D11_BLEND_DESC blend_desc;
	ZeroMemory(&blend_desc, sizeof(blend_desc));
	blend_desc.AlphaToCoverageEnable = false;
	blend_desc.IndependentBlendEnable = false;
	blend_desc.RenderTarget[0].BlendEnable = true;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = sc_pDevice->CreateBlendState(&blend_desc, &sc_pBlendAlphaEnable);
	if (FAILED(hr)) return hr;

	blend_desc.RenderTarget[0].BlendEnable = false;
	hr = sc_pDevice->CreateBlendState(&blend_desc, &sc_pBlendAlphaDisable);
	if (FAILED(hr)) return hr;
	
	sc_pReflect = new Reflective(sc_pDevice, sc_pContext);
	sc_pReflect->LoadObjModel((char*)"Resources/sphere.obj");




	sc_pCamera = new camera(0.0f, 1.0f, -5.0f, 0.0f, 0, sc_pRoot_node, true);
	sc_pCameraBirdsEye = new camera(0.0f, 20.0f, 0.0f, 0.0f, -90.0f, sc_pRoot_node, false);
	

	sc_pRainController = new RainController(sc_pDevice, sc_pContext, sc_pRainTex, sc_pCamera);
	sc_pRainController->Initialize();

	sc_directional_light_shines_from = XMVectorSet(2.0f, 3.0f, 1.0f, 0.0f);
	sc_directional_light_colour = { .5f,.5f, .5f, 0.0f };
	
	sc_ambient_light_colour = { .1f,.1f, .1f, 0.0f };
	
	sc_point_light_postition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	sc_point_light_colour = { 1.0f, 1.0f, 1.0f, 0.0f };
	
	sc_SpotDir = XMVectorSet(0.0f, -4.0f, -1.0f, 0.0f);
	sc_SpotPos = XMVectorSet(-3.0f, 10.0f, 0.0f, 0.0f);
	sc_SpotC = { 1.0f, 1.0f, 1.0f, 0.0f };

	return S_OK;
}

void SceneManager::Render(GameTimer* gameTimer, string fps)
{
	sc_pInput->ReadInputStates();
	//put into its own input player input class
	sc_pInput->KeyboardInput(sc_pCamera, sc_pCameraBirdsEye, gameTimer->DeltaTime());
	camera* drawcam;
	if (sc_pCamera->GetActive()) drawcam = sc_pCamera;
	else
	{
		drawcam = sc_pCameraBirdsEye;
	}
	
	XMMATRIX world, projection, view;
	world = XMMatrixIdentity();
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(65.0f), 640.0f / 480.0f, 1.0f, 1000.0f);
	
	view = drawcam->GetViewMatrix();

	sc_pSkybox->Draw(&view, &projection, drawcam);
	sc_pPlane->Draw(&world, &view, &projection, sc_ambient_light_colour, sc_directional_light_shines_from, sc_directional_light_colour, sc_point_light_postition, sc_point_light_colour, sc_SpotPos, sc_SpotC, sc_SpotDir);
	sc_pRoot_node->execute(&world, &view, &projection, sc_ambient_light_colour, sc_directional_light_shines_from, sc_directional_light_colour, sc_point_light_postition, sc_point_light_colour, sc_SpotPos, sc_SpotC, sc_SpotDir);

	sc_pReflect->SetZ(5.0f);
	sc_pReflect->SetX(5.0f);
	sc_pReflect->Draw(&view, &projection, sc_ambient_light_colour, sc_directional_light_shines_from, sc_directional_light_colour, sc_point_light_postition, sc_point_light_colour, sc_SpotPos, sc_SpotC, sc_SpotDir);

	sc_pRainController->Draw(&view, &projection, drawcam, gameTimer);
	for(int i = 0; i<5; i++)
	{
		sc_pNode_enemy[i]->LookAtAZ(sc_pCamera->GetX(), sc_pCamera->GetZ());
		sc_pNode_enemy[i]->MoveForward(0.5f, sc_pRoot_node, gameTimer->DeltaTime());
	}
	

	
	sc_pContext->OMSetBlendState(sc_pBlendAlphaEnable, 0, 0xffffffff);
	sc_pText2D->AddText(fps, -1.0, 1.0, .05);
	sc_pText2D->RenderText();
	sc_pContext->OMSetBlendState(sc_pBlendAlphaDisable, 0, 0xffffffff);

}
