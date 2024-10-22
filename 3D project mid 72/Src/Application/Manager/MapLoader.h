﻿#pragma once

class MapObj;
class Enemy;
class Player;
class Item;

using namespace std;
using namespace DirectX::SimpleMath;

enum RoomType {
	SpawnRoom,
	GoalRoom,
	MobEnemyRoom,
	MobEnemyRoom2,
	ChestRoom,
	BarrelRoom,
	TrapRoom,
	NormalRoom  
};

struct BSPNode;
struct Node;
struct Room;

class MapLoader : public KdGameObject
{
public:

	vector<std::vector<Vector3>>& GetMapPos() { return mapPos; }
	vector<std::vector<int>>& GetMapData() { return mapData; }
	vector<std::vector<bool>>& GetIsWalkable() { return isWalkable; }

	



	float Rnd(float _min, float _max) { return _min + (rand() / (float)RAND_MAX) * (_max - _min); } 

	void Init() override;
	void Update() override;

	void DrawDebug() override;
	void DrawUnLit() override;

	void DrawSprite() override;

	void DrawMoji() override;
	float testY = 0;
	float rectX = 1280;
	float rectY = 720;

	void LoadBattleStage();

	void LoadMap();
	void PlaceMapObj();

	void SetPathManagerData();

	void GenerateBSP(BSPNode* node, int depth);

	void CreateRoomsAndCorridors(BSPNode* node);

	pair<int, int> getCenter(BSPNode* node);

	void connectCenters(const pair<int, int>& center1, const pair<int, int>& center2);

	Node* AStarFindPath(const pair<int, int>& start, const pair<int, int>& goal);

	void EnsureWorkingDoor();
	void EnsureWallNearDoor(int mapW, int mapH);


	void AssignRoomType();


	void ClearOldMap();
	void InitMapData();
	void CreateBound();


	void AddDebugSphereinRoom();

	void PlaceBarrelInRoom(Room& room);

	void PlaceChestInRoom(Room& room);

	void PlaceSpikeInRoom(Room& room);

	void PlaceEnemyInRoom(Room& room);

	void PlaceNPCInRoom(float _posX, float _posY);

	void CounterDungeonInfo();
	int numOfLand = 0;
	int numOfWall = 0;
	float wallRatio = 0;
	int numOfDoor = 0;

	void CheckBarrelOverlapWithWall();



	void CallImgui() override;

	//setplayer
	void SetPlayer(const shared_ptr<Player>& player) { wpPlayer = player; }

	weak_ptr<Player> wpPlayer;

	bool isLoadMap = false;
	bool isInit = false;


	int mapHeight = 15;  //15-25
	int mapWidth = 15;   //15-25
	float mapChipSpace = 6.0f; //12

	shared_ptr<KdTexture> miniMapTex[15][15];
	Math::Rectangle miniMapRect = { 0,0,8,8 };
	Math::Rectangle playerminiMapRect = { 0,0,8,8 };





	vector<std::vector<Vector3>> mapPos;
	vector<std::vector<int>> mapData;
	vector<std::vector<int>> doorRotation;
	vector<std::vector<bool>> isWalkable;

	//BSP
	int minWidthOfRoom = 3;
	int minHeightOfRoom = 3;

	int chanceToGenerateRoom = 100;

	vector<Room> rooms;

	int roomCounter = 0;

	Vector3 spawnRoomPos;
	Vector3 goalRoomPos;
	Vector3 mobEnemyRoomPos;
	Vector3 mobEnemyRoom2Pos;
	Vector3 chestRoomPos;

	Vector3 mobPt1, mobPt2, mobPt3, mob2Pt1, mob2Pt2, mob2Pt3;

	weak_ptr<KdCamera> m_cam;

	void SetKdCamera(std::shared_ptr<KdCamera> camera)
	{
		m_cam = camera;
	}

	float scrollX = 0;
	float scrollY = 0;

	float playerMiniPosX = 0;
	float playerMiniPosY = 0;

	shared_ptr<KdTexture> playerMiniTex;

	float scaleMini = 2;


private:


private:
	MapLoader() {}
	~MapLoader() override {}

public:
	static MapLoader& Instance()
	{
		static MapLoader instance;
		return instance;
	}
};

struct BSPNode {
	int x, y;          
	int width, height; 
	BSPNode* left;     
	BSPNode* right;    
	BSPNode* parent;  

	BSPNode(int _x, int _y, int _w, int _h, BSPNode* _parent = nullptr)
		: x(_x), y(_y), width(_w), height(_h), left(nullptr), right(nullptr), parent(_parent) {}

};

struct Node {
	int x, y; 
	int g, h;  
	Node* parent;  
	Node(int x, int y) : x(x), y(y), g(0), h(0), parent(nullptr) {}
};

struct Room {
	int x, y;     
	int width, height; 
	float centerX, centerY;
	int lastX, lastY;    
	RoomType type = NormalRoom;       

	bool isFirstRoom; 
	bool isLastRoom;  
	int orderOfRoom;  

	Room(int _x, int _y, int _w, int _h, int _centerX, int _centerY, bool _isFirstRoom, RoomType _type, bool _isLastRoom, int _orderOfRoom)
		: x(_x), y(_y), width(_w), height(_h), centerX(_centerX), centerY(_centerY), lastX(_x + _w - 1), lastY(_y + _h - 1),
		isFirstRoom(_isFirstRoom), type(_type), isLastRoom(_isLastRoom), orderOfRoom(_orderOfRoom) {}
};