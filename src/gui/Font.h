#pragma once

#include <nano/nanovg.h>
#include <string>

class Font
{
public:
    Font(NVGcontext *vg, const std::string &fontPath);
    ~Font();

    void drawShadow(const std::string &text, float x, float y, float fontSize, NVGcolor color);
    void draw(const std::string &text, float x, float y, float fontSize, NVGcolor color, bool darkend = false);
    float getWidth(const std::string &text, float fontSize);

private:
    NVGcontext *vg;
    int fontId;
};
