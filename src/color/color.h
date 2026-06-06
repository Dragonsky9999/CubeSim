#pragma once

#include <glm/glm.hpp>

enum class COLOR {
	White,
	Black,
	Silver,
	Red,
	Orange,
	Yellow,
	Pink,
	Coral,
	Peach,
	Blue,
	Aqua,
	Cyan,
	Sky,
	Navy,
	Green,
	Mint,
	Lime,
	Olive,
	Purple,
	Lavender,
	Violet,
	Brown,
	Beige,
	Cream,
	Gold,
	RoseGold,
	Teal,
};

glm::vec4 toRGBA(COLOR color);