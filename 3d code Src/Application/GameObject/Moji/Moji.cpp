#include"Moji.h"


void Moji::Init()
{
	if (objType == MojiType::DmgMoji) InitDmgMoji();
	if (objType == MojiType::MessageMoji) InitMessageMoji();

}

void Moji::Update()
{
	

	if (objType == MojiType::DmgMoji) UpdateDmgMoji();
	if (objType == MojiType::MessageMoji) UpdateMessageMoji();


}

void Moji::DrawSprite()
{

	if (objType == MojiType::MessageMoji) {
		Math::Rectangle messaageRect = { 0,0,1280,250 };
		KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(0.35,0.19,0.3) * Matrix::CreateTranslation(mojiPos.x, mojiPos.y-35, 0));
		KdShaderManager::Instance().m_spriteShader.DrawTex(&tex, 0, 0, messaageRect.width, messaageRect.height, &messaageRect, &mojiColor, Math::Vector2{ 0, 0 });
		KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

	}

	
}

void Moji::DrawMoji()
{

	if (objType == MojiType::DmgMoji) DrawDmgMoji();
	if (objType == MojiType::MessageMoji) DrawMessageMoji();

}

void Moji::InitDmgMoji()
{
	mojiAlpha = 1;

	mojiDir.y = Utility::Rnd(1.9,2.8); //2.24 0.3
	mojiDir.x = Utility::Rnd(2.8,4.9);

	int leftRight = rand() % 2;
	if (leftRight == 0) mojiDir.x *= -1;

}

void Moji::InitMessageMoji()
{
	tex.Load("Asset/Textures/massageframe_01.png");

	mojiPos = GameManager::Instance().GetMojiSpawnPos();
	GameManager::Instance().SetMojiSpawnPos(GameManager::Instance().GetMojiSpawnPos() + Vector2(0, -35));
}

void Moji::UpdateDmgMoji()
{
	mojiColor = { 1,1,1,mojiAlpha };
	mojiAlpha -= 0.021;

	if (mojiAlpha < 0) SetExpired();

	const shared_ptr<GameCamera>_spCam = wpCamera.lock();
	if (_spCam)
	{
		_spCam->WorkCamera()->ConvertWorldToScreenDetail(pos, pos2D);

		pos2D.x += mojiOffset.x;
		pos2D.y += mojiOffset.y;

		mojiOffset.x += mojiDir.x;
		mojiOffset.y += mojiDir.y; 

		mojiDir.y += -0.7;
	}
}

void Moji::UpdateMessageMoji()
{
	float alphaPow = 0.007;
	float mojiAdd = 0.21;

	mojiAlpha -= alphaPow;
	mojiPos.y += mojiAdd;

	if (mojiAlpha < 0) {
		SetExpired();
		GameManager::Instance().SetMojiSpawnPos(GameManager::Instance().GetMojiSpawnPos() + Vector2(0, 35));
	}

}

void Moji::DrawDmgMoji()
{
	KdShaderManager::Instance().m_spriteShader.DrawString(pos2D.x, pos2D.y, std::to_string((int)dmgNum).c_str(), mojiColor);

}

void Moji::DrawMessageMoji()
{
	mojiColor = { 1,1,1,mojiAlpha };
	string mojiPrefix = "を手に入れました";
	std::string fullText =  mojiText + mojiPrefix;
	KdShaderManager::Instance().m_spriteShader.DrawString(mojiPos.x,mojiPos.y, fullText.c_str(), mojiColor, 0.7);

}

void Moji::CallImgui()
{
	//	ImGui::Begin("moji");
//
//	//show pos2D
//
//	ImGui::Text("pos2D: %f %f", pos2D.x, pos2D.y);
//
//	//show move
//
//ImGui::Text("move: %f %f", move.x, move.y);
//
//	ImGui::End();


}
