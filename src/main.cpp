#include "Application.h"

#include <memory>

using namespace std;

int main(int count, char **argv) {
    unique_ptr<Application> app(new rtsp_server::Application(count, argv));
    return app->run();
}
