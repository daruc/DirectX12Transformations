#pragma once
#include <d3d12.h>
#include "d3dx12.h"
#include <wrl.h>
#include <dxgi1_4.h>	// ?
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <chrono>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;

	Vertex(float x, float y, float z, float r, float g, float b, float a)
		: pos(x, y, z), color(r, g, b, a)
	{
	}
};

struct ColorMultiplier
{
	XMFLOAT4 colorMultiplier;
};

struct Wvp
{
	XMFLOAT4X4 wvp;
};

class Engine
{
private:

	UINT m_resolutionWidth;
	UINT m_resolutionHeight;
	HWND m_hwnd;

	float m_mouseX;
	float m_mouseY;
	float m_mouseDeltaX;	// when right mouse button was pressed
	float m_mouseDeltaY;

	high_resolution_clock::time_point m_prevTime;

	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12Resource> m_renderTarget[2];
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Fence> m_fence;
	ComPtr<ID3D12Device> m_device;

	// drawing triangles
	ComPtr<ID3D12RootSignature> m_rootSignature;

	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_vBufferUploadHeap;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	ComPtr<ID3D12Resource> m_indexBuffer;
	ComPtr<ID3D12Resource> m_iBufferUploadHeap;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

	ComPtr<ID3DBlob> m_vertexShader;
	ComPtr<ID3DBlob> m_pixelShader;
	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissorRect;

	// depth/stencil buffer
	ComPtr<ID3D12DescriptorHeap> m_dsDescriptorHeap;
	ComPtr<ID3D12Resource> m_dsBuffer;

	// constant buffers
	ComPtr<ID3D12DescriptorHeap> m_cbDescriptorHeap[2];
	ComPtr<ID3D12Resource> m_cbColorMultiplierUploadHeap[2];
	ColorMultiplier m_cbColorMultiplierData;
	UINT8* m_cbColorMultiplierGpuAddress[2];

	ComPtr<ID3D12Resource> m_cbWvpUploadHeap[2];
	Wvp m_wvpData;
	UINT8* m_cbWvpGpuAddress[2];

	XMFLOAT4X4 m_worldMat;
	XMFLOAT4X4 m_viewMat;
	XMFLOAT4X4 m_projectionMat;

	XMFLOAT4 m_scale;
	XMFLOAT4 m_position;
	XMFLOAT4 m_rotation;

	XMFLOAT4 m_cameraPosition;
	XMFLOAT4 m_cameraRotation;

	int m_frameIndex;	// render target index
	UINT m_rtvDescriptorSize;	// Render Target View descriptor heap size
	UINT64 m_fenceValue;
	HANDLE m_fenceEvent;

	void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);
	void WaitForPreviousFrame();

	void CreateRootSignature();
	void LoadShaders();
	void CreatePipelineStateObject();
	void CreateVertexBuffer();
	void FillOutViewportAndScissorRect();
	void InitWvp();
	void UpdateWvp(float deltaSec);
	void CreateConstantBuffers();

public:
	Engine(UINT resolutionWidth, UINT resolutionHeight);
	~Engine();

	void Init(HWND hwnd);
	void Input(float mouseX, float mouseY, bool rightMouseBtnPressed);
	void InputRightBtnPressed(float mouseX, float mouseY);
	void InputRightBtnReleased();
	void Update();
	void Render();
	void Destroy();
};