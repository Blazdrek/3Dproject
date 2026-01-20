#include <stdlib.h>
#include <X11/Xlib.h>

int main() {
    Display* display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    Window root = RootWindow(display,screen);

    Window window = XCreateSimpleWindow(display,root,0,0,800,600,1,BlackPixel(display,screen),WhitePixel(display,screen));
    XMapWindow(display,window);
    XFlush(display);


    XCloseDisplay(display);
    return 0;
}