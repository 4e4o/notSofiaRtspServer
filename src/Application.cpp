#include "Application.h"
#include "Board.h"

#include <Rtsp/RtspSession.h>
#include <Network/TcpServer.h>

namespace rtsp_server {

using namespace std;

Application::Application(int count, char **argv)
    : ::Application(count, argv) {
    srand(time(0));
    registerType<::boards::ABoard, Board>();
}

int Application::run() {
    if (args().size() < 3) {
        std::cout << "args: [ip] [port]" << std::endl;
        return 1;
    }

    Logger::Instance().add(std::make_shared<ConsoleChannel>("ConsoleChannel",
                           LogLevel::LDebug));

    TcpServer::Ptr rtspSrv(new TcpServer());
    rtspSrv->start<RtspSession>(std::stoi(std::string(args()[2])),
                                std::string(args()[1]));

    return ::Application::run();
}

}
