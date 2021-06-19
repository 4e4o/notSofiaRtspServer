#include "Board.h"
#include "NotSofiaSource/MediaSource.h"

#include <HiMPP/VENC/Channel/Channel.h>
#include <HiMPP/VENC/Channel/StreamReader.h>

namespace rtsp_server {

void Board::setStreamOut(hisilicon::mpp::venc::Channel *c) {
    NotSofiaMediaSource *s = new NotSofiaMediaSource(c);
    MediaSource::Ptr ptr(s);
    c->streamReader()->setOut(s, false);
    m_sources.push_back(ptr);
}

}
