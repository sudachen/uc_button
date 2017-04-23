
#include <~sudachen/uc_button/import.h>

bool uc_button$pressButtonProbe(struct Event *e)
{
    bool triggered = false;
    ButtonEvent *be = (ButtonEvent*)e;
    switch ( be->state )
    {
        case uc_button$STATE_UNKNOWN:
            if ( !get_boardButton(be->btnNo) )
                be->state = uc_button$STATE_RELEASED;
            break;
        case uc_button$STATE_RELEASED:
            if ( get_boardButton(be->btnNo) )
            {
                be->state = uc_button$STATE_PRESSED;
                triggered = true;
            }
            break;
        case uc_button$STATE_PRESSED:
            if ( !get_boardButton(be->btnNo) )
            {
                be->state = uc_button$STATE_RELEASED;
            }
            break;
    }
    return triggered;
}

bool uc_button$releaseButtonProbe(struct Event *e)
{
    bool triggered = false;
    ButtonEvent *be = (ButtonEvent*)e;
    switch ( be->state )
    {
        case uc_button$STATE_UNKNOWN:
            if ( !get_boardButton(be->btnNo) )
                be->state = uc_button$STATE_RELEASED;
            break;
        case uc_button$STATE_RELEASED:
            if ( get_boardButton(be->btnNo) )
            {
                be->state = uc_button$STATE_PRESSED;
            }
            break;
        case uc_button$STATE_PRESSED:
            if ( !get_boardButton(be->btnNo) )
            {
                be->state = uc_button$STATE_RELEASED;
                triggered = true;
            }
            break;
    }
    return triggered;
}

void uc_button$clickButtonTimeout(struct Event *e)
{
    ButtonEvent *be = C_BASE_OF(ButtonEvent,timeout,e);
    be->signalled = 1;
}

bool uc_button$clickButtonProbe(struct Event *e)
{
    bool triggered = false;
    ButtonEvent *be = (ButtonEvent*)e;
    switch ( be->state )
    {
        case uc_button$STATE_UNKNOWN:
            if ( !get_boardButton(be->btnNo) )
                be->state = uc_button$STATE_RELEASED;
            break;
        case uc_button$STATE_RELEASED:
            if ( get_boardButton(be->btnNo) )
            {
                be->state = uc_button$STATE_PRESSED;
                be->signalled = false;
                list_event(&be->timeout);
            }
            break;
        case uc_button$STATE_PRESSED:
            if ( !get_boardButton(be->btnNo) )
            {
                be->state = uc_button$STATE_RELEASED;
                unlist_event(&be->timeout);
                triggered = !be->signalled;
                be->signalled = false;
            }
            break;
    }
    return triggered;
}

void uc_button$longPressButtonTimeout(struct Event *e)
{
    ButtonEvent *be = C_BASE_OF(ButtonEvent,timeout,e);
    be->signalled = 1;
}

bool uc_button$longPressButtonProbe(struct Event *e)
{
    bool triggered = false;
    ButtonEvent *be = (ButtonEvent*)e;

    switch ( be->state )
    {
        case uc_button$STATE_UNKNOWN:
            if ( !get_boardButton(be->btnNo) )
                be->state = uc_button$STATE_RELEASED;
            break;
        case uc_button$STATE_RELEASED:
            if ( get_boardButton(be->btnNo) )
            {
                be->state = uc_button$STATE_PRESSED;
                be->signalled = false;
                list_event(&be->timeout);
            }
            break;
        case uc_button$STATE_PRESSED:
            if ( !get_boardButton(be->btnNo) )
            {
                be->state = uc_button$STATE_RELEASED;
                unlist_event(&be->timeout);
            }
            break;
    }

    if ( be->signalled )
    {
        be->signalled = false;
        triggered = true;
    }

    return triggered;
}
