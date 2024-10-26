#pragma once

enum GameTag
{
	NoTag = 0,
	PlayerTag,
	EnemyTag,
	MapTag,
	StageTag,
	ItemTag,
	PlayerWeaponTag,
	PlayerArmorTag,
	PlayerShieldTag,
	PlayerProjectileTag,
	EnemyWeaponTag,
	EnemyProjectileTag,
	DropTag,
};



//プレイヤー
enum PlayerType
{
	NoPlayer = 0,
	WarriorPlayer = 1,
	MagePlayer = 2,
	HealerPlayer = 3,
};

enum PlayerState
{
	pNoState,

	pIdle,
	pMove,
	pDash,
	pAttack,
	pDefense,

	pBlowAway,

	pSkill,
	pStrikeKill,
	pStun,
};

const std::map<PlayerState, const char*> PlayerStateNames = {
	{pNoState, "NoState"},

	{pIdle, "Idle"},
	{pMove, "Move"},
	{pDash, "Dash"},
	{pAttack, "Attack"},
	{pDefense, "Defense"},

	{pBlowAway, "BlowAway"},

	{pSkill, "Skill"},
	{pStrikeKill, "StrikeKill"},
	{pStun, "Stun"},
};

enum PlayerSkillType
{
	SpinSlash,
	FlySlash,
	MagicAtk,
	HealMagic,
};

enum pProjectileType
{
	NoPProjectileType,
	FlySlashType,
	MagicBallType,
	FireBallType,
	pRangeCircleType,
};



//敵
enum EnemyType
{
	NoEnemy = 0,
	eWanderer = 1,
	eFighter = 2,
	eMage = 3,
	eHealer = 4,
	eArcher = 5,
	eBoss = 6,
};

const std::map<EnemyType, const char*> EnemyTypeNames = {
	{NoEnemy, "No Enemy"},
	{eWanderer, "Wanderer"},
	{eFighter, "Fighter"},
	{eMage, "Mage"},
	{eHealer, "Healer"},
	{eArcher, "Archer"},
	{eBoss, "Boss"}
};

enum EnemyState
{
	eNoState = 0,

	eIdle = 1,
	eChase = 2,
	eAttack = 3,

	eRandomMove,
	eMagicHeal,
	eMageAtk,
	eMageWalk,
	eMageRangeAtk,
	eRevive,
	ePatrol,
	eFollow,
	eWanderDead,

	eBossJumpAtk,
	eBossSummon,
	eBossSlashWaveAtk,

	eStun,
	eDizzy,
	eBackJump,
	eBlowAway,
	eSpawn = 98,
	eDeath = 99,
	eStrikeKill,
};

const std::map<EnemyState, const char*> EnemyStateNames = {
	{eNoState, "No State"},

	{eIdle, "Idle"},
	{eChase, "Chase"},
	{eAttack, "Attack"},

	{eRandomMove, "Random Move"},
	{eMagicHeal, "Heal Magic"},
	{eMageAtk, "Attack Magic"},
	{eMageWalk, "Walk"},
	{eMageRangeAtk, "Range Attack"},
	{eRevive, "Revive"},

	{ePatrol, "Patrol"},
	{eFollow, "Patrol Follow"},
	{eWanderDead, "Wander Dead"},

	{eBossJumpAtk, "Boss Jump Attack"},
	{eBossSummon, "Boss Summon"},
	{eBossSlashWaveAtk, "Boss Slash Wave"},

	{eStun, "Stun"},
	{eDizzy, "Dizzy"},
	{eBackJump, "Back Jump"},
	{eBlowAway, "Blow Away"},
	{eSpawn, "Spawn"},
	{eDeath, "Death"},
	{eStrikeKill, "Strike Kill"},
};

enum eProjectileType
{
	NoProjectileType,
	ArrowType,
	MagicType,
	MagicRangeType,
	SlashWaveType,
};


//アイテム
enum ItemRarity
{
	Normal = 0,
	Rare = 1,
	Legendary = 2,
};

enum InventoryViewMode {
	FullInventory = 0,
	PotionInventory,
	WeaponInventory,
	ArmorInventory
};

enum ItemType
{
	NoItemType = 0,
	WeaponItem = 1,
	PotionItem = 2,
	ArmorItem = 3,
};

enum PotionType
{
	NoPotionType = 0,
	HpPotionType = 1,
	MpPoitionType = 2,
	AtkPotionType = 3,
};





//マップ
enum MapType
{
	NoMapType,
	WallType,
	CorridorType,
	GoalType,
	LandType,
	ExpCrystalType,
	DoorType,
	ChestType,
	MimicType,
	BottleType,
	TrapType,
	NPCType,
	EnemyPointType,
};

struct PathNode {
	int x, y;	// チップの座標
	int gCost;	// 開始チップから現在チップまでのの距離
	int hCost;	// 現在チップから終点チップまでの距離
	int fCost;  // GコストとHコストの合計 (gCost + hCost)

	PathNode(int _x, int _y) : x(_x), y(_y), gCost(0), hCost(0), fCost(0) {}
};

enum MojiType
{
	DmgMoji,
	MessageMoji,
};

enum KeyFlg
{
	LButton = 0x01,
	RButton = 0x02,
	ShiftKey = 0x04,
	CtrlKey = 0x08,
	EnterKey = 0x10,
	SpaceKey = 0x20,
	TabKey = 0x40,
	MoveKey = 0x80,

	QKey = 0x100,
	EKey = 0x200,
	RKey = 0x400,
	TKey = 0x800,
	YKey = 0x1000,
	UKey = 0x2000,
	IKey = 0x4000,
	OKey = 0x8000,
	PKey = 0x10000,

	FKey = 0x20000,
	GKey = 0x40000,
	HKey = 0x80000,
	JKey = 0x100000,
	KKey = 0x200000,
	LKey = 0x400000,

	ZKey = 0x800000,
	XKey = 0x1000000,
	CKey = 0x2000000,
	VKey = 0x4000000,
	BKey = 0x8000000,
	NKey = 0x10000000,
	MKey = 0x20000000,

	UpKey = 1111,
	DownKey = 2222,
	AKey = 0x40000000,
	DKey = 0x80000000,


};