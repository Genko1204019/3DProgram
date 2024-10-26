#include"GameCamera.h"
#include"Application/GameObject/Player/Player.h"
#include"Application/GameObject/MapObj/MapObj.h"
#include"Application/GameObject/Enemy/BattleEnemy/BattleEnemy.h"
#include"Application/Scene/SceneManager.h"
#include"Application/Manager/UiManager.h"

void GameCamera::Init()
{
	CameraBase::Init();

	pos = { 0,4,-10 };

	debugWire = std::make_unique<KdDebugWireFrame>();

}

void GameCamera::Update()
{
	CameraBase::Update();

	EnemyInfo();
	PlayerInfo();

	
	CameraEffect();
	ScreenShake();

	DoCameraMove();

	CameraTwist();

	ReturnToFollow();
	CounterManager();

	DebugKey();

}

void GameCamera::PreDraw()
{
	CameraBase::PreDraw();
}

void GameCamera::PostUpdate()
{
	UpdateMatrix();
	UpdateCameraRotation();

	
}



void GameCamera::UpdateCameraRotation()
{


	if (isLockOn) EnemyLockOn();	
	else 	RotateWithMouse();
	

	
}

void GameCamera::EnemyLockOn()
{
	camToEnemy = enemyPos - playerPos;
	camToEnemy.Normalize();

	float yaw = atan2(camToEnemy.x, camToEnemy.z);
	float pitch = atan2(camToEnemy.y, sqrt(camToEnemy.x * camToEnemy.x + camToEnemy.z * camToEnemy.z));
	m_DegAng.y = DirectX::XMConvertToDegrees(yaw);

	m_DegAng.x = 18;
}

void GameCamera::RotateWithMouse()
{
	if (!GetAsyncKeyState('Z') && !isMouseMode) {
		// マウスでカメラを回転させる処理
		POINT _nowPos;
		GetCursorPos(&_nowPos);

		POINT _mouseMove{};
		_mouseMove.x = _nowPos.x - m_FixMousePos.x;
		_mouseMove.y = _nowPos.y - m_FixMousePos.y;

		SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

		// 実際にカメラを回転させる処理(0.15はただの補正値)
		m_DegAng.x += _mouseMove.y * 0.15f;
		m_DegAng.y += _mouseMove.x * 0.15f;

		// 回転制御
		m_DegAng.x = std::clamp(m_DegAng.x, -14.f, 42.f);
	}
}

void GameCamera::CounterManager()
{
	standStillCnt--;

}

void GameCamera::CameraEffect()
{
	//WorkCamera()->SetFocus(focusDist, focusForRange, focusBackRange);
	WorkCamera()->SetProjectionMatrix(camFov);
	KdShaderManager::Instance().m_postProcessShader.SetBrightThreshold(camBrightness);

}

void GameCamera::PlayerInfo()
{
	const shared_ptr<Player> spPlayer = wpPlayer.lock(); 
	if (spPlayer) {
		playerPos = spPlayer->GetPosition();
		playerTransMat = Matrix::CreateTranslation(playerPos);
		isLockOn = spPlayer->GetisLockOn();
	}
}

void GameCamera::EnemyInfo()
{
	const shared_ptr<BattleEnemy> spEnemy = wpEnemy.lock();
	if (spEnemy) {
		enemyPos = spEnemy->GetPosition();
		enemyTransMat = Matrix::CreateTranslation(enemyPos);
	}

}

void GameCamera::UpdateMatrix()
{

	float x = DirectX::XMConvertToRadians(rotOffset.x);
	float y = DirectX::XMConvertToRadians(rotOffset.y);
	float z = DirectX::XMConvertToRadians(rotOffset.z);
	localRotMat = Matrix::CreateFromYawPitchRoll(y, x, z);

	localPosMat = Matrix::CreateTranslation(posOffset);
	rotMat = GetRotationMatrix();

	
	switch (nowState)
	{
	case CamMode::FollowMode:
		worldMat = localPosMat * localRotMat * rotMat * playerTransMat;
		break;
	case CamMode::DialogMode:
		worldMat = localPosMat * localRotMat;
		break;
	case CamMode::BattleSceneMode:
		worldMat = localPosMat * localRotMat;
		if (cutSceneCnt < 0 && !isCamMoving) {
			PlanCameraMove(observeEnemyPos, { 7,-9,-2.1 }, { 0,0,0 }, { 0,0,0 },40); 
			m_DegAng = { 2,-1.8,0 };
			UiManager::Instance().showBattleStart = true;
		}
		break;
	case CamMode::StandStillMode:
		worldMat = localPosMat * localRotMat;
		break;
	case CamMode::WinCutSceneMode:
		worldMat = localPosMat * localRotMat;
		break;


	default:
		break;
	}

	cutSceneCnt--;

	alphaMat = Matrix::CreateTranslation(posOffset) * rotMat * playerTransMat;


}

void GameCamera::ScreenShake()
{
	if (isScreenShake) {
		screenShakeTime += (1.0f / 60.0f); // Assuming your game runs at 60 FPS

		if (screenShakeTime < screenShakeDuration) {
			float shakeX = Utility::Rnd(-shakeIntensity, shakeIntensity);
			float shakeY = Utility::Rnd(-shakeIntensity, shakeIntensity);
			float shakeZ = Utility::Rnd(-shakeIntensity, shakeIntensity);

			posOffset.x = originalPosOffset.x + shakeX;
			posOffset.y = originalPosOffset.y + shakeY;
			posOffset.z = originalPosOffset.z + shakeZ;
		}
		else {
			posOffset = Vector3::Lerp(posOffset, originalPosOffset, 0.1f); 
			rotOffset = Vector3::Lerp(rotOffset, originalRotOffset, 0.1f); 

			if ((posOffset - originalPosOffset).Length() < 0.01f && (rotOffset - originalRotOffset).Length() < 0.01f) {
				posOffset = originalPosOffset; 
				rotOffset = originalRotOffset; 
				isScreenShake = false;
				screenShakeTime = 0.0f;
			}
		}
	}
}

void GameCamera::StartScreenShake(float _duration)
{
	screenShakeDuration = _duration;
	originalPosOffset = posOffset;
	originalRotOffset = rotOffset;
	isScreenShake = true;
	screenShakeTime = 0.0f;
}

void GameCamera::StartBattleCutScene()
{
	nowState = CamMode::BattleSceneMode;
	posOffset = { observeEnemyPos };
	cutSceneCnt = 280;

}

void GameCamera::StartWinCutScene()
{
	cutSceneCnt = 210;
	nowState = CamMode::WinCutSceneMode;
	posOffset = observePlayerPos;
	rotOffset = { 0,0,0 };
}

void GameCamera::PlanCameraMove(Vector3 _startPos, Vector3 _endPos, Vector3 _startRot, Vector3 _endRot, float _duration)
{
	posOffset = _startPos;
	rotOffset = _startRot;

	targetPos = _endPos;
	targetRot = _endRot;

	moveDuration = _duration;
	cutSceneProcess++;
	isCamMoving = true;
}

void GameCamera::DoCameraMove()
{
	if (isCamMoving) {
		moveElapsed += (1.0f / 60.0f);
		float time = min(moveElapsed / moveDuration, 1.0f);
		float dist = (targetPos - posOffset).Length();

		posOffset = Vector3::Lerp(posOffset, targetPos, time);
		rotOffset = Vector3::Lerp(rotOffset, targetRot, time);

		if (dist < 0.1f){
			moveElapsed = 0;
			isCamMoving = false;
			nowState = CamMode::FollowMode;
			posOffset = originalPosOffset;
			rotOffset = originalRotOffset;
		}
	}

}

void GameCamera::CameraTwist()
{
	if (isTwisting)
	{
		// Increment the elapsed time
		twistElapsed += (1.0f / 60.0f);

		//decrease brightness
		camBrightness -= 0.021f;

		// Normalize the elapsed time
		float normalizedTime = twistElapsed / twistDuration;

		// Whirlpool effect intensity (twist angle grows with time)
		float maxTwist = DirectX::XM_PI / 2.0f;  // 90 degrees max twist
		twistAngle = maxTwist * normalizedTime;

		// End the twist after the duration
		if (twistElapsed >= twistDuration)
		{
			twistAngle = 0.0f;   // Reset the twist
			isTwisting = false;  // Stop twisting
			camFov = 60;
			camBrightness = 1.0f;

			GameManager::Instance().StartBattle();


		}

		// Generate a whirlpool twist effect by adjusting the projection matrix
		// Modify the projection matrix with a rotation that increases towards the screen edges
		camFov += 0.4;
		float fov = DirectX::XMConvertToRadians(camFov);
		float aspect = KdDirect3D::Instance().GetBackBuffer()->GetAspectRatio();
		float nearZ = 0.01f;
		float farZ = 2000.0f;

		// Create the perspective matrix
		Matrix perspective = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);

		// Create a twist matrix that rotates more as you move away from the center
		Matrix twistMat = Matrix::Identity;
		float x, y;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i != j) {
					x = i - 1.5f;
					y = j - 1.5f;
					float distFromCenter = sqrtf(x * x + y * y);
					float angle = twistAngle * distFromCenter; // Increase twist based on distance
					twistMat *= Matrix::CreateRotationZ(angle);
				}
			}
		}

		// Apply the twist to the perspective matrix
		Matrix m_mProj =twistMat * perspective;

		// Set the updated projection matrix
		WorkCamera()->SetProjectionMatrix(m_mProj);
	}
}

void GameCamera::StartCameraTwist()
{
	twistElapsed = 0.0f;   
	isTwisting = true;     
	twistAngle = 0.0f;     
	camFov = 60;
}


void GameCamera::StartStandStill()
{
	standStillDuration = 210;
	standStillCnt = standStillDuration;
	nowState = CamMode::StandStillMode;
	isStandStill = true;
	posOffset = standStillPos;
	
}

void GameCamera::ReturnToFollow()
{
	if (isStandStill && standStillCnt < 0) {
		isStandStill = false;
		nowState = CamMode::FollowMode;
	}

}

void GameCamera::OpenInventoryMode()
{
	m_DegAng.x = 14;
	m_DegAng.y = -5;
	
}

void GameCamera::DebugKey()
{
	if (InputManager::Instance().IsKeyJustPressed(OKey)) isMouseMode = !isMouseMode;
}



void GameCamera::CallImgui()
{
	//text GameCamera
	ImGui::Begin("Camera");

	
	if (ImGui::Button("StandStill")) {
		StartStandStill();
	}

	//slider float observeEnemyPos .xyz
	ImGui::SliderFloat("observeEnemyPos.x", &observeEnemyPos.x, -20.0f, 20.0f);
	ImGui::SliderFloat("observeEnemyPos.y", &observeEnemyPos.y, -20.0f, 20.0f);
	ImGui::SliderFloat("observeEnemyPos.z", &observeEnemyPos.z, -20.0f, 20.0f);

	//show cutSceneCnt  
	ImGui::Text("cutSceneCnt:%.2f", cutSceneCnt);
	//show isCamMoving
	ImGui::Text("isCamMoving:%d", isCamMoving);

	//sliderint cameraMode 0 -7 :
	ImGui::SliderInt("camMode", &nowState, 0, 5);


	//show debug related to cam twist
	ImGui::Text("isTwisting:%d", isTwisting);
	ImGui::Text("twistAngle:%.2f", twistAngle);
	ImGui::Text("twistElapsed:%.2f", twistElapsed);
	ImGui::Text("twistDuration:%.2f", twistDuration);
	ImGui::Text("camFov:%.2f", camFov);


	//check iscamMoveing
	ImGui::Checkbox("isCamMoving", &isCamMoving);
	//show cameraMode
	ImGui::Text("cameraMode:%d", nowState);
	
	//slider shakeIntensity
	ImGui::SliderFloat("shakeIntensity", &shakeIntensity, 0.0f, 1.0f);
	//slider screenShakeDuration 0 - 5
	ImGui::SliderFloat("screenShakeDuration", &screenShakeDuration, 0.0f, 5.0f);

	//checkbox isScreenShake
	//if button Shake , call StartScreenShake
	if (ImGui::Button("Shake")) {
		StartScreenShake();
	}

	//show startPos endPos 
	ImGui::Text("startPos x:%.2f y:%.2f z:%.2f", startPos.x, startPos.y, startPos.z);
	ImGui::Text("endPos x:%.2f y:%.2f z:%.2f", endPos.x, endPos.y, endPos.z);

	//show startRot endRot

	ImGui::Text("startRot x:%.2f y:%.2f z:%.2f", startRot.x, startRot.y, startRot.z);
	ImGui::Text("endRot x:%.2f y:%.2f z:%.2f", endRot.x, endRot.y, endRot.z);
	
	
	//distToEndPos distToEndRot
	ImGui::Text("distToEndPos:%.2f", distToEndPos);
	ImGui::Text("distToEndRot:%.2f", distToEndRot);


//check is dialog mode
//ImGui::Checkbox("isDialogMode", &isDialogMode);

//slider float posOffset x y z
ImGui::SliderFloat("posOffset.x", &posOffset.x, -200.0f, 200.0f);
ImGui::SliderFloat("posOffset.y", &posOffset.y, -200.0f, 200.0f);
ImGui::SliderFloat("posOffset.z", &posOffset.z, -200.0f, 200.0f);

//slider float rotOffset x y z 0 - 360
ImGui::SliderFloat("rotOffset.x", &rotOffset.x, 0.0f, 360.0f);
ImGui::SliderFloat("rotOffset.y", &rotOffset.y, 0.0f, 360.0f);
ImGui::SliderFloat("rotOffset.z", &rotOffset.z, 0.0f, 360.0f);

	//show DegAng xyz
	ImGui::Text("DegAng x:%.2f y:%.2f z:%.2f", m_DegAng.x, m_DegAng.y, m_DegAng.z);

	//slider float for focusDist focusForRange focusBackRange
	ImGui::SliderFloat("focusDist", &focusDist, 0.0f, 200.0f);
	ImGui::SliderFloat("focusForRange", &focusForRange, -70.0f, 100.0f);
	ImGui::SliderFloat("focusBackRange", &focusBackRange, -40.0f, 100.0f);

	ImGui::End();
}
