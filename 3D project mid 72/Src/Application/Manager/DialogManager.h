#pragma once
#include"json.hpp"

using namespace std;
using namespace DirectX::SimpleMath;
using json = nlohmann::json;


enum ConversationType
{
	GreetingDialog = 0,
	TutorialDialog,
	ShopDialog,
	ChatingDialog,
};

struct DialogPart {
	std::vector<std::string> sentences;
};

class DialogManager : public KdGameObject
{
public:

	//get set isDialogEvent
	bool GetIsDialogEvent() { return isDialogEvent; }
	void SetIsDialogEvent(bool _isDialogEvent) { isDialogEvent = _isDialogEvent; }

	void Init() override;
	void Update() override;
	void DrawMoji() override;

	void HandleDialog();

	void ProcessConversation();

	void StartDialogTalk();

	void CallImgui() override;

private:


	string dialogueSentenceA = "Hello Adventure!";
	string dialogueSentenceB = "You are the first time right?";
	string dialogueSentenceC = "Let me teach you how to move!";

	Vector3 dialoguePos = { -370,-140,0 };
	Color mojiColor = { 1,1,1,1 };
	float mojiColorAlpha = 0;

	std::string displayedText[3] = { "", "", "" }; // For three sentences
	float textDisplaySpeed = 0.7f;  // Speed of text display (adjust as needed)
	float textTimer = 0.0f;  // Timer to track the reveal
	
	int currentCharIndex[3] = { 0, 0, 0 }; // Tracks which character is currently visible for each sentence
	int currentSentenceIndex = 0; // Tracks current sentence being processed in the part
	int currentPartIndex = 0;

	int conversationId = ConversationType::TutorialDialog;

	bool isConversationStarted = false;
	bool isPartCompleted = false; // To track if all three sentences are completed

	

	std::vector<DialogPart> conversations;

	bool isDialogEvent = false;




private:
	DialogManager() {}
	~DialogManager() override {}

public:
	static DialogManager& Instance()
	{
		static DialogManager instance;
		return instance;
	}
};

