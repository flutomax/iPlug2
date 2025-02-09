#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IControl.h"

const int kNumPresets = 1;

enum EParams
{
  kGain = 0,
  kNumParams
};

using namespace iplug;
using namespace igraphics;

class IPlugEffect final : public Plugin
{
private:
  double fSampleRate = 44100;
  ITextControl* fLabel = nullptr;
public:
  IPlugEffect(const InstanceInfo& info);
  ~IPlugEffect();
  void OnReset() override;
  void OnIdle() override;
  void OnUIClose() override;
#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
#endif
};
