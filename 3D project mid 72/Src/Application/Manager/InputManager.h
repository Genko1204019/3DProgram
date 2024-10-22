#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

class InputManager : public KdGameObject
{
public:
	
	void Init() override;
	void Update() override;

	void UpdateKeyInput();
	bool IsKeyPress(int key);
	bool IsKeyJustPressed(int key);
	bool IsKeyJustReleased(int key);

	void CallImgui() override;


private:

	int m_nowKey = 0;
	int m_prevKey = 0;


private:
	InputManager() {}
	~InputManager() override {}

public:
	static InputManager& Instance()
	{
		static InputManager instance;
		return instance;
	}
};

