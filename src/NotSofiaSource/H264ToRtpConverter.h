#ifndef H264_TO_RTP_CONVERTER_H
#define H264_TO_RTP_CONVERTER_H

#include <Extension/H264Rtp.h>

namespace mediakit {
class RtspMediaSource;
}

namespace rtsp_server {

class H264FrameToRtpConverter :
    public std::enable_shared_from_this<H264FrameToRtpConverter>,
    public RingDelegate<RtpPacket::Ptr> {
  public:
    using Ptr = std::shared_ptr<H264FrameToRtpConverter>;

    template<typename... Args>
    static Ptr create(Args &&... args) {
        Ptr r(new H264FrameToRtpConverter(std::forward<Args>(args)...));
        r->init();
        return r;
    }

    ~H264FrameToRtpConverter();

    void inputFrame(H264Frame::Ptr);
    void close();

  private:
    H264FrameToRtpConverter(RtspMediaSource *,
                            const int &mtu,
                            const int &sampleRate,
                            const int &fps);
    void init();

    void onWrite(RtpPacket::Ptr in, bool is_key = true) override final;

    RtspMediaSource *m_source;
    H264RtpEncoder::Ptr m_encoder;
    RtpRing::RingType::Ptr m_ring;
    uint32_t m_timestampIncrement;
    uint32_t m_rtpTimestamp;
};

}

#endif // H264_TO_RTP_CONVERTER_H
