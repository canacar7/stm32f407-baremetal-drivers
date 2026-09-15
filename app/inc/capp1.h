/*
 * capp1.h
 *
 *  Created on: Sep 11, 2026
 *      Author: can
 */

#ifndef CAPP1_H_
#define CAPP1_H_

#include "../ciapplication.h"
#include "../../caaDriver/Inc/stm32f407xx.h"

namespace c::driver::app
{
    class CApplication1 : public CIApplication
    {
    public:
        virtual ~CApplication1() = default;
        void initClock() override;
        void run() override;
    };
}

#endif /* CAPP1_H_ */
