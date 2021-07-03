#ifndef RTSP_SERVER_BOARD_H
#define RTSP_SERVER_BOARD_H

#include <Common/MediaSource.h>
#include <Boards/7004_lm_v3/Board.h>

namespace rtsp_server {

class Board : public boards::lm7004v3::Board {
  public:
    using boards::lm7004v3::Board::Board;

  private:
    void setStreamOut(hisilicon::mpp::venc::Channel *) override final;
    void setMotionEvent(int viChId,
                        ::nvp6134::Motion *) override final;

    std::vector<MediaSource::Ptr> m_sources;
};

}

#endif // RTSP_SERVER_BOARD_H
