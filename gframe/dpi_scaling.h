#ifndef YGOPRO_DPI_SCALING_H
#define YGOPRO_DPI_SCALING_H

namespace ygo {

float ParseUIScale(const char* value);
float DpiToUIScale(float dpi);
float GetAutoUIScale();
unsigned int GetTextFontSizeForScale(unsigned int baseSize, float scale);
unsigned int GetGuiFontSizeForScale(unsigned int baseSize, float scale);
unsigned int GetGuiSkinSizeForScale(unsigned int baseSize, float scale);

}

#endif // YGOPRO_DPI_SCALING_H
