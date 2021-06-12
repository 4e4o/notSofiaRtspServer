#include "Application.h"

#include <string>

#include <Rtsp/RtspSession.h>
#include <Network/TcpServer.h>

namespace rtsp_server {

using namespace std;

Application::Application() {
}

int Application::run() {
    Logger::Instance().add(std::make_shared<ConsoleChannel>("ConsoleChannel",
                           LogLevel::LDebug));

    TcpServer::Ptr rtspSrv(new TcpServer());
    rtspSrv->start<RtspSession>(554, "127.0.0.1");

    int code = ::Application::run();

    return code;
}

}
