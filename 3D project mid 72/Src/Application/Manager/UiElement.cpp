#include"UiElement.h"

void to_json(json& j, const UiElement& element)
{
	j = json{
	 {"name", element.name},
	 {"texture", element.tex->GetFilepath()}, // Assuming KdTexture has a GetFilepath() function
	 {"pos", {element.pos.x, element.pos.y}},
	 {"scale", {element.scale.x, element.scale.y}},
	 {"color", {element.color.x, element.color.y, element.color.z, element.color.w}},
	 {"rect", {element.rect.x, element.rect.y, element.rect.width, element.rect.height}},
	 {"tag", element.tag},          // New field for tag
	{"drawOrder", element.drawOrder},  // Serialize drawOrder
	{"canFadeOut", element.canFadeOut}, // New field for canFadeOut
	 {"rotation", element.rotation},  // Serialize rotation
	{"textInput", element.textInput} // Serialize textInput
	};
}

void from_json(const json& j, UiElement& element)
{
	element.name = j.at("name").get<std::string>();
	std::string texPath = j.at("texture").get<std::string>();
	element.tex = std::make_shared<KdTexture>(texPath);

	element.pos.x = j.at("pos")[0];
	element.pos.y = j.at("pos")[1];

	element.scale.x = j.at("scale")[0];
	element.scale.y = j.at("scale")[1];

	element.color.x = j.at("color")[0];
	element.color.y = j.at("color")[1];
	element.color.z = j.at("color")[2];
	element.color.w = j.at("color")[3];

	element.rect.x = j.at("rect")[0];
	element.rect.y = j.at("rect")[1];
	element.rect.width = j.at("rect")[2];
	element.rect.height = j.at("rect")[3];

	element.tag = j.at("tag").get<int>();          // New field for tag
	element.drawOrder = j.at("drawOrder").get<int>();  // Deserialize drawOrder
	element.canFadeOut = j.at("canFadeOut").get<bool>(); // New field for canFadeOut
	element.rotation = j.at("rotation").get<float>();  // Deserialize rotation
	element.textInput = j.at("textInput").get<std::string>(); // Deserialize textInput

	element.originPos = element.pos;

}
