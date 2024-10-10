
#ifndef TIMER0_H_
#define TIMER0_H_

#include <stdint.h>

/* Set up our timer to give us an interrupt every millisecond
 * and update our time reference.
 */
void init_timer0(void);

/* Return the current clock tick value - milliseconds since the timer was
 * initialised.
 */
uint32_t get_current_time(void);

#endif /* TIMER0_H_ */