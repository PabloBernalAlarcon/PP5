#include "stdafx.h"
#include "DirectXVault.h"

using namespace DirectX;


XMFLOAT4X4 Camera;

DirectXVault::DirectXVault()
{
	

}



DirectXVault::~DirectXVault()
{
	if(pipelineState.depthStencilBuffer != nullptr)pipelineState.depthStencilBuffer->Release();
	if(pipelineState.depthStencilState!= nullptr)pipelineState.depthStencilState->Release();
	if(pipelineState.depthStencilView != nullptr) pipelineState.depthStencilView->Release();
	pipelineState.input_layout->Release();
	pipelineState.pixel_shader->Release();
	if(pipelineState.rasterState != nullptr)pipelineState.rasterState->Release();
	pipelineState.vertex_shader->Release();
	pipelineState.indexbuffer->Release();
	attribute.device->Release();
	if (attribute.device_context) attribute.device_context->Release();
	attribute.render_target->Release();
	attribute.swap_chain->Release();

	matBuffer->Release();
	lineBufferx->Release();
	GridBuffer->Release();
	//lineBufferz->Release();
}


void DirectXVault::Start(HWND window, std::vector<float> _Position) {

	wind = window;

	UINT leDeviceFlags = 0;
#ifdef _DEBUG
	leDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	Positions = _Position;
	D3D_FEATURE_LEVEL leFeatureArray[] = {
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
		//D3D_FEATURE_LEVEL_11_1,
	};
	UINT lefeatureLevelsAmount = ARRAYSIZE(leFeatureArray);


	// TODO: PART 1 STEP 3b
	DXGI_SWAP_CHAIN_DESC leSwapchainDesc;
	ZeroMemory(&leSwapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	leSwapchainDesc.BufferCount = 1;
	leSwapchainDesc.BufferDesc.Width = WIDTH;
	leSwapchainDesc.BufferDesc.Height = HEIGHT;
	leSwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	leSwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	leSwapchainDesc.OutputWindow = wind;
	leSwapchainDesc.SampleDesc.Count = 1;
	leSwapchainDesc.Windowed = true;
	leSwapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT Result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		leDeviceFlags, leFeatureArray, lefeatureLevelsAmount, D3D11_SDK_VERSION, &leSwapchainDesc,
		&attribute.swap_chain, &attribute.device, &attribute.feature_levels, &attribute.device_context);

	ID3D11Texture2D * leRenderTargetTemp = 0;
	attribute.swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&leRenderTargetTemp);
	attribute.device->CreateRenderTargetView(leRenderTargetTemp, nullptr, &attribute.render_target);
	leRenderTargetTemp->Release();

	attribute.device_context->OMSetRenderTargets(1,&attribute.render_target,nullptr);

	ZeroMemory(&attribute.viewport, sizeof(D3D11_VIEWPORT));
	attribute.viewport.Width = WIDTH;
	attribute.viewport.Height = HEIGHT;
	attribute.viewport.MaxDepth = 1.0f;
	attribute.viewport.MinDepth = 0.0f;
	attribute.viewport.TopLeftX = 0;
	attribute.viewport.TopLeftY = 0;


	attribute.device_context->RSSetViewports(1, &attribute.viewport);
	GimmeACamera();
	SetUpShadersForACoolTriangle();
	BufferUpTheTriangle();
	BufferUpTheLines();
	BufferUpTheGrid();
	XMStoreFloat4x4(&matrix.translation, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&matrix.view,DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&matrix.projection, DirectX::XMMatrixIdentity());
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//	HRESULT Resultt = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
//		leDeviceFlags, leFeatureArray, lefeatureLevelsAmount, D3D11_SDK_VERSION, &leSwapchainDesc,
//		&attribute.swap_chain, &attribute.device, &attribute.feature_levels, &attribute.device_context);
//
//	ID3D11Texture2D * leRenderTargetTempp = 0;
//	attribute.swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&leRenderTargetTempp);
//	attribute.device->CreateRenderTargetView(leRenderTargetTempp, nullptr, &attribute.render_target);
//	leRenderTargetTempp->Release();
//
//	attribute.device_context->OMSetRenderTargets(1, &attribute.render_target, nullptr);
//
//	ZeroMemory(&attribute.viewport, sizeof(D3D11_VIEWPORT));
//	attribute.viewport.Width = width;
//	attribute.viewport.Height = height;
//	attribute.viewport.MaxDepth = 1.0f;
//	attribute.viewport.MinDepth = 0.0f;
//	attribute.viewport.TopLeftX = 0;
//	attribute.viewport.TopLeftY = 0;
//
//
//	attribute.device_context->RSSetViewports(1, &attribute.viewport);
//	SetUpShadersForCoolLines();
//	BufferUpTheLines();
}

void DirectXVault::Render() {

	UpdateCamera();
	//XMStoreFloat4x4(&matrix.translation, XMMatrixTranspose(DirectX::XMMatrixIdentity()));
	if (GetAsyncKeyState('Q') & 0x1)
	{
		RandomizeStuff();
	}
	leMatrix copy = matrix;


	XMStoreFloat4x4(&copy.view , XMMatrixInverse(nullptr, XMLoadFloat4x4(&copy.view)));


	transPose4X4(copy.view);
	attribute.device_context->UpdateSubresource(matBuffer, 0, NULL, &copy, 0, 0);
	attribute.device_context->VSSetConstantBuffers(0, 1, &matBuffer);

	attribute.device_context->ClearRenderTargetView(attribute.render_target, DirectX::Colors::SeaGreen);
	attribute.device_context->IASetInputLayout(pipelineState.input_layout);

	DrawTheCoolestTriangle();
	DrawTheCoolestGrid();
	//DrawTheCoolestLines();
	attribute.swap_chain->Present(0, 0);
}

void DirectXVault::SetUpShadersForACoolTriangle() {
	
	attribute.device->CreatePixelShader(trivial_PS, sizeof(trivial_PS), NULL, &pipelineState.pixel_shader);
	attribute.device->CreateVertexShader(trivial_VS, sizeof(trivial_VS), NULL, &pipelineState.vertex_shader);
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	attribute.device->CreateInputLayout(ied,2,trivial_VS,sizeof(trivial_VS),&pipelineState.input_layout);



}

void DirectXVault::BufferUpTheTriangle() {
	vertex OurVertices[] =
	{
		{DirectX::XMFLOAT4(0.0f, 0.5f, 0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{ DirectX::XMFLOAT4(0.45f, -0.5, 0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.45f, -0.5f, 0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
	};


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(vertex) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	attribute.device->CreateBuffer(&bd, NULL, &pipelineState.indexbuffer);       // create the buffer


												   // copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	attribute.device_context->Map(pipelineState.indexbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	attribute.device_context->Unmap(pipelineState.indexbuffer, NULL);                                      // unmap the buffer
}

void DirectXVault::BufferUpTheGrid() {
	vertex OurVertices[] =
	{
		 { DirectX::XMFLOAT4(-1.0f, 0.0f,-1.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(-1.0f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-0.8f, 0.0f,-1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(-0.8f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-0.6f, 0.0f,-1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		 { DirectX::XMFLOAT4(-0.6f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-0.4f, 0.0f,-1.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(-0.4f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-0.2f, 0.0f,-1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(-0.2f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		 { DirectX::XMFLOAT4( 0.0f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		 { DirectX::XMFLOAT4(0.2f, 0.0f, -1.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4( 0.2f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(0.4f, 0.0f, -1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4( 0.4f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(0.6f, 0.0f, -1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		 { DirectX::XMFLOAT4( 0.6f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		 { DirectX::XMFLOAT4(0.8f, 0.0f, -1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4( 0.8f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(1.0f, 0.0f, -1.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4( 1.0f, 0.0f, 1.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		 ///////////////////////////////////////////////////////////////////////////////////////////

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, -1.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, -1.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, -0.8f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, -0.8f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, -0.6f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, -0.6f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, -0.4f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, -0.4f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, -0.2f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, -0.2f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f ,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f ,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, 0.2f ,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, 0.2f ,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, 0.4f ,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, 0.4f ,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, 0.6f ,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, 0.6f ,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, 0.8f ,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, 0.8f ,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-1.0f, 0.0f, 1.0f ,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f ,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

	};


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(vertex) * 44;    // Positions.size() /3 size is the VERTEX struct * 2 (6)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	D3D11_SUBRESOURCE_DATA SUBRESx;
	SUBRESx.pSysMem = OurVertices;
	SUBRESx.SysMemPitch = 0;
	SUBRESx.SysMemSlicePitch = 0;

	attribute.device->CreateBuffer(&bd, &SUBRESx, &GridBuffer);       // create the buffer


	CD3D11_BUFFER_DESC bdm(sizeof(leMatrix), D3D11_BIND_CONSTANT_BUFFER);
	attribute.device->CreateBuffer(&bdm, NULL, &matBuffer);

}
void DirectXVault::BufferUpTheLines() {
	int size = Positions.size() / 3;

	vertex OurVerticesx[] = {
		{ DirectX::XMFLOAT4(0.5f,0.5f,0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.5f,0.5f,0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.5f,-0.5f,0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.5f,-0.5f,0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.5f,0.5f,0.5f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

	};
	/*vertex OurVerticesx[] = {
		{ DirectX::XMFLOAT4(Positions[0],Positions[1],Positions[2],1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(Positions[3],Positions[4],Positions[5],1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(Positions[6],Positions[7],Positions[8],1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(Positions[9],Positions[10],Positions[11],1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(Positions[12],Positions[13],Positions[14],1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

	};*/


	//vertex OurVerticesx[] =
	//{
	//	{ DirectX::XMFLOAT4(0.0,0.0,0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	//	{ DirectX::XMFLOAT4(0.5,0.0,0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	//	{ DirectX::XMFLOAT4(0.0,0.0,0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	//	{ DirectX::XMFLOAT4(0.0,0.5,0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

	//	{ DirectX::XMFLOAT4(0.0,0.0,0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	//	{ DirectX::XMFLOAT4(0.0,0.0,0.5f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
	//};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(vertex) * 5;    // Positions.size() /3 size is the VERTEX struct * 2 (6)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	D3D11_SUBRESOURCE_DATA SUBRESx;
	SUBRESx.pSysMem = OurVerticesx;
	SUBRESx.SysMemPitch = 0;
	SUBRESx.SysMemSlicePitch = 0;

	attribute.device->CreateBuffer(&bd, &SUBRESx, &lineBufferx);       // create the buffer


	CD3D11_BUFFER_DESC bdm(sizeof(leMatrix),D3D11_BIND_CONSTANT_BUFFER);
	attribute.device->CreateBuffer(&bdm,NULL,&matBuffer);

	
}
void DirectXVault::DrawTheCoolestTriangle() {



	attribute.device_context->PSSetShader(pipelineState.pixel_shader, NULL, 0);
	attribute.device_context->VSSetShader(pipelineState.vertex_shader, NULL, 0);
	// select which vertex buffer to display
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	attribute.device_context->IASetVertexBuffers(0, 1, &pipelineState.indexbuffer, &stride, &offset);

	// select which primtive type we are using
	attribute.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	attribute.device_context->Draw(3, 0);

}

void DirectXVault::DrawTheCoolestLines() {
	
	attribute.device_context->PSSetShader(pipelineState.pixel_shader, NULL, 0);
	attribute.device_context->VSSetShader(pipelineState.vertex_shader, NULL, 0);
	//// select which vertex buffer to display
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	//// select which primtive type we are using
	attribute.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	//// draw the vertex buffer to the back buffer
	attribute.device_context->IASetVertexBuffers(0, 1, &lineBufferx, &stride, &offset);
	attribute.device_context->Draw(5, 0);


}

void DirectXVault::DrawTheCoolestGrid() {
	attribute.device_context->PSSetShader(pipelineState.pixel_shader, NULL, 0);
	attribute.device_context->VSSetShader(pipelineState.vertex_shader, NULL, 0);
	//// select which vertex buffer to display
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	//// select which primtive type we are using
	attribute.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//// draw the vertex buffer to the back buffer
	attribute.device_context->IASetVertexBuffers(0, 1, &GridBuffer, &stride, &offset);
	attribute.device_context->Draw(44, 0);
}



void DirectXVault::GimmeACamera() {

	DirectX::XMVECTORF32 eye = { 0.5f, 0.7f, -1.5f, 0.0f };
	DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&matrix.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));

	float aspectRatio = WIDTH / HEIGHT;
	float fovAngleY = 70.0f * 3.14f / 180.0f;

	DirectX::XMMATRIX projj = DirectX::XMMatrixPerspectiveFovLH(fovAngleY,aspectRatio,0.01f,100.0f);
	XMStoreFloat4x4(&matrix.projection, XMMatrixTranspose(projj));
	
	XMStoreFloat4x4(&matrix.translation, XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up)));
	
}

void DirectXVault::RandomizeStuff() {

	DirectX::XMMATRIX model = DirectX::XMLoadFloat4x4(&matrix.translation);

	float rX = (float)rand() / RAND_MAX * 2.0f - 1.0f;
	float rY = (float)rand() / RAND_MAX * 2.0f - 1.0f;
	float rZ = (float)rand() / RAND_MAX * 2.0f - 1.0f;

	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(rX, rY, rZ);

	DirectX::XMMATRIX rotatex = DirectX::XMMatrixRotationX((rand() % 360)*DirectX::XM_PI  /180.0f);
	DirectX::XMMATRIX rotatey = DirectX::XMMatrixRotationY((rand() % 360)*DirectX::XM_PI / 180.0f);
	DirectX::XMMATRIX rotatez = DirectX::XMMatrixRotationZ((rand() % 360)*DirectX::XM_PI / 180.0f);

	model = rotatex;
	model *= rotatey;
	model *= rotatez;
	model *= trans;
	DirectX::XMStoreFloat4x4(&matrix.translation, DirectX::XMMatrixTranspose(model));
}


void DirectXVault::SetMousePos() {
	/*OldMousePos = NewMousePos;
	GetCursorPos(&NewMousePos);
	ScreenToClient(wind, &NewMousePos);*/
}
void DirectXVault::UpdateCamera() {

	if ( GetAsyncKeyState('W'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.005f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);

	}
	if (GetAsyncKeyState('S'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -0.005f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}
	if (GetAsyncKeyState('A'))
	{
		XMMATRIX translation = XMMatrixTranslation(-0.005f, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}
	if (GetAsyncKeyState('D'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.005f, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}
	if (GetAsyncKeyState('X'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, -0.005f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}



	if (GetAsyncKeyState('B'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.005f, 0.0f * 10);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}

	
		
		OldMousePos = NewMousePos;



	/*if (GetAsyncKeyState( 'W'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.0005f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);

	}
	if (GetAsyncKeyState('S'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -0.0005f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}
	if (GetAsyncKeyState('A'))
	{
		XMMATRIX translation = XMMatrixTranslation(-0.0005f, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}
	if (GetAsyncKeyState('D'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0005f, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}
	if (GetAsyncKeyState('X'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, -0.0005f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}

	if (GetAsyncKeyState(VK_UP))
	{
		XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0005f, 0.0f);
		XMMATRIX Temp = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX Result = XMMatrixMultiply(Translation, Temp);
		XMStoreFloat4x4(&matrix.translation, Result);
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		XMMATRIX Translation = XMMatrixTranslation(0.0f, -0.0005f, 0.0f);
		XMMATRIX Temp = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX Result = XMMatrixMultiply(Translation, Temp);
		XMStoreFloat4x4(&matrix.translation, Result);
	}*/

	/*if (RMouseClick)
	{
		float dx = NewMousePos.x - OldMousePos.x;
		float dy = NewMousePos.y - OldMousePos.y;

		XMFLOAT4 Pos = XMFLOAT4(Camera._41, Camera._42, Camera._43, Camera._44);

		Camera._41 = 0.0f;
		Camera._42 = 0.0f;
		Camera._43 = 0.0f;

		XMMATRIX RotationX = XMMatrixRotationX(dy * 0.01f);
		XMMATRIX RotationY = XMMatrixRotationY(dx * 0.01f);

		XMMATRIX Temp = XMLoadFloat4x4(&Camera);
		Temp = XMMatrixMultiply(RotationX, Temp);
		Temp = XMMatrixMultiply(Temp, RotationY);

		XMStoreFloat4x4(&Camera, Temp);

		Camera._41 = Pos.x;
		Camera._42 = Pos.y;
		Camera._43 = Pos.z;

		SetMousePos();
	}*/

}


void DirectXVault::transPose4X4(XMFLOAT4X4 & conv) {


	XMFLOAT4X4 temp = conv;

	conv._11 = temp._11; conv._12 = temp._21; conv._13 = temp._31; conv._14 = temp._41;
	conv._21 = temp._12; conv._22 = temp._22; conv._23 = temp._32; conv._24 = temp._42;
	conv._31 = temp._13; conv._32 = temp._23; conv._33 = temp._33; conv._34 = temp._43;
	conv._41 = temp._14; conv._42 = temp._24; conv._43 = temp._34; conv._44 = temp._44;


}
