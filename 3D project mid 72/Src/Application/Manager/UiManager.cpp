#include"UiManager.h"
#include"Application/GameObject/Player/Player.h"
#include"Application/Manager/DialogManager.h"
#include"Application/Manager/InventoryManager.h"

void UiManager::Init()     
{

	LoadUiSettings(settingPath);



}

void UiManager::Update()
{

	HandleStatusBars();

	ControlDialogFrame();

	ControlInvetoryRelated();

	ControlMessageBox();

	ShowBattleStart();

	ShowBattleWin();
}

void UiManager::DrawSprite()
{


	if (!hasUi) return;


	for (auto& element : uiElements) {
		if (element.drawOrder == LowBackground) {
			DrawUi(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == Background) {
			DrawUi(element);  
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == TopBackground) {
			DrawUi(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == Middle) {
			DrawUi(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == Font) {
			DrawUi(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == TopFont) {
			DrawUi(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == TopMost) {
			DrawUi(element);
		}

	}


	
	if (!GameManager::Instance().GetIsGameStart()) {
		for (auto& element : uiElements) {
				DrawUiTitle(element);
		}
	}



	StartBloodScreen();

	

}

void UiManager::DrawMoji()
{
	if (GameManager::Instance().GetIsBattleMode() || GameManager::Instance().GetIsInventoryOpen() || !GameManager::Instance().GetIsGameStart()) return;
	KdShaderManager::Instance().m_spriteShader.DrawString(-395, -316, "階層攻略度", { 1,1,1,1 },0.86);
	int floorNum = GameManager::Instance().GetFloorNum();
    KdShaderManager::Instance().m_spriteShader.DrawString(-560, -316, (std::to_string(floorNum) + "階").c_str(), {1,1,1,1}, 0.86);

	
}

void UiManager::DrawRenderTarget()
{

	for (auto& element : uiElements) {
		if (element.tag == UiTag::Inventory) {
			DrawUi(element);
		}
	}

}



void UiManager::SaveUiSettings(const std::string& filePath)
{

	json uiData = uiElements; 

	std::ofstream file(filePath);
	if (file.is_open()) {
		file << uiData.dump(4); 
		file.close();
	}


}



void UiManager::LoadUiSettings(const std::string& filePath)
{

	std::ifstream file(filePath);
	if (!file.is_open()) return;

	json uiData;
	file >> uiData;
	file.close();

	uiElements = uiData.get<std::vector<UiElement>>(); 


}

void UiManager::AddUiElement(const std::string& name, const std::string& texPath)
{
	UiElement newElement;
	newElement.name = name;
	newElement.tex = std::make_shared<KdTexture>(texPath);
	newElement.pos = { 0, 0 };
	newElement.scale = { 1, 1 };
	newElement.color = { 1, 1, 1, 1 };
	newElement.rect = { 0, 0, (long)newElement.tex.get()->GetWidth(), (long)newElement.tex.get()->GetHeight() }; 

	uiElements.push_back(newElement);
}



void UiManager::HandleStatusBars()
{
	auto player = wpPlayer.lock();
	if (!player) return;

	for (auto& element : uiElements) {
		float barWidth = element.tex->GetWidth();
		float width = 0.0f;

		float barHeight = element.tex->GetHeight();
		float height = 0.0f;

		switch (element.tag) {
		case UiTag::HpBar:
			width = player->Gethp() / player->GetmaxHp() * barWidth;
			element.rect.width = static_cast<long>(width);
			break;
		case UiTag::MpBar:
			width = player->GetMp() / player->GetMaxMp() * barWidth;
			element.rect.width = static_cast<long>(width);
			break;
		case UiTag::StaminaBar:
			height = player->Getstamina() / player->GetmaxStamina() * barHeight;
			element.rect.height = static_cast<long>(height);

			if (player->Getstamina() <= 49) {
				element.color = { 3,0,0,1 };
			}
			else if(player->Getstamina() < 100) {
				 element.color = { 1,1,1,1 };
			}

			break;
		case UiTag::HpBuffer:
			width = player->GethpBufferMax() / 100.0f * barWidth;
			element.rect.width = static_cast<long>(width);
			break;
		case UiTag::StageProgressBar:
			width = GameManager::Instance().GetStageProgress() / GameManager::Instance().GetStageProgressMax() * barWidth;
			element.rect.width = static_cast<long>(width);
			break;

		case UiTag::BattleSlogan:
			if (showBattleStart) {
				showBattleStart = false;
				element.color.w = 1;
			}
			element.color.w -= 0.005;
		break;

		case UiTag::WinSlogan:
			if (showBattleWin) {
				showBattleWin = false;
				element.color.w = 1;
			}
			element.color.w -= 0.005;

			break;


		default:
			continue;
		}

		/*if (element.tag == UiTag::StaminaBar || element.tag == UiTag::StaminaFrame) {
			if (player->Getstamina() >= player->GetmaxStamina())	element.color = { 1,1,1,0 };
			else													element.color = { 1,1,1,1 };
		}*/

		

	}
}



void UiManager::StartBloodScreen()
{
	for (auto& element : uiElements) {
		if (element.tag == UiTag::BloodEffect) {
			
			element.color = {1,1,1,bloodAlpha};
		}
	}

	bloodAlpha -= 0.03;

}

void UiManager::ControlDialogFrame()
{

	if (isDialogOpen) {
		if (dialogFrameAlpha < 1) dialogFrameAlpha += 0.03;
	}
	else {
		if (dialogFrameAlpha > 0) dialogFrameAlpha -= 0.03;
	}

	for (auto& element : uiElements) {
		if (element.tag == UiTag::DialogFrame || element.tag == UiTag::DialogArrow) {
			element.color = { 1,1,1,dialogFrameAlpha };
		}
	}

	
	for (auto& element : uiElements) {
		if (element.tag == UiTag::DialogArrow) {

			float amplitude = 7.0f; 
			float speed = 4.2f;     
			element.pos.x = element.originPos.x + amplitude * std::sin(GameManager::Instance().GetGlobalTime() * speed);

		}

	}
}

void UiManager::OpenDialogFrame()
{
	isDialogOpen = true;
	dialogFrameAlpha = 0;
}

void UiManager::CloseDialogFrame()
{
	isDialogOpen = false;
	dialogFrameAlpha = 0;
}

void UiManager::DrawInventory()
{
	for (auto& element : uiElements) {
		if (element.tag == UiTag::Inventory) {
			DrawUi(element);
		}
		if (element.tag == UiTag::InventoryMarker) {
			DrawUi(element);
		}
		if (element.tag == UiTag::InventorySelectSlot) {
			DrawUi(element);
		}
		if (element.tag == UiTag::itemMenuFrame) {
			DrawUi(element);
		}
		
	}
}

void UiManager::DrawMiniMap()
{

	for (auto& element : uiElements) {
		if (element.drawOrder == LowBackground) {
			DrawUiMiniMap(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == Background) {
			DrawUiMiniMap(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == TopBackground) {
			DrawUiMiniMap(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == Middle) {
			DrawUiMiniMap(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == Font) {
			DrawUiMiniMap(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == TopFont) {
			DrawUiMiniMap(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.drawOrder == TopMost) {
			DrawUiMiniMap(element);
		}

	}

	for (auto& element : uiElements) {
		if (element.tag == UiTag::PlayerMapIcon) {
			element.pos = wpPlayer.lock()->miniMapPos2D;
		}
	}



}

void UiManager::ControlInvetoryRelated()
{

	for (auto& element : uiElements) {
		if (element.tag == UiTag::InventorySelectSlot) {

			element.pos = InventoryManager::Instance().GetSelectedItemPos() + invenslotOffset;
		}
		if (element.tag == UiTag::InventoryMarker) {

			//element.pos = InventoryManager::Instance().GetSelectedItemPos() + invenMarkerOffset;
		}

		if (element.tag == UiTag::Inventory) {


		}

		if (element.tag == UiTag::itemMenuFrame) {
			element.pos = InventoryManager::Instance().GetItemMenuFramePos();
		}


	}


	for (auto& element : uiElements) {
		if (element.tag == UiTag::InventoryMarker) {

			float amplitude = 3.5f;
			float speed = 4.2f;
			element.originPos = InventoryManager::Instance().GetSelectedItemPos() + invenMarkerOffset;
			element.pos.x = element.originPos.x + amplitude * std::sin(GameManager::Instance().GetGlobalTime() * speed);
			element.pos.y = element.originPos.y;

		}

	}


}

void UiManager::ControlMessageBox()
{
	messageBoxAlpha -= 0.007;

	for (auto& element : uiElements) {
		if (element.tag == UiTag::MessageBoxFrame) {
			element.color = { 1,1,1,messageBoxAlpha };
			if (messageBoxAlpha < 1) element.pos.y += 0.4;

			//KdShaderManager::Instance().m_spriteShader.DrawString(element.pos.x, element.pos.y,messageMoji.c_str(),{1,1,1,messageBoxAlpha });


		}
	}

}

void UiManager::OpenMessageBox(Vector2 _boxPos, string _messageMoji)
{
	messageBoxAlpha = 1;
	messageMoji = _messageMoji;

	for (auto& element : uiElements) {
		if (element.tag == UiTag::MessageBoxFrame) {
			element.pos = _boxPos;
		}
	}

}

void UiManager::ShowBattleStart()
{
}

void UiManager::ShowBattleWin()
{
}

void UiManager::DrawUi(UiElement& _ui)
{
	if (!GameManager::Instance().GetIsGameStart() || _ui.tag == UiTag::GameTitle) return;

	if (_ui.tag == UiTag::MiniMapBack || (_ui.tag == UiTag::PlayerMapIcon)) return;

	if (_ui.tag == UiTag::CutScene)
	{
		//if (isCutScene == false) return;
	}

	if (_ui.tag == UiTag::DialogFrame)
	{
		//if (DialogManager::Instance().GetIsDialogEvent() == false) return;
	}

	if (_ui.tag == UiTag::Inventory || _ui.tag == UiTag::InventorySelectSlot || _ui.tag == UiTag::InventoryBackGround || _ui.tag == UiTag::InventoryMarker || _ui.tag == UiTag::itemMenuFrame)
	{
		if (GameManager::Instance().GetIsInventoryOpen() == false) return;
	} 

	if (_ui.tag == UiTag::StageProgressBar || _ui.tag == UiTag::StageProgressFrame) {
		if (GameManager::Instance().GetIsBattleMode() == true) return;
	}

	


	testX = _ui.pos.x;
	testY = _ui.pos.y;
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(_ui.scale.x, _ui.scale.y, 1) * Matrix::CreateRotationZ(DirectX::XMConvertToRadians(_ui.rotation)) * Matrix::CreateTranslation(_ui.pos.x,_ui.pos.y,0));
	
	if (_ui.tag == UiTag::StaminaBar || _ui.tag == UiTag::StaminaFrame) {

		KdShaderManager::Instance().m_spriteShader.DrawTex(_ui.tex.get(), 0, 0, _ui.rect.width, _ui.rect.height, &_ui.rect, &_ui.color, Math::Vector2{ 0, 0 });
		if (_ui.tag == UiTag::StaminaBar || _ui.tag == UiTag::StaminaFrame) {
			if (wpPlayer.lock()->Getstamina() >= wpPlayer.lock()->GetmaxStamina())	_ui.color.w -= 0.028;
			else													_ui.color = { 1,1,1,1 };
		}

	}
	else KdShaderManager::Instance().m_spriteShader.DrawTex(_ui.tex.get(), 0, 0, _ui.rect.width, _ui.rect.height, &_ui.rect, &_ui.color, Math::Vector2{ 0, 0.5 });
	
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);


}

void UiManager::DrawUiTitle(UiElement& _ui)
{
	if (_ui.tag != UiTag::GameTitle) return;

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(_ui.scale.x, _ui.scale.y, 1) * Matrix::CreateRotationZ(DirectX::XMConvertToRadians(_ui.rotation)) * Matrix::CreateTranslation(_ui.pos.x, _ui.pos.y, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(_ui.tex.get(), 0, 0, _ui.rect.width, _ui.rect.height, &_ui.rect, &_ui.color, Math::Vector2{ 0, 0.5 });
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

	if (_ui.canFadeOut) {
		float amplitude = 7.7f;
		float speed = 2.8f;
		_ui.pos.y = _ui.originPos.y + amplitude * std::sin(GameManager::Instance().GetGlobalTime() * speed);
	}
	else {
		// Add alpha fading based on a sine wave
		float alphaAmplitude = 0.5f; // Control how much the alpha changes (0.5 makes it range between 0.5 and 1)
		float alphaSpeed = 2.1f;     // Control how fast the alpha fades
		_ui.color.w = 0.77f + alphaAmplitude * std::sin(GameManager::Instance().GetGlobalTime() * alphaSpeed);

		//i want _ui's alpha (color.w) to fade along a sin wave 

	}
	

}

void UiManager::DrawUiMiniMap(UiElement& _ui)
{

	if (_ui.tag != UiTag::MiniMapBack && _ui.tag != UiTag::PlayerMapIcon) return;

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(_ui.scale.x, _ui.scale.y, 1) * Matrix::CreateRotationZ(DirectX::XMConvertToRadians(_ui.rotation)) * Matrix::CreateTranslation(_ui.pos.x, _ui.pos.y, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(_ui.tex.get(), 0, 0, _ui.rect.width, _ui.rect.height, &_ui.rect, &_ui.color, Math::Vector2{ 0, 0.5 });
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

}

void UiManager::CallImgui()
{
	ImGui::Begin("UiEditor");

	
	//slider float fontsize
	ImGui::SliderFloat("fontSize", &fontSize, 0.3, 7);
	//slider testPos x y
	ImGui::SliderFloat("testPosX", &testPos.x, -700, 700);
	ImGui::SliderFloat("testPosY", &testPos.y, -420, 420);

	//slider float invenslotOffset x y 
	ImGui::SliderFloat("invenslotOffsetX", &invenslotOffset.x, -700, 700);
	ImGui::SliderFloat("invenslotOffsetY", &invenslotOffset.y, -420, 420);

	//slider float invenmarkerOffset x y
	ImGui::SliderFloat("invenMarkerOffsetX", &invenMarkerOffset.x, -700, 700);
	ImGui::SliderFloat("invenMarkerOffsetY", &invenMarkerOffset.y, -420, 420);

	//sho messageAlpha
	ImGui::Text("messageAlpha:%f", messageBoxAlpha);
	//show messageMoji
	ImGui::Text("messageMoji:%s", messageMoji.c_str());

	//show testX and testY
	ImGui::Text("testX:%f", testX);
	ImGui::Text("testY:%f", testY);

	//check hasUi
	ImGui::Checkbox("hasUi", &hasUi);

	static char newName[128] = "";
	static char newTexPath[256] = "";
	static char newTextInput[256] = "";

	ImGui::InputText("Element Name", newName, sizeof(newName));
	ImGui::InputText("Texture Path", newTexPath, sizeof(newTexPath));

	string prefix = "Asset/Textures/";
	string surfix = ".png";

	if (ImGui::Button("Add UI Element")) {
		if (strlen(newName) > 0 && strlen(newTexPath) > 0) {
			AddUiElement(newName, prefix + newTexPath + surfix);
		}
	}

	for (size_t i = uiElements.size(); i-- > 0;){
		ImGui::PushID(i); 

		ImGui::Separator();
		ImGui::TextColored(imYellow, "%s", uiElements[i].name.c_str());

		//show uiElements.tag in int
		ImGui::Text("tag:%d", uiElements[i].tag);

		if (ImGui::TreeNode("Edit")) {
			ImGui::Text("moji:%s", uiElements[i].textInput.c_str());
			ImGui::Text("path:%s", uiElements[i].tex->GetFilepath().c_str());

			ImGui::SliderFloat("Pos X :", &uiElements[i].pos.x, -700, 700);
			ImGui::SliderFloat("Pos Y", &uiElements[i].pos.y, -420, 420);
			ImGui::SliderFloat("Scale X", &uiElements[i].scale.x, 0.3, 20);
			ImGui::SliderFloat("Scale Y", &uiElements[i].scale.y, 0.3, 20);

			ImGui::ColorEdit4("Color", (float*)&uiElements[i].color);
			ImGui::SliderFloat("Rotation", &uiElements[i].rotation, 0.0f, 360.0f);  

			const char* tagOptions[] = { "noTag", "FixUi", "HpBar", "MpBar", "StaminaBar", "StrikeBar","StartScene","EndScene","Inventory","BloodEffect","DialogFrame","DialogArrow","CutScene","InventorySelectSlot","MessageBox","InventoryBackGround","InventoryMarker", "StaminaFrame", "HpBuffer","StageProgressBar", "itemMenuFrame","StageProgressFrame","BattleSlogan", "WinSlogan", "miniMapBack" , "PlayerMapIcon", "GameTitle"};
			ImGui::Combo("Tag", &uiElements[i].tag, tagOptions, IM_ARRAYSIZE(tagOptions));

			const char* drawOrderOptions[] = { "LowBackGround","Background", "TopBackground", "Middle", "Font", "TopFont" ,"TopMost"};
			ImGui::Combo("DrawOrder", &uiElements[i].drawOrder, drawOrderOptions, IM_ARRAYSIZE(drawOrderOptions));

			ImGui::Checkbox("Can Fade Out", &uiElements[i].canFadeOut);

			ImGui::InputText("Text Input", newTextInput, sizeof(newTextInput));
			uiElements[i].textInput = newTextInput;  

			if (ImGui::Button("Remove Element")) {
				uiElements.erase(uiElements.begin() + i);
				--i;
			}

			ImGui::TreePop();
		}
		
		ImGui::PopID(); 

	}

	if (ImGui::Button("Save UI Settings")) {
		SaveUiSettings(settingPath);
	}

	if (ImGui::Button("Load UI Settings")) {
		LoadUiSettings(settingPath);
	}

	ImGui::End;


}
