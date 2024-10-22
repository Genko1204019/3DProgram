//KdStandardShader_PS_Lit.hlsl file: (pixel shader)

#include "inc_KdStandardShader.hlsli"
#include "../inc_KdCommon.hlsli"

// モデル描画用テクスチャ
Texture2D g_baseTex : register(t0); // ベースカラーテクスチャ
Texture2D g_metalRoughTex : register(t1); // メタリック/ラフネステクスチャ
Texture2D g_emissiveTex : register(t2); // 発光テクスチャ
Texture2D g_normalTex : register(t3); // 法線マップ

//water shader 6  since 4 - 9 is empty slot  , use one slot to send normal tex for water , also height map
Texture2D g_WaterNormalTex : register(t9); //  法線マップ

// 特殊処理用テクスチャ
Texture2D g_dirShadowMap : register(t10); // 平行光シャドウマップ
Texture2D g_dissolveTex : register(t11); // ディゾルブマップ
Texture2D g_environmentTex : register(t12); // 反射景マップ

// サンプラ
SamplerState g_ss : register(s0); // 通常のテクスチャ描画用
SamplerComparisonState g_ssCmp : register(s1); // 補間用比較機能付き

float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
	float3 H = normalize(-lightDir + vCam);
	float NdotH = saturate(dot(normal, H)); // カメラの角度差(0～1)
	float spec = pow(NdotH, specPower);

	// 正規化Blinn-Phong
	return spec * ((specPower + 2) / (2 * 3.1415926535));
}

//alphaRa 1
static const int spraMatrix[8][8] =
{
	{ 0, 0, 0, 1, 1, 0, 0, 0 },
	{ 0, 0, 0, 2, 2, 1, 0, 0 },
	{ 0, 0, 2, 3, 3, 2, 1, 0 },
	{ 1, 2, 3, 4, 4, 3, 2, 1 },
	{ 1, 2, 3, 4, 4, 3, 2, 1 },
	{ 0, 1, 2, 3, 3, 2, 1, 0 },
	{ 0, 0, 1, 2, 2, 1, 0, 0 },
	{ 0, 0, 0, 1, 1, 0, 0, 0 }
	
};




//================================
// ピクセルシェーダ
//================================
float4 main(VSOutput In) : SV_Target0
{

	//return float4(g_ConeLight.color,1); //黒崎先生

	// ディゾルブによる描画スキップ
	float discardValue = g_dissolveTex.Sample(g_ss, In.UV).r;
	if (discardValue < g_dissolveValue)
	{
		discard;
	}
	
	//------------------------------------------
	// 材質色
	//------------------------------------------
	float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_BaseColor * In.Color;
	
	// Alphaテスト
	if (baseColor.a < 0.05f)
	{
		discard;
	}
	
	// カメラへの方向
	float3 vCam = g_CamPos - In.wPos;
	float camDist = length(vCam); // カメラ - ピクセル距離
	vCam = normalize(vCam);

	//alphaRa 2
	{
		// fmod = [%]あまり求める遠算し
		//e.g pos x 200 = 0
		//but pos y 103 = 3
		// In : mean pixel info , Pos = 2d coordinate
		
		int x = (int) fmod(In.Pos.x, 8);
		int y = (int) fmod(In.Pos.y, 8);
		float dither = spraMatrix[x][y];
		//ditherの値を0 - 8　の範囲に算出
		dither = dither / 5;

		//テイザ抜きするカメラからの距離
		float ditherDist = 5;

		//CamDist :カメラからみた今からかこうとしているpixelの距離

		float range = max(0, camDist - ditherDist); //max function = the following calculation 
		//if (0 > camDist - ditherDist)	int a = 0;
		//else							int a  = camDist - ditherDist;

		//割合算出
		float rate = 1 - min(1, range);

		//if (dither - 1 * rate < 0)
		if (dither - 0.9 * rate < 0)
		{
			//discard; //pixelを破棄, just like return
		}
		
	}
	

	//float wN = g_normalTex.Sample(g_ss, In.UV).rgb;
	// UV座標（0～1）から 射影座標（-1～1）へ変換
	//wN = wN * 2.0 - 1.0;
	
	//water shader 7
	float3 wN = 0;
	if (g_waterEnable)
	{
		float4 n1 = g_WaterNormalTex.Sample(g_ss, In.UV + g_waterUVOffset);
		n1 = n1 * 2 - 1;

		float4 n2 = g_WaterNormalTex.Sample(g_ss, In.UV - g_waterUVOffset);
		n2 = n2 * 2 - 1;

		wN = mul(n1 + n2, g_mR).rgb;

		//under water effect
		//baseColor = g_baseTex.Sample(g_ss, In.UV + wN.xz) * g_BaseColor * In.Color;
		baseColor = g_baseTex.Sample(g_ss, In.UV + wN.xz / 50) * g_BaseColor * In.Color;

	}
	else
	{
		// 法線マップから法線ベクトル取得
		wN = g_normalTex.Sample(g_ss, In.UV).rgb;

		// UV座標（0～1）から 射影座標（-1～1）へ変換
		wN = wN * 2.0 - 1.0;
	}


	
	{
		// 3種の法線から法線行列を作成
		row_major float3x3 mTBN =
		{
			normalize(In.wT),
			normalize(In.wB),
			normalize(In.wN),
		};
	
		// 法線ベクトルをこのピクセル空間へ変換
		wN = mul(wN, mTBN);
	}

	// 法線正規化
	wN = normalize(wN);

	float4 mr = g_metalRoughTex.Sample(g_ss, In.UV);
	// 金属性
	float metallic = mr.b * g_Metallic;
	// 粗さ
	float roughness = mr.g * g_Roughness;
	// ラフネスを逆転させ「滑らか」さにする
	float smoothness = 1.0 - roughness;
	float specPower = pow(2, 11 * smoothness); // 1～2048
	
	//------------------------------------------
	// ライティング
	//------------------------------------------
	// 最終的な色
	float3 outColor = 0;
	
		// 材質の拡散色　非金属ほど材質の色になり、金属ほど拡散色は無くなる
	const float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);
		// 材質の反射色　非金属ほど光の色をそのまま反射し、金属ほど材質の色が乗る
	const float3 baseSpecular = lerp(0.04, baseColor.rgb, metallic);

	//-------------------------------
	// シャドウマッピング(影判定)
	//-------------------------------
	float shadow = 1;

	// ピクセルの3D座標から、DepthMapFromLight空間へ変換
	float4 liPos = mul(float4(In.wPos, 1), g_DL_mLightVP);
	liPos.xyz /= liPos.w;

	// 深度マップの範囲内？
	if (abs(liPos.x) <= 1 && abs(liPos.y) <= 1 && liPos.z <= 1)
	{
		// 射影座標 -> UV座標へ変換
		float2 uv = liPos.xy * float2(1, -1) * 0.5 + 0.5;
		// ライトカメラからの距離
		float z = liPos.z - 0.004; // シャドウアクネ対策
		
		// 画像のサイズからテクセルサイズを求める
		float w, h;
		g_dirShadowMap.GetDimensions(w, h);
		float tw = 1.0 / w;
		float th = 1.0 / h;
	
		// uvの周辺3x3も判定し、平均値を求める
		shadow = 0;
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				shadow += g_dirShadowMap.SampleCmpLevelZero(g_ssCmp, uv + float2(x * tw, y * th), z);
			}
		}
		shadow *= 0.11;
	}
		
	//-------------------------
	// 平行光
	//-------------------------
	// Diffuse(拡散光)
	{

		
		// 光の方向と法線の方向との角度さが光の強さになる
		float lightDiffuse = dot(-g_DL_Dir, wN);
		lightDiffuse = saturate(lightDiffuse); // マイナス値は0にする　0(暗)～1(明)になる


		
		//toon shader 1
		//if (lightDiffuse < 0.3)
		//{
		//	lightDiffuse = 0.3;
		//}
		//else if (lightDiffuse < 0.7)
		//{
		//	lightDiffuse = 0.7;
		//}
		//else
		//{
		//	lightDiffuse = 1.0;
		//}
		
		// 正規化Lambert
		lightDiffuse /= 3.1415926535;

		// 光の色 * 材質の拡散色 * 透明率
		outColor += (g_DL_Color * lightDiffuse) * baseDiffuse * baseColor.a * shadow;
	}

	// Specular(反射色)
	{
		// 反射した光の強さを求める
		// Blinn-Phong NDF
		float spec = BlinnPhong(g_DL_Dir, vCam, wN, specPower);

		////toon shader 2
		//spec = step(1, spec) * 10;
		
		
		// 光の色 * 反射光の強さ * 材質の反射色 * 透明率 * 適当な調整値
		outColor += (g_DL_Color * spec) * baseSpecular * baseColor.a * 0.5 * shadow;
	}

	// 全体の明度：環境光に1が設定されている場合は影響なし
	// 環境光の不透明度を下げる事により、明度ライトの周り以外は描画されなくなる
	float totalBrightness = g_AmbientLight.a;

	//-------------------------
	// 点光
	//-------------------------
	for (int i = 0; i < g_PointLightNum.x; i++)
	{
		// ピクセルから点光への方向
		float3 dir = g_PointLights[i].Pos - In.wPos;
		
		// 距離を算出
		float dist = length(dir);
		
		// 正規化
		dir /= dist;
		
		// 点光の判定以内
		if (dist < g_PointLights[i].Radius)
		{
			// 半径をもとに、距離の比率を求める
			float atte = 1.0 - saturate(dist / g_PointLights[i].Radius);
			
			// 明度の追加
			totalBrightness += (1 - pow(1 - atte, 2)) * g_PointLights[i].IsBright;
			
			// 逆２乗の法則
			atte *= atte;
			
			// Diffuse(拡散光)
			{
				// 光の方向と法線の方向との角度さが光の強さになる
				float lightDiffuse = dot(dir, wN);
				lightDiffuse = saturate(lightDiffuse); // マイナス値は0にする　0(暗)～1(明)になる

				lightDiffuse *= atte; // 減衰

				// 正規化Lambert
				lightDiffuse /= 3.1415926535;

				// 光の色 * 材質の拡散色 * 透明率
				outColor += (g_PointLights[i].Color * lightDiffuse) * baseDiffuse * baseColor.a;
			}

			// Specular(反射色)
			{
				// 反射した光の強さを求める
				// Blinn-Phong NDF
				float spec = BlinnPhong(-dir, vCam, wN, specPower);

				spec *= atte; // 減衰
				
				// 光の色 * 反射光の強さ * 材質の反射色 * 透明率 * 適当な調整値
				outColor += (g_PointLights[i].Color * spec) * baseSpecular * baseColor.a * 0.5;
			}
		}
	}

	//黒崎先生
	//ConeLight
	{
		

		//// Iterate through all active ConeLights
		//for (int i = 0; i < g_ConeLightNum; ++i)
		//{
		//	ConeLight cone = g_ConeLights[i];
		//	if (cone.enable)
		//	{
  //      // Calculate direction from pixel to ConeLight
		//		float3 toDir = normalize(cone.pos - In.wPos);
		//		float distance = length(cone.pos - In.wPos);

  //      // Check if within range
		//		if (distance < cone.range)
		//		{
  //          // Calculate angle between ConeLight direction and pixel direction
		//			float angle = acos(dot(toDir, normalize(-cone.dir)));

  //          // Check if within ConeLight angle
		//			if (angle < cone.angle)
		//			{
  //              // Attenuation based on distance and angle
		//				float distanceAttenuation = 1.0 - (distance / cone.range);
		//				float angleAttenuation = 1.0 - (angle / cone.angle);
		//				float attenuation = saturate(distanceAttenuation * angleAttenuation);

  //              // Diffuse lighting
		//				float lightDiffuse = saturate(dot(toDir, wN));
		//				lightDiffuse *= attenuation / 3.1415926535; // Normalized Lambert

  //              // Accumulate diffuse color
		//				outColor += (cone.color * lightDiffuse) * baseDiffuse * baseColor.a;

  //              // Specular lighting
		//				float spec = BlinnPhong(-toDir, vCam, wN, specPower);
		//				spec *= attenuation;
		//				outColor += (cone.color * spec) * baseSpecular * baseColor.a;
		//			}
		//		}
		//	}
		//}

		
		//Without atten and diffuse
		//for (int i = 0; i < g_ConeLightNum; ++i)
		//{
		//	ConeLight cone = g_ConeLights[i];
		//	if (cone.enable)
		//	{
  //      // Calculate direction from pixel to ConeLight
		//		float3 toDir = normalize(cone.pos - In.wPos);
		//		float distance = length(cone.pos - In.wPos);

  //      // Check if within range
		//		if (distance < cone.range)
		//		{
  //          // Calculate angle between ConeLight direction and pixel direction
		//			float angle = acos(dot(toDir, normalize(-cone.dir)));

  //          // Check if within ConeLight angle
		//			if (angle < cone.angle)
		//			{
		//				outColor += (cone.color) * baseDiffuse * baseColor.a;
		//				outColor += (cone.color ) * baseSpecular * baseColor.a;
		//			}
		//		}
		//	}
		//}


		for (int i = 0; i < g_ConeLightNum; ++i)
		{
			ConeLight cone = g_ConeLights[i];
			if (cone.enable)
			{
				float3 v = cone.pos - In.wPos;
				float d = length(v);
				if (d < cone.range && d > (cone.range - 1.7))
				{
					float3 col = (baseColor.r + baseColor.g + baseColor.b);
					col *= cone.color;
					baseColor.rgb = col;
					
				}

				if (d < cone.angle)
				{
					float3 col = (baseColor.r + baseColor.g + baseColor.b);
					col *= cone.color;
					baseColor.rgb = col;
				}
				
				
			}
		}



		//if (g_SingleLight.enable)
		//{
		//	float3 v = g_SingleLight.pos - In.wPos;
		//	float d = length(v);
		//	if (d < g_SingleLight.range && d > (g_SingleLight.range - 1.7))
		//	{
		//		float3 col = (baseColor.r + baseColor.g + baseColor.b);
		//		col *= g_SingleLight.color;
		//		baseColor.rgb = col;
					
		//	}

		//	if (d < g_SingleLight.angle)
		//	{
		//		float3 col = (baseColor.r + baseColor.g + baseColor.b);
		//		col *= g_SingleLight.color;
		//		baseColor.rgb = col;
		//	}
				
				
		//}

		{
		//光源に向いたベクトルを求める
			float3 toDir = g_SingleLight.pos - In.wPos; //pos for now = In
		//光源の距離
			float len = length(toDir); //hlsl's helper function //float4x4 is Matrix

			if (len < g_SingleLight.range)
			{ // mean within range
			//光源に向いたベクトルを正規化
				toDir = normalize(toDir); // = normalize in hlsl

			//光源の方向ベクトルと光源の逆ベクトルで角度を求める
			//float rad = dot(toDir, -g_ConeLight.dir); // = dot in hlsl, need to clamp
			//float rad = saturate(toDir, -g_ConeLight.dir); // saturate mean limit range to 0 to 1
				float rad = acos(saturate(dot(toDir, -g_SingleLight.dir)));

				if (rad < g_SingleLight.angle)
				{

				//atte for angle
					float angleIn = g_SingleLight.angle * 0.8; //within 80% is full light
					float side = 1.0 - (rad - angleIn) / (g_SingleLight.angle - angleIn); // = saturate in hlsl, limit range to 0 to 1))
				
				//atte for length
					float atte = saturate(1.0 - len / g_SingleLight.range) * side; // = saturate in hlsl, limit range to 0 to 1)
				
				//within angle of cone
				// 光の方向と法線の方向との角度さが光の強さになる
					float lightDiffuse = saturate(dot(normalize(wN), toDir));

					lightDiffuse *= atte; // 減衰

				// 光の色 * 材質の拡散色 * 透明率
				//outColor += (g_ConeLight.color ) * baseDiffuse * baseColor.a; //without attenuation
					outColor += (g_SingleLight.color * lightDiffuse) * baseDiffuse * baseColor.a; //with attenuation

				//add to specular (add to light inten) 
					float spec = BlinnPhong(-toDir, vCam, wN, specPower);
					spec *= atte; // 減衰
				// 光の色 * 反射光の強さ * 材質の反射色 * 透明率
					outColor += (g_SingleLight.color * spec) * baseSpecular * baseColor.a;

				
				}

			}

		}
	

		

	}






	

	outColor += g_AmbientLight.rgb * baseColor.rgb * baseColor.a;
	
	// 自己発光色の適応
	if (g_OnlyEmissie)
	{
		outColor = g_emissiveTex.Sample(g_ss, In.UV).rgb * g_Emissive * In.Color.rgb;
	}
	else
	{
		outColor += g_emissiveTex.Sample(g_ss, In.UV).rgb * g_Emissive * In.Color.rgb;
	}
	
	//------------------------------------------
	// 高さフォグ
	//------------------------------------------
	if (g_HeightFogEnable && g_FogEnable)
	{
		if (In.wPos.y < g_HeightFogTopValue)
		{
			float distRate = length(In.wPos - g_CamPos);
			distRate = saturate(distRate / g_HeightFogDistance);
			distRate = pow(distRate, 2.0);
			
			float heightRange = g_HeightFogTopValue - g_HeightFogBottomValue;
			float heightRate = 1 - saturate((In.wPos.y - g_HeightFogBottomValue) / heightRange);
			
			float fogRate = heightRate * distRate;
			outColor = lerp(outColor, g_HeightFogColor, fogRate);
		}
	}
	
	//------------------------------------------
	// 距離フォグ
	//------------------------------------------
	if (g_DistanceFogEnable && g_FogEnable)
	{
		// フォグ 1(近い)～0(遠い)
		float f = saturate(1.0 / exp(camDist * g_DistanceFogDensity));
		
		// 適用
		outColor = lerp(g_DistanceFogColor, outColor, f);
	}
	
	// ディゾルブ輪郭発光
	if (g_dissolveValue > 0)
	{
		// 閾値とマスク値の差分で、縁を検出
		if (abs(discardValue - g_dissolveValue) < g_dissolveEdgeRange)
		{
			// 輪郭に発光色追加
			outColor += g_dissolveEmissive;
		}
	}
	
	totalBrightness = saturate(totalBrightness);
	outColor *= totalBrightness;
	
	//------------------------------------------
	// 出力
	//------------------------------------------
	return float4(outColor, baseColor.a);
}
