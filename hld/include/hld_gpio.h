#ifndef HDL_GPIO_H
#define HDL_GPIO_H

#include <stdint.h>
#include "hld_api.h"

hld_pin_ro_t*  hld_gpio_a0_ro_init(void);
hld_pin_ro_t*  hld_gpio_a1_ro_init(void);
hld_pin_wo_t*  hld_gpio_a9_wo_init(void);
hld_pin_wo_t*  hld_gpio_a10_wo_init(void);
hld_pin_wo_t*  hld_gpio_b1_wo_init(void);

#endif
