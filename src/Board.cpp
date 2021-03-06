#include "Board.h"
#include "NotSofiaSource/MediaSource.h"
#include "Alarm/MotionBroadcast.h"

#include <HiMPP/VENC/Channel/Channel.h>
#include <HiMPP/VENC/Channel/StreamReader.h>

#include <ADC/nvp6134/Motion.h>

#include <Util/NoticeCenter.h>

namespace rtsp_server {

void Board::setMotionEvent(int viChId,
                           ::nvp6134::Motion *m) {
    using namespace ::nvp6134;
    m->setEvent([viChId](Motion *) {
        NoticeCenter::Instance().emitEvent(Broadcast::kBroadcastMotion, viChId);
    });
}

void Board::setStreamOut(hisilicon::mpp::venc::Channel *c) {
    NotSofiaMediaSource *s = new NotSofiaMediaSource(c);
    MediaSource::Ptr ptr(s);
    c->streamReader()->setOut(s, false);
    m_sources.push_back(ptr);
}

}
