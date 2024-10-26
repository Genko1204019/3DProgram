#include"StageEditor.h"
#include"Application/Scene/SceneManager.h"

void StageEditor::Init()
{
	LoadStageSettings(settingPath);
}

void StageEditor::Update()
{
}

void StageEditor::DrawUnLit()
{
	//if (!GameManager::Instance().GetCanDebugWire()) {

		/*for (auto& element : stageUnit) {
			DrawStageObj(element);

		}*/

	//}
	

}

void StageEditor::DrawLit()
{


}

void StageEditor::DrawBright()
{
}

void StageEditor::DrawStageObj(StageObj& _obj)
{
	Matrix worldMat = Matrix::CreateScale(_obj.scale) * Matrix::CreateRotationX(DirectX::XMConvertToRadians(_obj.rot.x)) * Matrix::CreateRotationY(DirectX::XMConvertToRadians(_obj.rot.y)) * Matrix::CreateRotationZ(DirectX::XMConvertToRadians(_obj.rot.z)) * Matrix::CreateTranslation(_obj.pos);
	 KdShaderManager::Instance().m_StandardShader.DrawModel(*_obj.model, worldMat, _obj.objColor);
}

void StageEditor::SaveStageSettings(const std::string& filePath)
{
	// Convert stageUnit to JSON
	json stageData = stageUnit;  // Automatically calls to_json for each StageObj

	// Write to file
	std::ofstream file(filePath);
	if (file.is_open()) {
		file << stageData.dump(4);  // Pretty print with 4 spaces
		file.close();
	}
}

void StageEditor::LoadStageSettings(const std::string& filePath)
{
	// Open the file and read the JSON data
	std::ifstream file(filePath);
	if (!file.is_open()) return;

	json stageData;
	file >> stageData;
	file.close();

	// Deserialize JSON into stageUnit
	stageUnit = stageData.get<std::vector<StageObj>>();  // Automatically calls from_json for each StageObj

}

void StageEditor::CallImgui()
{
	
	ImGui::Begin("StageObj Editor");

	static char newName[128] = "";
	static char newModelPath[256] = "";

	// Input for adding new StageObj objects
	ImGui::InputText("Object Name", newName, sizeof(newName));
	ImGui::InputText("Model Path", newModelPath, sizeof(newModelPath));

	if (ImGui::Button("Add StageObj")) {
		if (strlen(newName) > 0 && strlen(newModelPath) > 0) {
			StageObj newObj;
			newObj.objName = newName;
			string prefix = "Asset/Models/";
			string suffix = ".gltf";
			string modelName = prefix + newModelPath + suffix;
			newObj.modelPath = modelName;

			newObj.objColor = { 1, 1, 1, 1 };
			newObj.pos = { 0, -70, 0 };
			newObj.scale = { 1, 1, 1 };
			newObj.rot = { 0, 0, 0 };

			newObj.model = std::make_shared<KdModelWork>();
			newObj.model->SetModelData(modelName);

			newObj.anime = std::make_shared<KdAnimator>();


			stageUnit.push_back(newObj);
			
			std::shared_ptr<StageObj> newObjPtr = std::make_shared<StageObj>(stageUnit.back());
			newObjPtr->InitWithStageObj(newObj);
			SceneManager::Instance().AddObject(newObjPtr);

		}
	}

	//for (size_t i = 0; i < stageUnit.size(); ++i) {
	for (size_t i = stageUnit.size(); i-- > 0;) {

		ImGui::PushID(i);
		//ImGui::Text("%s", stageUnit[i].objName.c_str());
			ImGui::TextColored(imYellow, "%s", stageUnit[i].objName.c_str());

		//if (ImGui::CollapsingHeader("Edit")) {
		if (ImGui::TreeNode("Edit")) {
			ImGui::SliderFloat("Pos X", &stageUnit[i].pos.x, -280, 280);
			ImGui::SliderFloat("Pos Y", &stageUnit[i].pos.y, -100, 100);
			ImGui::SliderFloat("Pos Z", &stageUnit[i].pos.z, -280, 280);

			ImGui::SliderFloat("Scale X", &stageUnit[i].scale.x, 0.1f, 30);
			ImGui::SliderFloat("Scale Y", &stageUnit[i].scale.y, 0.1f, 30);
			ImGui::SliderFloat("Scale Z", &stageUnit[i].scale.z, 0.1f, 30);

			ImGui::SliderFloat("Rotation X", &stageUnit[i].rot.x, 0.0f, 360.0f);
			ImGui::SliderFloat("Rotation Y", &stageUnit[i].rot.y, 0.0f, 360.0f);
			ImGui::SliderFloat("Rotation Z", &stageUnit[i].rot.z, 0.0f, 360.0f);

			ImGui::ColorEdit4("Color", (float*)&stageUnit[i].objColor);

			const char* drawTagOptions[] = { "noDrawtags", "Lit", "UnLit", "Bright" };
			ImGui::Combo("Draw Tag", &stageUnit[i].drawTag, drawTagOptions, IM_ARRAYSIZE(drawTagOptions));

			const char* gameObjTagOptions[] = { "noTagObj", "FixedObj", "FadeObj", "MoveObj","BattleObj"};
			ImGui::Combo("Game Obj Tag", &stageUnit[i].stageTag, gameObjTagOptions, IM_ARRAYSIZE(gameObjTagOptions));

			ImGui::Checkbox("Can Fade", &stageUnit[i].canFade);
			ImGui::Checkbox("Can Collide", &stageUnit[i].canCollide);
			ImGui::Checkbox("Has Animation", &stageUnit[i].hasAnime);

			if (ImGui::Button("Remove StageObj")) {
				stageUnit.erase(stageUnit.begin() + i);
				--i;
			}

			ImGui::TreePop();
		}
		

		ImGui::PopID();

	}

	if (ImGui::Button("Save Stage Settings")) {
		SaveStageSettings(settingPath);
	}

	if (ImGui::Button("Load Stage Settings")) {
		LoadStageSettings(settingPath);
	}

	ImGui::End();
}
