﻿#include "CameraBase.h"

void CameraBase::Init()
{
	if (!m_spCamera)
	{
		m_spCamera = std::make_shared<KdCamera>();
	}

	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

}

void CameraBase::Update()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetCameraMatrix(worldMat);

}

void CameraBase::PreDraw()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetToShader();
}

void CameraBase::SetTarget(const std::shared_ptr<KdGameObject>& target)
{
	if (!target) { return; }

	m_wpTarget = target;
}

void CameraBase::CallImgui()
{
	
}

