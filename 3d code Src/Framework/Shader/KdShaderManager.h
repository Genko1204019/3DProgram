﻿//KdShaderManager.h file: 

#pragma once

#include "StandardShader/KdStandardShader.h"
#include "PostProcessShader/KdPostProcessShader.h"
#include "SpriteShader/KdSpriteShader.h"

// 点光源データ
struct PointLight
{
	PointLight() {}
	PointLight(const Math::Vector3& color, float radius, const Math::Vector3& pos, int isBright)
		:Color(color), Radius(radius), Pos(pos), IsBright(isBright) {}

	Math::Vector3 Color;	// 色
	float	Radius = 0.0f;	// 半径
	Math::Vector3 Pos;		// 座標
	int		IsBright = 0;	// 明度用ライトかどうか
};

//黒崎先生授業用
//コーンライト
struct ConeLight
{
	//remember to arrange as 16byte alignment

	//16byte
	int enable = 0;				// 有効無効
	Math::Vector3 pos = {};		// 座標

	//16byte
	Math::Vector3 dir = {};		// 方向
	float	range = 0.0f;				// 長さ

	float	angle = 0.0f;		// 角度

	//16byte
	Math::Vector3 color = { 3,3,3 };	// 色

	float deltaAngle;          // 4 bytes
	float pad1[3];             // 8 bytes (padding)



};

struct ColorLight
{
	int enable = 0;            // 4 bytes
	Math::Vector3 pos = {};            // 12 bytes

	Math::Vector3 color = { 3,3,3 };          // 12 bytes
	float radius = 1;          // 4 bytes
};

struct SingleLight
{
	//16byte
	int enable = 0;				// 有効無効
	Math::Vector3 pos = {};		// 座標

	//16byte
	Math::Vector3 dir = {};		// 方向
	float	range = 0.0f;				// 長さ

	float	angle = 0.0f;		// 角度
	//16byte
	Math::Vector3 color = { 3,3,3 };	// 色
};

struct SingleLight2
{
	//16byte
	int enable = 0;				// 有効無効
	Math::Vector3 pos = {};		// 座標

	//16byte
	Math::Vector3 dir = {};		// 方向
	float	range = 0.0f;				// 長さ

	float	angle = 0.0f;		// 角度
	//16byte
	Math::Vector3 color = { 3,3,3 };	// 色
};


//==========================
//
// 各パイプラインステートの呼び出しID
//
//==========================
// 深度情報の影響・書き込みの制御ステート
enum class KdDepthStencilState
{
	ZEnable,		// 手前の物に隠れる　○、自分の奥の物を隠す　○（基本の3D描画ステート
	ZWriteDisable,	// 手前の物に隠れる　○、自分の奥の物を隠す　×
	ZDisable,		// 手前の物に隠れる　×、自分の奥の物を隠す　×
	Max
};

// ポリゴンの面描画の扱い制御ステート
enum class KdRasterizerState
{
	CullNone,		// 描画省略なし
	CullFront,		// 表面描画省略
	CullBack,		// 裏面描画省略
	Max
};

// ピクセルの色合成方法の制御ステート
enum class KdBlendState
{
	Alpha,			// 透明度による色合成
	Add,			// 加算による色合成
	Max
};

// ピクセルの色取得方法の制御ステート
enum class KdSamplerState
{
	Anisotropic_Wrap,	// 異方性補間：テクスチャ繰り返し
	Anisotropic_Clamp,	// 異方性補間：テクスチャ範囲外は端のピクセルを延長
	Linear_Clamp,		// 線形補間：　テクスチャ範囲外は端のピクセルを延長
	Linear_Clamp_Cmp,	// 線形補間：　テクスチャ範囲外は端のピクセルを延長・比較機能付き
	Point_Wrap,			// 補間なし：　テクスチャ繰り返し
	Point_Clamp,		// 補間なし：　テクスチャ範囲外は端のピクセルを延長
	Max
};

//==========================================================
//
// シェーダ関係をまとめたクラス
//
//==========================================================
class KdShaderManager
{
public:

	// カメラ定数バッファ用構造体
	struct cbCamera
	{
		Math::Matrix	mView;		// ビュー行列
		Math::Matrix	mProj;		// 射影行列
		Math::Matrix	mProjInv;	// 射影行列：逆行列

		Math::Vector3	CamPos;		// カメラのワールド座標
		int				blank = 0;
	};

	// フォグ定数バッファ用構造体
	struct cbFog
	{
		int				DistanceFogEnable = 0;			// 距離フォグ有効
		Math::Vector3	DistanceFogColor = { 1.0f, 1.0f, 1.0f };// 距離フォグ色

		float			DistanceFogDensity = 0.001f;	// 距離フォグ減衰率
		float			HeightFogTopValue = 5.0f;		// 高さフォグエリアの最高位置
		float			HeightFogBottomValue = -5.0f;	// 高さフォグエリアの最低位置
		float			HeightFogBeginDistance = 0.0f;	// 高さフォグの開始の距離

		Math::Vector3	HeightFogColor = { 1.0f, 1.0f, 1.0f };// 高さフォグ色
		int				HeightFogEnable = 0;			// 高さフォグ有効
	};

	// ライト定数バッファ用構造体
	struct cbLight
	{
		static const int	MaxPointLightNum = 100;
		static const int	MaxConeLightNum = 17;

		// 環境光
		Math::Vector4		AmbientLight = { 0.3f, 0.3f, 0.3f, 1.0f };

		// 平行光
		Math::Vector3		DirLight_Dir = { 1, -1, 1 };			// 光の方向
		float				_blank1 = 0.0f;
		Math::Vector3		DirLight_Color = { 2.25f, 2.25f, 2.25f };	// 光の色
		float				_blank2 = 0.0f;

		Math::Matrix		DirLight_mVP;					// ビュー行列と正射影行列の合成行列

		//-----------------
		// 点光
		//-----------------
		// 使用数
		int				PointLight_Num = 0;
		float			_blank3[3] = { 0.0f, 0.0f ,0.0f };

		std::array<PointLight, MaxPointLightNum> PointLights;

		//黒崎先生授業用
		//ConeLight ConeLights;	 //only one cone light now,alter to add more cone
		//ConeLight ConeLights; // Only one ConeLight

		/*int				ConeLight_Num = 0;
		float			_blank3[3] = { 0.0f, 0.0f ,0.0f };
		std::array<ConeLight, MaxCointLightNum> CointLights;*/

		// Cone lights
		int ConeLight_Num = 0;
		float _blank4[3] = { 0.0f, 0.0f, 0.0f }; // Padding for alignment
		std::array<ConeLight, MaxConeLightNum> ConeLights;


		SingleLight SingleLight;
		SingleLight2 SingleLight2;

	};

	struct cdEffect
	{
		static const int	MaxColorLightNum = 17;
		int ColorLight_Num = 0;
		float _blank4[3] = { 0.0f, 0.0f, 0.0f }; // Padding for alignment
		std::array<ColorLight, MaxColorLightNum> ColorLights;

	};


	static KdShaderManager& Instance()
	{
		static KdShaderManager instance;
		return instance;
	}

	// シェーダー本体の生成・シェーダーで利用する定数バッファの生成・影用の光からの深度情報テクスチャを生成
	void Init();

	//==========================
	//
	// シェーダ
	//
	//==========================
	KdStandardShader		m_StandardShader;		// 標準描画シェーダ
	KdPostProcessShader		m_postProcessShader;	// ポストプロセスシェーダ
	KdSpriteShader			m_spriteShader;			// 2Dテクスチャ描画シェーダ

	//==========================
	//
	// 描画パイプライン系の設定
	//
	//==========================
	// パイプラインの設定
	bool SetVertexShader(ID3D11VertexShader* pSetVS);
	bool SetPixelShader(ID3D11PixelShader* pSetPS);

	// 入力レイアウトの設定
	bool SetInputLayout(ID3D11InputLayout* pSetLayout);

	// 定数バッファの設定
	bool SetVSConstantBuffer(int startSlot, ID3D11Buffer* const* pSetBuffer);
	bool SetPSConstantBuffer(int startSlot, ID3D11Buffer* const* pSetBuffer);

	//==========================
	//
	// パイプラインステートの変更
	//
	//==========================
	// 深度情報をどのように扱うかを変更：変更したかどうかが返ってくる
	void ChangeDepthStencilState(KdDepthStencilState stateId);
	void UndoDepthStencilState();

	// ポリゴンの面をどのように描画するのかを変更：変更したかどうかが返ってくる
	void ChangeRasterizerState(KdRasterizerState stateId);
	void UndoRasterizerState();

	// 画面の色をどのように合成するのかを変更：変更したかどうかが返ってくる
	void ChangeBlendState(KdBlendState stateId);
	void UndoBlendState();

	// テクスチャの色をどのように取ってくるのかを変更：変更したかどうかが返ってくる
	void ChangeSamplerState(KdSamplerState stateId, int slot = 0);
	void UndoSamplerState(int slot = 0);

	//==========================
	//
	// 定数バッファ系
	//
	//==========================
	const cbCamera& GetCameraCB() const { return m_cb7_Camera.Get(); }

	const cbFog& GetFogCB() const { return m_cb8_Fog.Get(); }

	const cbLight& GetLightCB() const { return m_cb9_Light.Get(); }

	void WriteCBCamera(const Math::Matrix& cam, const Math::Matrix& proj);

	void WriteCBFogEnable(bool distance, bool height);
	void WriteCBDistanceFog(const Math::Vector3& col, float density = 0.001f);
	void WriteCBHeightFog(const Math::Vector3& col, float top, float bottom, float beginDistance);

	void WriteCBAmbientLight(const Math::Vector4& col);
	void WriteCBDirectionalLight(const Math::Vector3& dir, const Math::Vector3& col);
	void WriteCBShadowArea(const Math::Matrix& proj, float dirLightHeight);
	void WriteCBPointLight(const std::list<PointLight>& pointLights);

	//黒崎先生授業用
	void WriteCBConeLightEnable(bool _enable); //add & in class Vector3 to prevent copy of data in stack , add & when copy class when define argument for function
	void WriteCBConeLights(const std::list<ConeLight>& coneLights); //send from cpu to gpu

	void WriteCBColorEnable(bool _enable); //& = will not be copy , const = can't be changed
	void WriteCBColor(const std::list<ColorLight>& colorLights);

	void WriteCBSingleLightEnable(bool _enable);
	void WriteCBSingleLight(const Math::Vector3& _pos, const Math::Vector3& _dir, const float _range, const float _angle, const Math::Vector3& _color = { 3,3,3 });

	void WriteCBSingleLightEnable2(bool _enable);
	void WriteCBSingleLight2(const Math::Vector3& _pos, const Math::Vector3& _dir, const float _range, const float _angle, const Math::Vector3& _color = { 3,3,3 });


	//==========================
	//
	// その他
	//
	//==========================

	bool IsPixelArtStyle() const { return m_pixelArtStyle; }

	const KdAmbientController& GetAmbientController() const { return m_ambientController; }
	KdAmbientController& WorkAmbientController() { return m_ambientController; }

	// 解放
	void Release();

	void CallImgui();
	
	//KdAmbientController m_ambientController;  // Member of KdShaderManager

private:

	KdShaderManager() {}

	~KdShaderManager() {}

	// カメラ定数バッファ
	KdConstantBuffer<cbCamera>	m_cb7_Camera;

	// フォグ定数バッファ
	KdConstantBuffer<cbFog>	m_cb8_Fog;

	// ライト定数バッファ
	KdConstantBuffer<cbLight>	m_cb9_Light;

	KdConstantBuffer<cdEffect> m_cb10_Effect;


	KdAmbientController m_ambientController;



	bool m_pixelArtStyle = true;

	//==========================
	//
	// パイプラインステート
	//
	//==========================

	//深度ステンシル（奥行情報の使い方・手前にあるものを無視して描画したりできる
	ID3D11DepthStencilState* m_depthStencilStates[(int)KdDepthStencilState::Max] = {};
	std::stack<ID3D11DepthStencilState*> m_ds_Undo;

	// ラスタライズステート（ポリゴンの任意の面の描画を省略できる、処理を軽減する目的で運用する事が多い
	ID3D11RasterizerState* m_rasterizerStates[(int)KdRasterizerState::Max] = {};
	std::stack<ID3D11RasterizerState*> m_rs_Undo;

	// ブレンドステート（ピクセルの最終色を決めるときに既に塗られている色と、どう合成するのかの選択ができる
	ID3D11BlendState* m_blendStates[(int)KdBlendState::Max] = {};
	std::stack<ID3D11BlendState*> m_bs_Undo;

	// サンプラーステート（テクスチャのピクセル色を取得する際にどのような補間や扱いで取得するかを選択できる
	ID3D11SamplerState* m_samplerStates[(int)KdSamplerState::Max] = {};
	std::stack<ID3D11SamplerState*> m_ss_Undo;
};