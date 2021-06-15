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
    Logger::Instance().add(std::make_shared<ConsoleChannel>("ConsoleChannel",
                           LogLevel::LDebug));

    TcpServer::Ptr rtspSrv(new TcpServer());
    rtspSrv->start<RtspSession>(554, "10.10.10.11");

    return ::Application::run();
}

}
