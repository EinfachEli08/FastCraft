// FontRenderer.cpp
#include "gui/Font.h"
#include <iostream>

Font::Font(NVGcontext *vg, const std::string &fontPath) : vg(vg)
{
    // Load the font from the provided path
    fontId = nvgCreateFont(vg, "MinecraftRegular", fontPath.c_str());
    if (fontId == -1)
    {
        std::cerr << "Failed to load font: " << fontPath << std::endl;
    }
}

Font::~Font()
{
    // NanoVG automatically handles font cleanup when the context is destroyed
}

void Font::drawShadow(const std::string &text, float x, float y, float fontSize, NVGcolor color)
{
    draw(text, x + 1, y + 1, fontSize, color, true);
    draw(text, x, y, fontSize, color, false);
}

void Font::draw(const std::string &text, float x, float y, float fontSize, NVGcolor color, bool darkend)
{
    if (darkend)
    {
        color.r = (color.r * 255.0f * 0.25f) / 255.0f;
        color.g = (color.g * 255.0f * 0.25f) / 255.0f; 
        color.b = (color.b * 255.0f * 0.25f) / 255.0f;

        nvgFontSize(vg, fontSize);
        nvgFontFaceId(vg, fontId);
        nvgFillColor(vg, color);
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(vg, x + 1, y + 1, text.c_str(), nullptr);
    }
    else
    {
        nvgFontSize(vg, fontSize);
        nvgFontFaceId(vg, fontId);
        nvgFillColor(vg, color);
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgText(vg, x, y, text.c_str(), nullptr);
    }
}

float Font::getWidth(const std::string &text, float fontSize)
{
    nvgFontSize(vg, fontSize);
    nvgFontFaceId(vg, fontId);
    return nvgTextBounds(vg, 0, 0, text.c_str(), nullptr, nullptr);
}
