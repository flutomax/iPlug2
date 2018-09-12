#include <cstring>
#include <cstdio>

#include "IGraphicsWeb.h"
#include "IControl.h"
#include "IPopupMenuControl.h"

using namespace emscripten;

extern IGraphics* gGraphics;

EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent* pEvent, void* pUserData)
{
  IGraphicsWeb* pGraphicsWeb = (IGraphicsWeb*) pUserData;

  switch (eventType) {
    case EMSCRIPTEN_EVENT_KEYDOWN: pGraphicsWeb->OnKeyDown(pGraphicsWeb->mPrevX, pGraphicsWeb->mPrevY, atoi(pEvent->key)); break;
    default:
      break;
  }
  
  return 0;
}

EM_BOOL outside_mouse_callback(int eventType, const EmscriptenMouseEvent* pEvent, void* pUserData)
{
  IGraphicsWeb* pGraphics = (IGraphicsWeb*) pUserData;
  
  IMouseMod modifiers(0, 0, pEvent->shiftKey, pEvent->ctrlKey, pEvent->altKey);
  
  double x = pEvent->targetX;
  double y = pEvent->targetY;
  
  val rect = GetCanvas().call<val>("getBoundingClientRect");
  x -= rect["left"].as<double>();
  y -= rect["top"].as<double>();

  x /= pGraphics->GetScale();
  y /= pGraphics->GetScale();
  
  switch (eventType) {
    case EMSCRIPTEN_EVENT_MOUSEUP: pGraphics->OnMouseUp(x, y, modifiers);
      pGraphics->OnMouseUp(x, y, modifiers); break;
      emscripten_set_mousemove_callback("#window", pGraphics, 1, nullptr);
      emscripten_set_mouseup_callback("#window", pGraphics, 1, nullptr);
      break;
    case EMSCRIPTEN_EVENT_MOUSEMOVE:
      if(pEvent->buttons != 0)
        pGraphics->OnMouseDrag(x, y, pEvent->movementX, pEvent->movementY, modifiers);
      break;
    default:
      break;
  }
  
  pGraphics->mPrevX = x;
  pGraphics->mPrevY = y;
    
  return true;
}

EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent* pEvent, void* pUserData)
{
  IGraphicsWeb* pGraphics = (IGraphicsWeb*) pUserData;
  
  IMouseMod modifiers(0, 0, pEvent->shiftKey, pEvent->ctrlKey, pEvent->altKey);
  
  double x = pEvent->targetX;
  double y = pEvent->targetY;
  
  x /= pGraphics->GetScale();
  y /= pGraphics->GetScale();
  
  switch (eventType) {
    case EMSCRIPTEN_EVENT_CLICK: break;
    case EMSCRIPTEN_EVENT_MOUSEDOWN: pGraphics->OnMouseDown(x, y, modifiers); break;
    case EMSCRIPTEN_EVENT_MOUSEUP: pGraphics->OnMouseUp(x, y, modifiers); break;
    case EMSCRIPTEN_EVENT_DBLCLICK: pGraphics->OnMouseDblClick(x, y, modifiers);break;
    case EMSCRIPTEN_EVENT_MOUSEMOVE:
      if(pEvent->buttons == 0)
        pGraphics->OnMouseOver(x, y, modifiers);
      else
        pGraphics->OnMouseDrag(x, y, pEvent->movementX, pEvent->movementY, modifiers);
      break;
    case EMSCRIPTEN_EVENT_MOUSEENTER:
      pGraphics->OnMouseOver(x, y, modifiers);
      emscripten_set_mousemove_callback("#window", pGraphics, 1, nullptr);
      break;
    case EMSCRIPTEN_EVENT_MOUSELEAVE:
      if(pEvent->buttons != 0) {
        emscripten_set_mousemove_callback("#window", pGraphics, 1, outside_mouse_callback);
        emscripten_set_mouseup_callback("#window", pGraphics, 1, outside_mouse_callback);
      }

      pGraphics->OnMouseOut(); break;
    default:
      break;
  }
  
  pGraphics->mPrevX = x;
  pGraphics->mPrevY = y;

  return true;
}

EM_BOOL wheel_callback(int eventType, const EmscriptenWheelEvent* pEvent, void* pUserData)
{
  IGraphics* pGraphics = (IGraphics*) pUserData;
  
  IMouseMod modifiers(0, 0, pEvent->mouse.shiftKey, pEvent->mouse.ctrlKey, pEvent->mouse.altKey);
  
  double x = pEvent->mouse.targetX;
  double y = pEvent->mouse.targetY;
  
  x /= pGraphics->GetScale();
  y /= pGraphics->GetScale();
  
  switch (eventType) {
    case EMSCRIPTEN_EVENT_WHEEL: pGraphics->OnMouseWheel(x, y, modifiers, pEvent->deltaY);
    default:
      break;
  }
  
  return true;
}

IGraphicsWeb::IGraphicsWeb(IGEditorDelegate& dlg, int w, int h, int fps, float scale)
: IGRAPHICS_DRAW_CLASS(dlg, w, h, fps, scale)
{
  val keys = val::global("Object").call<val>("keys", GetPreloadedImages());
  
  DBGMSG("Preloaded %i images\n", keys["length"].as<int>());
  
  emscripten_set_click_callback("canvas", this, 1, mouse_callback);
  emscripten_set_mousedown_callback("canvas", this, 1, mouse_callback);
  emscripten_set_mouseup_callback("canvas", this, 1, mouse_callback);
  emscripten_set_dblclick_callback("canvas", this, 1, mouse_callback);
  emscripten_set_mousemove_callback("canvas", this, 1, mouse_callback);
  emscripten_set_mouseenter_callback("canvas", this, 1, mouse_callback);
  emscripten_set_mouseleave_callback("canvas", this, 1, mouse_callback);
  emscripten_set_wheel_callback("canvas", this, 1, wheel_callback);
  emscripten_set_keydown_callback("#window", this, 1, key_callback);
}

IGraphicsWeb::~IGraphicsWeb()
{
}

void* IGraphicsWeb::OpenWindow(void* pHandle)
{
  OnViewInitialized(nullptr /* not used */);

  GetDelegate()->LayoutUI(this);
  
  return nullptr;
}

void IGraphicsWeb::HideMouseCursor(bool hide, bool returnToStartPos)
{
  if(hide)
    val::global("document")["body"]["style"].set("cursor", std::string("none"));
  else
    val::global("document")["body"]["style"].set("cursor", std::string("auto"));
}

bool IGraphicsWeb::OSFindResource(const char* name, const char* type, WDL_String& result)
{
  if (CStringHasContents(name))
  {
    WDL_String plusSlash;
    plusSlash.SetFormatted(strlen(name) + 1, "/%s", name);
    
    bool foundResource = false;
    
    if(strcmp(type, "png") == 0)
      foundResource = GetPreloadedImages().call<bool>("hasOwnProperty", std::string(plusSlash.Get()));
    else if(strcmp(type, "ttf") == 0)
      foundResource = true; // TODO: check ttf
    else if(strcmp(type, "svg") == 0)
      foundResource = true; // TODO: check svg
    
    if(foundResource)
    {
      result.Set(plusSlash.Get());
      return true;
    }
  }
  return false;
}

//static
void IGraphicsWeb::OnMainLoopTimer()
{
  IRECTList rects;
  
#ifdef IGRAPHICS_NANOVG
  gGraphics->SetAllControlsDirty();
#endif
  
  if (gGraphics->IsDirty(rects))
    gGraphics->Draw(rects);
}

bool IGraphicsWeb::GetTextFromClipboard(WDL_String& str)
{
  val clipboardText = val::global("window")["clipboardData"].call<val>("getData", std::string("Text"));
  
  str.Set(clipboardText.as<std::string>().c_str());

  return true; // TODO: return?
}

#define MB_OK 0
#define MB_OKCANCEL 1
#define MB_YESNOCANCEL 3
#define MB_YESNO 4
#define MB_RETRYCANCEL 5

int IGraphicsWeb::ShowMessageBox(const char* str, const char* caption, int type)
{
  
  switch (type)
  {
    case MB_OK:
        val::global("window").call<val>("alert", std::string(str));
      break;
    case MB_YESNO:
        val::global("window").call<val>("confirm", std::string(str));
       break;
    // case MB_CANCEL:
    //   break;
    default:
      break;
  }

  return 0; // TODO: return value?
}

IPopupMenu* IGraphicsWeb::CreatePopupMenu(IPopupMenu& menu, const IRECT& bounds, IControl* pCaller)
{
  ReleaseMouseCapture();
  
  if(mPopupControl)
    return mPopupControl->CreatePopupMenu(menu, bounds, pCaller);
  else
  {
    //TODO: implement select box
    return nullptr;
  }
}

bool IGraphicsWeb::OpenURL(const char* url, const char* msgWindowTitle, const char* confirmMsg, const char* errMsgOnFailure)
{
  val::global("window").call<val>("open", std::string(url), std::string("_blank"));
  
  return true;
}

#if defined IGRAPHICS_CANVAS
#include "IGraphicsCanvas.cpp"
#elif defined IGRAPHICS_NANOVG
#include "IGraphicsNanoVG.cpp"

#ifdef IGRAPHICS_FREETYPE
#define FONS_USE_FREETYPE
#endif

#include "nanovg.c"
#endif
