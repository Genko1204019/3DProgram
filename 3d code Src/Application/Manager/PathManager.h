#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

//struct PathNode;





class PathManager : public KdGameObject
{
public:

	
	void SetIsWalkable(const vector<vector<bool>>& _isWalkable)
	{
		isWalkable = _isWalkable;
	}

	//void SetIsWalkable(vector<vector<bool>>& _isWalkable) { isWalkable = _isWalkable; }

	//set mapPos
	void SetMapPos(vector<vector<Vector3>>& _mapPos) { mapPos = _mapPos; }

	void ResizeMapPos(int _mapH, int _mapW) { mapPos.resize(_mapH); for (auto& pos : mapPos) { pos.resize(_mapW); } }
	void ResizeIsWalkable(int _mapH, int _mapW) {
		
		//isWalkable.resize(_mapH); for (auto& walkable : isWalkable) { walkable.resize(_mapW); } 
		isWalkable.resize(_mapH); // Resize the outer vector
		for (auto& walkable : isWalkable) {
			walkable.resize(_mapW, false); // Resize each row and initialize with false
		}

	}
	void ResizeMapId(int _mapH, int _mapW) { mapId.resize(_mapH); for (auto& id : mapId) { id.resize(_mapW); } }
	


	//awt mapH mapW
	void SetMapH(int _mapH) { mapH = _mapH; }
	void SetMapW(int _mapW) { mapW = _mapW; }

	void InitMapId();
	
	bool isGround(int x, int y) { return x >= 0 && x < mapW && y >= 0 && y < mapH && isWalkable[x][y];}

	void Init() override;
	void Update() override;

	vector<PathNode> PathFinding(int startX, int startY, int goalX, int goalY);  //A*アルゴリズムで道を探す処理

	void FindEnemyPoint(float _ePosX, float _ePosZ, int& _ePointX, int& _ePointY);

	void FindPlayerPoint(float _px,float _py);

	Vector2 FindPointInMap(float _pointX, float _pointY);

	void CallImgui() override;

	bool CheckHit(float a_target1X, float a_target1Y, float a_target2X, float a_target2Y, float a_hitDist); //当たり判定

	int mapH = 10;
	int mapW = 10;

	vector<vector<Vector3>> mapPos;
	vector<vector<bool>> isWalkable;

	vector<vector<Vector2>> mapId;

	Vector2 playerPoint = { 3,3 };


private:

	
private:
	PathManager() {}
	~PathManager() override {}

public:
	static PathManager& Instance()
	{
		static PathManager instance;
		return instance;
	}
};



