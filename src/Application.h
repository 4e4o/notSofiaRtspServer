#ifndef RTSP_SERVER_APPLICATION_H
#define RTSP_SERVER_APPLICATION_H

#include <Application.h>
#include <Common/MediaSource.h>

namespace rtsp_server {

class Application : public ::Application {
  public:
    Application();

  private:
    int run() override final;
    void beforeBoardRun() override final;

    std::vector<MediaSource::Ptr> m_sources;
};

}

#endif // RTSP_SERVER_APPLICATION_H
