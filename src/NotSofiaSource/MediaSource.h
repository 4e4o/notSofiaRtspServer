#ifndef NOT_SOFIA_MEDIA_SOURCE_H
#define NOT_SOFIA_MEDIA_SOURCE_H

#include <Rtsp/RtspMediaSource.h>
#include <Extension/H264Rtp.h>

#include <HiMPP/ASubsystem/ReadLoop/IReaderOut.h>

namespace hisilicon::mpp::venc {
class Channel;
}

namespace rtsp_server {

class H264FrameToRtpConverter;

class NotSofiaMediaSource : public RtspMediaSource,
    public hisilicon::mpp::IReaderOut {
  public:
    NotSofiaMediaSource(hisilicon::mpp::venc::Channel *c);
    ~NotSofiaMediaSource();

  private:
    void write(const HI_U8 *, const HI_U32 &) override final;

    hisilicon::mpp::venc::Channel *m_channel;
    H264Frame::Ptr m_frame;
    Track::Ptr m_track;
    std::shared_ptr<H264FrameToRtpConverter> m_converter;
};

}

#endif // NOT_SOFIA_MEDIA_SOURCE_H
