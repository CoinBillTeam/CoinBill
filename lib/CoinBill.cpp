#include <Support/CLILogger.h>
#include <Support/CryptBase.h>
#include <Support/CryptDecl.h>
#include <Network/SocketFuncBinding.h>
#include <Network/Socket.h>

#include <User/Host.h>

#include <iostream>

using namespace CoinBill;

int main(int args, char* argc[], char* argv[])
{
    LogInf() << "CoinBill Core Client"                          << std::endl;
    LogInf() << "Host Info : "                                  << std::endl;
    LogInf() << "   - Version : " << Host::getHostVersion()     << std::endl;
    LogInf() << "   - Build   : " << __DATE__ << " " << __TIME__<< std::endl;

    LogInf() << "Starting Up Program Instance...."              << std::endl;

    // Initialize socket.
    bool socketInit = InitSocket();
    LogInf() << "Initialized socket."                           << std::endl;

    // check socket initialized successfully.
    if (!socketInit) {
        LogErr() << "Failed initialize socket!"                 << std::endl;
        return -1;
    }

    // Initialize crypt algorithms.
    InitCryption();
    LogInf() << "Initialized cryption algorithms."              << std::endl;

    return 0;
}
