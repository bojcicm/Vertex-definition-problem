#include "pch.h"
#include "Renderer3D.h"

//#include "..\..\..\Engine\Models\Basic Shapes\Pyramids.h"
//#include "..\..\..\Engine\Models\Basic Shapes\Cubes.h"
//#include "..\..\..\Engine\Models\Basic Shapes\Triangles.h"
//#include "..\..\..\Engine\Models\Explicit Surface.h"
#include "..\..\Engine\Models\Basic Shapes\Cubes.h"

using namespace std;
using namespace vxe;
using namespace concurrency;
using namespace DirectX;
using namespace Windows::Foundation;

void Renderer3D::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	vector<task<void>> tasks;
	
	_vertexshader = make_shared<VertexShader<DirectX::VertexPositionColorExtended>>();
	tasks.push_back(_vertexshader->CreateAsync(device, L"VertexShader1.cso"));
	_pixelshader = make_shared<PixelShader>();
	tasks.push_back(_pixelshader->CreateAsync(device, L"PixelShader1.cso"));
	
	_model = make_shared<vxe::Cube<VertexPositionColorExtended, unsigned short>>();
	tasks.push_back(_model->CreateAsync(device));

	_world = make_shared<WorldTransforms>(device);
	_world->Translate(0.0f, 3.0f, 10.0f);

	when_all(tasks.begin(), tasks.end()).then([this]()
	{
		m_loadingComplete = true;
		DebugPrint(string("\t -- A lambda: Loading is complete! \n"));
	});
}

void Renderer3D::CreateWindowSizeDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();
	Size outputSize = m_deviceResources->GetOutputSize();

	_view = make_shared<ViewTransform>(device);
	static const XMVECTORF32 eye = { 0.0f, 50.0f, 170.0f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	_view->SetView(eye, at, up);
	_view->Update(context);

	float r = outputSize.Width / outputSize.Height;
	float fov = 70.0f * XM_PI / 180.0f;
	float n = 0.01f;
	float f = 1000.0f;
	if (r < 1.0f) { fov *= 2.0f; }
	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);
	_projection = make_shared<ProjectionTransform>(device, Handedness::LeftHanded);
	_projection->SetProjection(orientationMatrix, fov, r, n, f);
	_projection->Update(context);

}

void Renderer3D::Update(DX::StepTimer const& timer)
{
	auto vertices = _model->GetVertices();
	for(auto i = 0; i < _model->GetVertexCount(); i++)
	{
		DebugPrint("Renderer3D::Update - vertices value X: " + std::to_string(vertices[i].valueX) + "\n");
		DebugPrint("Renderer3D::Update - vertices value Y: " + std::to_string(vertices[i].valueY) + "\n");
		assert(vertices[i].valueX == 1337);
		assert(vertices[i].valueY == 1338);
	}
}

void Renderer3D::Render()
{
	if (!m_loadingComplete) {
		return;
	}

	DebugPrint(std::string("Renderer3D::Render() called\n"));
	auto context = m_deviceResources->GetD3DDeviceContext();

	SetCamera();

	_vertexshader->Bind(context);
	_pixelshader->Bind(context);
	Draw(_model, _world);

	m_deviceResources->SetRasterizerState();
}

void Renderer3D::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	DebugPrint(string("\tRenderer3D::ReleaseDeviceDependentResources()...\n"));

	_vertexshader->Reset();
	_pixelshader->Reset();
	_world->Reset();

	_model->Reset();

}