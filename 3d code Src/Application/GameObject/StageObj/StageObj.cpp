#include"StageObj.h"
#include"Application/Scene/SceneManager.h"

void StageObj::Init()
{

	objTag = GameTag::StageTag;
	
	debugWire = std::make_unique<KdDebugWireFrame>();

	model = make_shared<KdModelWork>();
	model->SetModelData("Asset/Models/ChestAni/ChestAni01.gltf");

	SetCollider();

}

void StageObj::Update()
{

	worldMat = Matrix::CreateScale(scale) * Matrix::CreateTranslation(pos);

	
}

void StageObj::DrawLit()
{

	emissive.x += 0.01;
	emissive.y += 0.01;
	emissive.z += 0.01;

	if (emissive.x > 7) {
		emissive.x = 0;
		emissive.y = 0;
		emissive.z = 0;
	}

	if (drawTag == Lit) {
		if (stageTag == BattleObj) {
			if(GameManager::Instance().GetIsBattleMode())KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat, objColor);

		}
		else if (stageTag == FadeObj){
			if (GameManager::Instance().GetFloorNum() == 0)KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat, objColor);
		}
		else {
			KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat, objColor, emissive);
		}

	}

}

void StageObj::GenerateDepthMapFromLight()
{
	if (drawTag == Lit) {
		if (stageTag == BattleObj) {
			if (GameManager::Instance().GetIsBattleMode())KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat, objColor);

		}
		else {
			KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat, objColor);
		}

	}

}

void StageObj::DrawUnLit()
{
	if (drawTag == UnLit) KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat, objColor);

}

void StageObj::DrawBright()
{
	if (drawTag == Bright) KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat, objColor);

}

void StageObj::InitWithStageObj(StageObj sobj)
{
	modelPath = sobj.modelPath;

	pos = sobj.pos;
	scale = sobj.scale;
	rot = sobj.rot;
	objColor = sobj.objColor;
	canFade = sobj.canFade;
	canCollide = sobj.canCollide;
	hasAnime = sobj.hasAnime;
	drawTag = sobj.drawTag;
	stageTag = sobj.stageTag;
	objTag = GameTag::StageTag;

	model = make_shared<KdModelWork>();
	model->SetModelData(modelPath);


	if(canCollide)SetCollider();


}

void StageObj::SetCollider()
{
	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("StageCol", model, KdCollider::TypeGround);
	canCollide = false;
}

void StageObj::CallImgui()
{
	

	//ImGui::Begin("StageObj");
	
	//ImGui::End();
}

void to_json(json& j, const StageObj& obj)
{
	j = json{
	  {"objName", obj.objName},
	  {"modelPath", obj.modelPath},
	  {"pos", {obj.pos.x, obj.pos.y, obj.pos.z}},
	  {"scale", {obj.scale.x, obj.scale.y, obj.scale.z}},
	  {"rot", {obj.rot.x, obj.rot.y, obj.rot.z}},
	  {"color", {obj.objColor.x, obj.objColor.y, obj.objColor.z, obj.objColor.w}},
	  {"canFade", obj.canFade},
	  {"canCollide", obj.canCollide},
	  {"hasAnime", obj.hasAnime},
	  {"drawTag", obj.drawTag},
	  {"gameObjTag", obj.stageTag}
	};
}

void from_json(const json& j, StageObj& obj)
{
	obj.objName = j.at("objName").get<std::string>();
    obj.modelPath = j.at("modelPath").get<std::string>();

    obj.pos.x = j.at("pos")[0];
    obj.pos.y = j.at("pos")[1];
    obj.pos.z = j.at("pos")[2];

    obj.scale.x = j.at("scale")[0];
    obj.scale.y = j.at("scale")[1];
    obj.scale.z = j.at("scale")[2];

    obj.rot.x = j.at("rot")[0];
    obj.rot.y = j.at("rot")[1];
    obj.rot.z = j.at("rot")[2];

    obj.objColor.x = j.at("color")[0];
    obj.objColor.y = j.at("color")[1];
    obj.objColor.z = j.at("color")[2];
    obj.objColor.w = j.at("color")[3];

    obj.canFade = j.at("canFade").get<bool>();
    obj.canCollide = j.at("canCollide").get<bool>();
    obj.hasAnime = j.at("hasAnime").get<bool>();
    obj.drawTag = j.at("drawTag").get<int>();
    obj.stageTag = j.at("gameObjTag").get<int>();


	// Initialize model as shared_ptr and set model data from modelPath
	obj.model = std::make_shared<KdModelWork>();
	obj.model->SetModelData(obj.modelPath);
	obj.anime = std::make_shared<KdAnimator>();

	std::shared_ptr<StageObj> newObjPtr = std::make_shared<StageObj>(obj);
	newObjPtr->InitWithStageObj(obj);
	SceneManager::Instance().AddObject(std::static_pointer_cast<KdGameObject>(newObjPtr));

	
}
