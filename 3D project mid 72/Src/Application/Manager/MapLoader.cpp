#include"MapLoader.h"
#include"Application/GameObject/MapObj/MapObj.h"
#include"Application/Manager/GameManager.h"
#include"Application/Scene/SceneManager.h"
#include"Application/GameObject/Player/Player.h"
#include"Application/GameObject/Item/Item.h"
#include"Application/Manager/PathManager.h"
#include"Application/Manager/InventoryManager.h"
#include"Application/Manager/UiManager.h"

#include"Application/GameObject/Enemy/Enemy.h"
#include"Application/GameObject/Enemy/WanderEnemy/WanderEnemy.h"

void MapLoader::Init()
{
	

}

void MapLoader::Update()
{

	if (!isLoadMap) {
		isLoadMap = true;
		debugWire = make_unique<KdDebugWireFrame>();


		for (int h = 0; h < mapHeight; h++) {
			for (int w = 0; w < mapWidth; w++) {
				miniMapTex[h][w] = make_shared<KdTexture>();
				miniMapTex[h][w]->Load("Asset/Textures/miniMapChip01.png");
			}
		}

	

		playerMiniTex = make_shared<KdTexture>();
		playerMiniTex->Load("Asset/Textures/miniMapChip01.png");



	}

	LoadMap();

	//debugWire->AddDebugSphere(Vector3(0, -70, 0), 30000.0f, kRedColor);

	
	//debugWire->AddDebugSphere(Vector3(0, -70, 0), 300.0f, kRedColor);

	

}

void MapLoader::DrawDebug()
{
	//AddDebugSphereinRoom();
	//debugWire->AddDebugSphere(Vector3(0,-70,0), 30.0f, kRedColor);

	//AddDebugSphere in all isWalkable == false
	/*for (int h = 0; h < mapHeight; h++) {
		for (int w = 0; w < mapWidth; w++) {
			if (isWalkable[h][w]) {
				debugWire->AddDebugSphere(Vector3(mapChipSpace * w, -70, mapChipSpace * h), 30.0f, kRedColor);
			}
		}
	}*/





}

void MapLoader::DrawUnLit()
{
	//AddDebugSphereinRoom();
	//debugWire->AddDebugSphere(Vector3(0, -70, 0), 30.0f, kRedColor);

}

void MapLoader::DrawSprite()
{

	/*rtc.ChangeRenderTarget(rtp);
	rtp.ClearTexture(Math::Color(0, 0, 0, 0));

	for (int h = 0; h < mapHeight; h++) {
		for (int w = 0; w < mapWidth; w++) {
			Color miniColor;
			if (mapData[h][w] == MapType::WallType) miniColor = kRedColor;
			else miniColor = kWhiteColor;

			Vector2 miniPos;
			miniPos.x = (16 / 6) * mapPos[h][w].x -scrollX;
			miniPos.y = (16 / 6) * mapPos[h][w].z - scrollY;

			Math::Rectangle rect = { 0,0,1280,720 };

		

			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(2) * Matrix::CreateTranslation(miniPos.x, miniPos.y, 0));
			KdShaderManager::Instance().m_spriteShader.DrawTex(miniMapTex[h][w], 0, 0, miniMapRect.width, miniMapRect.height, &miniMapRect, &miniColor, Math::Vector2{ 0, 0.5 });
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

		

		}
	}

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(1) * Matrix::CreateTranslation(playerMiniPosX+scrollX, playerMiniPosY+scrollY, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(playerMiniTex, 0, 0, playerminiMapRect.width, playerminiMapRect.height, &playerminiMapRect, &kBlueColor, Math::Vector2{ 0, 0.5 });
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

	
	InventoryManager::Instance().DrawSprite();

	InventoryManager::Instance().DrawMoji();

	rtc.UndoRenderTarget();
	KdShaderManager::Instance().m_spriteShader.DrawTex(rtp.m_RTTexture, 350-scrollX, 140-scrollY);*/
}

void MapLoader::DrawMoji()
{



	//for (int h = 0; h < mapHeight; h++) {
	//	for (int w = 0; w < mapWidth; w++) {
	//		Color miniColor;
	//		if (mapData[h][w] == MapType::WallType) miniColor = kRedColor;
	//		else miniColor = kWhiteColor;

	//		Vector2 miniPos;
	//		miniPos.x = (16 / 6) * mapPos[h][w].x + 300;
	//		miniPos.y = (16 / 6) * mapPos[h][w].z + 42;

	//		KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(2) * Matrix::CreateTranslation(miniPos.x, miniPos.y, 0));
	//		KdShaderManager::Instance().m_spriteShader.DrawTex(miniMapTex[h][w], 0, 0, miniMapRect.width, miniMapRect.height, &miniMapRect, &miniColor, Math::Vector2{ 0, 0.5 });
	//		KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

	//		rtc.ChangeRenderTarget(rtp);
	//		rtp.ClearTexture(Math::Color(0, 0, 0, 0));

	//	

	//		//KdShaderManager::Instance().m_spriteShader.DrawString(0, testY, "hello, i am tony cheung", kRedColor);


	//		Math::Rectangle rect =  { 0,0,1280,720};
	//		rtc.UndoRenderTarget();
	//		KdShaderManager::Instance().m_spriteShader.DrawTex(rtp.m_RTTexture, (long)rectX, (long)rectY,&rect);
	//		//KdShaderManager::Instance().m_spriteShader.DrawTex(rtp.m_RTTexture, 0, 0);

	//	}
	//}

}



void MapLoader::LoadBattleStage()
{
	


}

void MapLoader::LoadMap()
{


	if (GameManager::Instance().GetCanNextFloor()) {


		switch (GameManager::Instance().GetFloorNum())
		{
		case 0:
			mapWidth = 7;
			mapHeight = 7;
			break;
		case 1:
			mapWidth = 15;
			mapHeight = 15;
			break;


		default:
			mapWidth = 15;
			mapHeight = 15;
			break;
		}

		mapPos.clear();
		mapData.clear();
		doorRotation.clear();
		isWalkable.clear();


		mapPos.resize(mapHeight, std::vector<Vector3>(mapWidth));
		mapData.resize(mapHeight, std::vector<int>(mapWidth));
		doorRotation.resize(mapHeight, std::vector<int>(mapWidth));
		isWalkable.resize(mapHeight, std::vector<bool>(mapWidth));



		rooms.clear();

		ClearOldMap();

		for (int h = 0; h < mapHeight; h++) {
			for (int w = 0; w < mapWidth; w++) {
				mapData[h][w] = WallType;
				mapPos[h][w] = { 0,-70,0 };
				isWalkable[h][w] = false;
				doorRotation[h][w] = -1;
			}
		}

		BSPNode* root = new BSPNode(0, 0, mapWidth, mapHeight);
		GenerateBSP(root, 4); //4
		CreateRoomsAndCorridors(root);

		if (!rooms.empty()) {
			rooms.back().isLastRoom = true;
		}
		

		shared_ptr<MapObj> boundObj = make_shared<MapObj>();
		boundObj->SetPosition(Vector3(mapChipSpace * 0 - mapChipSpace, -70, mapChipSpace * 0 - mapChipSpace));
		boundObj->SetType(MapType::WallType);
		boundObj->Init();
		SceneManager::Instance().AddObject(boundObj);

		shared_ptr<MapObj> boundObj2 = make_shared<MapObj>();
		boundObj2->SetPosition(Vector3(mapChipSpace * 1 - mapChipSpace, -70, mapChipSpace * 0 - mapChipSpace));
		boundObj2->SetType(MapType::WallType);
		boundObj2->Init();
		SceneManager::Instance().AddObject(boundObj2);

		EnsureWorkingDoor();
		EnsureWallNearDoor(0,0);
		


		for (int h = 0; h < mapHeight; h++) {
			for (int w = 0; w < mapWidth; w++) {

				if (h == 0 || h == mapHeight || w == 0 || w == mapWidth) {
					Vector3 boundPos;
					if (h == 0) boundPos = Vector3(mapChipSpace * w, -70, mapChipSpace * h - mapChipSpace);
					if (h == mapHeight) boundPos = Vector3(mapChipSpace * w, -70, mapChipSpace * h + mapChipSpace);
					if (w == 0) boundPos = Vector3(mapChipSpace * w - mapChipSpace, -70, mapChipSpace * h);
					if (w == mapWidth) boundPos = Vector3(mapChipSpace * w + mapChipSpace, -70, mapChipSpace * h);


					shared_ptr<MapObj> boundObj = make_shared<MapObj>();
					boundObj->SetPosition(boundPos);
					boundObj->SetType(WallType);
					boundObj->Init();
					SceneManager::Instance().AddObject(boundObj);
				}

				if (mapData[h][w] == WallType) {
					shared_ptr<MapObj> mapObj = make_shared<MapObj>();
					mapPos[h][w] = Vector3(mapChipSpace * w, -70, mapChipSpace * h);
					mapObj->SetPosition(mapPos[h][w]);
					mapObj->SetType(MapType::WallType);
					mapObj->Init();
					mapObj->SetIsWalkable(false);
					SceneManager::Instance().AddObject(mapObj);
				}

				if (mapData[h][w] == MapType::CorridorType || mapData[h][w] == MapType::LandType) {
					shared_ptr<MapObj> mapObj = make_shared<MapObj>();
					mapPos[h][w] = Vector3(mapChipSpace * w, -70, mapChipSpace * h);
					mapObj->SetPosition(mapPos[h][w]);
					if(mapData[h][w] == MapType::CorridorType)mapObj->SetType(MapType::CorridorType);
					else mapObj->SetType(MapType::LandType);
					mapObj->Init();
					mapObj->SetIsWalkable(true);
					SceneManager::Instance().AddObject(mapObj);
					isWalkable[h][w] = true;
				}
				if (mapData[h][w] == MapType::DoorType) {
					shared_ptr<MapObj> mapObj = make_shared<MapObj>();
					mapPos[h][w] = Vector3(mapChipSpace * w, -70, mapChipSpace * h);
					mapObj->SetPosition(mapPos[h][w]);
					mapObj->SetType(MapType::DoorType);
					mapObj->Init();
					mapObj->SetRotY(doorRotation[h][w]);
					mapObj->SetIsWalkable(true);
					SceneManager::Instance().AddObject(mapObj);
					isWalkable[h][w]= true;

				}

			}
		}

		AssignRoomType();


		GameManager::Instance().SetCanNextFloor(false);

		CounterDungeonInfo();

		SetPathManagerData();

		//AddDebugSphereinRoom();


	}

	

	


}

void MapLoader::PlaceMapObj()
{
}

void MapLoader::SetPathManagerData()
{
	PathManager::Instance().SetMapH(mapHeight);
	PathManager::Instance().SetMapW(mapWidth);

	PathManager::Instance().ResizeMapPos(mapHeight, mapWidth);
	PathManager::Instance().ResizeIsWalkable(mapHeight, mapWidth);

	PathManager::Instance().SetMapPos(mapPos);
	PathManager::Instance().SetIsWalkable(isWalkable);

	PathManager::Instance().ResizeMapId(mapHeight,mapWidth);
	PathManager::Instance().InitMapId();

	GameManager::Instance().mapPos = mapPos;
	GameManager::Instance().mapData = mapData;


}

void MapLoader::GenerateBSP(BSPNode* node, int depth)
{

	if (depth <= 0) return;

	if (node->width > minWidthOfRoom * 2 || node->height > minHeightOfRoom * 2) {
		bool splitVertically = node->width > node->height;

		int split = splitVertically ? Rnd(minWidthOfRoom, node->width - minWidthOfRoom)
			: Rnd(minHeightOfRoom, node->height - minHeightOfRoom);

		if (splitVertically) {
			node->left = new BSPNode(node->x, node->y, split, node->height, node);
			node->right = new BSPNode(node->x + split, node->y, node->width - split, node->height, node);
		}
		else {
			node->left = new BSPNode(node->x, node->y, node->width, split, node);
			node->right = new BSPNode(node->x, node->y + split, node->width, node->height - split, node);
		}

		GenerateBSP(node->left, depth - 1);
		GenerateBSP(node->right, depth - 1);
	}




	

}

void MapLoader::CreateRoomsAndCorridors(BSPNode* node)
{


	if (node == nullptr) return;

	if (node->left || node->right) {
		CreateRoomsAndCorridors(node->left);
		CreateRoomsAndCorridors(node->right);

		if (node->left && node->right) {
			auto leftCenter = getCenter(node->left);
			auto rightCenter = getCenter(node->right);
			if (leftCenter != pair<int, int>{0, 0}&& rightCenter != pair<int, int>{0, 0}) {
				connectCenters(leftCenter, rightCenter);
			}
		}
	}

	if (node->left == nullptr && node->right == nullptr) {
		if (Rnd(0, 100) < chanceToGenerateRoom) {
			int roomWidth = Rnd(minWidthOfRoom, node->width);
			int roomHeight = Rnd(minHeightOfRoom, node->height);
			int roomX = node->x + (node->width - roomWidth) / 2;
			int roomY = node->y + (node->height - roomHeight) / 2;

			int centerX = roomX + roomWidth / 2;
			int centerY = roomY + roomHeight / 2;
			bool isFirstRoom = (roomCounter == 0);
			roomCounter++;  
			rooms.push_back(Room(roomX, roomY, roomWidth, roomHeight, centerX, centerY, isFirstRoom, RoomType::NormalRoom, false, roomCounter));

			for (int i = roomY; i < roomY + roomHeight; i++) {
				for (int j = roomX; j < roomX + roomWidth; j++) {
					mapData[i][j] = MapType::LandType;  
					isWalkable[i][j] = true; //fix is walkable

				}
			}

			if (node->parent) {
				auto parentCenter = getCenter(node->parent);
				connectCenters({ roomX + roomWidth / 2, roomY + roomHeight / 2 }, parentCenter);
			}
		}
	}


}

pair<int, int> MapLoader::getCenter(BSPNode* node)
{
	if (!node) return { 0, 0 };

	int centerX = node->x + node->width / 2 ;
	int centerY = node->y + node->height / 2 ;
	return { centerX, centerY };
}

void MapLoader::connectCenters(const pair<int, int>& center1, const pair<int, int>& center2)
{
	Node* path = AStarFindPath(center1, center2);
	
	bool doorPlaced = false;


	while (path != nullptr) {
		if (mapData[path->y][path->x] == MapType::WallType && !doorPlaced) {
			bool leftWall = (path->x > 0 && mapData[path->y][path->x - 1] == MapType::WallType);
			bool rightWall = (path->x < mapWidth - 1 && mapData[path->y][path->x + 1] == MapType::WallType);
			bool upWall = (path->y > 0 && mapData[path->y - 1][path->x] == MapType::WallType);
			bool downWall = (path->y < mapHeight - 1 && mapData[path->y + 1][path->x] == MapType::WallType);

			int horizontalWalls = leftWall + rightWall;
			int verticalWalls = upWall + downWall;

			if (horizontalWalls == 2 && verticalWalls == 0) {
				mapData[path->y][path->x] = MapType::DoorType;
				doorPlaced = true;
				doorRotation[path->y][path->x] = 0;
				isWalkable[path->y][path->x] = true; //fix is walkable
			}
			else if (verticalWalls == 2 && horizontalWalls == 0) {
				mapData[path->y][path->x] = MapType::DoorType;
				doorPlaced = true;
				doorRotation[path->y][path->x] = 1;
				isWalkable[path->y][path->x] = true; //fix is walkable
			}
			
			else {
				mapData[path->y][path->x] = MapType::CorridorType;
				isWalkable[path->y][path->x] = true; //fix is walkable

			}
		}
		else {
			mapData[path->y][path->x] = MapType::CorridorType;
			isWalkable[path->y][path->x] = true; //fix is walkable
		}

		path = path->parent;
	}



}

Node* MapLoader::AStarFindPath(const pair<int, int>& start, const pair<int, int>& goal)
{
	vector<vector<bool>> closedSet(mapHeight, vector<bool>(mapWidth, false));
	vector<vector<bool>> openSet(mapHeight, vector<bool>(mapWidth, false));
	vector<vector<Node*>> nodes(mapHeight, vector<Node*>(mapWidth, nullptr));

	Node* startNode = new Node(start.first, start.second);
	startNode->g = 0;
	startNode->h = abs(start.first - goal.first) + abs(start.second - goal.second);
	startNode->parent = nullptr;
	nodes[start.first][start.second] = startNode;

	auto comp = [](Node* lhs, Node* rhs) { return lhs->g + lhs->h > rhs->g + rhs->h; };
	priority_queue<Node*, vector<Node*>, decltype(comp)> pq(comp);
	pq.push(startNode);
	openSet[start.first][start.second] = true;

	while (!pq.empty()) {
		Node* current = pq.top();
		pq.pop();
		openSet[current->x][current->y] = false;

		if (current->x == goal.first && current->y == goal.second) {
			return current; 
		}

		closedSet[current->x][current->y] = true;

		vector<pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };  
		for (auto& dir : directions) {
			int nx = current->x + dir.first, ny = current->y + dir.second;

			if (nx >= 0 && nx < mapWidth && ny >= 0 && ny < mapHeight && mapData[ny][nx] != 99 && !closedSet[nx][ny]) {
				int newG = current->g + 1;
				if (!openSet[nx][ny]) {
					Node* neighbor = new Node(nx, ny);
					neighbor->g = newG;
					neighbor->h = abs(nx - goal.first) + abs(ny - goal.second);
					neighbor->parent = current;
					nodes[nx][ny] = neighbor;
					pq.push(neighbor);
					openSet[nx][ny] = true;
				}
			}
		}
	}

	return nullptr;
}

void MapLoader::EnsureWorkingDoor()
{
	for (int h = 1; h < mapHeight - 1; h++) {
		for (int w = 1; w < mapWidth - 1; w++) {
			if (mapData[h][w] == MapType::DoorType) {
		
				bool leftWall = (w - 1 >= 0 && mapData[h][w - 1] == MapType::WallType);
				bool rightWall = (w + 1 < mapWidth && mapData[h][w + 1] == MapType::WallType);
				bool upWall = (h - 1 >= 0 && mapData[h - 1][w] == MapType::WallType);
				bool downWall = (h + 1 < mapHeight && mapData[h + 1][w] == MapType::WallType);

				bool left2Wall = (w - 2 >= 0 && mapData[h][w - 2] == MapType::WallType);
				bool right2Wall = (w + 2 < mapWidth && mapData[h][w + 2] == MapType::WallType);
				bool up2Wall = (h - 2 >= 0 && mapData[h - 2][w] == MapType::WallType);
				bool down2Wall = (h + 2 < mapHeight && mapData[h + 2][w] == MapType::WallType);

				if (leftWall && rightWall) {
					if (left2Wall && right2Wall) {

					}
					else {
						//mapData[h][w] = MapType::DoorType;
						//mapData[h + 1][w] = MapType::WallType;
						//mapData[h - 1][w] = MapType::WallType;



						mapData[h][w] = MapType::CorridorType;
						mapData[h + 1][w] = MapType::LandType;
						mapData[h - 1][w] = MapType::LandType;

						isWalkable[h][w] = true; //fix is walkable


					}
				}

				if (upWall && downWall) {
					if (up2Wall && down2Wall) {

					}
					else {
						//mapData[h][w] = MapType::DoorType;
						//mapData[h + 1][w] = MapType::WallType;
						//mapData[h - 1][w] = MapType::WallType;

						mapData[h][w] = MapType::CorridorType;
						mapData[h + 1][w] = MapType::LandType;
						mapData[h - 1][w] = MapType::LandType;

					}

				}

			}
		}
	}
}

void MapLoader::EnsureWallNearDoor(int mapW, int mapH)
{
	for (int h = 1; h < mapHeight - 1; h++) {
		for (int w = 1; w < mapWidth - 1; w++) {
			if (mapData[h][w] == MapType::DoorType) {
				bool leftWall = (mapData[h][w - 1] == MapType::WallType);
				bool rightWall = (mapData[h][w + 1] == MapType::WallType);
				bool upWall = (mapData[h - 1][w] == MapType::WallType);
				bool downWall = (mapData[h + 1][w] == MapType::WallType);

				bool left2Wall = (mapData[h][w - 2] == MapType::WallType);
				bool right2Wall = (mapData[h][w + 2] == MapType::WallType);
				bool up2Wall = (mapData[h - 2][w] == MapType::WallType);
				bool down2Wall = (mapData[h + 2][w] == MapType::WallType);

				if (leftWall && !rightWall) {
					mapData[h][w + 1] = MapType::WallType;
				}
				else if (!leftWall && rightWall) {
					mapData[h][w - 1] = MapType::WallType;
				}

				if (upWall && !downWall) {
					mapData[h + 1][w] = MapType::WallType;
				}
				else if (!upWall && downWall) {
					mapData[h - 1][w] = MapType::WallType;
				}

				if (!((leftWall && rightWall) || (upWall && downWall))) {
					mapData[h][w] = MapType::CorridorType;
				}
			}
		}
	}
}



void MapLoader::AssignRoomType()
{

	if (rooms.empty()) return;

	rooms.front().type = RoomType::SpawnRoom;
	rooms.back().type = RoomType::GoalRoom;

	vector<int> assignableRooms;
	for (int i = 1; i < rooms.size() - 1; ++i) {
		assignableRooms.push_back(i);
	}

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine rng(seed);

	std::shuffle(assignableRooms.begin(), assignableRooms.end(), rng);

	if (!assignableRooms.empty()) {
		rooms[assignableRooms[0]].type = RoomType::MobEnemyRoom; 
		if (assignableRooms.size() > 1) {
			rooms[assignableRooms[1]].type = RoomType::MobEnemyRoom2;  
		}
		if (assignableRooms.size() > 2) {
			rooms[assignableRooms[2]].type = RoomType::ChestRoom;  
		}
		if (assignableRooms.size() > 3) {
			rooms[assignableRooms[3]].type = RoomType::ChestRoom;
		}
		if (assignableRooms.size() > 4) {
			rooms[assignableRooms[4]].type = RoomType::MobEnemyRoom;
		}
		if (assignableRooms.size() > 5) {
			rooms[assignableRooms[5]].type = RoomType::MobEnemyRoom2;
		}
		if (assignableRooms.size() > 6) {
			rooms[assignableRooms[6]].type = RoomType::TrapRoom;
		}

	}

	for (int i = 6; i < assignableRooms.size(); ++i) {
		rooms[assignableRooms[i]].type = RoomType::NormalRoom;
	}

	spawnRoomPos = Vector3(rooms.front().centerX, rooms.front().centerY, 0);
	goalRoomPos = Vector3(rooms.back().centerX, rooms.back().centerY, 0);

	if (GameManager::Instance().GetFloorNum() == 0) {
		PlaceNPCInRoom((rooms.front().centerX + 1) * mapChipSpace, (rooms.front().centerY + 2) * mapChipSpace);
	}

	for (auto& room : rooms) {
		if (room.type == RoomType::MobEnemyRoom) {
			PlaceBarrelInRoom(room);
			PlaceSpikeInRoom(room);		

		}
		else if (room.type == RoomType::MobEnemyRoom2) {

			PlaceEnemyInRoom(room);


		
		}
		else if (room.type == RoomType::ChestRoom) {
			PlaceBarrelInRoom(room);
			PlaceChestInRoom(room);
			
		}
		else if (room.type == RoomType::BarrelRoom) {
			PlaceBarrelInRoom(room);

		}
		else if (room.type == RoomType::TrapRoom) {
			PlaceSpikeInRoom(room);
			PlaceBarrelInRoom(room);
		}



	}

	//Set Goal
	shared_ptr<MapObj> mapGoalObj = make_shared<MapObj>();
	mapGoalObj->SetType(GoalType);
	mapGoalObj->SetPosition(Vector3(goalRoomPos.x* mapChipSpace, -70, goalRoomPos.y* mapChipSpace));
	mapGoalObj->Init();
	SceneManager::Instance().AddObject(mapGoalObj);
	//Set Player
	wpPlayer.lock()->SetPosition({ spawnRoomPos.x * mapChipSpace,-70,spawnRoomPos.y * mapChipSpace });

	

}



void MapLoader::ClearOldMap()
{
	rooms.clear();

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->GetTag() == MapTag || obj->GetTag() == EnemyTag || obj->GetTag() == GameTag::ItemTag || obj->GetTag() == GameTag::EnemyWeaponTag ) {
			
			if (obj->GetTag() == MapTag && obj->GetType() == MapType::DoorType) {
				//cast to mapobj class	
				shared_ptr<MapObj> mapObj = static_pointer_cast<MapObj>(obj);
				mapObj->ClearFireEff();
			}
			
			obj->SetExpired();

			

		}
	}
}

void MapLoader::InitMapData()
{
	for (int h = 0; h < mapHeight; h++) {
		for (int w = 0; w < mapWidth; w++) {
			mapData[h][w] = MapType::WallType;
			mapPos[h][w] = { 0,-70,0 };
		}
	}




}

void MapLoader::CreateBound()
{
	shared_ptr<MapObj> boundObj = make_shared<MapObj>();
	boundObj->SetPosition(Vector3(mapChipSpace * 0 - mapChipSpace, -70, mapChipSpace * 0 - mapChipSpace));
	boundObj->SetType(MapType::WallType);
	boundObj->Init();
	SceneManager::Instance().AddObject(boundObj);

	shared_ptr<MapObj> boundObj2 = make_shared<MapObj>();
	boundObj2->SetPosition(Vector3(mapChipSpace * 1 - mapChipSpace, -70, mapChipSpace * 0 - mapChipSpace));
	boundObj2->SetType(MapType::WallType);
	boundObj2->Init();
	SceneManager::Instance().AddObject(boundObj2);

	for (int h = 0; h < mapHeight; h++) {
		for (int w = 0; w < mapWidth; w++) {
			if (h == 0 || h == mapHeight || w == 0 || w == mapWidth) {
				Vector3 boundPos;
				if (h == 0) boundPos = Vector3(mapChipSpace * w, -70, mapChipSpace * h - mapChipSpace);
				if (h == mapHeight) boundPos = Vector3(mapChipSpace * w, -70, mapChipSpace * h + mapChipSpace);
				if (w == 0) boundPos = Vector3(mapChipSpace * w - mapChipSpace, -70, mapChipSpace * h);
				if (w == mapWidth) boundPos = Vector3(mapChipSpace * w + mapChipSpace, -70, mapChipSpace * h);


				shared_ptr<MapObj> boundObj = make_shared<MapObj>();
				boundObj->SetPos2D(Vector2(16 * w + 350, 16 * h + 140));
				boundObj->SetPosition(boundPos);
				boundObj->SetType(MapType::WallType);
				boundObj->Init();
				SceneManager::Instance().AddObject(boundObj);
			}

			if (mapData[h][w] == MapType::WallType) {
				shared_ptr<MapObj> mapObj = make_shared<MapObj>();
				mapPos[h][w] = Vector3(mapChipSpace * w, -70, mapChipSpace * h);
				mapObj->SetPos2D(Vector2(16 * w + 350, 16 * h + 140));
				mapObj->SetPosition(mapPos[h][w]);
				mapObj->SetType(MapType::WallType);
				mapObj->Init();
				SceneManager::Instance().AddObject(mapObj);
			}

		}
	}

}

void MapLoader::AddDebugSphereinRoom()
{
	

	/*for (const auto& room : rooms) {
		if (room.type == RoomType::MobEnemyRoom) {
			Vector3 centerPos = Vector3(room.centerX * mapChipSpace, -70, room.centerY * mapChipSpace);
			debugWire->AddDebugSphere(Vector3(0,-70,0), 30.0f, kRedColor); 
		}
	}*/


}

void MapLoader::PlaceBarrelInRoom(Room& room)
{
	if (GameManager::Instance().GetFloorNum() < 1) return;

	float dungeonCenterX = mapWidth / 2.0f;
	float dungeonCenterY = mapHeight / 2.0f;

	bool isLeft = room.centerX < dungeonCenterX;
	bool isUp = room.centerY < dungeonCenterY;

	bool isHorizontal = room.width >= room.height;

	Vector3 startPos;

	if (isUp && isLeft) {
		startPos = Vector3(room.x * mapChipSpace, -70, room.y * mapChipSpace);
	}
	else if (isUp && !isLeft) {
		startPos = Vector3((room.x + room.width - 1) * mapChipSpace, -70, room.y * mapChipSpace);
	}
	else if (!isUp && isLeft) {
		startPos = Vector3(room.x * mapChipSpace, -70, room.y * mapChipSpace);

	}
	else {
		startPos = Vector3(room.x * mapChipSpace, -70, room.y * mapChipSpace);
	}

	for (int i = 0; i < 3; ++i) {
		Vector3 barrelPos = startPos;
		int mapX = room.x;
		int mapY = room.y;

		if (isHorizontal) {
			if (isLeft) {
				barrelPos.x += i * mapChipSpace;
				mapX = room.x + i;
			}
			else {
				barrelPos.x -= i * mapChipSpace;
				mapX = room.x + room.width - 1 - i;
			}
			
		}
		else {
			if (isUp) {
				barrelPos.z += i * mapChipSpace;
				mapY = room.y + i;
			}
			else {
				barrelPos.z -= i * mapChipSpace;
				mapY = room.y + room.height - 1 - i;
			}
		}


		if (mapData[mapY][mapX] != MapType::CorridorType && mapData[mapY][mapX] != MapType::WallType) {
			shared_ptr<MapObj> barrel = make_shared<MapObj>();
			barrel->SetPosition(barrelPos);
			barrel->SetType(MapType::BottleType); 
			barrel->Init();
			SceneManager::Instance().AddObject(barrel);
			isWalkable[mapY][mapX] = false;

		}

	
	}

}

void MapLoader::PlaceChestInRoom(Room& room)
{
	if (GameManager::Instance().GetFloorNum() < 1) return;

	float dungeonCenterX = mapWidth / 2.0f;
	float dungeonCenterY = mapHeight / 2.0f;

	bool isLeft = room.centerX < dungeonCenterX;
	bool isUp = room.centerY < (dungeonCenterY );

	float chestDirection = 0;

	shared_ptr<MapObj> chest = make_shared<MapObj>();
	if (rand() % 4 < 1)	chest->SetType(MapType::MimicType);
	else					chest->SetType(MapType::ChestType);
	chest->SetPosition(Vector3((room.centerX) * mapChipSpace, -70, (room.centerY) * mapChipSpace));
	chest->SetCamera(wpCamera.lock());
	chest->Init();
	if (isUp) chest->SetRotAngle(180);
	else chest->SetRotAngle(0);
	SceneManager::Instance().AddObject(chest);
	
	int chestMapX = static_cast<int>(room.centerX);
	int chestMapY = static_cast<int>(room.centerY);
	if (chestMapY >= 0 && chestMapY < mapHeight && chestMapX >= 0 && chestMapX < mapWidth)
	{
		isWalkable[chestMapY][chestMapX] = false;
	}


}

void MapLoader::PlaceSpikeInRoom(Room& room)
{
	if (GameManager::Instance().GetFloorNum() < 1) return;

	shared_ptr<MapObj> trap = make_shared<MapObj>();
	trap->SetType(MapType::TrapType);
	trap->SetPosition(Vector3((room.centerX) * mapChipSpace, -70, (room.centerY) * mapChipSpace));
	trap->SetCamera(wpCamera.lock());
	trap->Init();
	SceneManager::Instance().AddObject(trap);


}

void MapLoader::PlaceEnemyInRoom(Room& room)
{
	if (GameManager::Instance().GetFloorNum() < 1) return;

	shared_ptr<WanderEnemy> enemyObj = make_shared<WanderEnemy>();
	enemyObj->SetCamera(wpCamera.lock());
	enemyObj->SetPlayer(wpPlayer.lock());
	enemyObj->SetType(EnemyType::eWanderer);
	Vector3 ptA = { room.centerX * mapChipSpace,-70, room.centerY * mapChipSpace };
	Vector3 ptB = { (room.centerX + 1) * mapChipSpace,-70, room.centerY * mapChipSpace };
	Vector3 ptC = { (room.centerX - 1) * mapChipSpace,-70, (room.centerY + 1) * mapChipSpace };
	enemyObj->SetPatrolPt(ptA, ptB, ptC);
	enemyObj->Init();
	enemyObj->SetPosition(Vector3(room.centerX * mapChipSpace, -70, room.centerY * mapChipSpace));
	SceneManager::Instance().AddObject(enemyObj); 

	//KdShaderManager::Instance().WorkAmbientController().AddPointLight({ 1.4,0, 1.4 }, 49, { Vector3(room.centerX * mapChipSpace, -70, room.centerY * mapChipSpace)  }, true);

	shared_ptr<MapObj> ePoint = make_shared<MapObj>();
	ePoint->SetType(MapType::EnemyPointType);
	ePoint->SetPosition(Vector3((room.centerX) * mapChipSpace, -70, (room.centerY) * mapChipSpace));
	ePoint->SetCamera(wpCamera.lock());
	ePoint->Init();
	SceneManager::Instance().AddObject(ePoint);

}

void MapLoader::PlaceNPCInRoom(float _posX, float _posY)
{
	shared_ptr<MapObj> NPC = make_shared<MapObj>();
	NPC->SetType(MapType::NPCType);
	NPC->SetPosition(Vector3(_posX, -70, _posY));
	NPC->SetCamera(wpCamera.lock());
	NPC->SetPlayer(wpPlayer.lock());
	NPC->Init();
	SceneManager::Instance().AddObject(NPC);
}

void MapLoader::CounterDungeonInfo()
{
	// Reset counters
	numOfLand = 0;
	numOfWall = 0;
	numOfDoor = 0;

	// Traverse through the mapData to count each type of tile
	for (int h = 0; h < mapHeight; ++h) {
		for (int w = 0; w < mapWidth; ++w) {
			if (mapData[h][w] != MapType::WallType ) {
				numOfLand++; // Count land and corridor as land
			}
			else if (mapData[h][w] == MapType::WallType) {
				numOfWall++;
			}
			
			if (mapData[h][w] == MapType::DoorType) {
				numOfDoor++;
			}
		}
	}

	
	wallRatio = static_cast<float>(numOfWall) / numOfLand;
	
	/*if (numOfDoor < 2) {
		GameManager::Instance().SetCanNextFloor(true);
		LoadMap();
	}*/

	
	

}

void MapLoader::CheckBarrelOverlapWithWall()
{
	
}

void MapLoader::CallImgui()
{
	//show isInit
	ImGui::Begin("MapLoader");

	//show mapWidth mapHeight
	ImGui::Text("mapWidth : %d", mapWidth);
	ImGui::Text("mapHeight : %d", mapHeight);

	//show numofLand numofWall numofDoor
ImGui::Text("numOfLand : %d", numOfLand);
ImGui::Text("numOfWall : %d", numOfWall);
ImGui::Text("numOfDoor : %d", numOfDoor);
//show wallRatio
ImGui::Text("wallRatio : %f", wallRatio);

	//sldier scaleMini 0 to 77
ImGui::SliderFloat("scaleMini", &scaleMini, 0, 77);

	//slider float rectX Y
	ImGui::SliderFloat("rectX", &rectX, -1280, 1280);
	ImGui::SliderFloat("rectY", &rectY, -720, 720);

	//slider float testY -360 to 360
ImGui::SliderFloat("testY", &testY, -360, 360);

	ImGui::Text("isInit : %d", isInit);
	
	ImGui::Text("spawnRoomPos : %f %f %f", spawnRoomPos.x * mapChipSpace, spawnRoomPos.y * mapChipSpace, spawnRoomPos.z * mapChipSpace);

	if (ImGui::CollapsingHeader("isWalkable Map"))
	{
		for (int h = 0; h < mapHeight; h++)
		{
			for (int w = 0; w < mapWidth; w++)
			{
				ImGui::Text("isWalkable[%d][%d] = %s", h, w, isWalkable[h][w] ? "true" : "false");
			}
		}
	}

	for (int i = 0; i < rooms.size(); i++) {
		Room& room = rooms[i];

		ImGui::Separator();
		ImGui::Text("Room #%d", i + 1);
		ImGui::Text("Position: (X: %d, Y: %d)", room.x, room.y);
		ImGui::Text("Size: %d x %d", room.width, room.height);
		ImGui::Text("Center: (X: %.2f, Y: %.2f)", room.centerX, room.centerY);

		const char* roomTypeName = "";
		switch (room.type) {
		case SpawnRoom: roomTypeName = "Spawn Room"; break;
		case GoalRoom: roomTypeName = "Goal Room"; break;
		case MobEnemyRoom: roomTypeName = "Mob Enemy Room"; break;
		case MobEnemyRoom2: roomTypeName = "Mob Enemy Room 2"; break;
		case ChestRoom: roomTypeName = "Chest Room"; break;
		case NormalRoom: roomTypeName = "Normal Room"; break;
		}
		ImGui::Text("Room Type: %s", roomTypeName);

		ImGui::Text("Is First Room: %s", room.isFirstRoom ? "Yes" : "No");
		ImGui::Text("Is Last Room: %s", room.isLastRoom ? "Yes" : "No");
		ImGui::Text("Room Order: %d", room.orderOfRoom);
	}




	ImGui::End;





}
