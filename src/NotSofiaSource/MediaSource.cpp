#include "MediaSource.h"
#include "H264ToRtpConverter.h"

#include "HiMPP/VENC/Group.h"
#include <HiMPP/VENC/Channel/Channel.h>
#include <HiMPP/VENC/Channel/H264Attributes.h>

#include <HiMPP/VI/Channel.h>

#define URL_PATH    "stream"
#define MTU         1400

namespace rtsp_server {

using namespace std;

static int getViChannelId(hisilicon::mpp::venc::Channel *c) {
    return hisilicon::mpp::vi::Channel::associatedChannelId(c, c->group());
}

NotSofiaMediaSource::NotSofiaMediaSource(hisilicon::mpp::venc::Channel *c)
    : RtspMediaSource(DEFAULT_VHOST, URL_PATH, to_string(getViChannelId(c))),
      m_channel(c),
      m_frame(FrameImp::create<H264Frame>()),
      m_track(new H264Track()) {

    using  hisilicon::mpp::venc::H264Attributes;
    H264Attributes *attr = static_cast<H264Attributes *>(m_channel->attributes());
    const HI_S32 sample_rate = attr->get<H264Attributes::SampleRate>();
    const HI_S32 fps = attr->get<H264Attributes::FrameRate>();
    const HI_S32 nal_prefix_size = attr->get<H264Attributes::NALPrefixSize>();

    m_converter = H264FrameToRtpConverter::create(this, MTU, sample_rate, fps);
    m_frame->_prefix_size = nal_prefix_size;

    DebugL << this << " , " << c->id();
}

NotSofiaMediaSource::~NotSofiaMediaSource() {
    // это чтоб меморки лика не было
    m_converter->close();

    DebugL << this << " , " << getId();
}

void NotSofiaMediaSource::write(const HI_U8 *data, const HI_U32 &len) {
    if (len == 0)
        return;

    m_frame->_buffer.assign(reinterpret_cast<const char *>(data), len);

    if (!m_track->ready()) {
        m_track->inputFrame(m_frame);

        if (m_track->ready()) {
            H264Track::Ptr t = dynamic_pointer_cast<H264Track>(m_track);
            setSdp(m_track->getSdp()->getSdp());
            DebugL << this << " , Track becomes ready " << m_channel->id() << " , " << "\n"
                   << t->getVideoFps() << " , " << t->getVideoWidth() << " , " <<
                   t->getVideoHeight() << "\n"
                   << getSdp();
        }

        return;
    }

    m_converter->inputFrame(m_frame);
}

}
