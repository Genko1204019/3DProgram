#pragma once

namespace Utility {

	float Lerp(float start, float end, float t);			//線形補間
	float Rnd(float _min, float _max);						//ランダム値取得
	float SinRange(float x, float minVal, float maxVal);	//サインカーブの値を範囲で返す
	bool CheckHit2D(float a_target1X, float a_target1Y, float a_target2X, float a_target2Y, float a_hitDist); 	//2Dの当たり判定
	float min(float a, float b);
	float max(float a, float b);

	void CalOverlap(const std::list<KdCollider::CollisionResult>& _colRet, float& _overlap, bool& _isHit, Vector3& _hitPos, Vector3& _hitDir);
	void CalOverlap(const std::list<KdCollider::CollisionResult>& _colRet, float& _overlap, bool& _isHit, Vector3& _hitPos, Vector3& _hitDir, Vector3& _normal);
	void HitPositionAdjust(Vector3& _pos, const Vector3& _hitDir, float _overlap);
	void HitPositionAdjust2D(Vector3& _pos, const Vector3& _hitDir, float _overlap);



}