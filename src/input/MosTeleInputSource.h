#pragma once

#include "configuration.h"

#ifdef MOS_TELE

#include "InputBroker.h"
#include "MosTeleInput.h"
#include "concurrency/OSThread.h"

class MosTeleInputSource : public Observable<const InputEvent *>, public concurrency::OSThread
{
  public:
    MosTeleInputSource();

  protected:
    int32_t runOnce() override;

  private:
    MosTeleKey readKey();
    void sendKey(MosTeleKey key);
    bool isRepeatable(MosTeleKey key) const;

    MosTeleKey candidate = MosTeleKey::NONE;
    MosTeleKey stable = MosTeleKey::NONE;
    uint32_t candidateSinceMs = 0;
    uint32_t lastRepeatMs = 0;
    bool adcReady = false;
};

extern MosTeleInputSource *mosTeleInputSource;

#endif
