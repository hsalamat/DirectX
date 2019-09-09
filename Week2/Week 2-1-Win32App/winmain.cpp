#include "DXApp.h"

class TestApp :public DXApp
{
public:
	TestApp(HINSTANCE hInstance);
	~TestApp();

	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;


};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//MessageBox(NULL,L"Hello World", L"Test",NULL);

	TestApp tApp(hInstance);
	if (!tApp.Init()) return 1;

	return tApp.Run();

}

TestApp::TestApp(HINSTANCE hInstance): DXApp(hInstance)
{
}

TestApp::~TestApp()
{
}

bool TestApp::Init()
{
	return DXApp::Init();
}

void TestApp::Update(float dt)
{
}

void TestApp::Render(float dt)
{
}
