#include "../gframe/dpi_scaling.h"

#include <cstdlib>

int main() {
	if(ygo::GetGuiFontSizeForScale(14, 1484.0f / 640.0f) != 25)
		return EXIT_FAILURE;
	if(ygo::GetTextFontSizeForScale(14, 1484.0f / 640.0f) != 32)
		return EXIT_FAILURE;
	if(ygo::GetGuiFontSizeForScale(14, 1.0f) != 14)
		return EXIT_FAILURE;
	if(ygo::GetGuiSkinSizeForScale(15, 1484.0f / 640.0f) != 26)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
