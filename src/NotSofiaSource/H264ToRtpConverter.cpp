#include "H264ToRtpConverter.h"

#include <Rtsp/RtspMediaSource.h>

namespace rtsp_server {

using namespace std;

H264FrameToRtpConverter::H264FrameToRtpConverter(RtspMediaSource *s,
        const int &mtu,
        const int &sampleRate,
        const int &fps)
    : m_source(s),
      m_encoder(new H264RtpEncoder(rand(), mtu, sampleRate)),
      m_ring(new RtpRing::RingType()),
      m_timestampIncrement(sampleRate / fps),
      m_rtpTimestamp(rand()) {
    DebugL << this;
}

H264FrameToRtpConverter::~H264FrameToRtpConverter() {
    DebugL << this;
}

void H264FrameToRtpConverter::init() {
    m_encoder->setRtpRing(m_ring);
    m_ring->setDelegate(shared_from_this());
}

void H264FrameToRtpConverter::close() {
    m_ring->setDelegate(RingDelegate<RtpPacket::Ptr>::Ptr());
}

void H264FrameToRtpConverter::inputFrame(H264Frame::Ptr frame) {
    const char *ptr = frame->data() + frame->prefixSize();
    const auto nal_type = H264_TYPE(ptr[0]);

    m_encoder->inputFrame(frame);

    if ((nal_type == H264Frame::NAL_IDR) || (nal_type == H264Frame::NAL_B_P))
        m_rtpTimestamp += m_timestampIncrement;
}

void H264FrameToRtpConverter::onWrite(RtpPacket::Ptr in, bool is_key) {
    in->getHeader()->stamp = htonl(m_rtpTimestamp);
    m_source->onWrite(in, is_key);
}

}
