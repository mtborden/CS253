/*! \file PoseDisplay.h
    \brief Contains the PoseDisplay class declaration (header)

    Not surprisingly, the implementation for the PoseDisplay class is in PoseDisplay.cpp. This file
    depends on the X11 graphics library.
*/
#ifndef POSE_DISPLAY_H 
#define POSE_DISPLAY_H 

#include<Point3D.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <string>
using std::string;
#include<vector>
using std::vector;
#include<utility>
using std::pair;
#include<fstream>
using std::ofstream;
#include<thread>
#include<chrono>

//! Class for outputting and (optionally) displaying body poses expressed as a vector of 25 Point3Ds.
/*! 
    The PoseDisplay class has two purposes. The first is to display body poses to the screen so that you can see them.
    The second is to record all the displayed poses to a file, as a record of what your program displayed that we can grade.

    The public interface to PoseDisplay is simple. It has a constructor, a destructor, and a single public method called Pose.
    The constructor takes a file name as an argument. This is the file it will write the poses to. There is an optional second
    argument called visual_display that defaults to true. When this argument is true, poses will be drawn to the screen. When
    it is false, poses are written to the file but never displayed; in fact, no X11 window is ever created. This will be useful
    later in the semester when some assignments are graded by efficiency. The Pose method takes a body pose, i.e. a vector of
    25 Point3Ds, writes it to the output file, and displays it to the screen if visual_display is true. The destructor simply
    cleans up when a PoseDisplay is deleted or falls out of scope by destroying the window (if applicable) and closing the output
    file.
 */
class PoseDisplay {
 public:
  /// Constructor. May throw a std::exception if the window or file can't be opened. 
  PoseDisplay(const string& output_filename, bool visual_display = true);
  /// Destructor. Closes the X11 window (if its open) and the output file.
  ~PoseDisplay();
  /// Write a pose (25 body points) to the output file, and (optionally) display it to the window. ms_delay is the delay for viewing in milliseconds.
  bool Pose(const vector<Point3D>& data, int ms_delay = 33);
  /// Pause briefly, to allow display to be seen. Units of pause are millisecond, so 33 is roughly frame rate
  inline void Pause(int delay = 33) {std::this_thread::sleep_for(std::chrono::milliseconds(delay));}

 protected:
  /// Write a pose (25 body points) to the output file.
  bool WritePose(const vector<Point3D>& data);
  /// Draw a pose (25 body points) to the X11 window.
  void DrawPose(const vector<Point3D>& data) const;
  /// Draw a line connecting two body points in all three views.
  void DrawConnection(const Point3D& pt1, const Point3D& pt2) const;
  /// Open the output file. Return false if unable to open file.
  bool OpenOutputFile();
  /// Open an X11 window. Return false if unable to do so.
  bool OpenOutputWindow();
  /// Draw the borders, axes and labels associated with all three views to the X11 window.
  void DrawWindowFrames() const;
  /// Draw the axes and labels associated with one view to the X11 window.
  void DrawView(string label, string horizontal_axis, string vertical_axis, int frame_number) const;
  /// Draw a single line to a single image view.
  void DrawViewLine(int view_number, double x1, double y1, double x2, double y2) const;
  /// Initialize the vector of pairs that signals which body parts connect to which other body parts.
  void InitializeSkeleton();

  // X11 graphics data
  bool displayp;             //!< Whether or not poses are being displayed to a window.
  Display *display_ptr;      //!< Pointer to the X11 display structure
  Window window;             //!< X11 window structure (one application can have many windows)
  GC graphics_context;       //!< X11 graphics context structure (holds colors, line widths, etc.)

  // File output data
  string filename;           //!< Filename that poses will be recorded to.
  ofstream ostr;             //!< Output stream associated with filename.

  // Skeleton connection data from Microsoft
  vector<pair<int, int> > connections;   //!< Point connection information. An <i,j> pair indicates that Point i is connected to Point j.
};

#endif // POSE_DISPLAY_H 
