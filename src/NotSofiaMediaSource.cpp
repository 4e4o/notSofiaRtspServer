#include "NotSofiaMediaSource.h"

#include <HiMPP/VENC/Channel/Channel.h>

#define URL_PATH "stream"

namespace rtsp_server {

using namespace std;

NotSofiaMediaSource::NotSofiaMediaSource(hisilicon::mpp::venc::Channel *c)
    : ::RtspMediaSource(DEFAULT_VHOST, URL_PATH, to_string(c->id())),
      m_channel(c), m_encoder(new H264RtpEncoder(rand())),
      m_ring(new RtpRing::RingType()),
      m_delegate(new EncoderWriteDelegate(this)) {

    m_encoder->setRtpRing(m_ring);
    m_ring->setDelegate(m_delegate);

    m_h264Frame = FrameImp::create<H264Frame>();
    m_h264Frame->_prefix_size = 4;

    DebugL << this << " , " << c->id();

    const SIZE_S size = m_channel->imgSize();
    const string dimensions = to_string(size.u32Width) + "," +
                              to_string(size.u32Height) + "\r\n";

    const string sdp = "v=0\r\n"
                       "s=Media Presentation\r\n"
                       "e=NONE\r\n"
                       "t=0 0\r\n"
                       "m=video 0 RTP/AVP 96\r\n"
                       "a=recvonly\r\n"
                       "a=x-dimensions:" + dimensions +
                       "a=rtpmap:96 H264/90000\r\n";

    setSdp(sdp);
}

NotSofiaMediaSource::~NotSofiaMediaSource() {
    DebugL << this << " , " << getId();
}

void NotSofiaMediaSource::write(const HI_U8 *data, const HI_U32 &len) {
    if (len == 0)
        return;

    // TODO я незнаю что это за параметры, надо разбераться
    m_h264Frame->_pts = getCurrentMillisecond();
    m_h264Frame->_dts = m_h264Frame->_pts;
    m_h264Frame->_buffer.assign(reinterpret_cast<const char *>(data), len);

    m_encoder->inputFrame(m_h264Frame);

    // DebugL << this << " , " << getId() << " , " << len;
}

EncoderWriteDelegate::EncoderWriteDelegate(RtspMediaSource *s)
    : m_source(s) {
}

void EncoderWriteDelegate::onWrite(RtpPacket::Ptr in, bool is_key) {
    m_source->onWrite(in, is_key);
}

}
