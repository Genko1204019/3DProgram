#include"Inventory.h"

void Inventory::Init()
{
}

void Inventory::Update()
{
}

void Inventory::DrawSprite()
{

	DrawUi(inventoryFrame);

}

void Inventory::DrawMoji()
{
}

void Inventory::DrawUi(const UiElement& _ui)
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(_ui.scale.x, _ui.scale.y, 1) * Matrix::CreateTranslation(_ui.pos.x, _ui.pos.y, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(_ui.tex.get(), 0, 0, &_ui.rect, &_ui.color, Math::Vector2{ 0.5, 0.5 });
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

}
