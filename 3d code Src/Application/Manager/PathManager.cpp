#include"PathManager.h"
#include"Application/Manager/MapLoader.h"

void PathManager::Init()
{
	isWalkable.clear();
	isWalkable.resize(mapH, std::vector<bool>(mapW));

	for (int h = 0; h < mapH; h++) {
		for (int w = 0; w < mapW; w++) {	
			isWalkable[h][w] = false;
		}
	}

	debugWire = std::make_unique<KdDebugWireFrame>();


}

void PathManager::Update()
{
	

	//debugWire->AddDebugSphere(Vector3(0, -70, 0), 30.0f, kRedColor);


	//loop maph mapW draw spher in mapPos

	if (GameManager::Instance().GetCanDebugWire()) {
		for (int h = 0; h < mapH; h++) {
			for (int w = 0; w < mapW; w++) {
				if (!isWalkable[h][w]) {
					debugWire->AddDebugSphere(mapPos[h][w], 2.8, kRedColor);
				}
				else {
					//m_pDebugWire->AddDebugSphere(mapPos[h][w], 2, kRedColor);
				}
			}
		}
	}

	


}

vector<PathNode> PathManager::PathFinding(int startX, int startY, int goalX, int goalY)
{
	vector<PathNode> path;
	priority_queue<PathNode, vector<PathNode>, function<bool(PathNode&, PathNode&)>> nodeList([](PathNode& a, PathNode& b) { return a.fCost > b.fCost; });
	vector<vector<bool>> visited(mapH, vector<bool>(mapW, false));
	vector<vector<PathNode>> parent(mapH, vector<PathNode>(mapW, { -1, -1 }));

	PathNode start(startX, startY);
	PathNode goal(goalX, goalY);

	nodeList.push(start);
	visited[startY][startX] = true;

	while (!nodeList.empty()) {
		PathNode current = nodeList.top();
		nodeList.pop();

		if (current.x == goalX && current.y == goalY) {
			while (current.x != startX || current.y != startY) {
				path.push_back(current);
				current = parent[current.y][current.x];
			}
			path.push_back(start);
			reverse(path.begin(), path.end());
			return path;
		}

		int nearX[] = { 0, 0, -1, 1 };
		int nearY[] = { -1, 1, 0, 0 };

		for (int i = 0; i < 4; ++i) {
			int nextX = current.x + nearX[i];
			int nextY = current.y + nearY[i];

			if (isGround(nextX, nextY) && !visited[nextY][nextX]) {
				visited[nextY][nextX] = true;
				int newGCost = current.gCost + 1;
				int newHCost = abs(nextX - goalX) + abs(nextY - goalY);
				int newFCost = newGCost + newHCost;
				//if (showAllPath) mapColor[nextY][nextX] = { 1,1,0,0.4 };

				if (parent[nextY][nextX].x == -1 || newFCost < parent[nextY][nextX].fCost) {
					parent[nextY][nextX] = current;
					PathNode newCurrent(nextX, nextY);
					newCurrent.gCost = newGCost;
					newCurrent.hCost = newHCost;
					newCurrent.fCost = newFCost;
					nodeList.push(newCurrent);
				}
			}
		}
	}

	return path;
}

void PathManager::FindEnemyPoint(float _ePosX, float _ePosZ, int& _ePointX, int& _ePointY)
{
	for (int h = 0; h < mapH; h++) {
		for (int w = 0; w < mapW; w++) {
			if (CheckHit(mapPos[h][w].x, mapPos[h][w].z, _ePosX, _ePosZ, 3) && isWalkable[h][w]) {
				_ePointX = mapId[h][w].x;
				_ePointY = mapId[h][w].y;
			}
		}
	}

}



void PathManager::CallImgui()
{
	// Show mapPos, isWalkable, and mapId
	if (ImGui::Begin("PathManager")) {

		//show playerPoint
		ImGui::Text("playerPoint = (%f, %f)", playerPoint.x, playerPoint.y);

		// Display mapPos
		ImGui::Text("MapPos");
		for (int h = 0; h < mapH; h++) {
			for (int w = 0; w < mapW; w++) {
				ImGui::Text("mapPos[%d][%d] = (%f, %f, %f)", h, w, mapPos[h][w].x, mapPos[h][w].y, mapPos[h][w].z);
			}
		}

		// Display isWalkable
		ImGui::Text("isWalkable");
		for (int h = 0; h < mapH; h++) {
			for (int w = 0; w < mapW; w++) {
				ImGui::Text("isWalkable[%d][%d] = %d", h, w, static_cast<int>(isWalkable[h][w]));
			}
		}

		// Display mapId (assuming mapId is a 2D vector of Vector2 or similar)
		ImGui::Text("mapId");
		for (int h = 0; h < mapH; h++) {
			for (int w = 0; w < mapW; w++) {
				ImGui::Text("mapId[%d][%d] = (%f, %f)", h, w, mapId[h][w].x, mapId[h][w].y);
			}
		}

		ImGui::Text("mapH = %d", mapH);
		ImGui::Text("mapW = %d", mapW);

		ImGui::End();  // End the ImGui window
	}


}

void PathManager::InitMapId()
{
	for (int h = 0; h < mapH; h++) {
		for (int w = 0; w < mapW; w++) {
			mapId[h][w].x = h;
			mapId[h][w].y = w;
		}
	}
}

bool PathManager::CheckHit(float a_target1X, float a_target1Y, float a_target2X, float a_target2Y, float a_hitDist)
{
	float a = a_target1X - a_target2X;
	float b = a_target1Y - a_target2Y;
	float c = sqrt(a * a + b * b);
	if (c < a_hitDist) {
		return true;
	}
	else {
		return false;
	}
}

void PathManager::FindPlayerPoint(float _px, float _py)
{
	/*for (int h = 0; h < mapH; h++) {
		for (int w = 0; w < mapW; w++) {
			if (CheckHit(mapPos[h][w].x, mapPos[h][w].z, _px, _py, 30) && isWalkable[h][w]) {
				playerPoint.x = mapId[h][w].x;
				playerPoint.y = mapId[h][w].y;
			}
		}
	}*/

	//for (int h = 0; h < mapH; h++) {
	//	for (int w = 0; w < mapW; w++) {
	//		if (CheckHit(mapPos[h][w].x, mapPos[h][w].z, _px, _py, 3) && isWalkable[h][w]) {
	//			playerPoint.x = mapId[h][w].x;
	//			playerPoint.y = mapId[h][w].y;
	//		}
	//	}
	//}

	if(mapId.size() == 0) return;
	for (int h = 0; h < mapH; h++) {
		for (int w = 0; w < mapW; w++) {
			if (CheckHit(mapPos[h][w].x, mapPos[h][w].z, _px, _py, 3) && isWalkable[h][w]) {
				playerPoint.x = mapId[h][w].x;
				playerPoint.y = mapId[h][w].y;
			}
		}
	}

	//// Assuming mapPos is laid out in a grid with uniform tile sizes
	//float tileSize = 6; // Adjust according to your actual tile size

	//// Calculate the player's position on the grid
	//int gridX = static_cast<int>(_px / tileSize);
	//int gridY = static_cast<int>(_py / tileSize);

	//// Check bounds to prevent accessing out-of-bounds tiles
	//if (gridX >= 0 && gridX < mapW && gridY >= 0 && gridY < mapH)
	//{
	//	// Check if the tile is walkable
	//	if (isWalkable[gridY][gridX])
	//	{
	//		playerPoint.x = mapId[gridY][gridX].x;
	//		playerPoint.y = mapId[gridY][gridX].y;
	//	}
	//}

}

Vector2 PathManager::FindPointInMap(float _pointX, float _pointY)
{
	if (mapId.size() == 0) return {0,0};
	for (int h = 0; h < mapH; h++) {
		for (int w = 0; w < mapW; w++) {
			if (CheckHit(mapPos[h][w].x, mapPos[h][w].z, _pointX, _pointY, 3)) {
				return mapId[h][w];
			}
			//else return {0,0};
		}
	}
}

