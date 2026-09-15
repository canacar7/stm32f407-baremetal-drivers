#ifndef IAPPLICATION_HPP
#define IAPPLICATION_HPP


namespace c::driver::app
{
	class CIApplication
	{
	public:
		virtual ~CIApplication()
		{

		}

		virtual void initClock()
		{

		}
		virtual void initPeripherals()
		{

		}
		virtual void run() = 0;
	};
}

#endif
