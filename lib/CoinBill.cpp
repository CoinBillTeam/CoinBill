#include <Support/CLILogger.h>
#include <Support/CryptModule.h>
#include <Network/SocketFuncBinding.h>
#include <Network/Socket.h>

#include <User/Host.h>

#include <iostream>

using namespace CoinBill;

int main(int args, char* argc[], char* argv[])
{
    LogInf() << "Initialize CoinBill core client\n";
    LogInf() << "  Host Info : \n";
    LogInf() << "    - Version : " << Host::getHostVersion() << std::endl;
    LogInf() << "    - Build   : " << __DATE__ << " " << __TIME__ << std::endl;
    LogInf() << "Starting up CoinBill instance...."              << std::endl;

    // Initialize socket.
    bool socketInit = InitSocket();
    LogInf() << "Initialized socket. \n";

    // check socket initialized successfully.
    if (!socketInit) {
        LogErr() << "Failed initialize socket! \n";
        return -1;
    }

    return 0;
}
