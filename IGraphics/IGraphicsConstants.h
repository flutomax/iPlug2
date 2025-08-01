/*
 ==============================================================================

 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.

 See LICENSE.txt for  more info.

 ==============================================================================
*/

#pragma once

#include "IPlugPlatform.h"

BEGIN_IPLUG_NAMESPACE
BEGIN_IGRAPHICS_NAMESPACE

static constexpr int DEFAULT_FPS = 60;

// If not dirty for this many timer ticks, we call OnGUIIDle.
// Only looked at if USE_IDLE_CALLS is defined.
static constexpr int IDLE_TICKS = 20;

static constexpr int DEFAULT_ANIMATION_DURATION = 100;

#ifndef CONTROL_BOUNDS_COLOR
#define CONTROL_BOUNDS_COLOR COLOR_GREEN
#endif

static constexpr float PARAM_EDIT_W = 40.f; // TODO: remove?
static constexpr float PARAM_EDIT_H = 16.f; // TODO: remove?

#define MAX_URL_LEN 256
#define MAX_NET_ERR_MSG_LEN 1024

static constexpr int MAX_IMG_SCALE = 3;
static constexpr int DEFAULT_TEXT_ENTRY_LEN = 7;
static constexpr double DEFAULT_GEARING = 4.0;

static constexpr double DEFAULT_MIN_DRAW_SCALE = 0.5;
static constexpr double DEFAULT_MAX_DRAW_SCALE = 4.0;

//what is this stuff
#define TOOLWIN_BORDER_W 6
#define TOOLWIN_BORDER_H 23
#define MAX_CLASSNAME_LEN 128
//

#ifndef GRAYED_ALPHA
static constexpr float GRAYED_ALPHA = 0.25f;
#endif

#ifndef DEFAULT_PATH
static const char* DEFAULT_PATH = "~/Desktop";
#endif

#ifndef DEFAULT_FONT
const char* const DEFAULT_FONT = "Roboto-Regular";
#endif

static constexpr float DEFAULT_TEXT_SIZE = 14.f;
static constexpr int FONT_LEN = 64;

/** @enum EBlend Porter-Duff blend mode/compositing operators */
enum class EBlend
{
  SrcOver,
  SrcIn,
  SrcOut,
  SrcAtop,
  DstOver,
  DstIn,
  DstOut,
  DstAtop,
  Add,
  XOR,
  Default = SrcOver
};

/** Constants that determine if a file dialog is for opening or saving */
enum class EFileAction { Open, Save };

/** Constants used for layout direction */
enum class EDirection { Vertical, Horizontal };

/** Constants to specify text styles when loading fonts. */
enum class ETextStyle { Normal, Bold, Italic };

/** Constants to specify horizontal alignment */
enum class EAlign { Near, Center, Far };

/** Constants to specify vertical alignment */
enum class EVAlign { Top, Middle, Bottom };

/** Constants for layout orientation e.g. location of labels in a IVControl */
enum class EOrientation { North, East, South, West };

/** CStrings for EAlign options  */
static const char* kEAlignStrs[3] = { "Near", "Center", "Far" };

/** CStrings for EVAlign options  */
static const char* kEVAlignStrs[3] = { "Top", "Middle", "Bottom" };

/** Types of Gesture Recongnizer */
enum class EGestureType { Unknown, DoubleTap, TripleTap, LongPress1, LongPress2, SwipeLeft, SwipeRight, SwipeUp, SwipeDown, Pinch, Rotate, Pan};

/** CStrings for EGestureType options  */
static const char* kGestureTypeStrs[12] = { "Unknown", "DoubleTap", "TripleTap", "LongPress1", "LongPress2", "SwipeLeft", "SwipeRight", "SwipeUp", "SwipeDown", "Pinch", "Rotate", "Pan"};

/** Distinguised gesture states */
enum class EGestureState { Unknown, Began, InProcess, Ended };

/** Light/Dark mode on iOS/macOS */
enum class EUIAppearance { Light, Dark };

/** EVColors are 9 color indices that are used by IVControls and make up an IVColorSpec */
enum EVColor
{
  kBG = 0,         // background: transparent by default
  kFG, kOFF = kFG, // foreground/OFF states
  kPR, kON = kPR,  // pressed/ON states
  kFR,             // frame: the stroke around a button or knob handle, or border around the outside of the control
  kHL,             // highlight: mouse over and splash click animation
  kSH,             // shadow
  kX1,             // extra1: typically used for indicator tracks on knobs and sliders
  kX2,             // extra2
  kX3,             // extra3
  kNumVColors
};

/** CStrings for EVColor options  */
static const char* kVColorStrs[kNumVColors] =
{
  "bg",
  "fg/off ",
  "pressed/on",
  "frame",
  "highlight",
  "shadow",
  "extra1",
  "extra2",
  "extra3"
};

/** \todo */
enum class EVShape { Rectangle, Ellipse, Triangle, EndsRounded, AllRounded };

/** \todo */
enum class EWinding { CW, CCW };

/** \todo */
enum class EFillRule { Winding, EvenOdd, Preserve };

/** \todo */
enum class ELineCap { Butt, Round, Square };

/** \todo */
enum class ELineJoin { Miter, Round, Bevel };

/** \todo */
enum class EPatternType { Solid, Linear, Radial, Sweep };

/** \todo */
enum class EPatternExtend { None, Pad, Reflect, Repeat };

/** \todo */
enum class EColorReplacement { None, Fill, Stroke };

/** \todo */
enum class EUIResizerMode { Scale, Size };

/** \todo */
enum class ECursor
{
  ARROW,
  IBEAM,
  WAIT,
  CROSS,
  UPARROW,
  SIZENWSE,
  SIZENESW,
  SIZEWE,
  SIZENS,
  SIZEALL,
  INO,
  HAND,
	HANDCLOSED,
  HANDOPEN,
  APPSTARTING,
  HELP,
  DRAGNDROP
};

/** \todo */
enum class ETouchEvent { Began, Moved, Ended, Cancelled, Invalid };

// This enumeration must match win32 message box options
enum EMsgBoxType
{
  kMB_OK = 0,
  kMB_OKCANCEL = 1,
  kMB_YESNOCANCEL = 3,
  kMB_YESNO = 4,
  kMB_RETRYCANCEL = 5
};

// <-- Added by Vasan - enum MessageBox Icons

enum EMsgBoxIcon
{
  kMB_NONE = 0,
  kMB_ICONHAND = 1,
  kMB_ICONQUESTION = 2,
  kMB_ICONEXCLAMATION = 3,
  kMB_ICONASTERISK = 4
};

// This enumeration must match win32 message box results
 //If IGraphics::ShowMessageBox can't return inline, it returns kNoResult (e.g. because it requires an asynchronous call)
enum EMsgBoxResult
{
  kNoResult,
  kOK = 1,
  kCANCEL = 2,
  kABORT = 3,
  kRETRY = 4,
  kIGNORE = 5,
  kYES = 6,
  kNO = 7
};

static const char* kMessageResultStrs[8] = {"", "OK", "CANCEL", "ABORT", "RETRY", "IGNORE", "YES", "NO"};

END_IGRAPHICS_NAMESPACE
END_IPLUG_NAMESPACE
