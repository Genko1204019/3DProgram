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


	hpBarTex.Load("Asset/Textures/hpBarRed.png");
	hpBarBackTex.Load("Asset/Textures/hpBarGrey.png");
	fadeTex.Load("Asset/Textures/Black01.png");
	startTex.Load("Asset/Textures/Black01.png");



	KdAudioManager::Instance().PlayBGM();


	mapPos.resize(mapHeight); for (auto& pos : mapPos) { pos.resize(mapWidth); }
	mapData.resize(mapHeight); for (auto& data : mapData) { data.resize(mapWidth); }

	for (int h = 0; h < mapHeight; h++) {
		for (int w = 0; w < mapWidth; w++) {
			miniMapTex[h][w] = make_shared<KdTexture>();
			miniMapTex[h][w]->Load("Asset/Textures/miniMapChip01.png");
		}
	}

}

void GameManager::Update()
{

	SoundManagement();

	GetStringFromJson();

	DebugKey();

	GameTimeManager();

	//gameStart
	if(isGameStart)startAlpha -= 0.01;
	startColor = { 1,1,1,startAlpha };


	PlayerInfo();

	EndBattle();


	ManageStageProgress();

	fadeColor = { 1,1,1,fadeAlpha };

	fadeCnt--;
	if (canFade && fadeCnt < 0) fadeAlpha -= 0.014;
	//else fadeAlpha -= 0.03;

	if(canFadeFloor && fadeAlpha >0) fadeAlpha -= 0.007;
	if(canFadeFloor && fadeAlpha <= 0) canFadeFloor = false;

	if (fadeAlpha <= 0 && canFade) {
		canFade = false;
		
	}

	CalDeltaTime();
	

}

void GameManager::DrawSprite()
{
	
	float currentHpWidth = (playerHp / 100) * 190;


	Math::Rectangle hpBarRect = { 0,0,(int)currentHpWidth,20 };
	Math::Rectangle hpBarBackRect = { 0,0,190,20 };
	
	//KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(1.4) * Matrix::CreateTranslation(-599, 307, 0));
	//KdShaderManager::Instance().m_spriteShader.DrawTex(&hpBarBackTex, 0,0, hpBarBackRect.width, hpBarBackRect.height, &hpBarBackRect, &nColor, Math::Vector2{0, 0.5});
	//KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);
	

	

	//GameManager::Instance().rtc.ChangeRenderTarget(GameManager::Instance().rtp);
	//GameManager::Instance().rtp.ClearTexture(Math::Color(0, 0, 0, 0)); 
	for (int h = 0; h < mapHeight; h++) {
		for (int w = 0; w < mapWidth; w++) {
			Color miniColor;
			if (mapData[h][w] == MapType::WallType) miniColor = kRedColor;
			else miniColor = kWhiteColor;

			Vector2 miniPos;
			miniPos.x = (16 / 6) * mapPos[h][w].x + 300;
			miniPos.y = (16 / 6) * mapPos[h][w].z + 42;



			//KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(2) * Matrix::CreateTranslation(miniPos.x, miniPos.y, 0));
			//KdShaderManager::Instance().m_spriteShader.DrawTex(miniMapTex[h][w], 0, 0, miniMapRect.width, miniMapRect.height, &miniMapRect, &miniColor, Math::Vector2{ 0, 0.5 });
			//KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);


		}
	}
	/*KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(1.4) * Matrix::CreateTranslation(-599, hpY, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(&hpBarTex, 0, 0, hpBarRect.width, hpBarRect.height, &hpBarRect, &nColor, Math::Vector2{ 0, 0.5 });
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);*/

	//GameManager::Instance().rtc.UndoRenderTarget();
	//KdShaderManager::Instance().m_spriteShader.DrawTex(GameManager::Instance().rtp.m_RTTexture, 0, 0);


	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(1) * Matrix::CreateTranslation(0, 0, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(&fadeTex, 0, 0, 1280, 720, &fadeRect, &fadeColor, Math::Vector2{ 0.5, 0.5 });
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);
	
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(1) * Matrix::CreateTranslation(0, 0, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(&startTex, 0, 0, 1280, 720, &fadeRect, &startColor, Math::Vector2{ 0.5, 0.5 });
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

}

void GameManager::DrawMoji()
{


	//GameManager::Instance().rtc.ChangeRenderTarget(GameManager::Instance().rtp);
	//GameManager::Instance().rtp.ClearTexture(Math::Color(0, 0, 0, 0)); 

	//GameManager::Instance().rtc.UndoRenderTarget();
	//KdShaderManager::Instance().m_spriteShader.DrawTex(GameManager::Instance().rtp.m_RTTexture, 0, 0);


}

void GameManager::StartBattle()
{
	//Fade Screen -> BattleMode -> LoadEnemy ->Close MiniMap -> SetPlayerPos -> BGM
	 
	canFade = true;
	fadeAlpha = 1;
	fadeCnt = 70;

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

void GameManager::StartEndBattle()
{
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

	shared_ptr<Player> spPlayer = wpPlayer.lock();

	if (spPlayer) {
		playerHp = spPlayer->Gethp();
	}

	if (isBattleMode && battleEnemyNum <= 0 && !canFade) {
		isBattleMode = false;
		//wpPlayer.lock()->SetPosition(wpPlayer.lock()->GetoldPos());
		canFadeFloor = true;
		fadeAlpha = 1;

		//KdAudioManager::Instance().PlayVictorySE();
		//KdAudioManager::Instance().battleBgm->Stop();
		KdAudioManager::Instance().PlayBGM();
		
		isMiniMap = true;

		//set all warrior to expire!!

		canWandererDie = true;

		spPlayer->SetPosition(playerMapReturnPos);

		ClearAllBattleEnemy();

		/*if (floorNum == 10) {
			floorNum == 0;
		}*/

		isMiniMap = true;

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
		canFadeFloor = true;
		fadeAlpha = 1;
	}
	else if (floorNum == 0) {
		StartBossBattle();
		//floorNum = 0;
	}

	//KdEffekseerManager::GetInstance().StopEffect("Simple_Turbulence_Fireworks.efkefc");


}

void GameManager::FadeFloor()
{
	canFadeFloor = true;
	fadeAlpha = 1;

}

void GameManager::PlayerInfo()
{
	



}


void GameManager::GameTimeManager()
{
	if (isGameStop) {
		gameSpd = 0;
	}

	

}

void GameManager::StartBossBattle()
{

	canFade = true;
	fadeAlpha = 1;
	fadeCnt = 70;

	isBattleMode = true;
	canLoadBoss = true;
	canLoadBattleEnemy = true;

	isMiniMap = false;

	shared_ptr<Player> spPlayer = wpPlayer.lock();
	if (spPlayer) {
		spPlayer->SetPosition(battlePlayerPos);
	}

	KdAudioManager::Instance().gameBgm->Stop();
	KdAudioManager::Instance().PlayBattleBGM();

	wpCamera.lock()->StartBattleCutScene();

}

void GameManager::ManageStageProgress()
{
	if (stageProgress > stageProgressMax) stageProgress = stageProgressMax;

	//if (floorNum == 0) stageProgress = 100;


}

void GameManager::CalDeltaTime()
{
	// Get the current time
	currentTime = std::chrono::high_resolution_clock::now();

	// Calculate delta time in seconds
	std::chrono::duration<float> delta = currentTime - prevTime;
	deltaTime = delta.count();

	// Update prevTime to the current time for the next frame
	prevTime = currentTime;

	globalTime += deltaTime;

}

void GameManager::GetStringFromJson()
{
	if (!hasReadJson) {
		std::ifstream jsonFile(jsonPath); // Open the JSON file

		if (!jsonFile.is_open()) {
			return;
		}

		json jsonData; // Create a JSON object
		jsonFile >> jsonData; // Parse the JSON file

		if (jsonData.contains("gameName")) {
			gameName = jsonData["gameName"].get<std::string>(); // Get the gameName from the JSON file
		}
		

		jsonFile.close(); // Close the file after reading
		hasReadJson = true;
	}
}

void GameManager::SoundManagement()
{
	KdAudioManager::Instance().ControlSound();
}

void GameManager::ClearAllBattleEnemy()
{
	//loop through all objects in the scene and remove all battle enemies
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

	debugCnt--;

	//if (!isImgui) ShowCursor(false);
	//else ShowCursor(true);

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::PKey)) {
		if (!isImgui) {
			for (int i = 0; i < 20; i++) ShowCursor(true);
		}
		else {
			for (int i = 0; i < 20; i++) ShowCursor(false);
		}
		isImgui = !isImgui;
		KdAudioManager::Instance().PlayGameStartSE();
		
	}

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::YKey)) {
		for (int i = 0; i < 20; i++) ShowCursor(false);
	}
	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::UKey)) {
		for (int i = 0; i < 20; i++) ShowCursor(true);
	}


	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::EnterKey)) { 
		isGameStart = true; 
		wpCamera.lock()->m_DegAng.x = 21; 
		KdAudioManager::Instance().PlayGameStartSE();
	
	}

	if (!isEditorMode) {

		if (InputManager::Instance().IsKeyJustPressed(KeyFlg::BKey) && debugCnt < 0) {
			wpCamera.lock()->StartCameraTwist();
			debugCnt = 70;
		}

		/*if (InputManager::Instance().IsKeyJustPressed(KeyFlg::RKey) && debugCnt < 0) {
			debugCnt = 70;
			GameManager::Instance().SetCanNextFloor(true);
		}*/

		if (InputManager::Instance().IsKeyJustPressed(KeyFlg::MKey) && debugCnt < 0) {
			debugCnt = 35;
			isMiniMap = !isMiniMap;
			KdAudioManager::Instance().PlayGameStartSE();
		}

		if (InputManager::Instance().IsKeyJustPressed(KeyFlg::NKey) && debugCnt < 0) {
			debugCnt = 35;
			stageProgress = 100;
			KdAudioManager::Instance().PlayGameStartSE();
		}

		scrollX = wpPlayer.lock()->scrollX;
		scrollY = wpPlayer.lock()->scrollY;
		

	}


	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::LKey)) {
		floorNum = 9;
		KdAudioManager::Instance().PlayGameStartSE();
	}

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::KKey)) {

		battleEnemyNum = 0;
		KdAudioManager::Instance().battleBgm->Stop();
	}



	//if(GetAsyncKeyState('C')) ShowCursor(true);
	

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
			wpCamera.lock()->InventoryToFollowMode();
			gameSpd = 1;
		}

	}

	
	



}

void GameManager::CallImgui()
{
	ImGui::Begin("GameManager");

	//if button is clicked 
	if (ImGui::Button("Cursor")) {
		for (int i = 0; i < 10; i++) ShowCursor(true);
	}

	if (ImGui::Button("CursorOff")) {
		for (int i = 0; i < 10; i++)		ShowCursor(false);
	}

	//checkBox isGameStart
	ImGui::Checkbox("isGameStart", &isGameStart);


	//slider float  alphaPow mojiAdd
	ImGui::SliderFloat("alphaPow", &alphaPow, 0, 0.1);
	ImGui::SliderFloat("mojiAdd", &mojiAdd, 0, 1);


	//slider float kdAudiMAnager soundCd
	ImGui::SliderFloat("soundCd", &KdAudioManager::Instance().soundCdMax, 0, 70);

	//show soundCd
	ImGui::Text("soundCd : %f", KdAudioManager::Instance().soundCd);

	//inventoryOriginPos
	ImGui::Text("inventoryOriginPos : %f %f %f", inventoryOriginPos.x, inventoryOriginPos.y, inventoryOriginPos.z);

	//Color edit 4 sliderColor
	ImGui::ColorEdit4("sliderColor", &sliderColor.x);
	


	//slider float sliderA B -720 to 720
	ImGui::SliderFloat("sliderA", &sliderA, -720, 720);
	ImGui::SliderFloat("sliderB", &sliderB, -720, 720);


	//slider mojiMoveX Y degen
	ImGui::SliderFloat("mojiMoveX", &mojiMoveX, -10, 10);
	ImGui::SliderFloat("mojiMoveY", &mojiMoveY, -10, 10);
	ImGui::SliderFloat("degen", &mojiMoveDegen, -10, 10);

	//show gameName
	ImGui::Text("gameName : %s", gameName.c_str());

	//color4 itemColor
	ImGui::ColorEdit4("itemColor", &itemColor.x);

	//slider testNum testAngle 
	ImGui::SliderInt("testNum", &testNum, 1, 10 );
	ImGui::SliderFloat("testAngle", &testAngle, 1, 360);

	//slider float sliderVal 0 - 360
	ImGui::SliderFloat("sliderVal", &sliderVal, 0, 360);


	//check IsTypingMode
	ImGui::Checkbox("isTypingMode", &isTypingMode);

	//show floorNum
	ImGui::Text("floorNum : %d", floorNum);

	//slider float control xyz -1 to 1
	ImGui::SliderFloat("controlX", &controlX, -1, 1);
	ImGui::SliderFloat("controlY", &controlY, -1, 1);
	ImGui::SliderFloat("controlZ", &controlZ, -1, 1);

	//slider float camFov 0 to 360
	ImGui::SliderFloat("camFov", &camFov, 0, 360);

	//slider hpY -360 to 360
ImGui::SliderFloat("hpY", &hpY, -360, 360);

	//slider float inventoryX inventoryY decriptX decriptY -700 to 700
	ImGui::SliderFloat("inventoryX", &inventoryX, -700, 700);
	ImGui::SliderFloat("inventoryY", &inventoryY, -700, 700);
	ImGui::SliderFloat("decriptX", &decriptX, -700, 700);
	ImGui::SliderFloat("decriptY", &decriptY, -700, 700);

	//slider float invenSpace 0 to 100
	ImGui::SliderFloat("invenSpace", &invenSpace, 0, 100);

	//slider int  	 eSearchrange 1 to 28
	ImGui::SliderInt("eSearchRange", &eSearchrange, 1, 28);



	//slider float gamespd
ImGui::SliderFloat("gameSpd", &gameSpd, 0, 14);

	//check isGameStop

	ImGui::Checkbox("isGameStop", &isGameStop);

	

	//slider float effectAngle
	ImGui::SliderFloat("effectAngle", &effectAngle, 0, 360);

	//check isEditorMode
ImGui::Checkbox("isEditorMode", &isEditorMode);

	//color checkBox canDebugWire
	ImGui::Checkbox("canDebugWire", &canDebugWire);

	ImGui::Checkbox("canEnemyCollision", &canEnemyCollision);

	//slider float polyscale xyz 1 to 10
	ImGui::SliderFloat("polyscale.x", &polyScale.x, 0.1, 10);
	ImGui::SliderFloat("polyscale.y", &polyScale.y, 0.11, 10);
	ImGui::SliderFloat("polyscale.z", &polyScale.z, 1, 10);

	//show battleEnemyNum
	ImGui::Text("battleEnemyNum : %d", battleEnemyNum);

	//show playerHp
	ImGui::Text("playerHp:%f", playerHp);

	//checkBox isBattleMode
	ImGui::Checkbox("isBattleMode", &isBattleMode);

	//checkbox canNextFloor
	ImGui::Checkbox("canNextFloor", &canNextFloor);

	//sliderFloat hpX
	ImGui::SliderFloat("hpX", &hpX, 0, 190);

	ImGui::End();

}
