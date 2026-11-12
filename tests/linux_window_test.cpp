#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <cstring>

namespace {
bool hasClass = false;
bool hasTitle = false;
Display* const expectedDisplay = nullptr;
constexpr Window expectedWindow = 42;
}

extern "C" int __wrap_XMapRaised(Display*, Window);

extern "C" int XSetClassHint(Display* display, Window window, XClassHint* hint) {
	if(display != expectedDisplay || window != expectedWindow
		|| std::strcmp(hint->res_name, "ygopro") || std::strcmp(hint->res_class, "YGOPro"))
		std::abort();
	hasClass = true;
	return 1;
}

extern "C" int XStoreName(Display* display, Window window, const char* title) {
	if(display != expectedDisplay || window != expectedWindow || std::strcmp(title, "YGOPro"))
		std::abort();
	hasTitle = true;
	return 1;
}

extern "C" int __real_XMapRaised(Display* display, Window window) {
	// Both properties must already be present when the window is first mapped.
	if(display != expectedDisplay || window != expectedWindow || !hasClass || !hasTitle)
		std::abort();
	return 7;
}

int main() {
	return __wrap_XMapRaised(expectedDisplay, expectedWindow) == 7 ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif
