#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern "C" int __real_XMapRaised(Display* display, Window window);

// Irrlicht maps its main window before createDeviceEx returns. Setting the
// caption afterwards is too late for window-manager rules applied on mapping.
// Wrap that call in the Linux link step so the identity is set beforehand,
// without requiring changes to the separately downloaded Irrlicht sources.
extern "C" int __wrap_XMapRaised(Display* display, Window window) {
	char instance[] = "ygopro";
	char application[] = "YGOPro";
	XClassHint hint{instance, application};
	XSetClassHint(display, window, &hint);
	XStoreName(display, window, "YGOPro");
	return __real_XMapRaised(display, window);
}
#endif
