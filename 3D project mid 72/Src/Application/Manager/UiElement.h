#pragma once
#include "json.hpp"

using namespace std;
using namespace DirectX::SimpleMath;
using json = nlohmann::json;

struct UiElement {
	std::shared_ptr<KdTexture> tex;
	Math::Rectangle rect;
	Vector2 pos;
	float rotation = 0;
	Vector2 scale;
	Color color;
	string name;
	string textInput;
	int tag = 0;
	int drawOrder = 0;
	bool canFadeOut = false;
	//adding new variables
	Vector2 originPos;

	//make constructor
	UiElement() {
		tex = nullptr;
		rect = { 0, 0, 0, 0 };
		pos = { 0, 0 };
		rotation = 0;
		scale = { 1, 1 };
		color = { 1, 1, 1, 1 };
		name = "";
		textInput = "";
		tag = 0;
		drawOrder = 0;
		canFadeOut = false;
		originPos = { 0, 0 };
	}

};

//enum UiTag
//{
//	noTag = 0,
//	StartMenu,
//	ProgressBar,
//	StatusBar,
//	ResourceBar,
//	InventoryFrame,
//
//};



// Serialization function for UiElement
void to_json(json& j, const UiElement& element);
	


// Deserialization function for UiElement
void from_json(const json& j, UiElement& element);