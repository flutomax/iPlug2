#include "IPlugEffect.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

static const IText _TEXT = IText(DEFAULT_TEXT_SIZE + 6, COLOR_BLACK, DEFAULT_FONT, EAlign::Center, EVAlign::Middle);

IPlugEffect::IPlugEffect(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kGain)->InitDouble("Gain", 0., 0., 100.0, 0.01, "%");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds();
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(100).GetVShifted(-100), kGain));
    fLabel = new ITextControl(b.GetFromBottom(56), "", _TEXT);
    pGraphics->AttachControl(fLabel);
  };
#endif
}

IPlugEffect::~IPlugEffect()
{
  fLabel = nullptr;
}

void IPlugEffect::OnReset()
{
  fSampleRate = GetSampleRate();
}

void IPlugEffect::OnIdle()
{
  if (fLabel)
  {
    const double gain = GetParam(kGain)->Value();
    fLabel->SetStrFmt(32, "%f%%", gain);
  }
}

void IPlugEffect::OnUIClose()
{
  fLabel = nullptr;
}

#if IPLUG_DSP
void IPlugEffect::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const double gain = GetParam(kGain)->Value() / 100.;

  for (int s = 0; s < nFrames; s++)
  {
    outputs[0][s] = inputs[0][s] * gain;
    outputs[1][s] = inputs[1][s] * gain;
  }
}
#endif
