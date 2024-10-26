#include "SceneManager.h"

#include "BaseScene/BaseScene.h"
#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include"Application/Manager/GameManager.h"
#include"Application/Manager/UiManager.h"
#include"Application/Manager/PathManager.h"
#include"Application/Manager/MapLoader.h"
#include"Application/Manager/InventoryManager.h"
#include"Application/Manager/DialogManager.h"

void SceneManager::PreUpdate()
{
	// シーン切替
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}

	m_currentScene->PreUpdate();
}

void SceneManager::Update()
{
	m_currentScene->Update();
}

void SceneManager::PostUpdate()
{
	m_currentScene->PostUpdate();
}

void SceneManager::PreDraw()
{

	MapLoader::Instance().DrawMoji();
	

	m_currentScene->PreDraw();

	
}

void SceneManager::Draw()
{


	m_currentScene->Draw();
}

void SceneManager::DrawSprite()
{
	GameManager::Instance().DrawSprite();
	MapLoader::Instance().DrawSprite();

	m_currentScene->DrawSprite();

	//UiManager::Instance().DrawSprite();

	
}

void SceneManager::DrawDebug()
{
	m_currentScene->DrawDebug();
	PathManager::Instance().DrawDebug();

	MapLoader::Instance().DrawDebug();

}

void SceneManager::DrawMoji()
{
	m_currentScene->DrawMoji();

	


	if (GameManager::Instance().GetIsInventoryOpen()) {
		rtp.CreateRenderTarget(1280, 560, true);
		rtc.ChangeRenderTarget(rtp.m_RTTexture);
		rtp.ClearTexture();

		KdShaderManager::Instance().m_spriteShader.Begin();
		{
			InventoryManager::Instance().DrawSprite();
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

			UiManager::Instance().DrawInventory();
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

			InventoryManager::Instance().DrawMoji();
			KdShaderManager::Instance().m_spriteShader.DrawString(-640, -140, "ENTERキーで使用", kWhiteColor, 1);

		}
		KdShaderManager::Instance().m_spriteShader.End();

		KdShaderManager::Instance().m_StandardShader.BeginLit();
		{
			KdEffekseerManager::GetInstance().Draw();
			m_currentScene->DrawRender();

		}
		KdShaderManager::Instance().m_StandardShader.EndLit();

		rtc.UndoRenderTarget();
		KdShaderManager::Instance().m_spriteShader.DrawTex(rtp.m_RTTexture, 0, 0);

	}





	if (GameManager::Instance().GetIsMiniMap())
	{
		rtp.CreateRenderTarget(280, 280, true);

		rtc.ChangeRenderTarget(rtp.m_RTTexture);
		rtp.ClearTexture();


		m_currentScene->DrawMini();

		KdShaderManager::Instance().m_spriteShader.Begin();
		{
		}
		KdShaderManager::Instance().m_spriteShader.End();

		UiManager::Instance().DrawMiniMap();


		rtc.UndoRenderTarget();
		KdShaderManager::Instance().m_spriteShader.DrawTex(rtp.m_RTTexture, 470, 170);
	}





}

void SceneManager::DrawImgui()
{
	m_currentScene->CallBaseImgui();
}

const std::list<std::shared_ptr<KdGameObject>>& SceneManager::GetObjList()
{
	return m_currentScene->GetObjList();
}

void SceneManager::AddObject(const std::shared_ptr<KdGameObject>& obj)
{
	m_currentScene->AddObject(obj);
	GameManager::Instance().SetEntityNum(1);
}

void SceneManager::ChangeScene(SceneType sceneType)
{
	// 次のシーンを作成し、現在のシーンにする
	switch (sceneType)
	{
	case SceneType::Title:
		m_currentScene = std::make_shared<TitleScene>();
		break;
	case SceneType::Game:
		m_currentScene = std::make_shared<GameScene>();
		break;
	}

	// 現在のシーン情報を更新
	m_currentSceneType = sceneType;
}
