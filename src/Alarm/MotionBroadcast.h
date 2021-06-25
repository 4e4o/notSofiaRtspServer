#ifndef MOTION_BROADCAST_H
#define MOTION_BROADCAST_H

#include <string>

namespace hisilicon::mpp::venc {
class Channel;
}

namespace mediakit::Broadcast {

extern const std::string kBroadcastMotion;
#define BroadcastMotionArgs ::hisilicon::mpp::venc::Channel *channel

}

#endif /* MOTION_BROADCAST_H */
