#include "DebugBox.h"
#include <ShapeRenderer.h>


DebugBox::DebugBox()
{
}

void DebugBox::draw(int x, int y)
{
	AllegroExt::Graphics::ShapeRenderer::drawRectangle(x, y, 300, screenTexts.size() * FONT_SIZE, 200, 200, 200, 200);
	for (int i = 0; i < screenTexts.size(); i++)
	{
		screenTexts.at(i)->drawText(info.at(i).first + ": " + info.at(i).second, x, y, FONT_SIZE, 0, 0, 0, 255);
		y += FONT_SIZE;
	}
}


DebugBox::~DebugBox()
{
}
