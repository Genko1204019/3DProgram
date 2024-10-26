#include"Application/GameObject/WeaponPoly/WeaponPoly.h"
#include"Application/Manager/GameManager.h"

void WeaponPoly::Init()
{
	poly = std::make_shared<KdSquarePolygon>();
	poly->SetMaterial("Asset/Textures/sword_effect.png");
	trail.SetMaterial("Asset/Textures/sword_effect.png");

}

void WeaponPoly::Update()
{

}

void WeaponPoly::DrawUnLit()
{
	polyAlpha -= 0.014;
	polyColor = { 0.3,0.3,0.3,polyAlpha };

	//draw poly

}

void WeaponPoly::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*poly, scaleMat * polyMat, polyColor);

}

void WeaponPoly::CallImgui()
{


}
