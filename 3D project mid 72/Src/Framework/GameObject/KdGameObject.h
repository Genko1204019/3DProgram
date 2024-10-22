#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

class GameCamera;

// ゲーム上に存在するすべてのオブジェクトの基底となるクラス
class KdGameObject : public std::enable_shared_from_this<KdGameObject>
{
public:

	enum
	{
		eDrawTypeLit = 1 << 0,
		eDrawTypeUnLit = 1 << 1,
		eDrawTypeBright = 1 << 2,
		eDrawTypeUI = 1 << 3,
		eDrawTypeDepthOfShadow = 1 << 4,
	};

	KdGameObject() {}
	virtual ~KdGameObject() { Release(); }

	virtual void Init() {}
	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}

	virtual void GenerateDepthMapFromLight() {}
	virtual void PreDraw() {}
	virtual void DrawLit() {}
	virtual void DrawUnLit() {}
	virtual void DrawBright() {}
	virtual void DrawSprite() {}
	virtual void DrawMoji() {}
	virtual void DrawRenderTarget() {}
	virtual void DrawMini() {}

	virtual void CallImgui() {}
	virtual void DrawDebug();

	virtual void SetAsset(const std::string&) {}
	void SetScale(float scalar);
	virtual void SetScale(const Math::Vector3& scale);
	virtual Math::Vector3 GetScale() const;

	const Math::Matrix& GetMatrix() const { return worldMat; }
	virtual bool CheckInScreen(const DirectX::BoundingFrustum&) const { return false; }
	virtual void CalcDistSqrFromCamera(const Math::Vector3& camPos);
	float GetDistSqrFromCamera() const { return m_distSqrFromCamera; }
	UINT GetDrawType() const { return drawType; }

	bool Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);

	virtual bool IsExpired() const { return m_isExpired; }
	void SetExpired() { m_isExpired = true; }

	virtual bool IsVisible()	const { return false; }
	virtual bool IsRideable()	const { return false; }

	void SetPos(const Math::Vector3& pos) { worldMat.Translation(pos); }
	virtual Math::Vector3 GetPos() const { return worldMat.Translation(); }

	void SetPosition(const Math::Vector3& _pos) { pos = _pos; }
	const Math::Vector3& GetPosition() const { return pos; }
	
	void SetTag(int tag) { objTag = tag; }
	int GetTag() const { return objTag; }
	
	void SetType(int type) { objType = type; }
	int GetType() const { return objType; }

	void SetCamera(const shared_ptr<GameCamera>& _spCamera) { wpCamera = _spCamera; }

	//get set gameSPd
	void SetGameSpd(float spd) { gameSpd = spd; }
	float GetGameSpd() { return gameSpd; }

	virtual void OnHit() {};
	virtual void OnHit(int hitId) {};

	virtual void OnDeath() {};

	virtual void DebugKey() {};

	weak_ptr<GameCamera> wpCamera;

	
protected:

	void Release() {}



	UINT drawType = 0;


	float m_distSqrFromCamera = 0;

	bool m_isExpired = false;

	Math::Matrix	worldMat;




	unique_ptr<KdCollider> objCollider = nullptr;


	unique_ptr<KdDebugWireFrame> debugWire = nullptr;

	Vector3 pos = {};
	
	int objTag = -1;
	int objType = -1;


	float gameSpd = 1;

	

};
