#ifdef TAPPING_TERM
    #undef TAPPING_TERM
#endif
#define TAPPING_TERM 600
#define HOLD_ON_OTHER_KEY_PRESS
#define QUICK_TAP_TERM 80
#define TAPPING_TOGGLE 3
#define TAP_CODE_DELAY 10
#define USB_POLLING_INTERVAL_MS 10

#ifndef NO_DEBUG
#define NO_DEBUG
#endif // !NO_DEBUG
#if !defined(NO_PRINT) && !defined(CONSOLE_ENABLE)
#define NO_PRINT
#endif // !NO_PRINT
