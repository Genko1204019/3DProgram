#include "BaseScene.h"
#include"Application/Manager/StageEditor.h"
#include"Application/Manager/MapLoader.h"
#include"Application/Manager/InventoryManager.h"
#include"Application/Manager/DialogManager.h"
#include"Application/Manager/UiManager.h"


void BaseScene::Init()
{
}

void BaseScene::PreUpdate()
{
	
	auto it = m_objList.begin();
	while (it != m_objList.end()){
		if ((*it)->IsExpired()) {
			it = m_objList.erase(it);
			GameManager::Instance().SetEntityNum(-1);
		}
		else					++it;	
	}

	for (auto& obj : m_objList) obj->PreUpdate();
	
}

void BaseScene::Update()
{
	for (auto& obj : m_objList)	obj->Update();

	Event();
}

void BaseScene::PostUpdate()
{
	for (auto& obj : m_objList) obj->PostUpdate();
}

void BaseScene::PreDraw()
{
	for (auto& obj : m_objList)obj->PreDraw();

	
}

void BaseScene::Draw()
{
	
	
	KdShaderManager::Instance().m_StandardShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& obj : m_objList) obj->GenerateDepthMapFromLight();
		
	}
	KdShaderManager::Instance().m_StandardShader.EndGenerateDepthMapFromLight();

	KdShaderManager::Instance().m_StandardShader.BeginLit();
	{

		for (auto& obj : m_objList)	obj->DrawLit();
		
		KdEffekseerManager::GetInstance().Draw();

		KdShaderManager::Instance().m_spriteShader.Begin();
		{
			for (auto& obj : m_objList) obj->DrawMoji();
			

		}
		KdShaderManager::Instance().m_spriteShader.End();

	}
	KdShaderManager::Instance().m_StandardShader.EndLit();

	
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		for (auto& obj : m_objList) obj->DrawUnLit();
		//MapLoader::Instance().DrawUnLit();
		StageEditor::Instance().DrawUnLit();

	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();


	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		for (auto& obj : m_objList) obj->DrawBright();
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}

void BaseScene::DrawSprite()
{

	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		for (auto& obj : m_objList) obj->DrawSprite();
		UiManager::Instance().DrawSprite();
	}
	KdShaderManager::Instance().m_spriteShader.End();

}

void BaseScene::DrawMoji()
{
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		//for (auto& obj : m_objList) obj->DrawMoji();
		GameManager::Instance().DrawMoji();
		DialogManager::Instance().DrawMoji();

		MapLoader::Instance().DrawMoji();

		UiManager::Instance().DrawMoji();

	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void BaseScene::DrawDebug()
{
	
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		for (auto& obj : m_objList) obj->DrawDebug();
		//MapLoader::Instance().DrawDebug();
		
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();
}

void BaseScene::DrawRender()
{
	KdShaderManager::Instance().m_StandardShader.BeginLit();
	{

		for (auto& obj : m_objList)	obj->DrawRenderTarget();

	}
	KdShaderManager::Instance().m_StandardShader.EndLit();

}

void BaseScene::DrawMini()
{
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		for (auto& obj : m_objList) obj->DrawMini();
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void BaseScene::CallBaseImgui()
{
	for (auto& obj : m_objList) obj->CallImgui();
}

void BaseScene::Event()
{

}

