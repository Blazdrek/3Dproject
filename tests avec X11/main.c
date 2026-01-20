#include <stdlib.h>

#include <X11/Xlib.h>
#include <unistd.h>

int main() {

    Display* display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    Window root = RootWindow(display,screen);

    Window window = XCreateSimpleWindow(display,root,0,0,800,600,1,BlackPixel(display,screen),WhitePixel(display,screen));
    XMapWindow(display,window);
    XFlush(display);
    GC gc = XCreateGC(display,window, 0,NULL);

    

    XSetForeground(display,gc,0xFF0000);
    for (int x = 10 ; x < 50; x++){
        for (int y = 10;y<50;y++){  
            XDrawPoint(display, window, gc, 10,10);
            XFlush(display);
        }
    }

    XEvent event;
    XSelectInput(display,window,ExposureMask | KeyPressMask);
    
    while(1){
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            break;
        } 


    }

    XCloseDisplay(display);
    return 0;
}