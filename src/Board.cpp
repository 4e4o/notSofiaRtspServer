#include "Board.h"
#include "NotSofiaMediaSource.h"

#include <HiMPP/VENC/Channel/Channel.h>

namespace rtsp_server {

void Board::setStreamOut(hisilicon::mpp::venc::Channel *c) {
    NotSofiaMediaSource *s = new NotSofiaMediaSource(c);
    MediaSource::Ptr ptr(s);
    c->setStreamOut(s, false);
    m_sources.push_back(ptr);
}

}
