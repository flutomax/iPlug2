/*
 ==============================================================================

 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.

 See LICENSE.txt for  more info.

 ==============================================================================
*/

#pragma once

/**
 * @file
 * @brief This file contains the base IControl implementation, along with some base classes for specific types of control.
 */

#include <cstring>
#include <cstdlib>
#include <vector>
#include <unordered_map>

#if defined VST3_API || defined VST3C_API
#undef stricmp
#undef strnicmp
#include "pluginterfaces/vst/ivstcontextmenu.h"
#include "base/source/fobject.h"
#endif

#include "IPlugPlatform.h"

#include "wdlstring.h"
#include "ptrlist.h"

#include "IGraphics.h"

BEGIN_IPLUG_NAMESPACE
BEGIN_IGRAPHICS_NAMESPACE

/** The lowest level base class of an IGraphics control. A control is anything on the GUI 
*  @ingroup BaseControls */
class IControl
#if defined VST3_API || defined VST3C_API
: public Steinberg::Vst::IContextMenuTarget
, public Steinberg::FObject
#endif
{
public:
  /** Constructor
   * @brief Creates an IControl
   * NOTE: An IControl does not know about the delegate or graphics context to which it belongs in the constructor
   * If you need to do something once those things are known, see IControl::OnInit()
   * @param bounds The rectangular area that the control occupies
   * @param paramIdx If this is > -1 (kNoParameter) this control will be associated with a plugin parameter
   * @param actionFunc pass in a lambda function to provide custom functionality when the control "action" happens (usually mouse down). */
  IControl(const IRECT& bounds, int paramIdx = kNoParameter, IActionFunction actionFunc = nullptr);
  
  /** Constructor (range of parameters)
   * @brief Creates an IControl which is linked to multiple parameters
   * NOTE: An IControl does not know about the delegate or graphics context to which it belongs in the constructor
   * If you need to do something once those things are know, see IControl::OnInit()
   * @param bounds The rectangular area that the control occupies
   * @param params An initializer list of valid integer parameter indexes
   * @param actionFunc pass in a lambda function to provide custom functionality when the control "action" happens (usually mouse down). */
  IControl(const IRECT& bounds, const std::initializer_list<int>& params, IActionFunction actionFunc = nullptr);
  
  /** Constructor (no paramIdx)
   * @brief Creates an IControl which is not linked to a parameter
   * NOTE: An IControl does not know about the delegate or graphics context to which it belongs in the constructor
   * If you need to do something once those things are known, see IControl::OnInit()
   * @param bounds The rectangular area that the control occupies
   * @param actionFunc pass in a lambda function to provide custom functionality when the control "action" happens (usually mouse down). */
  IControl(const IRECT& bounds, IActionFunction actionFunc);
  
  IControl(const IControl&) = delete;
  void operator=(const IControl&) = delete;
  
  /** Destructor. Clean up any resources that your control owns. */
  virtual ~IControl() {}

  /** Implement this method to respond to a mouse down event on this control. 
   * @param x The X coordinate of the mouse event
   * @param y The Y coordinate of the mouse event
   * @param mod A struct indicating which modifier keys are held for the event */
  virtual void OnMouseDown(float x, float y, const IMouseMod& mod);

/** Implement this method to respond to a mouse up event on this control. 
   * @param x The X coordinate of the mouse event
   * @param y The Y coordinate of the mouse event
   * @param mod A struct indicating which modifier keys are held for the event */
  virtual void OnMouseUp(float x, float y, const IMouseMod& mod) {}

  /** Implement this method to respond to a mouse drag event on this control. 
   * @param x The X coordinate of the mouse event
   * @param y The Y coordinate of the mouse event
   * @param dX The X delta (difference) since the last event
   * @param dY The Y delta (difference) since the last event
   * @param mod A struct indicating which modifier keys are held for the event */
  virtual void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) {}
   
  /** Implement this method to respond to a mouse double click event on this control. 
   * @param x The X coordinate of the mouse event
   * @param y The Y coordinate of the mouse event
   * @param mod A struct indicating which modifier keys are held for the event */
  virtual void OnMouseDblClick(float x, float y, const IMouseMod& mod);

  /** Implement this method to respond to a mouse wheel event on this control. 
   * @param x The X coordinate of the mouse event
   * @param y The Y coordinate of the mouse event
   * @param mod A struct indicating which modifier keys are held for the event 
   * @param d The delta value (difference) since the last mouse wheel event */
  virtual void OnMouseWheel(float x, float y, const IMouseMod& mod, float d) {};

  /** Implement this method to respond to a key down event on this control. 
   * @param x The X coordinate of the mouse at the time of this key down event
   * @param y The Y coordinate of the mouse at the time of this key down event
   * @param key \todo */
  virtual bool OnKeyDown(float x, float y, const IKeyPress& key) { return false; }

  /** Implement this method to respond to a key up event on this control.
   * @param x The X coordinate of the mouse at the time of this key down event
   * @param y The Y coordinate of the mouse at the time of this key down event
   * @param key \todo */
  virtual bool OnKeyUp(float x, float y, const IKeyPress& key) { return false; }
  
  /** Implement this method to respond to a mouseover event on this control. Implementations should call base class, if you wish to use mMouseIsOver.
   * @param x The X coordinate of the mouse event
   * @param y The Y coordinate of the mouse event
   * @param mod A struct indicating which modifier keys are held for the event */
  virtual void OnMouseOver(float x, float y, const IMouseMod& mod);

  /** Implement this method to respond to a mouseout event on this control. Implementations should call base class, if you wish to use mMouseIsOver. */
  virtual void OnMouseOut();
  
  /** Implement this method to respond to a touch cancel event on this control.
   * @param x The X coordinate of the mouse event
   * @param y The Y coordinate of the mouse event
   * @param mod A struct indicating which modifier keys are held for the event */
  virtual void OnTouchCancelled(float x, float y, const IMouseMod& mod) {}

  /** Implement to do something when something was drag 'n dropped onto this control */
  virtual void OnDrop(const char* str, float x, float y) {};

  /** Implement to do something when graphics is scaled globally (e.g. moves to different DPI screen) */
  virtual void OnRescale() {}

  /** Called when IControl is constructed or resized using SetRect(). NOTE: if you call SetDirty() in this method, you should call SetDirty(false) to avoid triggering parameter changes */
  virtual void OnResize() {}
  
  /** Called just prior to when the control is attached, after its delegate and graphics member variable set */
  virtual void OnInit() {}
  
  /** Called after the control has been attached, and its delegate and graphics member variable set. Use this method for controls that might need to attach sub controls that should be above their parent in the stack */
  virtual void OnAttached() {}
  
  /** Implement to receive messages sent to the control, see IEditorDelegate:SendControlMsgFromDelegate() */
  virtual void OnMsgFromDelegate(int msgTag, int dataSize, const void* pData) {};
  
  /** Implement to receive MIDI messages sent to the control if mWantsMidi == true, see IEditorDelegate:SendMidiMsgFromDelegate() */
  virtual void OnMidi(const IMidiMsg& msg) {};

  /** @return true if supports this gesture */
  virtual bool OnGesture(const IGestureInfo& info);
  
  /** Called by default when the user right clicks a control. If IGRAPHICS_NO_CONTEXT_MENU is enabled as a preprocessor macro right clicking control will mean IControl::CreateContextMenu() and IControl::OnContextSelection() do not function on right clicking control. VST3 provides contextual menu support which is hard wired to right click controls by default. You can add custom items to the menu by implementing IControl::CreateContextMenu() and handle them in IControl::OnContextSelection(). In non-VST 3 hosts right clicking will still create the menu, but it will not feature entries added by the host. */
  virtual void CreateContextMenu(IPopupMenu& contextMenu) {}
  
  /** Implement this method to handle popup menu selection after IGraphics::CreatePopupMenu/IControl::PromptUserInput
   * @param pSelectedMenu If pSelectedMenu is invalid it means the user didn't select anything
   * @param valIdx An index that indicates which of the controls vals the menu relates to */
  virtual void OnPopupMenuSelection(IPopupMenu* pSelectedMenu, int valIdx);

  /** Implement this method to handle text input after IGraphics::CreateTextEntry/IControl::PromptUserInput
   * @param str A CString with the inputted text
   * @param valIdx An index that indicates which of the controls vals the text completion relates to */
  virtual void OnTextEntryCompletion(const char* str, int valIdx) {}

  /** Implement this to respond to a menu selection from CreateContextMenu(); @see CreateContextMenu() */
  virtual void OnContextSelection(int itemSelected) {}

  /** Draw the control to the graphics context. 
   * @param g The graphics context to which this control belongs. */
  virtual void Draw(IGraphics& g) = 0;

  /** Implement this to customise how a colored highlight is drawn on the control in ProTools (AAX format only), when a control is linked to a parameter that is automated.
   * @param g The graphics context to which this control belongs. */
  virtual void DrawPTHighlight(IGraphics& g);

  /** Call this method in response to a mouse event to create an edit box so the user can enter a value, or pop up a pop-up menu, if the control is linked to a parameter (mParamIdx > kNoParameter)
   * @param valIdx An index to choose which of the controls linked parameters to retrieve. NOTE: since controls usually have only 1 parameter you can omit this argument and use the default index of 0 */
  void PromptUserInput(int valIdx = 0);
  
  /** Create a text entry box so the user can enter a value, or pop up a pop-up menu, if the control is linked to a parameter (mParamIdx > kNoParameter) specifying the bounds
   * @param bounds The rectangle for the text entry. Pop-up menu's will appear below the rectangle.
   * @param valIdx An index to choose which of the controls linked parameters to retrieve. NOTE: since controls usually have only 1 parameter you can omit this argument and use the default index of 0 */
  void PromptUserInput(const IRECT& bounds, int valIdx = 0);
  
  /** Set an Action Function for this control. 
   * actionfunc @see Action Functions */
  inline void SetActionFunction(IActionFunction actionFunc) { mActionFunc = actionFunc; }

  /** Set an Action Function to be called at the end of an animation.
   * actionfunc @see Action Functions */
  inline void SetAnimationEndActionFunction(IActionFunction actionFunc) { mAnimationEndActionFunc = actionFunc; }
  
  /** Set a tooltip for the control
   * @param str CString tooltip to be displayed */
  inline void SetTooltip(const char* str) { mTooltip.Set(str); }
  
  /** @return Currently set tooltip text */
  inline const char* GetTooltip() const { return mTooltip.Get(); }

  /** Get the index of a parameter that the control is linked to
   * Normaly controls are either linked to a single parameter or no parameter but some may be linked to multiple parameters
   * @param valIdx An index to choose which of the controls linked parameters to retrieve. NOTE: since controls usually have only 1 parameter you can omit this argument and use the default index of 0
   * @return Parameter index, or kNoParameter if there is no parameter linked with this control at valIdx */
  int GetParamIdx(int valIdx = 0) const;
  
  /** Set the index of a parameter that the control is linked to
   * If you are calling this "manually" to reuse a control for multiple parameters, you probably want to call IEditorDelegate::SendCurrentParamValuesFromDelegate() afterward, to update the control values
   * @param paramIdx Parameter index, or kNoParameter if there is no parameter linked with this control at valIdx
   * @param valIdx An index to choose which of the controls vals to set */
  void SetParamIdx(int paramIdx, int valIdx = 0);
 
  /** Check if the control is linked to a particular parameter
   * @param paramIdx The paramIdx to test
   * @return the valIdx if linked, or kNoValIdx if not */
  int LinkedToParam(int paramIdx) const;
  
  /** @return The number of values for this control */
  int NVals() const { return (int) mVals.size(); }

  /** Check to see which of the control's values relates to this x and y coordinate
   * @param x x coordinate to check
   * @param y x coordinate to check
   * @return An integer specifying which value matches the x, y coordinates, or kNoValIdx if the position is not linked to a value. */
  virtual int GetValIdxForPos(float x, float y) const { return mVals.size() == 1 ? 0 : kNoValIdx; }
  
  /** Get a const pointer to the IParam object (owned by the editor delegate class), associated with this control
   * @return const pointer to an IParam or nullptr if the control is not associated with a parameter */ 
  const IParam* GetParam(int valIdx = 0) const;
  
  /** Set the control's value from the delegate
   * This method is called from the class implementing the IEditorDelegate interface in order to update a control's value members and set it to be marked dirty for redraw.
   * @param value Normalised incoming value
   * @param valIdx The index of the value to set, which should be between 0 and NVals() */
  virtual void SetValueFromDelegate(double value, int valIdx = 0);
  
  /** Set the control's value after user input.
   * This method is called after a text entry or popup menu prompt triggered by PromptUserInput(), calling SetDirty(true), which will mean that the new value gets sent back to the delegate
   * @param value the normalised value after user input via text entry or pop-up menu
   * @param valIdx An index to choose which of the controls linked parameters to retrieve. NOTE: since controls usually have only 1 parameter you can omit this argument and use the default index of 0 */
  virtual void SetValueFromUserInput(double value, int valIdx = 0);
    
  /** Set one or all of the control's values to the default value of the associated parameter.
   * @param valIdx either an integer > -1 (kNoValIdx) in order to set an individual value to the default value of the associated parameter, or kNoValIdx to default all values
   * This method will call through to SetDirty(true, valIdx), which will mean that the new value gets sent back to the delegate */
  virtual void SetValueToDefault(int valIdx = kNoValIdx);
  
  /** Set one of the control's values.
   * @param value The normalized 0-1 value
   * @param valIdx The index of the value to set, which should be between 0 and NVals() */
  virtual void SetValue(double value, int valIdx = 0);
  
  /** Get the control's value
   * @return Value of the control, normalized in the range 0-1
   * @param valIdx The index of the value to set, which should be between 0 and NVals() */
  double GetValue(int valIdx = 0) const;
  
  /** Assign the control to a control group @see Control Groups
   * @param groupName A CString indicating the control group that this control should belong to */
  void SetGroup(const char* groupName) { mGroup.Set(groupName); }
  
  /** Get the group that the control belongs to, if any
   * @return A CString indicating the control group that this control belongs to (may be empty) */
  const char* GetGroup() const { return mGroup.Get(); }

  /** Get the Text object for the control
   * @return const IText& The control's mText object, typically used to determine font/layout/size etc of the main text in a control. */
  const IText& GetText() const { return mText; }

  /** Set the Text object typically used to determine font/layout/size etc of the main text in a control
   * @param txt An IText struct with the desired formatting */
  void SetText(const IText& txt) { mText = txt; }

  /** Set the Blend for this control. This can be used differently by different controls, or not at all.
   *  By default it is used to change the opacity of controls when they are disabled */
  void SetBlend(const IBlend& blend) { mBlend = blend; }

  /** Get the Blend for this control */
  IBlend GetBlend() const { return mBlend; }

  /** Get the max number of characters that are allowed in text entry 
   * @return int The max number of characters allowed in text entry */
  int GetTextEntryLength() const { return mTextEntryLength; }

  /** Set the max number of characters that are allowed in text entry 
   * @param len The max number of characters allowed in text entry */
  void SetTextEntryLength(int len) { mTextEntryLength = len;  }
  
  /** Get the rectangular draw area for this control, within the graphics context
   * @return The control's bounds */
  const IRECT& GetRECT() const { return mRECT; }

  /** Set the rectangular draw area for this control, within the graphics context
   * @param bounds The control's bounds */
  void SetRECT(const IRECT& bounds) { mRECT = bounds; mMouseIsOver = false; OnResize(); }
  
  /** Get the rectangular mouse tracking target area, within the graphics context for this control
   * @return The control's target bounds within the graphics context */
  const IRECT& GetTargetRECT() const { return mTargetRECT; } // The mouse target area (default = draw area).

  /** Set the rectangular mouse tracking target area, within the graphics context for this control
   * @param bounds The control's new target bounds within the graphics context */
  void SetTargetRECT(const IRECT& bounds) { mTargetRECT = bounds; mMouseIsOver = false; }
  
  /** Set BOTH the draw rect and the target area, within the graphics context for this control
   * @param bounds The control's new draw and target bounds within the graphics context */
  void SetTargetAndDrawRECTs(const IRECT& bounds) { mRECT = mTargetRECT = bounds; mMouseIsOver = false; OnResize(); }

  /** Set the position of the control, preserving the width and height. This may need to be overriden if you maintain custom positioning data in your control
   * @param x the new x coordinate of the top left corner of the control
   * @param y the new y coordinate of the top left corner of the control */
  virtual void SetPosition(float x, float y);

  /** Set the size of the control, preserving the current position. This may need to be overriden if you maintain custom positioning data in your control, or if your TargetRECT is not the same as the main RECT.
   * @param w the new width of the control
   * @param h the new height of the control */
  virtual void SetSize(float w, float h);

  /** Used internally by the AAX wrapper view interface to set the control parmeter highlight 
   * @param isHighlighted /c true if the control should be highlighted 
   * @param color An integer representing one of three colors that ProTools assigns automated controls */
  void SetPTParameterHighlight(bool isHighlighted, int color);
  
  /** Get double click as single click 
   * By default, mouse double click has its own handler. A control can set mDblAsSingleClick to true 
   * which maps double click to single click for this control (and also causes the mouse to be captured by the control on double click).
   * @return /c true if double clicks should be mapped to single clicks */
  bool GetMouseDblAsSingleClick() const { return mDblAsSingleClick; }

  /** Shows or hides the IControl.
   * @param hide Set to \c true to hide the control */
  virtual void Hide(bool hide);
  
  /** @return \c true if the control is hidden. */
  bool IsHidden() const { return mHide; }

  /** Sets disabled mode for the control, the default implementation modifies the mBlend member
   * @param disable \c true for disabled */
  virtual void SetDisabled(bool disable);
  
  /** @return \c true if the control is disabled */
  bool IsDisabled() const { return mDisabled; }

  /** Specify whether the control should respond to mouse overs when disabled
   * @param allow \c true if it should respond to mouse overs when disabled (false by default) */
  void SetMouseOverWhenDisabled(bool allow) { mMouseOverWhenDisabled = allow; }

  /** Specify whether the control should respond to other mouse events when disabled
   * @param allow \c true if it should respond to other mouse events when disabled (false by default) */
  void SetMouseEventsWhenDisabled(bool allow) { mMouseEventsWhenDisabled = allow; }

  /** @return \c true if the control responds to mouse overs when disabled */
  bool GetMouseOverWhenDisabled() const { return mMouseOverWhenDisabled; }

  /** @return \c true if the control responds to other mouse events when disabled */
  bool GetMouseEventsWhenDisabled() const { return mMouseEventsWhenDisabled; }
  
  /** @return \c true if the control ignores mouse events */
  bool GetIgnoreMouse() const { return mIgnoreMouse; }
  
  /** @return \c true if the control should show parameter labels/units e.g. "Hz" in text entry prompts */
  bool GetPromptShowsParamLabel() const { return mPromptShowsParamLabel; }

  /** Set if the control should show parameter labels/units e.g. "Hz" in text entry prompts */
  void SetPromptShowsParamLabel(bool enable) { mPromptShowsParamLabel = enable; }
  
  /** Hit test the control. Override this method if you want the control to be hit only if a visible part of it is hit, or whatever.
   * @param x The X coordinate within the control to test 
   * @param y The y coordinate within the control to test
   * @return \c Return true if the control was hit. */
  virtual bool IsHit(float x, float y) const { return mTargetRECT.Contains(x, y); }

  /** Mark the control as dirty, i.e. it should be redrawn on the next display refresh
   * @param triggerAction If this is true and the control is linked to a parameter
   * notify the class implementing the IEditorDelegate interface that the parameter changed. If this control has an ActionFunction, that can also be triggered.
   * NOTE: it is easy to forget that this method always sets the control dirty, the argument refers to whether a consecutive action should be performed */
  virtual void SetDirty(bool triggerAction = true, int valIdx = kNoValIdx);

  /* Set the control clean, i.e. Called by IGraphics draw loop after control has been drawn */
  virtual void SetClean() { mDirty = false; }

  /* Called at each display refresh by the IGraphics draw loop, triggers the control's AnimationFunc if it is set */
  void Animate();

  /** Called at each display refresh by the IGraphics draw loop, after IControl::Animate(), to determine if the control is marked as dirty. 
   * @return \c true if the control is marked dirty. */
  virtual bool IsDirty();

  /** Disable/enable right-clicking the control to prompt for user input /todo check this
   * @param disable \c true*/
  void DisablePrompt(bool disable) { mDisablePrompt = disable; }

  /** This is an idle call from the GUI thread, only active if USE_IDLE_CALLS is defined. /todo check this */
  virtual void OnGUIIdle() {}

  /** This is an called when focus is lost. /todo check this */
  virtual void OnLostFocus() {}
  
  /** Set the control's tag. Controls can be given tags, in order to direct messages to them. @see Control Tags
   * @param tag A unique integer to identify this control */
  void SetTag(int tag) { mTag = tag; }
  
  /** Get the control's tag. @see Control Tags */
  int GetTag() const { return mTag; }
  
  /** Specify whether this control wants to know about MIDI messages sent to the UI. See OnMIDIMsg() */
  void SetWantsMidi(bool enable = true) { mWantsMidi = enable; }

  /** @return /c true if this control wants to know about MIDI messages send to the UI. See OnMIDIMsg() */
  bool GetWantsMidi() const { return mWantsMidi; }

  /** Specify whether this control supports multiple touches */
  void SetWantsMultiTouch(bool enable = true) { mWantsMultiTouch = enable; }
  
  /** @return /c true if this control supports multiple touches */
  bool GetWantsMultiTouch() const { return mWantsMultiTouch; }
  
  /** Add a IGestureFunc that should be triggered in response to a certain type of gesture
   * @param type The type of gesture to recognize on this control
   * @param func the function to trigger */
  IControl* AttachGestureRecognizer(EGestureType type, IGestureFunc func);
  
  /** @return /c true if this control supports multiple gestures */
  bool GetWantsGestures() const { return mGestureFuncs.size() > 0 && !mAnimationFunc; }
  
  /** @return the last recognized gesture */
  EGestureType GetLastGesture() const { return mLastGesture; }
  
  /** Gets a pointer to the class implementing the IEditorDelegate interface that handles parameter changes from this IGraphics instance.
   * If you need to call other methods on that class, you can use static_cast<PLUG_CLASS_NAME>(GetDelegate();
   * @return The class implementing the IEditorDelegate interface that handles communication to/from from this IGraphics instance.*/
  IGEditorDelegate* GetDelegate() { return mDelegate; }
  
  /** Used internally to set the mDelegate (and mGraphics) variables */
  void SetDelegate(IGEditorDelegate& dlg)
  {
    mDelegate = &dlg;
    mGraphics = dlg.GetUI();
    OnInit();
    OnResize();
    OnRescale();
  }
  
  /** @return A pointer to the IGraphics context that owns this control */ 
  IGraphics* GetUI() { return mGraphics; }
    
  /** @return A const pointer to the IGraphics context that owns this control */
  const IGraphics* GetUI() const { return mGraphics; }

  /* This can be used in IControl::Draw() to check if the mouse is over the control, without implementing mouse over methods 
   * @return \true if the mouse is over this control. */
  bool GetMouseIsOver() const { return mMouseIsOver; }
  
  /** Set control value based on x, y position within a rectangle. Commonly used for slider/fader controls.
   * @param x The X coordinate for snapping
   * @param y The Y coordinate for snapping
   * @param direction The direction of the control's travel- horizontal or vertical fader
   * @param bounds The area in which the track of e.g. a slider should be snapped
   * @param valIdx /todo */
  virtual void SnapToMouse(float x, float y, EDirection direction, const IRECT& bounds, int valIdx = -1, double minClip = 0., double maxClip = 1.);

  /* if you override this you must call the base implementation, to free mAnimationFunc */
  virtual void OnEndAnimation();
  
  /** @param duration Duration in milliseconds for the animation  */
  void StartAnimation(int duration);
  
  /** Set the animation function
   * @param func A std::function conforming to IAnimationFunction */
  void SetAnimation(IAnimationFunction func) { mAnimationFunc = func;}
  
  /** Set the animation function and starts it
   * @param func A std::function conforming to IAnimationFunction
   * @param duration Duration in milliseconds for the animation  */
  void SetAnimation(IAnimationFunction func, int duration) { mAnimationFunc = func; StartAnimation(duration); }

  /** Get the control's animation function, if it exists */
  IAnimationFunction GetAnimationFunction() { return mAnimationFunc; }

  /** Get the control's action function, if it exists */
  IAnimationFunction GetActionFunction() { return mActionFunc; }

  /** Get the progress in a control's animation, in the range 0-1 */
  double GetAnimationProgress() const;
  
  /** Get the duration of animations applied to the control */
  Milliseconds GetAnimationDuration() const { return mAnimationDuration; }

  /** Helper function to dynamic cast an IControl to a subclass */
  template <class T>
  T* As() { return dynamic_cast<T*>(this); }
    
#if defined VST3_API || defined VST3C_API
  Steinberg::tresult PLUGIN_API executeMenuItem (Steinberg::int32 tag) override { OnContextSelection(tag); return Steinberg::kResultOk; }
#endif
  
#pragma mark - IControl Member variables
protected:
  
  /** A helper template function to call a method for an individual value, or for all values
   * @param valIdx If this is > kNoValIdx execute the function for an individual value. If equal to kNoValIdx call the function for all values
   * @param func A function that takes a single integer argument, the valIdx \todo
   * @param args Arguments to the function */
  template<typename T, typename... Args>
  void ForValIdx(int valIdx, T func, Args... args)
  {
    if (valIdx > kNoValIdx)
      func(valIdx, args...);
    else
    {
      const int nVals = NVals();
      for (int v = 0; v < nVals; v++)
        func(v, args...);
    }
  }
    
  int mTag = kNoTag;
  IRECT mRECT;
  IRECT mTargetRECT;
  
  /** Controls can be grouped for hiding and showing panels */
  WDL_String mGroup;
  
  IText mText;
  IBlend mBlend;
  int mTextEntryLength = DEFAULT_TEXT_ENTRY_LEN;
  bool mDirty = true;
  bool mHide = false;
  bool mDisabled = false;
  bool mDisablePrompt = true;
  bool mDblAsSingleClick = false;
  bool mMouseOverWhenDisabled = false;
  bool mMouseEventsWhenDisabled = false;
  bool mIgnoreMouse = false;
  bool mWantsMidi = false;
  bool mWantsMultiTouch = false;
  bool mPromptShowsParamLabel = false;
  /** if mGraphics::mHandleMouseOver = true, this will be true when the mouse is over control. If you need finer grained control of mouseovers, you can override OnMouseOver() and OnMouseOut() */
  bool mMouseIsOver = false;
  WDL_String mTooltip;

  IColor mPTHighlightColor = COLOR_RED;
  bool mPTisHighlighted = false;
  
  void SetNVals(int nVals)
  {
    assert(nVals > 0);
    mVals.resize(nVals);
  }

#if defined VST3_API || defined VST3C_API
  OBJ_METHODS(IControl, FObject)
  DEFINE_INTERFACES
  DEF_INTERFACE (IContextMenuTarget)
  END_DEFINE_INTERFACES (FObject)
  REFCOUNT_METHODS(FObject)
#endif
  
private:
  IGEditorDelegate* mDelegate = nullptr;
  IGraphics* mGraphics = nullptr;
  IActionFunction mActionFunc = nullptr;
  IActionFunction mAnimationEndActionFunc = nullptr;
  IAnimationFunction mAnimationFunc = nullptr;
  TimePoint mAnimationStartTime;
  Milliseconds mAnimationDuration;
  std::vector<ParamTuple> mVals { {kNoParameter, 0.} };
  std::unordered_map<EGestureType, IGestureFunc> mGestureFuncs;
  EGestureType mLastGesture = EGestureType::Unknown;
};

#pragma mark - Base Controls

/**
 * \addtogroup BaseControls
 * @{
 */

/** A base interface, to be combined with IControl for bitmap-based controls "IBControls", managing an IBitmap */
class IBitmapBase
{
public:
  IBitmapBase(const IBitmap& bitmap)
  : mBitmap(bitmap)
  {
  }
  
  virtual ~IBitmapBase() {}
  
  void AttachIControl(IControl* pControl) { mControl = pControl; }
  
  void DrawBitmap(IGraphics& g)
  {
    int i = 1;
    if (mBitmap.N() > 1)
    {
      i = 1 + int(0.5 + mControl->GetValue() * (double) (mBitmap.N() - 1));
      i = Clip(i, 1, mBitmap.N());
    }
    IBlend blend = mControl->GetBlend();
    g.DrawBitmap(mBitmap, mControl->GetRECT().GetCentredInside(IRECT(0, 0, mBitmap)), i, &blend);
  }

protected:
  IBitmap mBitmap;
  IControl* mControl = nullptr;
};

/** A base interface to be combined with IControl for vectorial controls "IVControls", in order for them to share a common style
 * If you need more flexibility, you're on your own! */
class IVectorBase
{
public:
  IVectorBase(const IVStyle& style, bool labelInWidget = false, bool valueInWidget = false)
  : mLabelInWidget(labelInWidget)
  , mValueInWidget(valueInWidget)
  {
    SetStyle(style);
  }
  
  void AttachIControl(IControl* pControl, const char* label)
  {
    mControl = pControl;
    mLabelStr.Set(label);
  }

  void SetColor(EVColor colorIdx, const IColor& color)
  {
    mStyle.colorSpec.mColors[static_cast<int>(colorIdx)] = color;
    mControl->SetDirty(false);
  }

  void SetColors(const IVColorSpec& spec)
  {
    mStyle.colorSpec = spec;
  }

  const IColor& GetColor(EVColor color) const
  {
    return mStyle.colorSpec.GetColor(color);
  }
  
  void SetLabelStr(const char* label) { mLabelStr.Set(label); mControl->SetDirty(false); }
  void SetValueStr(const char* value) { mValueStr.Set(value); mControl->SetDirty(false); }
  void SetWidgetFrac(float frac) { mStyle.widgetFrac = Clip(frac, 0.f, 1.f);  mControl->OnResize(); mControl->SetDirty(false); }
  void SetAngle(float angle) { mStyle.angle = Clip(angle, 0.f, 360.f);  mControl->SetDirty(false); }
  void SetShowLabel(bool show) { mStyle.showLabel = show;  mControl->OnResize(); mControl->SetDirty(false); }
  void SetShowValue(bool show) { mStyle.showValue = show;  mControl->OnResize(); mControl->SetDirty(false); }
  void SetRoundness(float roundness) { mStyle.roundness = Clip(roundness, 0.f, 1.f); mControl->SetDirty(false); }
  void SetDrawFrame(bool draw) { mStyle.drawFrame = draw; mControl->SetDirty(false); }
  void SetDrawShadows(bool draw) { mStyle.drawShadows = draw; mControl->SetDirty(false); }
  void SetEmboss(bool draw) { mStyle.emboss = draw; mControl->SetDirty(false); }
  void SetShadowOffset(float offset) { mStyle.shadowOffset = offset; mControl->SetDirty(false); }
  void SetFrameThickness(float thickness) { mStyle.frameThickness = thickness; mControl->SetDirty(false); }
  void SetSplashRadius(float radius) { mSplashRadius = radius * mMaxSplashRadius; }
  void SetSplashPoint(float x, float y) { mSplashPoint.x = x; mSplashPoint.y = y; }
  void SetShape(EVShape shape) { mShape = shape; mControl->SetDirty(false); }

  virtual void SetStyle(const IVStyle& style)
  {
    mStyle = style;
    SetColors(style.colorSpec);
  }

  IVStyle GetStyle() const { return mStyle; }
  
  IRECT GetAdjustedHandleBounds(IRECT handleBounds) const
  {
    if(mStyle.drawFrame)
      handleBounds.Pad(- 0.5f * mStyle.frameThickness);
    
    if (mStyle.drawShadows)
      handleBounds.Alter(mStyle.shadowOffset, 0, -mStyle.shadowOffset, -mStyle.shadowOffset);
    
    return handleBounds;
  }
  
  float GetRoundedCornerRadius(const IRECT& bounds) const
  {
    if(bounds.W() < bounds.H())
      return mStyle.roundness * (bounds.W() / 2.f);
    else
      return mStyle.roundness * (bounds.H() / 2.f);
  }
  
  void DrawSplash(IGraphics& g, const IRECT& clipRegion = IRECT())
  {
    g.PathClipRegion(clipRegion);
    g.FillCircle(GetColor(kHL), mSplashPoint.x, mSplashPoint.y, mSplashRadius);
    g.PathClipRegion(IRECT());
  }
  
  virtual void DrawBackGround(IGraphics& g, const IRECT& rect)
  {
    IBlend blend = mControl->GetBlend();
    g.FillRect(GetColor(kBG), rect, &blend);
  }
  
  virtual void DrawWidget(IGraphics& g)
  {
    // no-op
  }
  
  virtual void DrawLabel(IGraphics& g)
  {
    if (mLabelBounds.H() && mStyle.showLabel)
    {
      IBlend blend = mControl->GetBlend();
      g.DrawText(mStyle.labelText, mLabelStr.Get(), mLabelBounds, &blend);
    }
  }
  
  virtual void DrawValue(IGraphics& g, bool mouseOver)
  {
    if(mouseOver)
      g.FillRect(COLOR_TRANSLUCENT, mValueBounds);
    
    if (mStyle.showValue)
    {
      IBlend blend = mControl->GetBlend();
      g.DrawText(mStyle.valueText, mValueStr.Get(), mValueBounds, &blend);
    }
  }
  
  virtual void DrawPressableShape(IGraphics& g, EVShape shape, const IRECT& bounds, bool pressed, bool mouseOver, bool disabled)
  {
    switch (shape)
    {
    case EVShape::Ellipse:
      DrawPressableEllipse(g, bounds, pressed, mouseOver, disabled);
      break;
    case EVShape::Rectangle:
      DrawPressableRectangle(g, bounds, pressed, mouseOver, disabled);
      break;
    case EVShape::Triangle:
      DrawPressableTriangle(g, bounds, pressed, mouseOver, mStyle.angle, disabled);
      break;
    case EVShape::EndsRounded:
      DrawPressableRectangle(g, bounds, pressed, mouseOver, disabled, true, true, false, false);
      break;
    case EVShape::AllRounded:
      DrawPressableRectangle(g, bounds, pressed, mouseOver, disabled, true, true, true, true);
    default:
      break;
    }
  }

  void DrawPressableEllipse(IGraphics&g, const IRECT& bounds, bool pressed, bool mouseOver, bool disabled)
  {
    IRECT handleBounds = bounds;
    IRECT centreBounds = bounds.GetPadded(-mStyle.shadowOffset);
    IRECT shadowBounds = bounds.GetTranslated(mStyle.shadowOffset, mStyle.shadowOffset);
    const IBlend blend = mControl->GetBlend();
    const float contrast = disabled ? -GRAYED_ALPHA : 0.f;
    
    if(!pressed && !disabled && mStyle.drawShadows)
      g.FillEllipse(GetColor(kSH), shadowBounds);
   
    if (pressed)
    {
      if (mStyle.emboss)
      {
        shadowBounds.ReduceFromRight(mStyle.shadowOffset);
        shadowBounds.ReduceFromBottom(mStyle.shadowOffset);
        // Fill background with pressed color and shade it
        g.FillEllipse(GetColor(kPR), bounds, &blend);
        g.FillEllipse(GetColor(kSH), bounds, &blend);

        // Inverse shading for recessed look - shadowBounds = inner shadow
        g.FillEllipse(GetColor(kFG).WithContrast(contrast), shadowBounds/*, &blend*/);

        // Fill in center with pressed color
        g.FillEllipse(GetColor(kPR).WithContrast(contrast), centreBounds/*, &blend*/);
      }
      else
        g.FillEllipse(GetColor(kPR).WithContrast(contrast), handleBounds/*, &blend*/);
    }
    else
    {
      // Embossed style unpressed
      if (mStyle.emboss)
      {
        // Positive light TODO: use thes kPR color for now, maybe change the name?
        g.FillEllipse(GetColor(kPR).WithContrast(contrast), bounds/*, &blend*/);

        // Negative light TODO: clip this?
        g.FillEllipse(GetColor(kSH).WithContrast(contrast), shadowBounds/*, &blend*/);

        // Fill in foreground
        g.FillEllipse(GetColor(kFG).WithContrast(contrast), centreBounds/*, &blend*/);

        // Shade when hovered
        if (mouseOver)
          g.FillEllipse(GetColor(kHL), centreBounds, &blend);
      }
      else
      {
        g.FillEllipse(GetColor(kFG).WithContrast(contrast), handleBounds/*, &blend*/);

        // Shade when hovered
        if (mouseOver)
          g.FillEllipse(GetColor(kHL), handleBounds, &blend);
      }
    }
    
    if(pressed && mControl->GetAnimationFunction())
      DrawSplash(g, handleBounds);
    
    if(mStyle.drawFrame)
      g.DrawEllipse(GetColor(kFR), handleBounds, &blend, mStyle.frameThickness);
  }
  
  /** /todo
   @param IGraphics&g /todo
   @param bounds /todo
   @param pressed /todo
   @param mouseOver /todo
   @return /todo */
  IRECT DrawPressableRectangle(IGraphics&g, const IRECT& bounds, bool pressed, bool mouseOver, bool disabled,
                               bool rtl = true, bool rtr = true, bool rbl = true, bool rbr = true)
  {
    IRECT handleBounds = GetAdjustedHandleBounds(bounds);
    IRECT centreBounds = handleBounds.GetPadded(-mStyle.shadowOffset);
    IRECT shadowBounds = handleBounds.GetTranslated(mStyle.shadowOffset, mStyle.shadowOffset);
    const IBlend blend = mControl->GetBlend();
    const float contrast = disabled ? -GRAYED_ALPHA : 0.f;
    float cR = GetRoundedCornerRadius(handleBounds);

    const float tlr = rtl ? cR : 0.f;
    const float trr = rtr ? cR : 0.f;
    const float blr = rbl ? cR : 0.f;
    const float brr = rbr ? cR : 0.f;

    if (pressed)
    {
      shadowBounds.ReduceFromRight(mStyle.shadowOffset);
      shadowBounds.ReduceFromBottom(mStyle.shadowOffset);

      if (mStyle.emboss)
      {
        // Fill background with pressed color and shade it
        g.FillRoundRect(GetColor(kPR), handleBounds, tlr, trr, blr, brr, &blend);
        g.FillRoundRect(GetColor(kSH), handleBounds, tlr, trr, blr, brr, &blend);

        // Inverse shading for recessed look - shadowBounds = inner shadow
        g.FillRoundRect(GetColor(kFG).WithContrast(contrast), shadowBounds, tlr, trr, blr, brr/*, &blend*/);

        // Fill in center with pressed color
        g.FillRoundRect(GetColor(kPR), centreBounds, tlr, trr, blr, brr, &blend);
      }
      else
      {
        g.FillRoundRect(GetColor(kPR).WithContrast(contrast), handleBounds, tlr, trr, blr, brr/*, &blend*/);
      }
    }
    else
    {
      //outer shadow
      if (mStyle.drawShadows)
        g.FillRoundRect(GetColor(kSH), shadowBounds, tlr, trr, blr, brr, &blend);

      // Embossed style unpressed
      if (mStyle.emboss)
      {
        // Positive light TODO: use thes kPR color for now, maybe change the name?
        g.FillRoundRect(GetColor(kPR).WithContrast(contrast), handleBounds, tlr, trr, blr, brr/*, &blend*/);

        // Negative light TODO: clip this?
        g.FillRoundRect(GetColor(kSH).WithContrast(contrast), shadowBounds, tlr, trr, blr, brr/*, &blend*/);

        // Fill in foreground
        g.FillRoundRect(GetColor(kFG).WithContrast(contrast), centreBounds, tlr, trr, blr, brr/*, &blend*/);

        // Shade when hovered
        if (mouseOver)
          g.FillRoundRect(GetColor(kHL), centreBounds, tlr, trr, blr, brr, &blend);
      }
      else
      {
        g.FillRoundRect(GetColor(kFG).WithContrast(contrast), handleBounds, tlr, trr, blr, brr/*, &blend*/);

        // Shade when hovered
        if (mouseOver)
          g.FillRoundRect(GetColor(kHL), handleBounds, tlr, trr, blr, brr, &blend);
      }
    }
    
    if(pressed && mControl->GetAnimationFunction())
      DrawSplash(g, handleBounds);
    
    if(mStyle.drawFrame)
      g.DrawRoundRect(GetColor(kFR), handleBounds, tlr, trr, blr, brr, &blend, mStyle.frameThickness);
    
    return handleBounds;
  }
  
  /** Draw a triangle-shaped vector button
   * @param g The IGraphics context used for drawing
   * @param bounds Where to draw the button
   * @param pressed Whether to draw the button pressed or unpressed
   * @param mouseOver Whether mouse is currently hovering on control */
  IRECT DrawPressableTriangle(IGraphics&g, const IRECT& bounds, bool pressed, bool mouseOver, float angle, bool disabled)
  {
    float x1, x2, x3, y1, y2, y3;
    
    float theta = DegToRad(angle);
    
    IRECT handleBounds = GetAdjustedHandleBounds(bounds);
    
    // Center bounds around origin for rotation
    float xT = handleBounds.L + handleBounds.W() * 0.5f;
    float yT = handleBounds.T + handleBounds.H() * 0.5f;
    IRECT centered = handleBounds.GetTranslated(-xT, -yT);
    
    // Do rotation and translate points back into view space
    float c = cosf(theta);
    float s = sinf(theta);
    x1 = centered.L * c - centered.B * s + xT;
    y1 = centered.L * s + centered.B * c + yT;
    x2 = centered.MW() * c - centered.T * s + xT;
    y2 = centered.MW() * s + centered.T * c + yT;
    x3 = centered.R * c - centered.B * s + xT;
    y3 = centered.R * s + centered.B * c + yT;

    const IBlend blend = mControl->GetBlend();
    const float contrast = disabled ? -GRAYED_ALPHA : 0.f;

    if (pressed)
      g.FillTriangle(GetColor(kPR).WithContrast(contrast), x1, y1, x2, y2, x3, y3/*, &blend*/);
    else
    {
      //outer shadow
      if (mStyle.drawShadows)
        g.FillTriangle(GetColor(kSH), x1 + mStyle.shadowOffset, y1 + mStyle.shadowOffset,
                                      x2 + mStyle.shadowOffset, y2 + mStyle.shadowOffset,
                                      x3 + mStyle.shadowOffset, y3 + mStyle.shadowOffset, &blend);
      
      g.FillTriangle(GetColor(kFG).WithContrast(contrast), x1, y1, x2, y2, x3, y3/*, &blend*/);
    }
    
    if (mouseOver)
      g.FillTriangle(GetColor(kHL), x1, y1, x2, y2, x3, y3, &blend);
    
    if(pressed && mControl->GetAnimationFunction())
      DrawSplash(g);
    
    if (mStyle.drawFrame)
      g.DrawTriangle(GetColor(kFR), x1, y1, x2, y2, x3, y3, &blend, mStyle.frameThickness);
    
    return handleBounds;
  }
  
  IRECT MakeRects(const IRECT& parent, bool hasHandle = false)
  {
    IRECT clickableArea = parent;
    
    if(!mLabelInWidget)
    {
      if(mStyle.showLabel && CStringHasContents(mLabelStr.Get()))
      {
        IRECT textRect;
        mControl->GetUI()->MeasureText(mStyle.labelText, mLabelStr.Get(), textRect);

        mLabelBounds = parent.GetFromTop(textRect.H()).GetCentredInside(textRect.W(), textRect.H());
      }
      else
        mLabelBounds = IRECT();
      
      if(mLabelBounds.H())
        clickableArea = parent.GetReducedFromTop(mLabelBounds.H());
    }
    
    if (mStyle.showValue && !mValueInWidget)
    {
      IRECT textRect;
      
      if(CStringHasContents(mValueStr.Get()))
        mControl->GetUI()->MeasureText(mStyle.valueText, mValueStr.Get(), textRect);

      const float valueDisplayWidth = textRect.W() * mValueDisplayFrac;

      switch (mStyle.valueText.mVAlign)
      {
        case EVAlign::Middle:
          mValueBounds = clickableArea.GetMidVPadded(textRect.H()/2.f).GetMidHPadded(valueDisplayWidth);
          mWidgetBounds = clickableArea.GetScaledAboutCentre(mStyle.widgetFrac);
          break;
        case EVAlign::Bottom:
        {
          mValueBounds = clickableArea.GetFromBottom(textRect.H()).GetMidHPadded(valueDisplayWidth);
          mWidgetBounds = clickableArea.GetReducedFromBottom(textRect.H()).GetScaledAboutCentre(mStyle.widgetFrac);
          break;
        }
        case EVAlign::Top:
          mValueBounds = clickableArea.GetFromTop(textRect.H()).GetMidHPadded(valueDisplayWidth);
          mWidgetBounds = clickableArea.GetReducedFromTop(textRect.H()).GetScaledAboutCentre(mStyle.widgetFrac);
          break;
        default:
          break;
      }
    }
    else
    {
      mWidgetBounds = clickableArea.GetScaledAboutCentre(mStyle.widgetFrac);
    }
    
    if(hasHandle)
      mWidgetBounds = GetAdjustedHandleBounds(clickableArea).GetScaledAboutCentre(mStyle.widgetFrac);
    
    if(mLabelInWidget)
      mLabelBounds = mWidgetBounds;
    
    if(mValueInWidget)
      mValueBounds = mWidgetBounds;
    
    return clickableArea;
  }
  
protected:
  IControl* mControl = nullptr;
  IVStyle mStyle; // IVStyle that defines certain common properties of an IVControl
  bool mLabelInWidget = false; // Should the Label text be displayed inside the widget
  bool mValueInWidget = false; // Should the Value text be displayed inside the widget
  float mSplashRadius = 0.f; // Modified during the default SplashClickAnimationFunc to specify the radius of the splash
  IVec2 mSplashPoint = {0.f, 0.f}; // Set at the start of the SplashClickActionFunc to set the position of the splash
  float mMaxSplashRadius = 50.f;
  float mTrackSize = 2.f;
  float mValueDisplayFrac = 0.66f; // the fraction of the control width for the text entry
  IRECT mWidgetBounds; // The knob/slider/button
  IRECT mLabelBounds; // A piece of text above the control
  IRECT mValueBounds; // Text below the contol, usually displaying the value of a parameter
  WDL_String mLabelStr;
  WDL_String mValueStr;
  EVShape mShape = EVShape::Rectangle;
};

/** A base class for controls that can do do multitouch */
class IMultiTouchControlBase
{
public:
  struct TrackedTouch
  {
    int index = 0;
    float x = 0.f;
    float y = 0.f;
    float sx = 0.f;
    float sy = 0.f;
    float radius = 1.f;
    TimePoint startTime;
    
    TrackedTouch(int index, float x, float y, float radius, TimePoint time)
    : index(index), x(x), y(y), sx(x), sy(y), radius(radius), startTime(time)
    {}
    
    TrackedTouch()
    {}
  };
  
  virtual void AddTouch(ITouchID touchID, float x, float y, float radius)
  {
    int touchIndex = 0;
    for (int i = 0; i < MAX_TOUCHES; i++)
    {
      if (mTouchStatus[i] == false)
      {
        touchIndex = i;
        mTouchStatus[i] = true;
        break;
      }
    }

    if(NTrackedTouches() < MAX_TOUCHES)
      mTrackedTouches.insert(std::make_pair(touchID, TrackedTouch(touchIndex, x, y, radius, std::chrono::high_resolution_clock::now())));
  }
  
  virtual void ReleaseTouch(ITouchID touchID)
  {
    mTouchStatus[GetTouchWithIdentifier(touchID)->index] = false;
    mTrackedTouches.erase(touchID);
  }
  
  virtual void UpdateTouch(ITouchID touchID, float x, float y, float radius)
  {
    mTrackedTouches[touchID].x = x;
    mTrackedTouches[touchID].y = y;
    mTrackedTouches[touchID].radius = radius;
  }
  
  void ClearAllTouches()
  {
    mTrackedTouches.clear();
    memset(mTouchStatus, 0, MAX_TOUCHES * sizeof(bool));
  }
  
  int NTrackedTouches() const
  {
    return static_cast<int>(mTrackedTouches.size());
  }
  
  TrackedTouch* GetTouch(int index)
  {
    auto itr = std::find_if(mTrackedTouches.begin(), mTrackedTouches.end(),
    [index](auto element) {
      return(element.second.index == index);
    });

    if(itr != mTrackedTouches.end())
      return &itr->second;
    else
      return nullptr;
  }
  
  TrackedTouch* GetTouchWithIdentifier(ITouchID touchID)
  {
    auto itr = mTrackedTouches.find(touchID);
    
    if(itr != mTrackedTouches.end())
      return &itr->second;
    else
      return nullptr;
  }
  
protected:
  static constexpr int MAX_TOUCHES = 10;
  std::unordered_map<ITouchID, TrackedTouch> mTrackedTouches;
  bool mTouchStatus[MAX_TOUCHES] = { 0 };
};

/** A base class for knob/dial controls, to handle mouse action and Sender. */
class IKnobControlBase : public IControl
{
public:
  IKnobControlBase(const IRECT& bounds, int paramIdx = kNoParameter, EDirection direction = EDirection::Vertical, double gearing = DEFAULT_GEARING)
  : IControl(bounds, paramIdx)
  , mDirection(direction)
  , mGearing(gearing)
  {}

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    mMouseDown = true;
    mMouseDragValue = GetValue();

    if (mHideCursorOnDrag)
      GetUI()->HideMouseCursor(true, true);

    IControl::OnMouseDown(x, y, mod);
  }

  void OnMouseUp(float x, float y, const IMouseMod& mod) override
  {
    mMouseDown = false;

    if (mHideCursorOnDrag)
      GetUI()->HideMouseCursor(false);
  }

  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;
  void OnMouseWheel(float x, float y, const IMouseMod& mod, float d) override;
  
  void SetGearing(double gearing) { mGearing = gearing; }
  bool IsFineControl(const IMouseMod& mod, bool wheel) const;

protected:
  /** Get the area for which mouse deltas will be used to calculate the amount dragging changes the control value. This is usually the area that contains the knob handle, can override if your control contains extra elements such as labels
   * @return IRECT The bounds over which mouse deltas will be used to calculate the amount dragging changes the control value */
  virtual IRECT GetKnobDragBounds() { return mTargetRECT; }

  bool mHideCursorOnDrag = true;
  EDirection mDirection;
  double mGearing;
  bool mMouseDown = false;
  double mMouseDragValue = 0.0;
};

/** A base class for slider/fader controls, to handle mouse action and Sender. */
class ISliderControlBase : public IControl
{
public:
  ISliderControlBase(const IRECT& bounds, int paramIdx = kNoParameter, EDirection dir = EDirection::Vertical, double gearing = DEFAULT_GEARING, float handleSize = 0.f);
  ISliderControlBase(const IRECT& bounds, IActionFunction aF = nullptr, EDirection dir = EDirection::Vertical, double gearing = DEFAULT_GEARING, float handleSize = 0.f);
  
  void OnMouseDown(float x, float y, const IMouseMod& mod) override;
  void OnMouseUp(float x, float y, const IMouseMod& mod) override;
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;
  void OnMouseWheel(float x, float y, const IMouseMod& mod, float d) override;

  void SetGearing(double gearing) { mGearing = gearing; }
  bool IsFineControl(const IMouseMod& mod, bool wheel) const;
  
protected:
  bool mHideCursorOnDrag = true;
  EDirection mDirection;
  IRECT mTrackBounds;
  float mHandleSize;
  double mGearing;
  bool mMouseDown = false;
  double mMouseDragValue = 0.0;
};

/** A base class for mult-strip/track controls, such as multi-sliders, meters */
class IVTrackControlBase : public IControl
                         , public IVectorBase
{
public:
  IVTrackControlBase(const IRECT& bounds, const char* label, const IVStyle& style, int maxNTracks = 1, EDirection dir = EDirection::Horizontal, float minTrackValue = 0.f, float maxTrackValue = 1.f, const char* trackNames = 0, ...)
  : IControl(bounds)
  , IVectorBase(style)
  , mMinTrackValue(minTrackValue)
  , mMaxTrackValue(maxTrackValue)
  , mDirection(dir)
  {
    SetNVals(maxNTracks);

    for (int i=0; i<maxNTracks; i++)
    {
      SetParamIdx(kNoParameter, i);
      mTrackBounds.Add(IRECT());
    }
    
    AttachIControl(this, label);
  }

  IVTrackControlBase(const IRECT& bounds, const char* label, const IVStyle& style, int lowParamidx, int maxNTracks = 1, EDirection dir = EDirection::Horizontal, float minTrackValue = 0.f, float maxTrackValue = 1.f, const char* trackNames = 0, ...)
  : IControl(bounds)
  , IVectorBase(style)
  , mMinTrackValue(minTrackValue)
  , mMaxTrackValue(maxTrackValue)
  , mDirection(dir)
  {
    SetNVals(maxNTracks);

    for (int i = 0; i < maxNTracks; i++)
    {
      SetParamIdx(lowParamidx+i, i);
      mTrackBounds.Add(IRECT());
    }

    AttachIControl(this, label);
  }
  
  IVTrackControlBase(const IRECT& bounds, const char* label, const IVStyle& style, const std::initializer_list<int>& params, EDirection dir = EDirection::Horizontal, float minTrackValue = 0.f, float maxTrackValue = 1.f, const char* trackNames = 0, ...)
  : IControl(bounds)
  , IVectorBase(style)
  , mMinTrackValue(minTrackValue)
  , mMaxTrackValue(maxTrackValue)
  , mDirection(dir)
  {
    SetNVals(static_cast<int>(params.size()));
  
    int valIdx = 0;
    for (auto param : params)
    {
      SetParamIdx(param, valIdx++);
      mTrackBounds.Add(IRECT());
    }
    
    AttachIControl(this, label);
  }
  
  virtual void MakeTrackRects(const IRECT& bounds)
  {
    int nVals = NVals();
    int dir = static_cast<int>(mDirection); // 0 = horizontal, 1 = vertical
    for (int ch = 0; ch < nVals; ch++)
    {
      mTrackBounds.Get()[ch] = bounds.SubRect(EDirection(!dir), nVals, ch).
                                     GetPadded(0, -mTrackPadding * (float) dir, -mTrackPadding * (float) !dir, -mTrackPadding);
    }
  }
  
  void DrawWidget(IGraphics& g) override
  {
    int nVals = NVals();
    
    for (int ch = 0; ch < nVals; ch++)
    {
      DrawTrack(g, mTrackBounds.Get()[ch], ch);
    }
  }
  
  /** Update the parameters based on a parameter group name.
   * You probably want to call IEditorDelegate::SendCurrentParamValuesFromDelegate() after this, to update the control values */
  void SetParamsByGroup(const char* paramGroup)
  {
    int nParams = GetDelegate()->NParams();
    std::vector<int> paramIdsForGroup;
    
    for (auto p = 0; p < nParams; p++)
    {
      IParam* pParam = GetDelegate()->GetParam(p);
      
      if(strcmp(pParam->GetGroupForHost(), paramGroup) == 0)
      {
        paramIdsForGroup.push_back(p);
      }
    }
    
    mTrackBounds.Resize(0);
    
    int nParamsInGroup = static_cast<int>(paramIdsForGroup.size());
        
    SetNVals(nParamsInGroup);
  
    int valIdx = 0;
    for (auto param : paramIdsForGroup)
    {
      SetParamIdx(param, valIdx++);
      mTrackBounds.Add(IRECT());
    }
    
    OnResize();
  }
  
  /** Update the parameters based on a parameter group name.
   * You probably want to call IEditorDelegate::SendCurrentParamValuesFromDelegate() after this, to update the control values */
  void SetParams(std::initializer_list<int> paramIds)
  {
    mTrackBounds.Resize(0);

    SetNVals(static_cast<int>(paramIds.size()));
  
    int valIdx = 0;
    for (auto param : paramIds)
    {
      SetParamIdx(param, valIdx++);
      mTrackBounds.Add(IRECT());
    }
    
    OnResize();
  }
  
  //void SetAllTrackData(float val) { memset(mTrackData.Get(), (int) Clip(val, mMinTrackValue, mMaxTrackValue), mTrackData.GetSize() * sizeof(float) ); }
protected:
  
  virtual void DrawTrack(IGraphics& g, const IRECT& r, int chIdx)
  {
    DrawTrackBG(g, r, chIdx);
    DrawTrackHandle(g, r, chIdx);
    
    if(mStyle.drawFrame && mDrawTrackFrame)
      g.DrawRect(GetColor(kFR), r, &mBlend, mStyle.frameThickness);
  }
  
  virtual void DrawTrackBG(IGraphics& g, const IRECT& r, int chIdx)
  {
    g.FillRect(kBG, r, &mBlend);
  }
  
  virtual void DrawTrackHandle(IGraphics& g, const IRECT& r, int chIdx)
  {
    IRECT fillRect = r.FracRect(mDirection, static_cast<float>(GetValue(chIdx)));
    
    g.FillRect(GetColor(kFG), fillRect, &mBlend); // TODO: shadows!
    
    IRECT peakRect;
    
    if(mDirection == EDirection::Vertical)
      peakRect = IRECT(fillRect.L, fillRect.T, fillRect.R, fillRect.T + mPeakSize);
    else
      peakRect = IRECT(fillRect.R - mPeakSize, fillRect.T, fillRect.R, fillRect.B);
    
    DrawPeak(g, peakRect, chIdx);
  }
  
  virtual void DrawPeak(IGraphics& g, const IRECT& r, int chIdx)
  {
    g.FillRect(GetColor(kFR), r, &mBlend);
  }
  
  virtual void OnResize() override
  {
    SetTargetRECT(MakeRects(mRECT));
    MakeTrackRects(mWidgetBounds);
    SetDirty(false);
  }
  
protected:
  EDirection mDirection = EDirection::Vertical;
  WDL_TypedBuf<IRECT> mTrackBounds;
  float mMinTrackValue;
  float mMaxTrackValue;
  float mTrackPadding = 0.;
  float mPeakSize = 1.;
  bool mDrawTrackFrame = true;
};

/** A base class for buttons/momentary switches - cannot be linked to parameters.
 * The default action function triggers the default click function, which returns mValue to 0. after DEFAULT_ANIMATION_DURATION */
class IButtonControlBase : public IControl
{
public:
  IButtonControlBase(const IRECT& bounds, IActionFunction aF);
  
  virtual ~IButtonControlBase() {}
  void OnMouseDown(float x, float y, const IMouseMod& mod) override;
  void OnEndAnimation() override;
};

/** A base class for switch controls */
class ISwitchControlBase : public IControl
{
public:
  ISwitchControlBase(const IRECT& bounds, int paramIdx = kNoParameter, IActionFunction aF = nullptr, int numStates = 2);

  virtual ~ISwitchControlBase() {}
  void OnInit() override;
  void OnMouseDown(float x, float y, const IMouseMod& mod) override;
  void OnMouseUp(float x, float y, const IMouseMod& mod) override;
  
  int GetSelectedIdx() const { return int(0.5 + GetValue() * (double) (mNumStates - 1)); }
protected:
  int mNumStates;
  bool mMouseDown = false;
};

/** An abstract IControl base class that you can inherit from in order to make a control that pops up a menu to browse files */
class IDirBrowseControlBase : public IControl
{
public:
  IDirBrowseControlBase(const IRECT& bounds, const char* extension /* e.g. ".txt"*/)
  : IControl(bounds)
  {
    mExtension.Set(extension);
  }

  virtual ~IDirBrowseControlBase();

  int NItems();

  void AddPath(const char* path, const char* label);

  void SetupMenu();

//  void GetSelectedItemLabel(WDL_String& label);
//  void GetSelectedItemPath(WDL_String& path);

private:
  void ScanDirectory(const char* path, IPopupMenu& menuToAddTo);
  void CollectSortedItems(IPopupMenu* pMenu);
  
protected:
  bool mShowEmptySubmenus = false;
  bool mShowFileExtensions = true;
  int mSelectedIndex = -1;
  IPopupMenu* mSelectedMenu = nullptr;
  IPopupMenu mMainMenu;
  WDL_PtrList<WDL_String> mPaths;
  WDL_PtrList<WDL_String> mPathLabels;
  WDL_PtrList<WDL_String> mFiles;
  WDL_PtrList<IPopupMenu::Item> mItems; // ptr to item for each file
  WDL_String mExtension;
};

/**@}*/

#pragma mark - BASIC CONTROLS

/**
 * \addtogroup Controls
 * @{
 */

/** A basic control to fill a rectangle with a color or gradient */
class IPanelControl : public IControl
{
public:
  IPanelControl(const IRECT& bounds, const IColor& color, bool drawFrame = false)
  : IControl(bounds, kNoParameter)
  , mPattern(color)
  , mDrawFrame(drawFrame)
  {
    mIgnoreMouse = true;
  }
  
  IPanelControl(const IRECT& bounds, const IPattern& pattern, bool drawFrame = false)
  : IControl(bounds, kNoParameter)
  , mPattern(pattern)
  , mDrawFrame(drawFrame)
  {
    mIgnoreMouse = true;
  }

  void Draw(IGraphics& g) override
  {
    if(g.HasPathSupport())
    {
      g.PathRect(mRECT);
      g.PathFill(mPattern);
    }
    else
      g.FillRect(mPattern.GetStop(0).mColor, mRECT);
    
    if(mDrawFrame)
      g.DrawRect(COLOR_LIGHT_GRAY, mRECT);
  }
  
  void SetPattern(const IPattern& pattern)
  {
    mPattern = pattern;
    SetDirty(false);
  }
  
  IPattern GetPattern() const { return mPattern; }
  
private:
  IPattern mPattern;
  bool mDrawFrame;
};

/** A control that can be specialised with a lambda function, for quick experiments without making a custom IControl */
class ILambdaControl : public IControl
{
public:
  ILambdaControl(const IRECT& bounds, ILambdaDrawFunction drawFunc, int animationDuration = DEFAULT_ANIMATION_DURATION,
    bool loopAnimation = false, bool startImmediately = false, int paramIdx = kNoParameter, bool ignoreMouse = false)
  : IControl(bounds, paramIdx, DefaultClickActionFunc)
  , mDrawFunc(drawFunc)
  , mLoopAnimation(loopAnimation)
  , mAnimationDuration(animationDuration)
  {
    if (startImmediately)
      SetAnimation(DefaultAnimationFunc, mAnimationDuration);
    
    mIgnoreMouse = ignoreMouse;
  }
  
  void Draw(IGraphics& g) override
  {
    if(mDrawFunc)
      mDrawFunc(this, g, mRECT);
  }
  
  virtual void OnEndAnimation() override // if you override this you must call the base implementation, to free mAnimationFunc
  {
    if(mLoopAnimation && mAnimationDuration)
      StartAnimation(mAnimationDuration);
    else
      SetAnimation(nullptr);
    
    SetDirty(false);
  }
  
  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    mMouseInfo.x = x; mMouseInfo.y = y; mMouseInfo.ms = mod;
    SetAnimation(DefaultAnimationFunc, mAnimationDuration);
  }
  
  void OnMouseUp(float x, float y, const IMouseMod& mod) override { mMouseInfo.x = x; mMouseInfo.y = y; mMouseInfo.ms = mod; SetDirty(false); }
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override { mMouseInfo.x = x; mMouseInfo.y = y; mMouseInfo.ms = mod; SetDirty(false); }
  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override { mMouseInfo.x = x; mMouseInfo.y = y; mMouseInfo.ms = mod; SetDirty(false); }
  
public: // public for easy access :-)
  ILayerPtr mLayer;
  IMouseInfo mMouseInfo;
private:
  ILambdaDrawFunction mDrawFunc = nullptr;
  bool mLoopAnimation;
  int mAnimationDuration;
};

/** A basic control to draw a bitmap, or one frame of a stacked bitmap depending on the current value. */
class IBitmapControl : public IControl
                     , public IBitmapBase
{
public:
  /** Creates a bitmap control
   * @param paramIdx Parameter index (-1 or kNoParameter, if this should not be linked to a parameter)
   * @param bitmap Image to be drawn */
  IBitmapControl(float x, float y, const IBitmap& bitmap, int paramIdx = kNoParameter, EBlend blend = EBlend::Default)
  : IControl(IRECT(x, y, bitmap), paramIdx)
  , IBitmapBase(bitmap)
  {
    AttachIControl(this);
    mBlend = blend;
  }
  
  IBitmapControl(const IRECT& bounds, const IBitmap& bitmap, int paramIdx = kNoParameter, EBlend blend = EBlend::Default)
  : IControl(bounds, paramIdx)
  , IBitmapBase(bitmap)
  {
    AttachIControl(this);
    mBlend = blend;
  }
  
  virtual ~IBitmapControl() {}

  void Draw(IGraphics& g) override { DrawBitmap(g); }

  /** If you override this make sure you call the parent method in order to rescale mBitmap */
  void OnRescale() override { mBitmap = GetUI()->GetScaledBitmap(mBitmap); }
};

/** A basic control to draw an SVG image to the screen. Optionally, cache SVG to an ILayer. */
class ISVGControl : public IControl
{
public:
  ISVGControl(const IRECT& bounds, const ISVG& svg, bool useLayer = false)
  : IControl(bounds)
  , mSVG(svg)
  , mUseLayer(useLayer)
  {}

  virtual ~ISVGControl() {}

  void Draw(IGraphics& g) override
  {
    if(mUseLayer)
    {
      if (!g.CheckLayer(mLayer))
      {
        g.StartLayer(this, mRECT);
        g.DrawSVG(mSVG, mRECT);
        mLayer = g.EndLayer();
      }

      g.DrawLayer(mLayer, &mBlend);
    }
    else
      g.DrawSVG(mSVG, mRECT, &mBlend);
  }
  
  void SetSVG(const ISVG& svg)
  {
    mSVG = svg;
  }
  
private:
  bool mUseLayer;
  ILayerPtr mLayer;
  ISVG mSVG;
};

/** A basic control to display some text */
class ITextControl : public IControl
{
public:
  ITextControl(const IRECT& bounds, const char* str = "", const IText& text = DEFAULT_TEXT, const IColor& BGColor = DEFAULT_BGCOLOR, bool setBoundsBasedOnStr = false);

  void Draw(IGraphics& g) override;
  void OnInit() override;

  virtual void SetStr(const char* str);
  virtual void SetStrFmt(int maxlen, const char* fmt, ...);
  virtual void ClearStr() { SetStr(""); }
  const char* GetStr() const { return mStr.Get(); }
  
  void SetBoundsBasedOnStr();
  // Added by bluelab
  const IColor& GetTextColor() { return IControl::mText.mFGColor; }
  void SetTextColor(const IColor& color) { IControl::mText.mFGColor = color; };
  const IColor& GetTextBGColor() { return mBGColor; }
  void SetTextBGColor(const IColor& color) { mBGColor = color; };
protected:
  WDL_String mStr;
  IColor mBGColor;
  bool mSetBoundsBasedOnStr = false;
};

/** A basic control to display some editable text */
class IEditableTextControl : public ITextControl
{
public:
  IEditableTextControl(const IRECT& bounds, const char* str, const IText& text)
  : ITextControl(bounds, str, text)
  {
    mIgnoreMouse = false;
  }
  
  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    GetUI()->CreateTextEntry(*this, mText, mRECT, GetStr());
  }
  
  void OnTextEntryCompletion(const char* str, int valIdx) override
  {
    SetStr(str);
  }
};

class IURLControl : public ITextControl
{
public:
  IURLControl(const IRECT& bounds, const char* str, const char* url, const IText& text = DEFAULT_TEXT, const IColor& BGColor = DEFAULT_BGCOLOR, const IColor& MOColor = COLOR_WHITE, const IColor& CLColor = COLOR_BLUE);
  
  void Draw(IGraphics& g) override;
  
  void OnMouseDown(float x, float y, const IMouseMod& mod) override;
  void OnMouseOver(float x, float y, const IMouseMod& mod) override { GetUI()->SetMouseCursor(ECursor::HAND); IControl::OnMouseOver(x, y, mod); };
  void OnMouseOut() override { GetUI()->SetMouseCursor(); IControl::OnMouseOut(); }

protected:
  WDL_String mURLStr;
  IColor mOriginalColor, mMOColor, mCLColor;
  bool mClicked = false;
};

class ITextToggleControl : public ITextControl
{
public:
  ITextToggleControl(const IRECT& bounds, int paramIdx = kNoParameter, const char* offText = "OFF", const char* onText = "ON", const IText& text = DEFAULT_TEXT, const IColor& BGColor = DEFAULT_BGCOLOR);
  
  ITextToggleControl(const IRECT& bounds, IActionFunction aF = nullptr, const char* offText = "OFF", const char* onText = "ON", const IText& text = DEFAULT_TEXT, const IColor& BGColor = DEFAULT_BGCOLOR);

  void OnMouseDown(float x, float y, const IMouseMod& mod) override;
  void SetDirty(bool push, int valIdx = 0) override;
protected:
  WDL_String mOffText;
  WDL_String mOnText;
};

/** A control to display the textual representation of a parameter */
class ICaptionControl : public ITextControl
{
public:
  /** Creates an ICaptionControl
   * @param bounds The control's bounds
   * @param paramIdx The parameter index to link this control to
   * @param text The styling of this control's text
   * @param BGColor The control's background color
   * @param showParamLabel Whether the parameter's label, e.g. "Hz" should be appended to the caption */
  ICaptionControl(const IRECT& bounds, int paramIdx, const IText& text = DEFAULT_TEXT, const IColor& BGColor = DEFAULT_BGCOLOR, bool showParamLabel = true);
  void Draw(IGraphics& g) override;
  void OnMouseDown(float x, float y, const IMouseMod& mod) override;
  void OnResize() override;
protected:
  bool mShowParamLabel;
  IColor mTriangleColor = COLOR_BLACK;
  IColor mTriangleMouseOverColor = COLOR_WHITE;
  IRECT mTri;
};

/** A control to use as a placeholder during development */
class PlaceHolder : public ITextControl
{
public:
  PlaceHolder(const IRECT& bounds, const char* str = "Place Holder");
  
  void Draw(IGraphics& g) override;
  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override { GetUI()->CreateTextEntry(*this, mText, mRECT, mStr.Get()); }
  void OnTextEntryCompletion(const char* str, int valIdx) override { SetStr(str); }
  void OnResize() override;

protected:
  IRECT mCentreLabelBounds;
  WDL_String mTLHCStr;
  WDL_String mWidthStr;
  WDL_String mHeightStr;
  IText mTLGCText = DEFAULT_TEXT.WithAlign(EAlign::Near);
  IText mWidthText = DEFAULT_TEXT;
  IText mHeightText = DEFAULT_TEXT.WithAngle(270.f);
  static constexpr float mInset = 10.f;
};

END_IGRAPHICS_NAMESPACE
END_IPLUG_NAMESPACE

/**@}*/
