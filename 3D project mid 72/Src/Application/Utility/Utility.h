#pragma once

namespace Utility {

	float Lerp(float start, float end, float t);			//線形補間
	float Rnd(float _min, float _max);						//ランダム値取得
	float SinRange(float x, float minVal, float maxVal);	//サインカーブの値を範囲で返す
	bool CheckHit2D(float a_target1X, float a_target1Y, float a_target2X, float a_target2Y, float a_hitDist); 	//2Dの当たり判定
	float min(float a, float b);
	float max(float a, float b);

}