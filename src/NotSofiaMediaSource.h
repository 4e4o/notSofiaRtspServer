#ifndef NOT_SOFIA_MEDIA_SOURCE_H
#define NOT_SOFIA_MEDIA_SOURCE_H

#include <Common/MediaSource.h>
#include <Rtsp/RtspMediaSource.h>
#include <Extension/H264Rtp.h>

#include <HiMPP/VENC/Channel/IStreamOut.h>

namespace hisilicon::mpp::venc {
class Channel;
}

namespace rtsp_server {

class EncoderWriteDelegate : public RingDelegate<RtpPacket::Ptr> {
  public:
    EncoderWriteDelegate(RtspMediaSource *);

  private:
    void onWrite(RtpPacket::Ptr in, bool is_key = true) override final;

    RtspMediaSource *m_source;
};

class NotSofiaMediaSource : public ::RtspMediaSource,
    public hisilicon::mpp::venc::IStreamOut {
  public:
    NotSofiaMediaSource(hisilicon::mpp::venc::Channel *c,
                        const string &app, const string &stream_id);
    ~NotSofiaMediaSource();

  private:
    void write(const HI_U8 *, const HI_U32 &) override final;

    hisilicon::mpp::venc::Channel *m_channel;
    H264RtpEncoder::Ptr m_encoder;
    RtpRing::RingType::Ptr m_ring;
    EncoderWriteDelegate::Ptr m_delegate;
    H264Frame::Ptr m_h264Frame;
};

}

#endif // NOT_SOFIA_MEDIA_SOURCE_H
