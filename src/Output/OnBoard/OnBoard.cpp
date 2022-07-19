#include "./OnBoard.h"
#include "Board/Board.h"

static uint8_t on_board_output_pin[ONBOARD_RELAY_NUM] = ONBOARD_RELAY_PIN;

OutputStatus_t OnBoard_setValue(void* args, int ch, bool value) {
    if ((ch < 0) || (ch >= ONBOARD_RELAY_NUM)) {
        return OUTPUT_CH_INVALID;
    }

    pinMode(on_board_output_pin[ch], OUTPUT);
    digitalWrite(on_board_output_pin[ch], value ? ONBOARD_RELAY_ACTIVE : !ONBOARD_RELAY_ACTIVE);
    return OUTPUT_WORK_WELL;
}

OutputStatus_t OnBoard_getValue(void* args, int ch, bool* value) {
    if ((ch < 0) || (ch >= ONBOARD_RELAY_NUM)) {
        return OUTPUT_CH_INVALID;
    }

    *value = digitalRead(on_board_output_pin[ch]) == ONBOARD_RELAY_ACTIVE;
    return OUTPUT_WORK_WELL;
}