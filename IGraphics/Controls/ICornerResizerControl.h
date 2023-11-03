/*
 ==============================================================================

 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.

 See LICENSE.txt for  more info.

 ==============================================================================
 */

#pragma once

/**
 * @file
 * @copydoc ICornerResizerControl
 */

#include "IControl.h"

BEGIN_IPLUG_NAMESPACE
BEGIN_IGRAPHICS_NAMESPACE

/** A control for resizing the plug-in window by clicking and dragging in the bottom right-hand corner
 * This can be added with IGraphics::AttachCornerResizer().
 * @ingroup SpecialControls */
class ICornerResizerControl : public IControl
{
public:
  ICornerResizerControl(IRECT graphicsBounds, float size)
  : IControl(graphicsBounds.GetFromBRHC(size, size).GetPadded(-1))
  , mInitialGraphicsBounds(graphicsBounds)
  , mSize(size)
  {
  }

  void Draw(IGraphics& g) override
  {
    /* ZEF -> Old drawing
    if(GetMouseIsOver() || GetUI()->mResizingInProcess)
      g.FillTriangle(COLOR_BLACK, mRECT.L, mRECT.B, mRECT.R, mRECT.T, mRECT.R, mRECT.B);
    else
      g.FillTriangle(COLOR_TRANSLUCENT, mRECT.L, mRECT.B, mRECT.R, mRECT.T, mRECT.R, mRECT.B);
    */
    g.FillTriangle(COLOR_TRANSLUCENT, mRECT.L, mRECT.B, mRECT.R, mRECT.T, mRECT.R, mRECT.B);
    const float k = 1.f / GetUI()->GetDrawScale();
    const float delta = 5.f * k;
    IRECT r = mRECT.GetPadded(0.f, 0.f, -2.f * k, -2.f * k);
    IColor clr = (GetMouseIsOver() || GetUI()->mResizingInProcess) ? COLOR_DARK_GRAY : COLOR_TRANSLUCENT;
    for (int i = 0; i < 3; ++i)
    {
      r.L += delta;
      r.T += delta;
      g.DrawLine(clr, r.L, r.B, r.R, r.T);
    }
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    GetUI()->StartDragResize();
  }
    
  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override
  {
    GetUI()->Resize(static_cast<int>(mInitialGraphicsBounds.W()), static_cast<int>(mInitialGraphicsBounds.H()), 1.f);
  }

  void OnRescale() override
  {
    float size = mSize * (1.f/GetUI()->GetDrawScale());
    IRECT r = GetUI()->GetBounds().GetFromBRHC(size, size);
    SetTargetAndDrawRECTs(r);
  }

  void OnMouseOver(float x, float y, const IMouseMod& mod) override
  {
    if (!mMouseOver)
      mPrevCursorType = GetUI()->SetMouseCursor(ECursor::SIZENWSE);
    mMouseOver = true;
    IControl::OnMouseOver(x, y, mod);
  }

  void OnMouseOut() override
  {
    if (mMouseOver)
      GetUI()->SetMouseCursor(mPrevCursorType);
    mMouseOver = false;
    IControl::OnMouseOut();
  }
private:
  float mSize;
  bool mMouseOver = false;
  ECursor mPrevCursorType = ECursor::ARROW;
  IRECT mInitialGraphicsBounds;
};

END_IGRAPHICS_NAMESPACE
END_IPLUG_NAMESPACE
