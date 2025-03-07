/*
 ==============================================================================

 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.

 See LICENSE.txt for  more info.

 ==============================================================================
*/

#pragma once

#include <CoreGraphics/CoreGraphics.h>

#include "IGraphics_select.h"
#include "IGraphicsCoreText.h"

BEGIN_IPLUG_NAMESPACE
BEGIN_IGRAPHICS_NAMESPACE

/** IGraphics platform class for macOS
*   @ingroup PlatformClasses */
class IGraphicsMac final : public IGRAPHICS_DRAW_CLASS
{
public:
  IGraphicsMac(IGEditorDelegate& dlg, int w, int h, int fps, float scale);
  virtual ~IGraphicsMac();

  void SetBundleID(const char* bundleID) { mBundleID.Set(bundleID); }
  void SetAppGroupID(const char* appGroupID) { mAppGroupID.Set(appGroupID); }

  void* OpenWindow(void* pWindow) override;
  void CloseWindow() override;
  bool WindowIsOpen() override;
  void PlatformResize(bool parentHasResized) override;
  void AttachPlatformView(const IRECT& r, void* pView) override;
  void RemovePlatformView(void* pView) override;
  void HidePlatformView(void* pView, bool hide) override;
  void CheckFocus() override;

  void HideMouseCursor(bool hide, bool lock) override;
  void MoveMouseCursor(float x, float y) override;
  ECursor SetMouseCursor(ECursor cursorType) override;
  
  void GetMouseLocation(float& x, float&y) const override;

  void DoCursorLock(float x, float y, float& prevX, float& prevY);
    
  EMsgBoxResult ShowMessageBox(const char* str, const char* title, EMsgBoxType type, EMsgBoxIcon icon, IMsgBoxCompletionHandlerFunc completionHandler) override;

  void ForceEndUserEdit() override;

  const char* GetPlatformAPIStr() override;

  void UpdateTooltips() override;

  bool RevealPathInExplorerOrFinder(WDL_String& path, bool select) override;
  void PromptForFile(WDL_String& fileName, WDL_String& path, EFileAction action, const char* ext, IFileDialogCompletionHandlerFunc completionHandler) override;
  void PromptForDirectory(WDL_String& dir, IFileDialogCompletionHandlerFunc completionHandler) override;
  bool PromptForColor(IColor& color, const char* str, IColorPickerHandlerFunc func) override;
    
  bool OpenURL(const char* url, const char* msgWindowTitle, const char* confirmMsg, const char* errMsgOnFailure) override;

  void* GetWindow() override;

  const char* GetBundleID() const override { return mBundleID.Get(); }
  const char* GetAppGroupID() const override { return mAppGroupID.Get(); }
  static int GetUserOSVersion();

  bool GetTextFromClipboard(WDL_String& str) override;
  bool SetTextInClipboard(const char* str) override;
  bool SetFilePathInClipboard(const char* path) override;

  bool InitiateExternalFileDragDrop(const char* path, const IRECT& iconBounds) override API_AVAILABLE(macos(10.13));

  float MeasureText(const IText& text, const char* str, IRECT& bounds) const override;

  EUIAppearance GetUIAppearance() const override;
protected:

  IPopupMenu* CreatePlatformPopupMenu(IPopupMenu& menu, const IRECT bounds, bool& isAsync) override;
  void CreatePlatformTextEntry(int paramIdx, const IText& text, const IRECT& bounds, int length, const char* str) override;
private:
  void PointToScreen(float& x, float& y) const;
  void ScreenToPoint(float& x, float& y) const;

  PlatformFontPtr LoadPlatformFont(const char* fontID, const char* fileNameOrResID) override;
  PlatformFontPtr LoadPlatformFont(const char* fontID, const char* fontName, ETextStyle style) override;
  PlatformFontPtr LoadPlatformFont(const char* fontID, void* pData, int dataSize) override;
  void CachePlatformFont(const char* fontID, const PlatformFontPtr& font) override;

  void RepositionCursor(CGPoint point);
  void StoreCursorPosition();
  
  void* mView = nullptr;
  CGPoint mCursorLockPosition;
  bool fFocused = false;
  WDL_String mBundleID, mAppGroupID;
  friend int GetMouseOver(IGraphicsMac* pGraphics);
};

END_IGRAPHICS_NAMESPACE
END_IPLUG_NAMESPACE
