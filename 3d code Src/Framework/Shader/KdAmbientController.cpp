//KdAmbientController.cpp file:

#include "KdAmbientController.h"

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// シェーダーマネージャで設定したシェーダーの初期値を取得してくる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::Init()
{
	// 光源のパラメータ
	{
		const KdShaderManager::cbLight& cbLight = KdShaderManager::Instance().GetLightCB();

		m_parameter.m_ambientLightColor = cbLight.AmbientLight;

		m_parameter.m_directionalLightDir = cbLight.DirLight_Dir;
		m_parameter.m_directionalLightDir.Normalize();

		m_parameter.m_directionalLightColor = cbLight.DirLight_Color;
	}

	// フォグのパラメータ
	{
		const KdShaderManager::cbFog& cbFog = KdShaderManager::Instance().GetFogCB();

		m_parameter.m_distanceFogColor = cbFog.DistanceFogColor;
		m_parameter.m_distanceFogDensity = cbFog.DistanceFogDensity;

		m_parameter.m_heightFogColor = cbFog.HeightFogColor;
		m_parameter.m_heightFogTopValue = cbFog.HeightFogTopValue;
		m_parameter.m_heightFogBottomValue = cbFog.HeightFogBottomValue;
		m_parameter.m_heightFogBeginDistance = cbFog.HeightFogBeginDistance;
	}

	// 影用深度バッファ生成情報の初期化
	SetDirLightShadowArea({ 25.0f, 25.0f }, 30.0f);

	// 光のパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteLightParams();
	WriteEffectParams();

	// フォグのパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteFogParams();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 点光源は毎フレーム登録する形なので0クリアする
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::Update()
{
	m_pointLights.clear();

	m_coneLights.clear();

}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 描画の直前の更新が終わったタイミングで実施
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::Draw()
{
	// 光のパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteLightParams();

	// フォグのパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteFogParams();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ポイントライトの追加
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::AddPointLight(const Math::Vector3& Color, float Radius, const Math::Vector3& Pos, bool IsBright)
{
	m_pointLights.push_back(PointLight(Color, Radius, Pos, IsBright));
}

void KdAmbientController::AddPointLight(const PointLight& pointLight)
{
	m_pointLights.push_back(pointLight);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 平行光影の生成エリアの設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetDirLightShadowArea(const Math::Vector2& lightingArea, float dirLightHeight)
{
	m_shadowProj = DirectX::XMMatrixOrthographicLH(lightingArea.x, lightingArea.x, 0, dirLightHeight * 2.0f);

	m_dirLightHeight = dirLightHeight;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 平行光の設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetDirLight(const Math::Vector3& dir, const Math::Vector3& col)
{
	m_parameter.m_directionalLightDir = dir;
	m_parameter.m_directionalLightDir.Normalize();

	m_parameter.m_directionalLightColor = col;

	m_dirtyLightDir = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 環境光の設定
// アルファ値の変更でデフォルトの暗さを指定できる。0にした場合は点光源の周囲以外は真っ暗
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetAmbientLight(const Math::Vector4& col)
{
	m_parameter.m_ambientLightColor = col;

	m_dirtyLightAmb = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// フォグの有効/無効をそれぞれ切り替える
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetFogEnable(bool distance, bool height)
{
	KdShaderManager::Instance().WriteCBFogEnable(distance, height);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 距離フォグの設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetDistanceFog(const Math::Vector3& col, float density)
{
	m_parameter.m_distanceFogColor = col;

	m_parameter.m_distanceFogDensity = density;

	m_dirtyFogDist = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 高さフォグの設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetheightFog(const Math::Vector3& col, float topValue, float bottomValue, float distance)
{
	m_parameter.m_heightFogColor = col;

	m_parameter.m_heightFogTopValue = topValue;

	m_parameter.m_heightFogBottomValue = bottomValue;

	m_parameter.m_heightFogBeginDistance = distance;

	m_dirtyFogHeight = true;
}

void KdAmbientController::SetConeLightEnable(bool _enable)
{
	KdShaderManager::Instance().WriteCBConeLightEnable(_enable);
}

void KdAmbientController::SetConeLight(const Math::Vector3& _pos, const Math::Vector3& _dir, const float _range, const float _angle, const Math::Vector3& _color)
{
	/*m_parameter.m_coneLightPos = _pos;
	m_parameter.m_coneLightDir = _dir;
	m_parameter.m_coneLightRange = _range;
	m_parameter.m_coneLightAngle = _angle;
	m_parameter.m_coneLightColor = _color;

	m_dirtyConeLight = true;*/

	//m_drityConeLight = true; //did not declare data
}

void KdAmbientController::AddConeLight(Math::Vector3 pos, Math::Vector3 dir, float range, float angle, Math::Vector3 color)
{
	//if (m_coneLights.size() >= 17)
		//return; 

	ConeLight cone;
	cone.enable = 1;
	cone.pos = pos;
	cone.dir = dir;
	cone.range = range;
	cone.angle = angle;
	cone.color = color;

	m_coneLights.push_back(cone);
	m_dirtyConeLight = true;

}

void KdAmbientController::AddColorLight(const Math::Vector3& pos, float radius, const Math::Vector3& _color)
{
	if (m_colorLights.size() >= 17)
		return; // Optionally log or handle overflow

	ColorLight color;
	color.enable = 1;
	color.pos = pos;
	color.radius = radius;
	color.color = _color;


	m_colorLights.push_back(color);
	m_dirtyColorLight = true;
}

void KdAmbientController::SetSingleLightEnable(bool _enable)
{
	KdShaderManager::Instance().WriteCBSingleLightEnable(_enable);

}

void KdAmbientController::SetSingleLight(const Math::Vector3& _pos, const Math::Vector3& _dir, const float _range, const float _angle, const Math::Vector3& _color)
{
	m_parameter.m_singleLightPos = _pos;
	m_parameter.m_singleLightDir = _dir;
	m_parameter.m_singleLightRange = _range;
	m_parameter.m_singleLightAngle = _angle;
	m_parameter.m_singleLightColor = _color;

	m_dirtySingleLight = true;
}

void KdAmbientController::SetSingleLight2Enable(bool _enable)
{
	KdShaderManager::Instance().WriteCBSingleLightEnable2(_enable);

}

void KdAmbientController::SetSingleLight2(const Math::Vector3& _pos, const Math::Vector3& _dir, const float _range, const float _angle, const Math::Vector3& _color)
{
	m_parameter.m_singleLight2Pos = _pos;
	m_parameter.m_singleLight2Dir = _dir;
	m_parameter.m_singleLight2Range = _range;
	m_parameter.m_singleLight2Angle = _angle;
	m_parameter.m_singleLight2Color = _color;

	m_dirtySingleLight2 = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 光関連パラメータの書き込み
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::WriteLightParams()
{
	// 環境光
	if (m_dirtyLightAmb)
	{
		KdShaderManager::Instance().WriteCBAmbientLight(m_parameter.m_ambientLightColor);

		m_dirtyLightAmb = false;
	}

	// 平行光
	if (m_dirtyLightDir)
	{
		KdShaderManager::Instance().WriteCBDirectionalLight(
			m_parameter.m_directionalLightDir, m_parameter.m_directionalLightColor);

		m_dirtyLightDir = false;
	}

	// 点光源
	if (m_pointLights.size())
	{
		KdShaderManager::Instance().WriteCBPointLight(m_pointLights);
	}

	//黒崎先生
	//ConeLight
	if (m_dirtyConeLight) {
		KdShaderManager::Instance().WriteCBConeLights(m_coneLights);
		m_dirtyConeLight = false;
	}

	/*if (!m_coneLights.empty())
	{
		KdShaderManager::Instance().WriteCBConeLights(m_coneLights);
	}*/

	/*if (m_coneLights.size()) {
		KdShaderManager::Instance().WriteCBConeLights(m_coneLights);
	}*/


	if (m_dirtySingleLight) {
		KdShaderManager::Instance().WriteCBSingleLight(m_parameter.m_singleLightPos, m_parameter.m_singleLightDir, m_parameter.m_singleLightRange, m_parameter.m_singleLightAngle, m_parameter.m_singleLightColor);
		m_dirtySingleLight = false;
	}

	//singleLight2
	if (m_dirtySingleLight2) {
		KdShaderManager::Instance().WriteCBSingleLight2(m_parameter.m_singleLight2Pos, m_parameter.m_singleLight2Dir, m_parameter.m_singleLight2Range, m_parameter.m_singleLight2Angle, m_parameter.m_singleLight2Color);
		m_dirtySingleLight2 = false;
	}



	// 影描画エリアの更新
	KdShaderManager::Instance().WriteCBShadowArea(m_shadowProj, m_dirLightHeight);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// フォグ関連パラメータの書き込み
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::WriteFogParams()
{
	// 距離フォグ
	if (m_dirtyFogDist)
	{
		KdShaderManager::Instance().WriteCBDistanceFog(m_parameter.m_distanceFogColor,
			m_parameter.m_distanceFogDensity);

		m_dirtyFogDist = false;
	}

	// 高さフォグ
	if (m_dirtyFogHeight)
	{
		KdShaderManager::Instance().WriteCBHeightFog(m_parameter.m_heightFogColor,
			m_parameter.m_heightFogTopValue, m_parameter.m_heightFogBottomValue,
			m_parameter.m_heightFogBeginDistance);

		m_dirtyFogHeight = false;
	}
}

void KdAmbientController::WriteEffectParams()
{
	/*if (m_dirtyColorLight)
	{
		KdShaderManager::Instance().WriteCBColor(m_colorLights);
		m_dirtyColorLight = false;
	}*/

	if (!m_colorLights.empty())
	{
		KdShaderManager::Instance().WriteCBColor(m_colorLights);
	}

}
