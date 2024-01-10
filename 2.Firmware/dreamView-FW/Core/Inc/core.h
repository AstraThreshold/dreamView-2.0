//
// Created by Fir on 2023/4/18.
//

#ifndef CORE_H
#define CORE_H

//#include <cstdint>
#include "../../3rdParty/u8g2/u8g2.h"

#ifdef __cplusplus
extern "C" {
#endif

void dreamViewKernelStart(void);

  extern uint64_t sysTime;

#ifdef __cplusplus
}
#endif

#endif //CORE_H
