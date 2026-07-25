#include "dpi_scaling.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>

#if defined(__linux__) && defined(__has_include)
#if __has_include(<X11/Xlib.h>) && __has_include(<X11/Xresource.h>)
#define YGOPRO_HAS_X11_DPI 1
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#endif
#endif

namespace ygo {

namespace {

unsigned int GetScaledFontSize(unsigned int baseSize, float scale, float maxScale, unsigned int minSize) {
	if(scale > maxScale)
		scale = maxScale;
	unsigned int size = static_cast<unsigned int>(baseSize * scale + 0.5f);
	return size > minSize ? size : minSize;
}

float ClampScale(float scale) {
	if(scale < 0.5f)
		return 0.0f;
	if(scale > 4.0f)
		return 4.0f;
	return scale;
}

float ParseScaleFromList(const char* value) {
	float scale = ParseUIScale(value);
	if(scale > 0.0f)
		return scale;
	const char* equals = value ? std::strchr(value, '=') : nullptr;
	if(equals)
		return ParseUIScale(equals + 1);
	return 0.0f;
}

#ifdef YGOPRO_HAS_X11_DPI
float ReadX11Scale() {
	XrmInitialize();
	Display* display = XOpenDisplay(nullptr);
	if(!display)
		return 0.0f;

	float scale = 0.0f;
	char* resourceManager = XResourceManagerString(display);
	if(resourceManager) {
		XrmDatabase db = XrmGetStringDatabase(resourceManager);
		if(db) {
			XrmValue value{};
			char* type = nullptr;
			if(XrmGetResource(db, "Xft.dpi", "Xft.Dpi", &type, &value) && value.addr)
				scale = DpiToUIScale(std::strtof(value.addr, nullptr));
			XrmDestroyDatabase(db);
		}
	}

	if(scale <= 0.0f) {
		int screen = DefaultScreen(display);
		int width = DisplayWidth(display, screen);
		int widthMM = DisplayWidthMM(display, screen);
		if(width > 0 && widthMM > 0)
			scale = DpiToUIScale(width * 25.4f / widthMM);
	}

	XCloseDisplay(display);
	return scale;
}
#endif

}

float ParseUIScale(const char* value) {
	if(!value)
		return 0.0f;
	while(std::isspace(static_cast<unsigned char>(*value)))
		++value;
	if(!*value)
		return 0.0f;

	char* end = nullptr;
	float scale = std::strtof(value, &end);
	if(end == value)
		return 0.0f;
	while(std::isspace(static_cast<unsigned char>(*end)))
		++end;
	if(*end == '%') {
		scale /= 100.0f;
		++end;
	}
	while(std::isspace(static_cast<unsigned char>(*end)))
		++end;
	if(*end != '\0' && *end != ';' && *end != ',')
		return 0.0f;
	return ClampScale(scale);
}

float DpiToUIScale(float dpi) {
	if(dpi < 110.0f)
		return 1.0f;
	return ClampScale(dpi / 96.0f);
}

float GetAutoUIScale() {
	const char* scaleVars[] = {
		"YGOPRO_UI_SCALE",
		"GDK_SCALE",
		"QT_SCALE_FACTOR",
		"QT_SCREEN_SCALE_FACTORS",
		"ELM_SCALE",
	};
	for(const char* var : scaleVars) {
		float scale = ParseScaleFromList(std::getenv(var));
		if(scale > 0.0f)
			return std::max(1.0f, scale);
	}

	const char* dpi = std::getenv("XFT_DPI");
	if(dpi) {
		float scale = DpiToUIScale(std::strtof(dpi, nullptr));
		if(scale > 0.0f)
			return std::max(1.0f, scale);
	}

#ifdef YGOPRO_HAS_X11_DPI
	float x11Scale = ReadX11Scale();
	if(x11Scale > 0.0f)
		return std::max(1.0f, x11Scale);
#endif

	return 1.0f;
}

unsigned int GetTextFontSizeForScale(unsigned int baseSize, float scale) {
	return GetScaledFontSize(baseSize, scale, scale, 9);
}

unsigned int GetGuiFontSizeForScale(unsigned int baseSize, float scale) {
	return GetScaledFontSize(baseSize, scale, 1.75f, 9);
}

unsigned int GetGuiSkinSizeForScale(unsigned int baseSize, float scale) {
	return GetScaledFontSize(baseSize, scale, 1.75f, baseSize);
}

}
