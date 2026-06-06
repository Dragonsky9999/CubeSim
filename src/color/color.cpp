#include "color.h"
#include <glm/vec4.hpp>

glm::vec4 toRGBA(COLOR color) {
    switch (color) {
         // 基本色
        case COLOR::White: return { 0.95f, 0.95f, 0.95f, 1.0f };
        case COLOR::Black: return { 0.00f, 0.00f, 0.00f, 1.0f };
        case COLOR::Silver: return { 0.80f, 0.80f, 0.80f, 1.0f };

         // 暖色
        case COLOR::Red: return { 0.90f, 0.55f, 0.55f, 1.0f };
        case COLOR::Orange: return { 0.95f, 0.75f, 0.50f, 1.0f };
        case COLOR::Yellow: return { 0.95f, 0.90f, 0.55f, 1.0f };
        case COLOR::Pink: return { 0.95f, 0.70f, 0.80f, 1.0f };
        case COLOR::Coral: return { 0.95f, 0.65f, 0.60f, 1.0f };
        case COLOR::Peach: return { 0.98f, 0.80f, 0.70f, 1.0f };

         // 寒色
        case COLOR::Blue: return { 0.60f, 0.75f, 0.95f, 1.0f };
        case COLOR::Aqua: return { 0.60f, 0.90f, 0.90f, 1.0f };
        case COLOR::Cyan: return { 0.65f, 0.95f, 0.95f, 1.0f };
        case COLOR::Sky: return { 0.70f, 0.85f, 0.98f, 1.0f };
        case COLOR::Navy: return { 0.40f, 0.50f, 0.75f, 1.0f };

         // 緑系
        case COLOR::Green: return { 0.60f, 0.85f, 0.60f, 1.0f };
        case COLOR::Mint: return { 0.70f, 0.95f, 0.80f, 1.0f };
        case COLOR::Lime: return { 0.80f, 0.95f, 0.60f, 1.0f };
        case COLOR::Olive: return { 0.70f, 0.75f, 0.50f, 1.0f };

            // 紫系
        case COLOR::Purple: return { 0.75f, 0.60f, 0.90f, 1.0f };
        case COLOR::Lavender: return { 0.85f, 0.80f, 0.95f, 1.0f };
        case COLOR::Violet: return { 0.80f, 0.65f, 0.95f, 1.0f };

         // 茶系
        case COLOR::Brown: return { 0.70f, 0.55f, 0.45f, 1.0f };
        case COLOR::Beige: return { 0.90f, 0.85f, 0.75f, 1.0f };
        case COLOR::Cream: return { 0.98f, 0.95f, 0.85f, 1.0f };

         // 特殊
        case COLOR::Gold: return { 0.95f, 0.85f, 0.45f, 1.0f };
        case COLOR::RoseGold: return { 0.95f, 0.75f, 0.65f, 1.0f };
        case COLOR::Teal: return { 0.45f, 0.75f, 0.75f, 1.0f };
    }

    return { 0, 0, 0, 0 };
}