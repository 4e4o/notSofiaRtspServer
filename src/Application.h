#ifndef RTSP_SERVER_APPLICATION_H
#define RTSP_SERVER_APPLICATION_H

#include <Application.h>

namespace rtsp_server {

class Application : public ::Application {
  public:
    Application(int count, char **argv);

  private:
    int run() override final;
};

}

#endif // RTSP_SERVER_APPLICATION_H
