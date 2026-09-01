#pragma once

#include "configuration.h"

#ifdef USE_ST7305

#include "ST7305Frame.h"
#include <OLEDDisplay.h>

class ST7305Display : public OLEDDisplay
{
  public:
    ST7305Display();
    ~ST7305Display() override;

    void display() override;

  protected:
    bool connect() override;
    void sendCommand(uint8_t command) override;
    int getBufferOffset() override { return 0; }

  private:
    void sendPanelCommand(uint8_t command, const uint8_t *data = nullptr, size_t length = 0);
    void sendPanelCommandUnlocked(uint8_t command, const uint8_t *data, size_t length);

    uint8_t *wireBuffer = nullptr;
};

#endif
