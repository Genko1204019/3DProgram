#include "KdAudio.h"

// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
// 
// KdAudioManager
// 
// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 初期化
// ・DirectXAudioEngineの初期化 
// ・3Dリスナーの設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAudioManager::Init()
{
	// AudioEngine初期化
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_ReverbUseFilters;

	m_audioEng = std::make_unique<DirectX::AudioEngine>(eflags);
	m_audioEng->SetReverb(DirectX::Reverb_Default);

	m_listener.OrientFront = { 0, 0, 1 };
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 更新
// ・DirectXAudioEngineの更新
// ・プレイリストから不要なサウンドインスタンスを削除
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAudioManager::Update()
{
	// 実はこれを実行しなくても音はなる:定期的に実行する必要はある
	if (m_audioEng != nullptr)
	{
		m_audioEng->Update();
	}

	// ストップさせたインスタンスは終了したと判断してリストから削除
	for (auto iter = m_playList.begin(); iter != m_playList.end();)
	{
		if (iter->second->IsStopped())
		{
			iter = m_playList.erase(iter);

			continue;
		}

		++iter;
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// リスナーの座標と正面方向の設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAudioManager::SetListnerMatrix(const Math::Matrix& mWorld)
{
	// 座標
	m_listener.SetPosition(mWorld.Translation());

	// 正面方向
	m_listener.OrientFront = mWorld.Backward();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 2Dサウンドの再生
// ・サウンドアセットの取得orロード
// ・再生用インスタンスの生成
// ・管理用プレイリストへの追加
// ・戻り値で再生インスタンスを取得可能（音量・ピッチなどを変更する場合に必要
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
std::shared_ptr<KdSoundInstance> KdAudioManager::Play(std::string_view rName, bool loop)
{
	if (!m_audioEng) { return nullptr; }

	std::shared_ptr<KdSoundEffect> soundData = GetSound(rName);

	if (!soundData) { return nullptr; }

	std::shared_ptr<KdSoundInstance> instance = std::make_shared<KdSoundInstance>(soundData);

	if(!instance->CreateInstance()){ return nullptr; }

	instance->Play(loop);

	AddPlayList(instance);

	return instance;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 3Dサウンドの再生
// ・サウンドアセットの取得orロード
// ・再生用インスタンスの生成、3D座標のセット
// ・管理用プレイリストへの追加
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
std::shared_ptr<KdSoundInstance3D> KdAudioManager::Play3D(std::string_view rName, const Math::Vector3& rPos, bool loop)
{
	if (!m_audioEng) { return nullptr; }

	std::shared_ptr<KdSoundEffect> soundData = GetSound(rName);

	if (!soundData) { return nullptr; }

	std::shared_ptr<KdSoundInstance3D> instance = std::make_shared<KdSoundInstance3D>(soundData, m_listener);

	if (!instance->CreateInstance()) { return nullptr; }

	instance->Play(loop);

	instance->SetPos(rPos);

	AddPlayList(instance);

	return instance;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 再生リストの全ての音を停止する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAudioManager::StopAllSound()
{
	auto it = m_playList.begin();
	while (it != m_playList.end()) {
		(*it).second->Stop();
		++it;
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 再生リストの全ての音を一時停止する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAudioManager::PauseAllSound()
{
	auto it = m_playList.begin();
	while (it != m_playList.end()) {
		(*it).second->Pause();
		++it;
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 再生リストの全ての音を再開する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAudioManager::ResumeAllSound()
{
	auto it = m_playList.begin();
	while (it != m_playList.end()) {
		(*it).second->Resume();
		++it;
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 再生中のサウンドの停止・サウンドアセットの解放
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAudioManager::SoundReset()
{
	StopAllSound();

	m_soundMap.clear();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// サウンドアセットの読み込み
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAudioManager::LoadSoundAssets(std::initializer_list<std::string_view>& fileNames)
{
	for (std::string_view fileName : fileNames)
	{
		auto itFound = m_soundMap.find(fileName.data());

		// ロード済みならスキップ
		if (itFound != m_soundMap.end()) { continue; }

		// 生成 & 読み込み
		auto newSound = std::make_shared<KdSoundEffect>();
		if (!newSound->Load(fileName, m_audioEng))
		{
			// 読み込み失敗時
			assert(0 && "LoadSoundAssets:ファイル名のデータが存在しません。名前を確認してください。");

			continue;
		}

		// リスト(map)に登録
		m_soundMap.emplace(fileName, newSound);

	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 解放
// ・再生中のプレイリストの解放
// ・サウンドアセットの解放
// ・エンジンの解放
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAudioManager::Release()
{
	StopAllSound();

	m_playList.clear();

	m_soundMap.clear();

	m_audioEng = nullptr;
}

void KdAudioManager::PlayBGM()
{
	/*if (!gameBgm) {
		gameBgm = KdAudioManager::Instance().Play("Asset/Sounds/StageBgm01.wav", true);
		gameBgm->SetVolume(0.49f);
	}
	else {
		gameBgm->Play(true);
	}*/
	

	gameBgm = KdAudioManager::Instance().Play("Asset/Sounds/CS_Creepy_Little_Things_FULL_Loop.wav", true);
	gameBgm->SetVolume(0.21f * totalVolAdjust * bgmVolAdjust);

}

void KdAudioManager::PlayBattleBGM()
{
	battleBgm = KdAudioManager::Instance().Play("Asset/Sounds/WOW Sound_CS_Battle_of_the_Teddy_Bears_FULL_Loop.wav", true);
	battleBgm->SetVolume(0.07f * totalVolAdjust * bgmVolAdjust);
}

void KdAudioManager::PlaySwordSE()
{
	if (!swordSE) {
		swordSE = KdAudioManager::Instance().Play("Asset/Sounds/sword.wav", false);
		swordSE->SetVolume(0.14f * totalVolAdjust * seVolAdjust);
	}
	else {
		swordSE->SetVolume(0.14f * totalVolAdjust * seVolAdjust);
		swordSE->Play(false);
	}

}

void KdAudioManager::PlaySwordHitSE()
{
	if (!swordHitSE) {
		swordHitSE = KdAudioManager::Instance().Play("Asset/Sounds/sword_hit.wav", false);
		swordHitSE->SetVolume(0.1f * totalVolAdjust * seVolAdjust);
	}
	else {
		swordHitSE->SetVolume(0.1f * totalVolAdjust * seVolAdjust);
		swordHitSE->Play(false);
	}
}

void KdAudioManager::PlayGetHitSE()
{
	if (!getHitSE) {
		getHitSE = KdAudioManager::Instance().Play("Asset/Sounds/damaged.wav", false); //damage
		getHitSE->SetVolume(0.1f* totalVolAdjust * seVolAdjust);
	}
	else {
		getHitSE->SetVolume(0.1f * totalVolAdjust * seVolAdjust);
		getHitSE->Play(false);
	}
}

void KdAudioManager::PlayNextLevelSE()
{
	if (!nextLevelSE) {
		nextLevelSE = KdAudioManager::Instance().Play("Asset/Sounds/stair.wav", false);
		nextLevelSE->SetVolume(0.49f * totalVolAdjust * seVolAdjust);
	}
	else {
		nextLevelSE->SetVolume(0.49f * totalVolAdjust * seVolAdjust);
		nextLevelSE->Play(false);
	}
}

void KdAudioManager::PlayPotionSE()
{
	if (!potionSE) {
		potionSE = KdAudioManager::Instance().Play("Asset/Sounds/hpPotion.wav", false);
		potionSE->SetVolume(0.5f * totalVolAdjust * seVolAdjust);
	}
	else {
		potionSE->SetVolume(0.5f * totalVolAdjust * seVolAdjust);
		potionSE->Play(false);
	}


}

void KdAudioManager::PlayStairSE()
{

	if (!stairSE) {
		stairSE = KdAudioManager::Instance().Play("Asset/Sounds/stair.wav", false);
		stairSE->SetVolume(0.79f * totalVolAdjust * seVolAdjust);
	}
	else {
		stairSE->SetVolume(0.79f * totalVolAdjust * seVolAdjust);
		stairSE->Play(false);
	}

}

void KdAudioManager::PlayMagicAtkSE()
{

	if (!magicAtkSE) {
		magicAtkSE = KdAudioManager::Instance().Play("Asset/Sounds/stair.wav", false);
		magicAtkSE->SetVolume(0.779f * totalVolAdjust * seVolAdjust);
	}
	else {
		magicAtkSE->SetVolume(0.779f * totalVolAdjust * seVolAdjust);
		magicAtkSE->Play(false);
	}

}

void KdAudioManager::PlayRelifeSE()
{

	if (!relifeSE) {
		relifeSE = KdAudioManager::Instance().Play("Asset/Sounds/relife.wav", false);
		relifeSE->SetVolume(0.779f * totalVolAdjust * seVolAdjust);
	}
	else {
		relifeSE->SetVolume(0.779f * totalVolAdjust * seVolAdjust);
		relifeSE->Play(false);
	}

}

void KdAudioManager::PlayTextSE()
{
	if (!textSE) {
		textSE = KdAudioManager::Instance().Play("Asset/Sounds/Retro_Single_v3_wav.wav", false);
		textSE->SetVolume(0.779f * totalVolAdjust * seVolAdjust);
	}
	else {
		textSE->SetVolume(0.779f * totalVolAdjust * seVolAdjust);
		textSE->Play(false);
	}
}

void KdAudioManager::PlayBarrelSE()
{
	if (!barrelSE) {
		barrelSE = KdAudioManager::Instance().Play("Asset/Sounds/wood_box.wav", false);
		barrelSE->SetVolume(0.079f * totalVolAdjust * totalVolAdjust * seVolAdjust);
	}
	else {
		barrelSE->SetVolume(0.079f * totalVolAdjust * totalVolAdjust * seVolAdjust);
		barrelSE->Play(false);
	}

}

void KdAudioManager::PlayChestSE()
{
	if (!chestSE) {
		chestSE = KdAudioManager::Instance().Play("Asset/Sounds/01_chest_open_1.wav", false);
		chestSE->SetVolume(0.779f * totalVolAdjust * seVolAdjust);
	}
	else {
		chestSE->SetVolume(0.179f * totalVolAdjust * seVolAdjust);
		chestSE->Play(false);
	}
}

void KdAudioManager::PlayChestShineSE()
{
	if (!chestShineSE) {
		chestShineSE = KdAudioManager::Instance().Play("Asset/Sounds/relife.wav", false);
		chestShineSE->SetVolume(0.179f * totalVolAdjust * seVolAdjust);
	}
	else {
		chestShineSE->SetVolume(0.179f * totalVolAdjust * seVolAdjust);
		chestShineSE->Play(false);
	}
}

void KdAudioManager::PlayGameStartSE()
{
	if (!gameStartSE) {
		gameStartSE = KdAudioManager::Instance().Play("Asset/Sounds/020_Confirm_10.wav", false);
		gameStartSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
	}
	else {
		gameStartSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
		gameStartSE->Play(false);
	}

}

void KdAudioManager::PlayDoorSE()
{
	if (!doorSE) {
		doorSE = KdAudioManager::Instance().Play("Asset/Sounds/05_door_open_1.wav", false);
		doorSE->SetVolume(1.49f * totalVolAdjust * seVolAdjust);
	}
	else {
		doorSE->SetVolume(1.49f * totalVolAdjust * seVolAdjust);
		doorSE->Play(false);
	}
}

void KdAudioManager::PlayDashSE()
{
	if (!dashSE) {
		dashSE = KdAudioManager::Instance().Play("Asset/Sounds/15_human_dash_2.wav", false);
		dashSE->SetVolume(0.56f * totalVolAdjust * seVolAdjust);
	}
	else {
		dashSE->SetVolume(0.56f * totalVolAdjust * seVolAdjust);
		dashSE->Play(false);
	}
}

void KdAudioManager::PlayUiHoverSE()
{
	if (!uiHoverSE) {
		uiHoverSE = KdAudioManager::Instance().Play("Asset/Sounds/004_Hover_04.wav", false);
		uiHoverSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
	}
	else {
		uiHoverSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
		uiHoverSE->Play(false);
	}
}

void KdAudioManager::PlayUiDenieSE()
{
	if (!uiDenieSE) {
		uiDenieSE = KdAudioManager::Instance().Play("Asset/Sounds/033_Denied_03.wav", false);
		uiDenieSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
	}
	else {
		uiDenieSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
		uiDenieSE->Play(false);
	}
}

void KdAudioManager::PlayEquipItemSE()
{
	if (!equipItemSE) {
		equipItemSE = KdAudioManager::Instance().Play("Asset/Sounds/054_use_item_04.wav", false);
		equipItemSE->SetVolume(0.779f * totalVolAdjust * seVolAdjust);
	}
	else {
		equipItemSE->SetVolume(0.779f * totalVolAdjust * seVolAdjust);
		equipItemSE->Play(false);
	}
}

void KdAudioManager::PlayUseItemSE()
{
	if (!useItemSE) {
		useItemSE = KdAudioManager::Instance().Play("Asset/Sounds/hpPotion.wav", false);
		useItemSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
	}
	else {
		useItemSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
		useItemSE->Play(false);
	}
}

void KdAudioManager::PlayPickUpSE()
{
	if (!pickUpSE) {
		pickUpSE = KdAudioManager::Instance().Play("Asset/Sounds/hpPotion.wav", false);
		pickUpSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
	}
	else {
		pickUpSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
		pickUpSE->Play(false);
	}
}

void KdAudioManager::PlayEnemyGiveUpSE()
{
	if (!enemyGiveUpSE) {
		enemyGiveUpSE = KdAudioManager::Instance().Play("Asset/Sounds/Goblin_00.wav", false);
		enemyGiveUpSE->SetVolume(0.179f * totalVolAdjust * seVolAdjust);
	}
	else {
		enemyGiveUpSE->SetVolume(0.179f * totalVolAdjust * seVolAdjust);
		enemyGiveUpSE->Play(false);
	}
	
}

void KdAudioManager::PlayAlertSE()
{
	if (!warnSE) {
		warnSE = KdAudioManager::Instance().Play("Asset/Sounds/warn.wav", false);
		warnSE->SetVolume(0.179f * totalVolAdjust * seVolAdjust);
	}
	else {
		warnSE->SetVolume(0.179f * totalVolAdjust * seVolAdjust);
		warnSE->Play(false);
	}
}

void KdAudioManager::PlayTrapSE()
{
	if (!trapSE) {
		trapSE = KdAudioManager::Instance().Play("Asset/Sounds/Trap_00.wav", false);
		trapSE->SetVolume(0.179f * totalVolAdjust * seVolAdjust);
	}
	else {
		trapSE->SetVolume(0.179f * totalVolAdjust * seVolAdjust);
		trapSE->Play(false);
	}
}

void KdAudioManager::PlayFootStepSE()
{

	if (!footStepSE && soundCd < 0) {
		footStepSE = KdAudioManager::Instance().Play("Asset/Sounds/Footstep_Dirt_01.wav", false);
		footStepSE->SetVolume(0.28f * totalVolAdjust * seVolAdjust);
		soundCd = soundCdMax;
	}
	else if(soundCd < 0){
		footStepSE->SetVolume(0.28f * totalVolAdjust * seVolAdjust);
		footStepSE->Play(false);
		soundCd = soundCdMax;
	}
}

void KdAudioManager::StopFootStepSE()
{
	if (footStepSE) {
		footStepSE->Stop();
	}
}

void KdAudioManager::PlayWpSpinSE()
{
	if (!wpSpinSE && spinSoundCd < 0) {
		wpSpinSE = KdAudioManager::Instance().Play("Asset/Sounds/wpSpin.wav", false);
		wpSpinSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
		spinSoundCd = spinSoundCdMax;
	}
	else if (spinSoundCd < 0) {
		wpSpinSE->SetVolume(0.28f * totalVolAdjust * seVolAdjust);
		wpSpinSE->Play(false);
		spinSoundCd = spinSoundCdMax;
	}
}

void KdAudioManager::StopWpSpinSE()
{
	if (wpSpinSE) {
		wpSpinSE->Stop();
	}
}

void KdAudioManager::PlayOpenInvenSE()
{
	if (!openInvenSE) {
		openInvenSE = KdAudioManager::Instance().Play("Asset/Sounds/Inventory_Open_01.wav", false);
		openInvenSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
	}
	else {
		openInvenSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
		openInvenSE->Play(false);
	}
}

void KdAudioManager::PlayEncounterSE()
{
	if (!encounterSE) {
		encounterSE = KdAudioManager::Instance().Play("Asset/Sounds/Boss_Encounter_2.wav", false);
		encounterSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
	}
	else {
		encounterSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
		encounterSE->Play(false);
	}
}

void KdAudioManager::PlayDizzySE()
{
	if (!dizzySE) {
		dizzySE = KdAudioManager::Instance().Play("Asset/Sounds/mewomawasu.wav", false);
		dizzySE->SetVolume(0.28f * totalVolAdjust * seVolAdjust);
	}
	else {
		dizzySE->SetVolume(0.28f * totalVolAdjust * seVolAdjust);
		dizzySE->Play(false);
	}
}

void KdAudioManager::PlayVictorySE()
{
	if (!victorySE) {
		victorySE = KdAudioManager::Instance().Play("Asset/Sounds/Jingle_Achievement_00.wav", false);
		victorySE->SetVolume(0.279f * totalVolAdjust * seVolAdjust);
	}
	else {
		victorySE->SetVolume(0.279f * totalVolAdjust * seVolAdjust);
		victorySE->Play(false);
	}
}

void KdAudioManager::PlayMonsterVoiceSE()
{
	if (!monsterVoiceSE) {
		monsterVoiceSE = KdAudioManager::Instance().Play("Asset/Sounds/Goblin_03.wav", false);
		monsterVoiceSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
	}
	else {
		monsterVoiceSE->SetVolume(0.479f * totalVolAdjust * seVolAdjust);
		monsterVoiceSE->Play(false);
	}
}

void KdAudioManager::PlayFinalHitSE()
{
	if (!finalHitSE) {
		finalHitSE = KdAudioManager::Instance().Play("Asset/Sounds/Boss_Final_Blow.wav", false);
		finalHitSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
	}
	else {
		finalHitSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
		finalHitSE->Play(false);
	}
}

void KdAudioManager::PlayBossSlashWaveSE()
{
	if (!bossSlashWaveSE) {
		bossSlashWaveSE = KdAudioManager::Instance().Play("Asset/Sounds/EVFX02_14_VacuumBlade.wav", false);
		bossSlashWaveSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
	}
	else {
		bossSlashWaveSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
		bossSlashWaveSE->Play(false);
	}
}

void KdAudioManager::PlayPlayerSlashWaveSE()
{

	if (!playerSlashWaveSE) {
		playerSlashWaveSE = KdAudioManager::Instance().Play("Asset/Sounds/EVFX02_14_VacuumBlade.wav", false);
		playerSlashWaveSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
	}
	else {
		playerSlashWaveSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
		playerSlashWaveSE->Play(false);
	}
		

}

void KdAudioManager::PlayPlayerSpinSE()
{
	if (!playerSpinSE) {
		playerSpinSE = KdAudioManager::Instance().Play("Asset/Sounds/EVFX02_14_VacuumBlade.wav", false);
		playerSpinSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
	}
	else {
		playerSpinSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
		playerSpinSE->Play(false);
	}

}

void KdAudioManager::PlayMageAtkSE()
{
	if (!mageAtkSE) {
		mageAtkSE = KdAudioManager::Instance().Play("Asset/Sounds/Spell_00.wav", false);
		mageAtkSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
	}
	else {
		mageAtkSE->SetVolume(0.28 * totalVolAdjust * seVolAdjust);
		mageAtkSE->Play(false);
	}
}

void KdAudioManager::PlayMagicExplosionSE()
{
	if (!magicExplosionSE) {
		magicExplosionSE = KdAudioManager::Instance().Play("Asset/Sounds/04_Fire_explosion_04_medium.wav", false);
		magicExplosionSE->SetVolume(0.42 * totalVolAdjust * seVolAdjust);
	}
	else {
		magicExplosionSE->SetVolume(0.42 * totalVolAdjust * seVolAdjust);
		magicExplosionSE->Play(false);
	}
}

void KdAudioManager::PlayBossSmashSE()
{
	if (!bossSmashSE) {
		bossSmashSE = KdAudioManager::Instance().Play("Asset/Sounds/bossSmash.wav", false);
		bossSmashSE->SetVolume(0.42 * totalVolAdjust * seVolAdjust);
	}
	else {
		bossSmashSE->SetVolume(0.42 * totalVolAdjust * seVolAdjust);
		bossSmashSE->Play(false);
	}

}

void KdAudioManager::PlayGameClearSE()
{
	if (!gameClearSE) {
		gameClearSE = KdAudioManager::Instance().Play("Asset/Sounds/Jingle_Win_00.wav", false);
		gameClearSE->SetVolume(0.42 * totalVolAdjust * seVolAdjust);
	}
	else {
		gameClearSE->SetVolume(0.42 * totalVolAdjust * seVolAdjust);
		gameClearSE->Play(false);
	}
}

void KdAudioManager::ControlSound()
{
	soundCd--;
	spinSoundCd--;
}

void KdAudioManager::CallImgui()
{
	//imgui begin audiomanager:
	ImGui::Begin("AudioManager");	
	//slider float totalVolAdjust
	ImGui::SliderFloat("totalVolAdjust", &totalVolAdjust, 0.0f, 4.0f);
	//se vol adjust slide float
	ImGui::SliderFloat("seVolAdjust", &seVolAdjust, 0.0f, 4.0f);
	//bgm vol adjust slide float
	ImGui::SliderFloat("bgmVolAdjust", &bgmVolAdjust, 0.0f, 4.0f);

	//use imgui to gameBgm->SetVolume
	
	//if button is clicked, set gameBgm->SetVolume
	if (ImGui::Button("SetVolume")) {
		gameBgm->SetVolume(0.21f * totalVolAdjust * bgmVolAdjust);
	}

	//end imgui
	ImGui::End();


}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// サウンドアセットの取得
// ・
// ・サウンドアセットの解放
// ・エンジンの解放
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
std::shared_ptr<KdSoundEffect> KdAudioManager::GetSound(std::string_view fileName)
{
	// filenameのサウンドアセットがロード済みか？
	auto itFound = m_soundMap.find(fileName.data());

	// ロード済み
	if (itFound != m_soundMap.end())
	{
		return (*itFound).second;
	}
	else
	{
		// 生成 & 読み込み
		auto newSound = std::make_shared<KdSoundEffect>();
		if (!newSound->Load(fileName, m_audioEng))
		{
			// 読み込み失敗時は、nullを返す
			return nullptr;
		}
		// リスト(map)に登録
		m_soundMap.emplace(fileName, newSound);

		// リソースを返す
		return newSound;
	}
}


// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
// 
// KdSoundEffect
// 
// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####

// 音データの読み込み
bool KdSoundEffect::Load(std::string_view fileName, const std::unique_ptr<DirectX::AudioEngine>& engine)
{
	if (engine.get() != nullptr)
	{
		try
		{
			// wstringに変換
			std::wstring wFilename = sjis_to_wide(fileName.data());

			// 読み込み
			m_soundEffect = std::make_unique<DirectX::SoundEffect>(engine.get(), wFilename.c_str());
		}
		catch (...)
		{
			assert(0 && "Sound File Load Error");

			return false;
		}
	}

	return true;
}


// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
// 
// KdSoundInstance
// 
// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####

KdSoundInstance::KdSoundInstance(const std::shared_ptr<KdSoundEffect>& soundEffect)
	:m_soundData(soundEffect){}

bool KdSoundInstance::CreateInstance()
{
	if (!m_soundData) { return false; }

	DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default;

	m_instance = (m_soundData->CreateInstance(flags));

	return true;
}

void KdSoundInstance::Play(bool loop)
{
	if (!m_instance) { return; }

	// 再生状態がどんな状況か分からないので一度停止してから
	Stop();

	m_instance->Play(loop);
}

void KdSoundInstance::SetVolume(float vol)
{
	if (m_instance == nullptr) { return; }

	m_instance->SetVolume(vol);
}

void KdSoundInstance::SetPitch(float pitch)
{
	if (m_instance == nullptr) { return; }

	m_instance->SetPitch(pitch);
}

bool KdSoundInstance::IsPlaying()
{
	if (!m_instance) { return false; }

	return (m_instance->GetState() == DirectX::SoundState::PLAYING);
}

bool KdSoundInstance::IsPause()
{
	if (!m_instance) { return false; }

	return (m_instance->GetState() == DirectX::SoundState::PAUSED);
}

bool KdSoundInstance::IsStopped()
{
	if (!m_instance) { return false; }

	return (m_instance->GetState() == DirectX::SoundState::STOPPED);
}



// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 
// KdSoundInstance3D
// 
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////

KdSoundInstance3D::KdSoundInstance3D(const std::shared_ptr<KdSoundEffect>& soundEffect, const DirectX::AudioListener& ownerListener)
	:KdSoundInstance(soundEffect), m_ownerListener(ownerListener){}

bool KdSoundInstance3D::CreateInstance()
{
	if (!m_soundData) { return false; }

	DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default |
		DirectX::SoundEffectInstance_Use3D | DirectX::SoundEffectInstance_ReverbUseFilters;

	m_instance = (m_soundData->CreateInstance(flags));

	return true;
}

void KdSoundInstance3D::Play(bool loop)
{
	if (!m_instance) { return; }

	KdSoundInstance::Play(loop);
}

void KdSoundInstance3D::SetPos(const Math::Vector3& rPos)
{
	if (!m_instance) { return; }

	m_emitter.SetPosition(rPos);

	m_instance->Apply3D(m_ownerListener, m_emitter, false);
}

void KdSoundInstance3D::SetCurveDistanceScaler(float val)
{
	if (!m_instance) { return; }

	m_emitter.CurveDistanceScaler = val;

	m_instance->Apply3D(m_ownerListener, m_emitter, false);
}
