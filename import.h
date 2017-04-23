
#pragma once
#include <uccm/board.h>
#include <~sudachen/uc_waitfor/import.h>

#pragma uccm file(uccm_dynamic_defs.h) ~= #define EVENT_ID_CLICK_BUTTON ({#EVENT_ID:1} + EVENT_ID_FIRST)\n
#pragma uccm file(uccm_dynamic_defs.h) ~= #define EVENT_ID_PRESS_BUTTON ({#EVENT_ID:1} + EVENT_ID_FIRST)\n
#pragma uccm file(uccm_dynamic_defs.h) ~= #define EVENT_ID_RELEASE_BUTTON ({#EVENT_ID:1} + EVENT_ID_FIRST)\n
#define EVENT_IS_BY_BUTTON(E) ((E)->o.id >= EVENT_ID_CLICK_BUTTON && (E)->o.id <= EVENT_ID_RELEASE_BUTTON )

#pragma uccm require(source) += [@inc]/~sudachen/uc_button/uc_button.c

enum uc_button$State
{
    uc_button$STATE_UNKNOWN  = 0,
    uc_button$STATE_PRESSED  = 1,
    uc_button$STATE_RELEASED = 2,
};

typedef struct ButtonEvent ButtonEvent;
struct ButtonEvent
{
    Event e;
    Event timeout;
    uint8_t btnNo;
    uint8_t state : 7;
    uint8_t signalled : 1;
};

bool uc_button$pressButtonProbe(struct Event *e);
bool uc_button$releaseButtonProbe(struct Event *e);

#define PRESS_BUTTON_EVENT(BtnNo) PRESS_BUTTON_EVENT_w_CALLBACK(BtnNo,Callback)
#define PRESS_BUTTON_EVENT_w_CALLBACK(BtnNo,Callback) {\
    .e = {\
        NULL, Callback, \
        {.probe=uc_button$pressButtonProbe}, \
        {.id = EVENT_ID_PRESS_BUTTON, .kind = ACTIVATE_BY_PROBE, .repeat = 1}},\
    .btnNo = BtnNo, \
    .state = uc_button$STATE_UNKNOWN }

#define RELEASE_BUTTON_EVENT(BtnNo,Callback) RELEASE_BUTTON_EVENT_w_CALLBACK(BtnNo,NULL)
#define RELEASE_BUTTON_EVENT_w_CALLBACK(BtnNo,Callback) {\
    .e = {\
        NULL, Callback, \
        {.probe=uc_button$releaseButtonProbe}, \
        {.id = EVENT_ID_RELEASE_BUTTON, .kind = ACTIVATE_BY_PROBE, .repeat = 1}},\
    .btnNo = BtnNo, \
    .state = uc_button$STATE_UNKNOWN }

bool uc_button$longPressButtonProbe(struct Event *e);
void uc_button$longPressButtonTimeout(struct Event *e);

#define LONG_PRESS_BUTTON_EVENT(BtnNo,Ms) LONG_PRESS_BUTTON_EVENT_w_CALLBACK(BtnNo,Ms,NULL)
#define LONG_PRESS_BUTTON_EVENT_w_CALLBACK(BtnNo,Ms,Callback) {\
    .e = {\
        NULL, Callback, \
        {.probe=uc_button$longPressButtonProbe}, \
        {.id = EVENT_ID_CLICK_BUTTON, .kind = ACTIVATE_BY_PROBE, .repeat = 1}}, \
    .timeout = RTC_ONESHOT_EVENT_w_CALLBACK(Ms,uc_button$longPressButtonTimeout), \
    .btnNo = BtnNo, \
    .state = uc_button$STATE_UNKNOWN, \
    .signalled = 0 }

bool uc_button$clickButtonProbe(struct Event *e);
void uc_button$clickButtonTimeout(struct Event *e);

#define CLICK_BUTTON_EVENT(BtnNo,Ms) CLICK_BUTTON_EVENT_w_CALLBACK(BtnNo,Ms,NULL)
#define CLICK_BUTTON_EVENT_w_CALLBACK(BtnNo,Ms,Callback) {\
    .e = {\
        NULL, Callback, \
        {.probe=uc_button$clickButtonProbe}, \
        {.id = EVENT_ID_CLICK_BUTTON, .kind = ACTIVATE_BY_PROBE, .repeat = 1}}, \
    .timeout = RTC_ONESHOT_EVENT_w_CALLBACK(Ms,uc_button$clickButtonTimeout), \
    .btnNo = BtnNo, \
    .state = uc_button$STATE_UNKNOWN }
