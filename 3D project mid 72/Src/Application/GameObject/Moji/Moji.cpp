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

	//KdShaderManager::Instance().m_spriteShader.DrawTex(tex.get(), mojiPos.x, mojiPos.y, mojiScale, mojiColor);

	if (objType == MojiType::MessageMoji) {
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

	//move.x = Utility::Rnd(-0.07f, 0.07f); // Random initial horizontal movement
	//move.y = Utility::Rnd(0.005f, 0.015f);  // Random initial upward movement
	
	move.x = GameManager::Instance().mojiMoveX;
	move.y = GameManager::Instance().mojiMoveY;

	mojiDir.y = Utility::Rnd(1.9,2.8); //2.24 0.3

	mojiDir.x = Utility::Rnd(2.8,4.9);
	plusMinus = rand() % 2;
	if (plusMinus == 0) mojiDir.x *= -1;

}

void Moji::InitMessageMoji()
{
	tex.Load("Asset/Textures/massageframe_01.png");

	mojiPos = GameManager::Instance().mojiSpawnPos;
	GameManager::Instance().mojiSpawnPos.y -= 35;

}

void Moji::UpdateDmgMoji()
{
	mojiColor = { 1,1,1,mojiAlpha };
	mojiAlpha -= 0.021;

	if (mojiAlpha < 0) SetExpired();
	
	//pos += move;
	//move.y -= 0.001;
	//move.y -= GameManager::Instance().mojiMoveDegen;

	const shared_ptr<GameCamera>_spCam = wpCamera.lock();
	if (_spCam)
	{
		_spCam->WorkCamera()->ConvertWorldToScreenDetail(pos, pos2D);

		pos2D.x += mojiOffset.x;
		pos2D.y += mojiOffset.y;
		//mojiOffset.x += GameManager::Instance().mojiMoveDegen;

		mojiOffset.x += mojiDir.x;

		mojiOffset.y += mojiDir.y; //moveDir 

		//if(mojiDir.x < 0) mojiDir.x += -0.14 ;
		//else mojiDir.x += 0.35;
		
		mojiDir.y += -0.7;
	}
}

void Moji::UpdateMessageMoji()
{
	mojiAlpha -= GameManager::Instance().alphaPow;
	mojiPos.y += GameManager::Instance().mojiAdd;

	if (mojiAlpha < 0) {
		SetExpired();
		GameManager::Instance().mojiSpawnPos.y += 35;
	}

}

void Moji::DrawDmgMoji()
{
	KdShaderManager::Instance().m_spriteShader.DrawString(pos2D.x, pos2D.y, std::to_string((int)dmgNum).c_str(), mojiColor);

}

void Moji::DrawMessageMoji()
{
	mojiColor = { 1,1,1,mojiAlpha };
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
