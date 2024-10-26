#include"DialogManager.h"
#include"Application/Manager/InputManager.h"
#include"Application/Manager/UiManager.h"

void DialogManager::Init()
{
	conversations.resize(4);

	for (auto& part : conversations) {
		part.sentences.resize(3); 
	}

	conversations[0].sentences[0] = "ようこそ、冒険者よ！";
	conversations[0].sentences[1] = "まずは基本的な操作を学びましょう。";
	conversations[0].sentences[2] = "W、A、S、Dキーでキャラクターを移動させることができます。";

	conversations[1].sentences[0] = "左クリックで攻撃を行います。";
	conversations[1].sentences[1] = "バレルを壊すと、アイテムを入手できます。";
	conversations[1].sentences[2] = "部屋には宝箱があり、中には貴重なアイテムが入っています";

	conversations[2].sentences[0] = "トラップに踏み入れると、ダメージを受けてしまいます。";
	conversations[2].sentences[1] = "ダメージを受けてしまいます。";
	conversations[2].sentences[2] = "注意して進みましょう。";

	conversations[3].sentences[0] = "ゴールに到達すると、";
	conversations[3].sentences[1] = "次のフロアへ進むことができます。";
	conversations[3].sentences[2] = "それでは、冒険を始めましょう！";

}

void DialogManager::Update()
{
	HandleDialog();
	ProcessConversation();
}

void DialogManager::DrawMoji()
{
	

	if (isDialogEvent) {
		mojiColor = { 1, 1, 1, 1 };
		if (mojiColorAlpha < 1) mojiColorAlpha += 0.005;

		float yOffset = 0;

		for (int i = 0; i < 3; ++i) {
			KdShaderManager::Instance().m_spriteShader.DrawString(dialoguePos.x, dialoguePos.y - yOffset, displayedText[i].c_str(), mojiColor,0.9);
			yOffset += 42;
		}
	}
	


	



}

void DialogManager::HandleDialog()
{

	if (!isDialogEvent || isPartCompleted) {
		return;
	}

	textTimer += 0.21f; //moji play spd

	for (int i = 0; i <= currentSentenceIndex; ++i) {
		if (currentCharIndex[i] < conversations[currentPartIndex].sentences[i].size() && textTimer >= textDisplaySpeed) {
			displayedText[i] += conversations[currentPartIndex].sentences[i][currentCharIndex[i]];
			currentCharIndex[i]++;
			textTimer = 0.0f; 

			if (!displayedText[i].empty() && displayedText[i].back() != ' ' && i % 2 == 0) {
				KdAudioManager::Instance().PlayTextSE();
			}
		}
	}

	if (currentSentenceIndex < 2 && currentCharIndex[currentSentenceIndex] == conversations[currentPartIndex].sentences[currentSentenceIndex].size()) {
		currentSentenceIndex++;
	}
	else if (currentSentenceIndex == 2 && currentCharIndex[2] == conversations[currentPartIndex].sentences[2].size()) {
		isPartCompleted = true;
	}

	

}

void DialogManager::ProcessConversation()
{
	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::EnterKey) && isPartCompleted) {
		currentPartIndex++;
		if (currentPartIndex < conversations.size()) {
			currentSentenceIndex = 0;
			for (int i = 0; i < 3; ++i) {
				currentCharIndex[i] = 0;
				displayedText[i].clear();
			}
			isPartCompleted = false;
		}
		else {
			isDialogEvent = false;
			currentPartIndex = 0;
		}
	}

}

void DialogManager::StartDialogTalk()
{
	isDialogEvent = true;
	UiManager::Instance().OpenDialogFrame();

}

void DialogManager::CallImgui()
{
	ImGui::Begin("DialogManager");

	//check isDialogEvent
	ImGui::Checkbox("isDialogEvent", &isDialogEvent);

	ImGui::Checkbox("isConversationStarted", &isConversationStarted);
	ImGui::Checkbox("isPartCompleted", &isPartCompleted);

	ImGui::Text("Displayed Text:");
	for (int i = 0; i < 3; ++i) {
		ImGui::Text("Sentence %d: %s", i + 1, displayedText[i].c_str());
	}

	ImGui::Text("Current Character Index:");
	for (int i = 0; i < 3; ++i) {
		ImGui::Text("Sentence %d Char Index: %d", i + 1, currentCharIndex[i]);
	}

	ImGui::Text("Current Part Index: %d", currentPartIndex);
	ImGui::Text("Current Sentence Index: %d", currentSentenceIndex);

	if (ImGui::TreeNode("Conversation Data")) {
		for (size_t partIdx = 0; partIdx < conversations.size(); ++partIdx) {
			if (ImGui::TreeNode((void*)(intptr_t)partIdx, "Part %d", partIdx + 1)) {
				for (size_t sentenceIdx = 0; sentenceIdx < conversations[partIdx].sentences.size(); ++sentenceIdx) {
					ImGui::Text("Sentence %d: %s", sentenceIdx + 1, conversations[partIdx].sentences[sentenceIdx].c_str());
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::Text("Text Timer: %.2f", textTimer);
	ImGui::Text("Text Display Speed: %.2f", textDisplaySpeed);

	ImGui::End();


}
