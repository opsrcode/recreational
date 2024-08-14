#include <stdio.h>
#include <X11/Xlib.h>

/*
 * Client <-> Display <-> Server -> Output 
 *                          |
 *                     Input/Event
 *
 * Display, Graphics Context (GC), Objects Handles, Events...
 *
 * Perform initialization routines
 * Connect to the X Server
 * Perform X-related initialization
 * While not finished:
 *   Receive the next event from the X Server
 *   Handle the event, possibly sending various drawing requests to
 *   the X Server
 *   If the event was a quit message, exit the loop
 * Close down the connection to the X Server
 * Perform cleanup operations
 *
 * Compilation command:
 *   gcc -Wall -Wextra -Werror -Wpedantic -O3 -std=c99 \
 *       `pkg-config --cflags x11` -o paint main.c \
 *       `pkg-config --libs x11`
 *
 * Display *XOpenDisplay(_Xconst char *display_name);
 * int XCloseDisplay(Display *display);
 * int DefaultScreen(Display *display);
 * int DisplayWidth(Display *display, int screen_number);
 * int DisplayHeight(Display *display, int screen_number);
 * Window RootWindow(Display *display, int screen_number);
 * Window DefaultRootWindow(Display *display);
 * int XMapWindow(Display *display, Window w);
 * int XUnmapWindow(Display *display, Window w);
 * GC DefaultGC(Display *display, int screen_number);
 * GC XCreateGC(Display *display, Drawable d, unsigned long valuemask,
 *              XGCValues *values);
 * int XSetLineAttributes(Display *display, GC gc,
 *                        unsigned int line_width, int line_style,
 *                        int cap_style, int join_style);
 * Window XCreateWindow(Display *display, Window parent, int x, int y,
 *                      unsigned int  width, unsigned int  height,
 *                      unsigned int border_width, int depth,
 *                      unsigned int class, Visual *visual,
 *                      unsigned long valuemask,
 *                      XSetWindowAttributes *attributes);
 * @opsrcode
 */

int
main(void)
{
  Display *display;
  Window parent, client;
  XSetWindowAttributes attributes;
  XEvent event;
  GC gc;
  XGCValues gc_values;
  int screen_number;
  register int draw;
  unsigned long int attributes_mask, gc_mask;

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "ERROR: Can't open display: %s\n",
            XDisplayName(NULL));
    return -1;
  }

  screen_number = DefaultScreen(display);
  parent = RootWindow(display, screen_number);
  attributes.background_pixel = BlackPixel(display, screen_number);
  attributes.border_pixel = WhitePixel(display, screen_number);
  attributes_mask = CWBackPixel | CWBorderPixel | CWEventMask;
  attributes.event_mask = PointerMotionMask | KeyPressMask |
                          ButtonPressMask | ButtonReleaseMask;

  client = XCreateWindow(display, parent, 0, 0,
                         DisplayWidth(display, screen_number),
                         DisplayHeight(display, screen_number), 20,
                         DefaultDepth(display, screen_number),
                         InputOutput,
                         DefaultVisual(display, screen_number),
                         attributes_mask, &attributes);

  gc_values.line_width = 3;
  gc_values.foreground = WhitePixel(display, screen_number);
  gc_mask = GCForeground | GCLineWidth;
  gc = XCreateGC(display, client, gc_mask, &gc_values);

  XMapWindow(display, client);

  for (draw = 0;;) {
    XNextEvent(display, &event);
    switch (event.type) {
    case MotionNotify:
      if (draw)
        XFillArc(display, client, gc, event.xmotion.x,
                 event.xmotion.y, (2*4), (2*4), 0, (360*64));
      break;
    case ButtonPress:
      if (event.xbutton.button == Button3)
        XClearWindow(display, client);
      else
        draw = 1;
      break;
    case ButtonRelease:
      draw = 0;
      break;
    case KeyPress:
      XUnmapWindow(display, client);
      XDestroyWindow(display, client);
      XCloseDisplay(display);
      return 0;
    }
  }
}
