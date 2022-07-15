#include "./Output.h"

OutputStatus_t Output_setValueOne(int ch, bool value) {
    // Support onboard only
    Output_t *output_select = (Output_t *) &Output[0];
    return output_select->setValue(output_select, ch, value);
}

OutputStatus_t Output_getValueOne(int ch, bool *value) {
    // Support onboard only
    Output_t *output_select = (Output_t *) &Output[0];
    return output_select->getValue(output_select, ch, value);
}
