﻿#pragma once

using namespace std;

class KdSoundEffect;
class KdSoundInstance;
class KdSoundInstance3D;

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ゲーム内の音を全て管理するクラス
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// DirectXAudioEngineの管理
// 2Dと3Dサウンドの簡易再生関数
// 再生中のサウンドインスタンスの管理（全停止・一時停止などが可能）
// サウンドアセットの管理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////

enum KdSoundType
{
	SwordSE,
	SwordHitSE,
	GetHitSE,
	NextLevelSE,
	PotionSE,
	StairSE,
	MagicAtkSE,
	RelifeSE,
	TextSE,
	BarrelSE,
	ChestSE,
	ChestShineSE,
	GameStartSE,
	DoorSE,
	DashSE,
	UiHoverSE,
	UiDenieSE,
	EquipItemSE,
	UseItemSE,
	PickUpSE,
	EnemyGiveUpSE,
	WarnSE,
	TrapSE,

	FootStepSE,
	WpSpinSE,
	OpenInvenSE,
	EncounterSE,
	DizzySE,
	VictorySE,
	MonsterVoiceSE,
	FinalHitSE,

	BossSlashWaveSE,
	PlayerSlashWaveSE,
	PlayerSpinSE,
	MageAtkSE,
	MagicExplosionSE,
};

struct KdSoundData
{
	std::string fileName;
	float volumeAdjust;
	bool loop;
};

class KdAudioManager
{
public:

	// 初期化
	void Init();

	// 不要なインスタンス削除など
	void Update();

	void SetListnerMatrix(const Math::Matrix& mWorld);

	// サウンド再生
	std::shared_ptr<KdSoundInstance>  Play(std::string_view rName, bool loop = false);
	std::shared_ptr<KdSoundInstance3D> Play3D(std::string_view rName, const Math::Vector3& rPos, bool loop = false);

	std::shared_ptr<KdSoundInstance> Play(KdSoundType soundType);


	void AddPlayList(const std::shared_ptr<KdSoundInstance>& rSound)
	{
		if (!rSound.get()) { return; }

		m_playList[(size_t)(rSound.get())] = rSound;
	}

	// 再生中の音をすべて停止する
	void StopAllSound();
	// 再生中の音をすべて一時停止する
	void PauseAllSound();
	// 再生中の音をすべて停止する
	void ResumeAllSound();

	// 再生中の音をすべて停止・サウンドアセットの解放を行う
	void SoundReset();

	// DirectXAudioEngine取得
	const std::unique_ptr<DirectX::AudioEngine>& GetAudioEngine() { return m_audioEng; }

	const DirectX::AudioListener& GetListener() { return m_listener; }

	// サウンドアセットの一括読込
	void LoadSoundAssets(std::initializer_list<std::string_view>& fileName);

	// 解放
	void Release();

	void PlayBGM();
	void PlayBattleBGM();
	void PlaySwordSE();
	void PlaySwordHitSE();
	void PlayGetHitSE();
	void PlayNextLevelSE();
	void PlayPotionSE();
	void PlayStairSE();
	void PlayMagicAtkSE();
	void PlayRelifeSE();

	void PlayTextSE();

	void PlayBarrelSE();
	void PlayChestSE();
	void PlayChestShineSE();
	void PlayGameStartSE();
	void PlayDoorSE();
	void PlayDashSE();

	void PlayUiHoverSE();
	void PlayUiDenieSE();
	void PlayEquipItemSE();
	void PlayUseItemSE();
	void PlayPickUpSE();

	void PlayEnemyGiveUpSE();
	void PlayAlertSE();
	void PlayTrapSE();
	void PlayFootStepSE();
	void StopFootStepSE();
	void PlayWpSpinSE();
	void StopWpSpinSE();
	void PlayOpenInvenSE();
	void PlayEncounterSE();
	void PlayDizzySE();
	void PlayVictorySE();
	void PlayMonsterVoiceSE();
	void PlayFinalHitSE();

	void PlayBossSlashWaveSE();
	void PlayPlayerSlashWaveSE();
	void PlayPlayerSpinSE();
	void PlayMageAtkSE();
	void PlayMagicExplosionSE();

	void PlayBossSmashSE();

	void PlayGameClearSE();

	void ControlSound();

	void CallImgui();

	shared_ptr<KdSoundInstance> swordSE;
	shared_ptr<KdSoundInstance> swordHitSE;
	shared_ptr<KdSoundInstance> getHitSE;
	shared_ptr<KdSoundInstance> nextLevelSE;
	shared_ptr<KdSoundInstance> gameBgm;
	shared_ptr<KdSoundInstance> battleBgm;
	shared_ptr<KdSoundInstance> potionSE;
	shared_ptr<KdSoundInstance> stairSE;
	shared_ptr<KdSoundInstance> magicAtkSE;
	shared_ptr<KdSoundInstance> relifeSE;


	shared_ptr<KdSoundInstance> textSE;

	shared_ptr<KdSoundInstance> barrelSE;
	shared_ptr<KdSoundInstance> chestSE;
	shared_ptr<KdSoundInstance> chestShineSE;
	shared_ptr<KdSoundInstance> gameStartSE;
	//door
	shared_ptr<KdSoundInstance> doorSE;
	//dash
	shared_ptr<KdSoundInstance> dashSE;

	//uiHover
	shared_ptr<KdSoundInstance> uiHoverSE;
	//uiDenie
	shared_ptr<KdSoundInstance> uiDenieSE;
	//equip item
	shared_ptr<KdSoundInstance> equipItemSE;
	
	//use item 
	shared_ptr<KdSoundInstance> useItemSE;
	//pickUp se 
	shared_ptr<KdSoundInstance> pickUpSE;

	//warn
	shared_ptr<KdSoundInstance> warnSE;

	//enemyGiveUp
	shared_ptr<KdSoundInstance> enemyGiveUpSE;

	//trap
	shared_ptr<KdSoundInstance> trapSE;
	//footstep
	shared_ptr<KdSoundInstance> footStepSE;
	//wpSpin
	shared_ptr<KdSoundInstance> wpSpinSE;

	//openInven
	shared_ptr<KdSoundInstance> openInvenSE;

	//encounter 
	shared_ptr<KdSoundInstance> encounterSE;
	//dizzy
	shared_ptr<KdSoundInstance> dizzySE;
	//victory
	shared_ptr<KdSoundInstance> victorySE;
	//monsterVoice
	shared_ptr<KdSoundInstance> monsterVoiceSE;

	//finalHit
	shared_ptr<KdSoundInstance> finalHitSE;

	//bossSlahWave
	shared_ptr<KdSoundInstance> bossSlashWaveSE;
	//playerSlashWave
	shared_ptr<KdSoundInstance> playerSlashWaveSE;
	//playerSpin 
	shared_ptr<KdSoundInstance> playerSpinSE;
	//MageAtk
	shared_ptr<KdSoundInstance> mageAtkSE;
	//magicExplosion
	shared_ptr<KdSoundInstance> magicExplosionSE;

	//bossSmash
	shared_ptr<KdSoundInstance> bossSmashSE;

	//gameClear
	shared_ptr<KdSoundInstance> gameClearSE;

	float soundCd = 0;
	float soundCdMax = 21.9f;

	float spinSoundCd = 0;
	float spinSoundCdMax = 21;

	float totalVolAdjust= 0.5;
	float bgmVolAdjust = 0.5;
	float seVolAdjust = 0.5;

	//sound data can be adjust and edit using a json file


private:

	// サウンドデータの取得orロード
	std::shared_ptr<KdSoundEffect> GetSound(std::string_view fileName);

	// DirectXのAudioEngine本体
	std::unique_ptr<DirectX::AudioEngine>	m_audioEng;

	// 3Dサウンド用リスナー
	DirectX::AudioListener					m_listener;

	// 再生中のサウンドリスト
	std::map<size_t, std::shared_ptr<KdSoundInstance>>	m_playList;

	// サウント管理マップ
	std::unordered_map< std::string, std::shared_ptr<KdSoundEffect>> m_soundMap;

	// シングルトンパターン
public:
	static KdAudioManager& Instance()
	{
		static KdAudioManager instance;
		return instance;
	}

private:
	KdAudioManager() {}
	~KdAudioManager() { Release(); }
};


// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 2Dサウンド再生用のインスタンス
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// 再生や停止、再生状況の確認
// 各種パラメータの変更が可能
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class KdSoundInstance : public std::enable_shared_from_this<KdSoundInstance>
{
public:
	KdSoundInstance(const std::shared_ptr<KdSoundEffect>& soundEffect);

	virtual bool CreateInstance();

	virtual void Play(bool loop = false);
	void Stop() { if (m_instance) { m_instance->Stop(); } }
	void Pause() { if (m_instance) { m_instance->Pause(); } }
	void Resume() { if (m_instance) { m_instance->Resume(); } }

	// ・vol	… ボリューム設定(1.0が100%)
	void SetVolume(float vol);

	// ・pitch	… 振動設定(低音-1.0～1.0高音)
	void SetPitch(float pitch);

	// 再生状態の取得
	bool IsPlaying();
	bool IsPause();
	bool IsStopped();

protected:

	// サウンドの再生インスタンス
	std::unique_ptr<DirectX::SoundEffectInstance>	m_instance;

	// 再生サウンドの元データ
	std::shared_ptr<KdSoundEffect>					m_soundData;

	// コピー禁止用
	KdSoundInstance(const KdSoundInstance& src) = delete;
	void operator=(const KdSoundInstance& src) = delete;
};


// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 3Dサウンド再生用のインスタンス
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// KdSoundInstanceに3D座標情報を追加
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class KdSoundInstance3D : public KdSoundInstance
{
public:
	KdSoundInstance3D(const std::shared_ptr<KdSoundEffect>& soundEffect, const DirectX::AudioListener& ownerListener);

	bool CreateInstance() override;

	void Play(bool loop = false) override;

	void SetPos(const Math::Vector3& rPos);

	// 減衰倍率設定 1:通常 FLT_MIN～FLT_MAX
	void SetCurveDistanceScaler(float val);

protected:

	// エミッター 主に3Dサウンドソースの定義
	DirectX::AudioEmitter			m_emitter;

	// 3Dサウンド用リスナー
	const DirectX::AudioListener&	m_ownerListener;

	// コピー禁止用
	KdSoundInstance3D(const KdSoundInstance3D& src) = delete;
	void operator=(const KdSoundInstance3D& src) = delete;

};


// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// サウンドデータを扱う
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// Wave形式のデータを格納できる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class KdSoundEffect {
public:

	KdSoundEffect() {}

	~KdSoundEffect() {
		m_soundEffect = nullptr;
	}

	std::unique_ptr<DirectX::SoundEffectInstance> CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS flag)
	{
		if (!m_soundEffect){ return nullptr; }

		return m_soundEffect->CreateInstance(flag);
	}

	// WAVEサウンド読み込み
	bool Load(std::string_view fileName, const std::unique_ptr<DirectX::AudioEngine>& engine);

private:
	// サウンドエフェクト
	std::unique_ptr<DirectX::SoundEffect>	m_soundEffect;

	// コピー禁止用:単一のデータはコピーできない
	KdSoundEffect(const KdSoundEffect& src) = delete;
	void operator=(const KdSoundEffect& src) = delete;
};