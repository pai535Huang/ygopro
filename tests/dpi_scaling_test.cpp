#include "../gframe/dpi_scaling.h"

#include <cstdlib>

int main() {
	// GUI text and drawn text must retain the same proportions above 175%,
	// including the fractional scale of a 1484-pixel-high window.
	const float scales[] = {1.0f, 1.25f, 1.5f, 1.75f, 2.0f, 1484.0f / 640.0f, 3.0f, 4.0f};
	for(float scale : scales) {
		const unsigned int fontSize = static_cast<unsigned int>(14 * scale + 0.5f);
		if(ygo::GetGuiFontSizeForScale(14, scale) != fontSize
			|| ygo::GetTextFontSizeForScale(14, scale) != fontSize)
			return EXIT_FAILURE;
		// Small padding and large controls must both grow with their rectangles.
		const unsigned int skinSizes[] = {2u, 15u, 18u, 25u, 80u};
		for(unsigned int baseSize : skinSizes) {
			if(ygo::GetGuiSkinSizeForScale(baseSize, scale) != static_cast<unsigned int>(baseSize * scale + 0.5f))
				return EXIT_FAILURE;
		}
	}
	if(ygo::GetGuiFontSizeForScale(14, 0.5f) != 9
		|| ygo::GetGuiSkinSizeForScale(18, 0.5f) != 9)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
