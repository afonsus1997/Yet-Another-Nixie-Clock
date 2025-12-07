#ifndef __LWBTN_OPTS__H__
#define __LWBTN_OPTS__H__

/**
 * \brief           Enables `1` or disables `0` fast polling of button state.
 *                 Set to 1 if you want fast button responsiveness
 */
#define LWBTN_CFG_USE_CUSTOM_MILLIS        0   /* Set to 1 if you provide your own millis function */
#define LWBTN_CFG_USE_STATIC_MEMORY        1   /* Use static memory (no malloc) for lightweight use */

/**
 * \brief           Tick period in milliseconds. Must match how often you call `lwbtn_process()`.
 *                 For fast buttons, set to something like 5 or 10 ms.
 */
#define LWBTN_CFG_TICK_MS                  5

/**
 * \brief           Debounce time in milliseconds
 *                 If set too high, fast clicks may be missed.
 */
#define LWBTN_CFG_DEBOUNCE_TIME_MS         20

/**
 * \brief           Time to recognize a press-and-hold (long press)
 */
#define LWBTN_CFG_PRESS_TIME_MS            500

/**
 * \brief           Timeout between sequential clicks (e.g. double click)
 */
#define LWBTN_CFG_CLICK_TIMEOUT_MS         300

/**
 * \brief           Maximum number of buttons supported
 */
#define LWBTN_CFG_MAX_BUTTONS              8

#endif  //!__LWBTN_OPTS__H__