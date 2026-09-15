#include "capp1.h"


/*
 * 1-) GPIOA protunu acmka istersen. GPIOA portunun oldugu bus aktif edilmellidir.
 * 			GPIOA AHB1 bus baglıdır. Bus ın 0 biti a portuna denk gelmektedir.
 * 2-)
 *
 *
 *
 */

namespace c::driver::app
{
    void initClock()
    {
    	RCC->AHB1ENR |= (1 << 0);
    }


    void CApplication1::run()
    {

    }

}
