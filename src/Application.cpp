#include "Application.h"

#include <string>

#include <Rtsp/RtspSession.h>
#include <Network/TcpServer.h>

#include <HiMPP/MPP.h>
#include <HiMPP/VENC/Subsystem.h>
#include <HiMPP/VENC/Group.h>
#include <HiMPP/VENC/Channel/Channel.h>

#include "NotSofiaMediaSource.h"

namespace rtsp_server {

using namespace std;

Application::Application() {
    srand(time(0));
}

void Application::beforeBoardRun() {
    auto &groups = board()->mpp()->venc()->groups();

    for (auto &group : groups) {
        for (auto &channel : group->channels()) {
            NotSofiaMediaSource *s = new NotSofiaMediaSource(channel);
            MediaSource::Ptr ptr(s);
            channel->setStreamOut(s, false);
            m_sources.push_back(ptr);
        }
    }
}

int Application::run() {
    Logger::Instance().add(std::make_shared<ConsoleChannel>("ConsoleChannel",
                           LogLevel::LDebug));

    TcpServer::Ptr rtspSrv(new TcpServer());
    rtspSrv->start<RtspSession>(554, "10.10.10.11");

    int code = ::Application::run();

    return code;
}

}
