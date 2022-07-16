#include "./Display.h"

DisplayStatus_t Display_process() {
    const Display_t *display_select = (Display_t *) &Display[0];
    return display_select->process((void*) display_select);
}
