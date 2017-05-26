#pragma once
//#include <D3DX11.h>
#include <d3d11.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <vector>
class DirectXVault
{
public:

	DirectXVault();
	~DirectXVault();
	
ID3D11Debug * whatyouplease = nullptr;
	

struct vert {
	float Position[4];
	float Color[4];
	float Normals[4];
};
	struct pipeline_state_t
	{
		ID3D11InputLayout *input_layout = nullptr;
		ID3D11VertexShader *vertex_shader = nullptr;
		ID3D11PixelShader * pixel_shader = nullptr;
		ID3D11Texture2D *depthStencilBuffer = nullptr;
		ID3D11DepthStencilState *depthStencilState = nullptr;
		ID3D11DepthStencilView *depthStencilView = nullptr;
		ID3D11RasterizerState *rasterState = nullptr;
		ID3D11RasterizerState * debugRasterState = nullptr;
		ID3D11Buffer *indexbuffer = nullptr;
		ID3D10Buffer *constantBuffer = nullptr;
	}default_pipeline;

	struct Attributes
	{
		ID3D11Device * device = nullptr;
		ID3D11DeviceContext * device_context = nullptr;
		IDXGISwapChain * swap_chain = nullptr;
		ID3D11RenderTargetView *render_target = nullptr;
		D3D_DRIVER_TYPE driver_type;
		D3D_FEATURE_LEVEL feature_levels;
		D3D11_VIEWPORT viewport;
	};

	struct vertex {
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT4 Normals;
	};


	struct leMatrix {

		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 translation;

	};


	ID3D11RasterizerState * mainRasterizer;
	ID3D11RasterizerState * debugRasterizer;
	std::vector<float> Positions;
	std::vector<float> Bones;
	std::vector<uint32_t> _indices;
	int sizetodraw;
	int sizetodrawBones;
	pipeline_state_t pipelineState;
	pipeline_state_t debugpipelineState;
	Attributes attribute;
	Attributes debugattribute;
	bool RMouseClick = false;
	bool toggle = false;
	/*Attributes attributeline;
	pipeline_state_t linesState;*/

	leMatrix matrix;
	ID3D11Buffer* matBuffer = nullptr;
	
	DirectX::XMFLOAT4X4 cam;

	POINT currPoint;
	POINT prevPoint;

	ID3D11Buffer* lineBufferx = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;
	ID3D11Buffer* lineBuffery = nullptr;
	ID3D11Buffer* GridBuffer = nullptr;
	//ID3D11Buffer* lineBufferz = nullptr;

	POINT MousePos;
	//POINT NewMousePos;

	HWND wind;

	void Start(HWND window, std::vector<float>& _Position, std::vector<uint32_t> &indices);
	void Render();
	void SetUpShadersForACoolTriangle();
	void BufferUpTheTriangle();
	void BufferUpTheLines();
	void bufferdemBones(std::vector<float>& _Bones);
	void BufferUpTheGrid();
	void DrawTheCoolestTriangle();
	void DrawTheCoolestLines();
	void DrawTheCoolestGrid();
	void GimmeACamera();
	void RandomizeStuff();
	void SetMousePos();
	void UpdateCamera();
	void transPose4X4(DirectX::XMFLOAT4X4 & conv);
	void KeyPressed(bool didtheyPress);
	POINT getCurrMousePos();
	void setCurrMouse(POINT p);
};

