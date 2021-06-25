#include "Application.h"
#include "Board.h"
#include "Alarm/AlarmSession.h"

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
    if (args().size() < 4) {
        std::cout << "args: [ip] [rtsp_port] [alarm_port]" << std::endl;
        return 1;
    }

    Logger::Instance().add(std::make_shared<ConsoleChannel>("ConsoleChannel",
                           LogLevel::LDebug));

    const std::string ip = std::string(args()[1]);
    const int rtsp_port = std::stoi(std::string(args()[2]));
    const int alarm_port = std::stoi(std::string(args()[3]));

    TcpServer::Ptr rtspSrv(new TcpServer());
    rtspSrv->start<RtspSession>(rtsp_port, ip);

    TcpServer::Ptr alarmSrv(new TcpServer());
    alarmSrv->start<AlarmSession>(alarm_port, ip);

    return ::Application::run();
}

}
