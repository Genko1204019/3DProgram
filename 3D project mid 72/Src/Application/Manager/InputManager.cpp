#include "InputManager.h"

void InputManager::Init()
{
}

void InputManager::Update()
{
	UpdateKeyInput();

}

void InputManager::UpdateKeyInput()
{

	if (GameManager::Instance().GetIsTypingMode()) return;

	m_prevKey = m_nowKey;
	m_nowKey = 0;

	//up down left right
	if (GetAsyncKeyState(VK_UP) & 0x8000)   m_nowKey |= KeyFlg::UpKey;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)   m_nowKey |= KeyFlg::DownKey;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)   m_nowKey |= KeyFlg::LeftKey;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)   m_nowKey |= KeyFlg::RightKey;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)   m_nowKey |= KeyFlg::LButton;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)   m_nowKey |= KeyFlg::RButton;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)   m_nowKey |= KeyFlg::ShiftKey;

	//Return	key
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)   m_nowKey |= KeyFlg::EnterKey;
	//Space key
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)   m_nowKey |= KeyFlg::SpaceKey;
	//Tab key
	if (GetAsyncKeyState(VK_TAB) & 0x8000)   m_nowKey |= KeyFlg::TabKey;
	//Ctrl key
	//if (GetAsyncKeyState(VK_CONTROL) & 0x8000)   m_nowKey |= KeyFlg::CtrlKey;


	//Q key
	if(GetAsyncKeyState('Q') & 0x8000)   m_nowKey |= KeyFlg::QKey;
	//E key
	if(GetAsyncKeyState('E') & 0x8000)   m_nowKey |= KeyFlg::EKey;
	//R key
	if(GetAsyncKeyState('R') & 0x8000)   m_nowKey |= KeyFlg::RKey;
	//T key
	if(GetAsyncKeyState('T') & 0x8000)   m_nowKey |= KeyFlg::TKey;
	//Y key
	if(GetAsyncKeyState('Y') & 0x8000)   m_nowKey |= KeyFlg::YKey;
	//U key
	if(GetAsyncKeyState('U') & 0x8000)   m_nowKey |= KeyFlg::UKey;
	//I key
	if(GetAsyncKeyState('I') & 0x8000)   m_nowKey |= KeyFlg::IKey;
	//O key
	if(GetAsyncKeyState('O') & 0x8000)   m_nowKey |= KeyFlg::OKey;
	//p key
	if(GetAsyncKeyState('P') & 0x8000)   m_nowKey |= KeyFlg::PKey;

	//F key
	if (GetAsyncKeyState('F') & 0x8000)   m_nowKey |= KeyFlg::FKey;
	//G key
	if (GetAsyncKeyState('G') & 0x8000)   m_nowKey |= KeyFlg::GKey;
	//H key
	if (GetAsyncKeyState('H') & 0x8000)   m_nowKey |= KeyFlg::HKey;
	//J key
	if (GetAsyncKeyState('J') & 0x8000)   m_nowKey |= KeyFlg::JKey;
	//K key
	if (GetAsyncKeyState('K') & 0x8000)   m_nowKey |= KeyFlg::KKey;
	//L key
	if (GetAsyncKeyState('L') & 0x8000)   m_nowKey |= KeyFlg::LKey;


	//Z key
	if(GetAsyncKeyState('Z') & 0x8000)   m_nowKey |= KeyFlg::ZKey;
	//X key
	if(GetAsyncKeyState('X') & 0x8000)   m_nowKey |= KeyFlg::XKey;
	if (GetAsyncKeyState('C') & 0x8000)   m_nowKey |= KeyFlg::CKey;
	if (GetAsyncKeyState('V') & 0x8000)   m_nowKey |= KeyFlg::VKey;
	if (GetAsyncKeyState('B') & 0x8000)   m_nowKey |= KeyFlg::BKey;
	if (GetAsyncKeyState('N') & 0x8000)   m_nowKey |= KeyFlg::NKey;
	if (GetAsyncKeyState('M') & 0x8000)   m_nowKey |= KeyFlg::MKey;




}

bool InputManager::IsKeyPress(int key)
{
	return (m_nowKey & key) != 0;

}

bool InputManager::IsKeyJustPressed(int key)
{
	return ((m_nowKey & key) != 0) && ((m_prevKey & key) == 0);
}

bool InputManager::IsKeyJustReleased(int key)
{
	return ((m_nowKey & key) == 0) && ((m_prevKey & key) != 0);
}

void InputManager::CallImgui()
{
	ImGui::Begin("KeyInputs");

	std::vector<std::pair<std::string, int>> keyList = {
		{"LButton", KeyFlg::LButton},
		{"RButton", KeyFlg::RButton},
		{"ShiftKey", KeyFlg::ShiftKey},
		{"EnterKey", KeyFlg::EnterKey},
		{"SpaceKey", KeyFlg::SpaceKey},
		{"TabKey", KeyFlg::TabKey},
		{"QKey", KeyFlg::QKey},
		{"EKey", KeyFlg::EKey},
		{"RKey", KeyFlg::RKey},
		{"TKey", KeyFlg::TKey},
		{"YKey", KeyFlg::YKey},
		{"UKey", KeyFlg::UKey},
		{"IKey", KeyFlg::IKey},
		{"OKey", KeyFlg::OKey},
		{"PKey", KeyFlg::PKey},
		{"FKey", KeyFlg::FKey},
		{"GKey", KeyFlg::GKey},
		{"HKey", KeyFlg::HKey},
		{"JKey", KeyFlg::JKey},
		{"KKey", KeyFlg::KKey},
		{"LKey", KeyFlg::LKey},
		{"ZKey", KeyFlg::ZKey},
		{"XKey", KeyFlg::XKey},
		{"CKey", KeyFlg::CKey},
		{"VKey", KeyFlg::VKey},
		{"BKey", KeyFlg::BKey},
		{"NKey", KeyFlg::NKey},
		{"MKey", KeyFlg::MKey},
		{"UpKey", KeyFlg::UpKey},
		{"DownKey", KeyFlg::DownKey},
		{"LeftKey", KeyFlg::LeftKey},
		{"RightKey", KeyFlg::RightKey},
	};

	for (const auto& key : keyList)
	{
		const std::string& keyName = key.first;
		int keyFlag = key.second;

		if (IsKeyPress(keyFlag)) ImGui::Text("%s: Pressed", keyName.c_str());
		if (IsKeyJustPressed(keyFlag)) ImGui::Text("%s: Just Pressed", keyName.c_str());
		if (IsKeyJustReleased(keyFlag)) ImGui::Text("%s: Just Released", keyName.c_str());
	}

	ImGui::End;



}
