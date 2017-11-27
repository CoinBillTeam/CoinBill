#ifndef COINBILL_SUPPORT_LOGGER
#define COINBILL_SUPPORT_LOGGER

#include <iostream>
#include <ctime>
#include <User/Host.h>

#pragma warning(push)
#pragma warning(disable:4996)

namespace CoinBill
{
    inline auto& LogErr() {
        char buff[20];
        time_t time = (time_t)Host::getHostTime();
        strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&time));

        return std::cout << " [EROR] " << buff << " : ";
    }

    inline auto& LogInf() {
        char buff[20];
        time_t time = (time_t)Host::getHostTime();
        strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&time));

        return std::cout << " [INFO] " << buff << " : ";
    }
}
#pragma warning(pop)

#endif // COINBILL_SUPPORT_LOGGER