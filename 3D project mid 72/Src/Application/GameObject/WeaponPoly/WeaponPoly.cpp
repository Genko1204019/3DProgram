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
	scaleMat = Matrix::CreateScale(GameManager::Instance().GetPolyScale());

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

	//slider float scale xyz
	ImGui::SliderFloat("Scale.x", &scale.x, 1, 10);

	//.y
	ImGui::SliderFloat("Scale.y", &scale.y, 1, 10);

	//.z

	ImGui::SliderFloat("Scale.z", &scale.z, 1, 10);




}
