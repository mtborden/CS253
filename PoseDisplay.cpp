/*! \file PoseDisplay.cpp
    \brief Contains the PoseDisplay class implementation

    Not surprisingly, the declaration for the PoseDisplay class is in PoseDisplay.h.
*/
#include<PoseDisplay.h>
#include<exception>
using std::exception;
#include<iostream>
using std::cerr;
using std::endl;
#include<math.h> // for round

// View is the area containing the pose data (-1 to 1)
// Frame is the view plus the axis display
// Window is the physical window
// The view and frames must be square. From FrameSize, ViewSize and HeraderHeight, all else follows.
const int HeaderHeight = 20;                           //!< Height of header used to label view planes (e.g. "XY Plane")
const int FrameSize = 240;                             //!< Size of (square) frame that contains both the pose view and the labeled axes
const int ViewSize = 200;                              //!< Height of (square) pose view containing the skeleton figure representing a pose
const int ViewOffset = (FrameSize - ViewSize) / 2;     //!< Distance from the beginning of the frame to the beginning of the view
const int WindowHeight = FrameSize + HeaderHeight;     //!< Total height of the X11 window (frame + header)
const int WindowWidth = 3 * FrameSize;                 //!< Total width of the X11 window (holds three frames sides by side)
const int ViewTop = HeaderHeight + ViewOffset;         //!< Y coordinate of the top of a view (all three views are at the same height)
const int ViewMiddle = ViewTop + (ViewSize / 2);       //!< Y coordinate of the middle of a view (all three views are at the same height)
const int ViewBottom = ViewTop + ViewSize;             //!< Y coordinate of the bottom of a view  (all three views are at the same height)
const double ViewScale = ViewSize / 2.0;               //!< Scale factor for going from logical coordinates (-1 to 1) to pixels in the view

/// X11 Error Handler. Make X11 throw std::exception(), so that all errors can be handled the same way.
int HandleX11Errors(Display* d, XErrorEvent* e) {throw std::exception();}

/*! \param output_filename is the name of the file that will be opened for output, so that all poses will be saved to this file.
    \param visual_display determines whether or not poses are displayed to an X11 window (and whether an X11 window is ever created).

    This constructor will throw a std::exception() if it is unable to open the output file, or if visual_display is true but it is unable to open an X11 window.
 */
PoseDisplay::PoseDisplay(const string& output_filename, bool visual_display) : displayp(visual_display), filename(output_filename)
{
  if (displayp) {
    XSetErrorHandler( HandleX11Errors );
    if (!OpenOutputWindow()) throw std::exception();
    InitializeSkeleton();
  }
  if (!OpenOutputFile()) throw std::exception();
}

PoseDisplay::~PoseDisplay()
{
  if (displayp) {
    XFreeGC(display_ptr, graphics_context);
    XDestroyWindow(display_ptr, window);
    XCloseDisplay(display_ptr);
  }
  ostr.close();
}

bool PoseDisplay::OpenOutputFile()
{
  ostr.open(filename.c_str());
  if (!ostr) {
    cerr << "Error: unable to open output file " << filename << endl;
    return false;
  }
  return true;
}

/*! This method relies on the X11 library to crate the display window. If XOpenDisplay, XCreateWindow or
    XCreateCG return NULL (in their attempt to create a display, window and graphics context, respectively),
    this method will return false. Otherwise it returns true. The window created will be empty (cleared); the
    headers and boundary are drawn when poses are displayed.
 */
bool PoseDisplay::OpenOutputWindow()
{
  // Open the X Server display
  display_ptr = XOpenDisplay(0);
  if (!display_ptr) {
    cerr << "Error: unable to open X display" << endl;
    return false;
  }
  int screen = DefaultScreen(display_ptr);

  // Create the window (set black & white, or else XClearWindow is a no-op)
  int blackcolor = BlackPixel(display_ptr, screen);
  int whitecolor = WhitePixel(display_ptr, screen);
  window = XCreateSimpleWindow(display_ptr, DefaultRootWindow(display_ptr), 0, 0, WindowWidth, WindowHeight,
			       0, blackcolor, whitecolor);
  if (!window) {
    cerr << "Error: unable to create skeleton window" << endl;
    return false;
  }

  // Map window to the screen, and wait for it to appear
  XSelectInput(display_ptr, window, StructureNotifyMask);
  XMapWindow(display_ptr, window);

  XEvent Event;
  do
    XNextEvent(display_ptr, &Event);
  while
    (Event.type != MapNotify);

  // Create the graphics context, and make sure window is clear
  graphics_context = XCreateGC(display_ptr, window, 0, NULL);
  if (!graphics_context) {
    cerr << "Error: unable to create graphics context" << endl;
    return false;
  }
  XClearWindow(display_ptr, window);
  XFlush(display_ptr);
  return true;
}

void PoseDisplay::DrawWindowFrames() const
{
  // Vertical and Horizontal Bars
  XDrawLine(display_ptr, window, graphics_context, 0, HeaderHeight, WindowWidth, HeaderHeight);
  XDrawLine(display_ptr, window, graphics_context, FrameSize, HeaderHeight, FrameSize, WindowHeight);
  XDrawLine(display_ptr, window, graphics_context, FrameSize+1, HeaderHeight, FrameSize+1, WindowHeight);
  XDrawLine(display_ptr, window, graphics_context, 2*FrameSize, HeaderHeight, 2*FrameSize, WindowHeight);
  XDrawLine(display_ptr, window, graphics_context, 2*FrameSize+1, HeaderHeight, 2*FrameSize+1, WindowHeight);

  DrawView("XY Plane", "X=0", "Y", 0);
  DrawView("ZY Plane", "Z=0", "Y", 1);
  DrawView("XZ Plane", "X=0", "Z", 2);
}

void PoseDisplay::DrawView(string label, string horizontal_axis, string vertical_axis, int frame_number) const
{
  int view_left = (frame_number*FrameSize) + ViewOffset;
  int view_right = view_left + ViewSize;
  int view_lr_middle = view_left + (ViewSize / 2);

  // Text on top
  XDrawString(display_ptr, window, graphics_context, view_lr_middle-(label.size()*4), HeaderHeight-5, label.c_str(), label.size());

  // Horizontal Axis with Ticks and Labels
  XDrawLine(display_ptr, window, graphics_context, view_left, ViewBottom, view_right, ViewBottom);
  XDrawLine(display_ptr, window, graphics_context, view_left, ViewBottom, view_left, ViewBottom+5);
  XDrawString(display_ptr, window, graphics_context, view_left-4, ViewBottom+15, "-1", 2);
  XDrawLine(display_ptr, window, graphics_context, view_lr_middle, ViewBottom, view_lr_middle, ViewBottom+5);
  XDrawString(display_ptr, window, graphics_context, 
	      view_lr_middle-(horizontal_axis.size()*2), ViewBottom+15, horizontal_axis.c_str(), horizontal_axis.size());
  XDrawLine(display_ptr, window, graphics_context, view_right, ViewBottom, view_right, ViewBottom+5);
  XDrawString(display_ptr, window, graphics_context, view_right-2, ViewBottom+15, "1", 1);

  // Vertical Axis with Ticks and Labels
  XDrawLine(display_ptr, window, graphics_context, view_left, ViewBottom, view_left, ViewTop);
  XDrawLine(display_ptr, window, graphics_context, view_left, ViewBottom, view_left-5, ViewBottom);
  XDrawString(display_ptr, window, graphics_context, view_left-15, ViewBottom+4, "-1", 2);
  XDrawLine(display_ptr, window, graphics_context, view_left, ViewMiddle, view_left-5, ViewMiddle);
  XDrawString(display_ptr, window, graphics_context, view_left-15, ViewMiddle+4, "0", 1);
  XDrawLine(display_ptr, window, graphics_context, view_left, ViewTop, view_left-5, ViewTop);
  XDrawString(display_ptr, window, graphics_context, view_left-15, ViewTop+4, "1", 1);
}

bool PoseDisplay::Pose(const vector<Point3D>& data, int ms_delay)
{
  if (data.size() != 25) return false;

  if (displayp) {
    DrawPose(data);
    if (ms_delay != 0) Pause(ms_delay);
  }

  return WritePose(data);
}

bool PoseDisplay::WritePose(const vector<Point3D>& data)
{
  for(vector<Point3D>::const_iterator iter = data.begin(); iter != data.end(); iter++) {
    ostr << *iter;
  }
  ostr << endl;
  return !(ostr.fail());
}

void PoseDisplay::DrawPose(const vector<Point3D>& data) const
{
  XClearWindow(display_ptr, window);
  XFlush(display_ptr);
  DrawWindowFrames();

  for(vector<pair<int, int> >::const_iterator iter = connections.begin(); iter != connections.end(); iter++) {
    DrawConnection(data[iter->first], data[iter->second]);
  }

  XFlush(display_ptr);
}

void PoseDisplay::DrawConnection(const Point3D& pt1, const Point3D& pt2) const
{
  DrawViewLine(0, pt1.X(), pt1.Y(), pt2.X(), pt2.Y());
  DrawViewLine(1, pt1.Z(), pt1.Y(), pt2.Z(), pt2.Y());
  DrawViewLine(2, pt1.X(), pt1.Z(), pt2.X(), pt2.Z());
}

void PoseDisplay::DrawViewLine(int view_number, double u1, double v1, double u2, double v2) const
{
  int view_lr_middle = (view_number + 0.5) * FrameSize;
  int U1 = round(view_lr_middle + (u1 * ViewScale)); 
  int V1 = round(ViewMiddle + (v1 * -ViewScale)); 
  int U2 = round(view_lr_middle + (u2 * ViewScale)); 
  int V2 = round(ViewMiddle + (v2 * -ViewScale)); 
  XDrawLine(display_ptr, window, graphics_context, U1, V1, U2, V2);
}

// source: https://msdn.microsoft.com/en-us/library/microsoft.kinect.jointtype.aspx
void PoseDisplay::InitializeSkeleton()
{
  connections.push_back(pair<int,int>(0, 1));    // spine_base to spine_mid
  connections.push_back(pair<int,int>(1, 20));   // spine_mid to spine_shoulder
  connections.push_back(pair<int,int>(20, 2));   // spine_shoulder to neck
  connections.push_back(pair<int,int>(2, 3));    // neck to head
  connections.push_back(pair<int,int>(20, 4));   // spine_shoulder to shoulder_left
  connections.push_back(pair<int,int>(4, 5));    // shoulder_left to elbow_left
  connections.push_back(pair<int,int>(5, 6));    // elbow_left to wrist_left
  connections.push_back(pair<int,int>(6, 7));    // wrist_left to hand_left
  connections.push_back(pair<int,int>(7, 21));   // hand_left to hand_tip_left
  connections.push_back(pair<int,int>(7, 22));   // hand_left to thumb_left
  connections.push_back(pair<int,int>(20, 8));   // spine_shoulder to shoulder_right
  connections.push_back(pair<int,int>(8, 9));    // shoulder_right to elbow_right
  connections.push_back(pair<int,int>(9, 10));   // elbow_right to wrist_right
  connections.push_back(pair<int,int>(10, 11));  // wrist_right to hand_right
  connections.push_back(pair<int,int>(11, 23));  // hand_right to hand_tip_right
  connections.push_back(pair<int,int>(11, 24));  // hand_right to thumb_right
  connections.push_back(pair<int,int>(0, 12));   // spine_base to hip_left
  connections.push_back(pair<int,int>(12, 13));  // hip_left to knee_left
  connections.push_back(pair<int,int>(13, 14));  // knee_left to ankle_left
  connections.push_back(pair<int,int>(14, 15));  // ankle_left to foot_left
  connections.push_back(pair<int,int>(0, 16));   // spine_base to hip_right
  connections.push_back(pair<int,int>(16, 17));  // hip_right to knee_right
  connections.push_back(pair<int,int>(17, 18));  // knee_right to ankle_right
  connections.push_back(pair<int,int>(18, 19));  // ankle_right to foot_right
}

