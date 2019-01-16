#pragma once
#include "objfilemodel.h"

class Model
{
private:
	struct MODEL_CONSTANT_BUFFER {
		XMMATRIX WorldViewProjection; //64 bytes
		XMMATRIX world;			//64


	};

	struct Light_CB
	{
		XMVECTOR EyePos;
		XMVECTOR DirToLight;
		XMFLOAT4 DirLightColor;
		XMVECTOR PointLightPos;
		XMFLOAT4 PointLightC;
		XMFLOAT4 ambientdown;
		XMFLOAT4 ambientrange;
		XMVECTOR SpotLightPos;
		XMFLOAT4 SpotLightC;
		XMVECTOR SpotLightDir;
		float	 SpotOuterCone;
		float	 SpotInnerCone;
		float	 SpotRange;
		float	 PointLightRange;

	};

	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext*  m_pImmediateContext;

	ObjFileModel* m_pObject;
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader*  m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	
	ID3D11ShaderResourceView* m_pTexture0;
	ID3D11SamplerState* m_pSampler0;
	ID3D11RasterizerState* m_pRaster;
	//Constantbuffers
	ID3D11Buffer* m_pCBVertIn;
	ID3D11Buffer* m_pCBPixelIn;
	//Collision 
	float m_bounding_sphere_center_x;
	float m_bounding_sphere_center_y;
	float m_bounding_sphere_center_z;
	float m_bounding_sphere_radius;


	float m_px, m_py, m_pz;
	float m_xAngle, m_yAngle, m_zAngle;
	float m_Scale;



	void CalculateModelCenterPoint();
	void CalculateBoundingSphereRadius();

public:
	Model(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DeviceContext);
	~Model();
	HRESULT LoadObjModel(char* filename);
	void Draw(XMMATRIX* world, XMMATRIX * view, XMMATRIX * projection, XMFLOAT4 AmbC, XMVECTOR DirV, XMFLOAT4 DirC, XMVECTOR PLightV, XMFLOAT4 PLightC, XMVECTOR SpotV, XMFLOAT4 SpotC, XMVECTOR SpotD);
	XMVECTOR GetBoundingSphereWorldSpacePosition();
	bool CheckCollision(Model* model);
	void SetTexture(ID3D11ShaderResourceView* tex);
	void SetSampler(ID3D11SamplerState* sampler);
	ObjFileModel* GetObject();
	void LookAtAZ(float x, float z);
	void MoveForward(float d);

	void IncX(float num);
	void IncY(float num);
	void IncZ(float num);
	void IncXAngle(float num);
	void IncYAngle(float num);
	void IncZAngle(float num);
	void IncScale(float num);
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetXAngle(float xAngle);
	void SetYAngle(float yAngle);
	void SetZAngle(float zAngle);
	void SetScale(float scale);
	float GetX();
	float GetY();
	float GetZ();
	float GetXAngle();
	float GetYAngle();
	float GetZAngle();
	float GetScale();
	float GetBoundingSphereRadius();
};

