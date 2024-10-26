#include"GameManager.h"
#include"Application/Manager/EnemyLoader.h"
#include"Application/Manager/MapLoader.h"
#include"Application/GameObject/Player/Player.h"
#include"Application/Scene/SceneManager.h"
#include"Application/GameObject/Camera/GameCamera/GameCamera.h"
#include"Application/GameObject/StageObj/StageObj.h"
#include"Application/Manager/InputManager.h"
#include"Application/Manager/UiManager.h"





void GameManager::Init()
{
	//view this link for locale : https://chatgpt.com/share/670df3ab-c450-800a-b623-47b2ed9588f6
	//std::locale::global(std::locale("en_US.UTF-8"));  // Change to UTF-8
	std::locale::global(std::locale("ja_JP.UTF-8"));  // Change to UTF-8
	prevTime = std::chrono::high_resolution_clock::now();

	fadeTex.Load("Asset/Textures/Black01.png");

	KdAudioManager::Instance().PlayBGM();
}

void GameManager::Update()
{
	UpdateSound();
	UpdateStageProgress();
	UpdateFogLight();
	UpdateDeltaTime();
	UpdateSceneFade();

	EndBattle();

	DebugKey();

}

void GameManager::DrawSprite()
{
	Math::Rectangle fadeRect = { 0,0,1280,720 };

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(1) * Matrix::CreateTranslation(0, 0, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(&fadeTex, 0, 0, 1280, 720, &fadeRect, &fadeColor, Math::Vector2{ 0.5, 0.5 });
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);
	
}

void GameManager::DrawMoji()
{

}

void GameManager::StartBattle()
{
	//Fade Screen -> BattleMode -> LoadEnemy ->Close MiniMap -> SetPlayerPos -> BGM
	 
	canFadeScene = true;
	fadeAlpha = 1;
	isBattleMode = true;
	canLoadBattleEnemy = true;
	isMiniMap = false;

	shared_ptr<Player> spPlayer = wpPlayer.lock();
	if (spPlayer) {
		playerMapReturnPos = spPlayer->GetPosition();
		spPlayer->SetPosition(battlePlayerPos);
	}

	KdAudioManager::Instance().gameBgm->Stop();
	KdAudioManager::Instance().PlayBattleBGM();
	wpCamera.lock()->StartBattleCutScene();

}

void GameManager::EndBattle()
{

	if (isEndBattle) {
		endBattleCnt--;
		gameSpd = 0.14;
	}
	if (isEndBattle && endBattleCnt < 0) {
		gameSpd = 1;
		isEndBattle = false;
		KdAudioManager::Instance().PlayVictorySE();
		UiManager::Instance().showBattleWin = true;

		if (floorNum == 0) {
			canNextFloor = true;
			KdAudioManager::Instance().PlayGameClearSE();
		}
	}

	if (isBattleMode && battleEnemyNum <= 0 && !canFadeScene) {
		isBattleMode = false;
		canFadeScene = true;
		fadeAlpha = 1;
		isMiniMap = true;
		wpPlayer.lock()->SetPosition(playerMapReturnPos);
		canWandererDie = true;
		ClearAllBattleEnemy();
		KdAudioManager::Instance().PlayBGM();

	}

	

}

void GameManager::GoToNextFloor()
{
	if (stageProgress < stageProgressMax) return;

	stageProgress = 0;
	if (floorNum < 9) floorNum += 1;
	else floorNum = 0;

	KdAudioManager::Instance().PlayStairSE();

	if (floorNum != 0) {
		canNextFloor = true;
		canFadeScene = true;
		fadeAlpha = 1;
	}
	else if (floorNum == 0) {
		StartBattle();
	}

}

void GameManager::UpdateSceneFade()
{
	fadeColor = { 1,1,1,fadeAlpha };
	if (canFadeScene) fadeAlpha -= 0.014;
	if (canFadeScene && fadeAlpha <= 0) canFadeScene = false;

}

void GameManager::UpdateFogLight()
{
	KdShaderManager::Instance().WorkAmbientController().SetDirLight(lightDir, { lightColor.x,lightColor.y,lightColor.z });//add a bit z to create shadow,face and shine player in orange light in win scene //dir(default ) and color  (strong light)
	KdShaderManager::Instance().WorkAmbientController().SetAmbientLight(ambientColor); //color

	if (GameManager::Instance().GetIsBattleMode())	topVal = 0;
	else if (GameManager::Instance().GetFloorNum() == 0) topVal = 0;
	else											topVal = 0.7; //0.7
	KdShaderManager::Instance().WorkAmbientController().SetFogEnable(false, true); // false & true or true & false
	KdShaderManager::Instance().WorkAmbientController().SetheightFog({ hfogColor.x,hfogColor.y ,hfogColor.z }, topVal, botVal, hfogDist);

}


void GameManager::UpdateStageProgress()
{
	if (stageProgress > stageProgressMax) stageProgress = stageProgressMax;

}

void GameManager::UpdateDeltaTime()
{
	currentTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> delta = currentTime - prevTime;
	deltaTime = delta.count();

	prevTime = currentTime;
	globalTime += deltaTime;

}

void GameManager::UpdateSound()
{
	KdAudioManager::Instance().ControlSound();
}

void GameManager::ClearAllBattleEnemy()
{
	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->GetTag() == GameTag::EnemyTag && obj->GetType() != EnemyType::eWanderer) {
			obj->SetExpired();
		}

		if (obj->GetTag() == GameTag::EnemyWeaponTag) {
			obj->SetExpired();
		}
	}


}

void GameManager::DebugKey()
{

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::PKey)) {
		isImgui = !isImgui;
		KdAudioManager::Instance().PlayGameStartSE();
		
	}

	/*if (InputManager::Instance().IsKeyJustPressed(KeyFlg::YKey)) {
		for (int i = 0; i < 20; i++) ShowCursor(false);
	}
	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::UKey)) {
		for (int i = 0; i < 20; i++) ShowCursor(true);
	}*/


	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::EnterKey)) { 
		isGameStart = true; 
		canFadeScene = true;
		wpCamera.lock()->m_DegAng.x = 21; 
		KdAudioManager::Instance().PlayGameStartSE();
	
	}

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::BKey)) {
		wpCamera.lock()->StartCameraTwist();
	}

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::RKey)) {
		GameManager::Instance().SetCanNextFloor(true);
	}

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::MKey)) {
		isMiniMap = !isMiniMap;
		KdAudioManager::Instance().PlayGameStartSE();
	}

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::NKey)) {
		stageProgress = 100;
		KdAudioManager::Instance().PlayGameStartSE();
	}

	playerScrollX = wpPlayer.lock()->scrollX;
	playerScrollY = wpPlayer.lock()->scrollY;

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::LKey)) {
		floorNum = 9;
		KdAudioManager::Instance().PlayGameStartSE();
	}

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::KKey)) {

		battleEnemyNum = 0;
		KdAudioManager::Instance().battleBgm->Stop();
	}
	

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::TabKey)) {
		isInventoryOpen = !isInventoryOpen;
		KdAudioManager::Instance().PlayOpenInvenSE();
		isMiniMap = false;

		if (isInventoryOpen) {
			inventoryOriginPos = wpPlayer.lock()->GetPosition();
			wpPlayer.lock()->SetPosition(wpPlayer.lock()->GetPosition() + Vector3(0, 210, 0));
			wpCamera.lock()->OpenInventoryMode();
			gameSpd = 0;
		}
		else {
			wpPlayer.lock()->SetPosition(inventoryOriginPos);
			gameSpd = 1;
		}

	}

	
	



}

void GameManager::CallImgui()
{
	ImGui::Begin("GameManager");

	if (ImGui::CollapsingHeader("Fog,Ambient,Color")) {



	}

	//show entityNum
	ImGui::Text("ListSize : %d", SceneManager::Instance().GetObjList().size());
	//show entityNum
	ImGui::Text("entityNum : %d", entityNum);

	//show stagePorgess
	ImGui::Text("stageProgress : %d", stageProgress);

	//if button is clicked 
	if (ImGui::Button("Cursor")) {
		for (int i = 0; i < 10; i++) ShowCursor(true);
	}

	if (ImGui::Button("CursorOff")) {
		for (int i = 0; i < 10; i++)		ShowCursor(false);
	}

	//checkBox isGameStart
	ImGui::Checkbox("isGameStart", &isGameStart);

	//slider float kdAudiMAnager soundCd
	ImGui::SliderFloat("soundCd", &KdAudioManager::Instance().soundCdMax, 0, 70);

	//show soundCd
	ImGui::Text("soundCd : %f", KdAudioManager::Instance().soundCd);

	//inventoryOriginPos
	ImGui::Text("inventoryOriginPos : %f %f %f", inventoryOriginPos.x, inventoryOriginPos.y, inventoryOriginPos.z);

	//check IsTypingMode
	ImGui::Checkbox("isTypingMode", &isTypingMode);

	//show floorNum
	ImGui::Text("floorNum : %d", floorNum);

//slider float gamespd
	ImGui::SliderFloat("gameSpd", &gameSpd, 0, 14);

	//color checkBox canDebugWire
	ImGui::Checkbox("canDebugWire", &canDebugWire);

	

	//show battleEnemyNum
	ImGui::Text("battleEnemyNum : %d", battleEnemyNum);


	//checkBox isBattleMode
	ImGui::Checkbox("isBattleMode", &isBattleMode);

	//checkbox canNextFloor
	ImGui::Checkbox("canNextFloor", &canNextFloor);

	ImGui::End();

}
