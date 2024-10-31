#include "gauge.h"
#include <Input/Input.h>
#include"itemManager.h"

Gauge::Gauge()
{
	sprGauge = new Sprite("Data/Sprite/gaugeCola.png");
}

Gauge::~Gauge()
{
	if (sprGauge != nullptr)
	{
		delete sprGauge;
		sprGauge = nullptr;
	}

	
	
}

void Gauge::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();

	if (ItemManager::Instance().GetMentos())
	{
		gaugeNum = 4;
	}

	if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
	{
		if (gaugeNum <= 4)
			gaugeNum += 0.5f;
		

	}
}

void Gauge::Render(ID3D11DeviceContext* dc)
{
	sprGauge->Render(dc, 100, 100, 100, 300, 160 * static_cast<int>(gaugeNum), 0, 160, 640, 0, 1, 1, 1, 1);
}


