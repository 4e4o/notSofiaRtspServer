#include "AlarmSession.h"
#include "MotionBroadcast.h"

#include <Util/NoticeCenter.h>

#define MAX_BUFFER_SIZE         10 * 1024
#define SESSION_TIMEOUT_SEC     10
#define CMD_SEPARATOR           "\n"

using namespace toolkit;

namespace mediakit {

namespace Broadcast {
const std::string kBroadcastMotion = "kBroadcastMotion";
}

AlarmSession::AlarmSession(const Socket::Ptr &_sock) : TcpSession(_sock) {
    DebugP(this);

    auto callback = [this](BroadcastMotionArgs) {
        this->onMotion(viChId);
    };

    NoticeCenter::Instance().addListener(this,
                                         Broadcast::kBroadcastMotion, callback);
}

AlarmSession::~AlarmSession() {
    NoticeCenter::Instance().delListener(this,
                                         Broadcast::kBroadcastMotion);
    DebugP(this);
}

void AlarmSession::onMotion(int viChId) {
    weak_ptr<AlarmSession> weakSelf = dynamic_pointer_cast<AlarmSession>
                                      (shared_from_this());
    auto strongSelf =  weakSelf.lock();

    if (!strongSelf)
        return;

    strongSelf->async([viChId, weakSelf]() {
        auto strongSelf =  weakSelf.lock();
        if (!strongSelf)
            return;

        strongSelf->sendMotion(viChId);
    });
}

void AlarmSession::sendCmd(const string &cmd) {
    SockSender::send(cmd + CMD_SEPARATOR);
}

void AlarmSession::sendMotion(int viChId) {
    sendCmd("MD " + std::to_string(viChId));
}

void AlarmSession::onRecv(const Buffer::Ptr &buf) {
    //    DebugL << hexdump(buf->data(), buf->size());

    if (_strRecvBuf.size() + buf->size() >= MAX_BUFFER_SIZE) {
        shutdown(SockException(Err_other, "recv buffer overflow"));
        return;
    }

    _beatTicker.resetTime();
    _strRecvBuf.append(buf->data(), buf->size());

    size_t index;
    string line;
    while ((index = _strRecvBuf.find(CMD_SEPARATOR)) != std::string::npos) {
        line = _strRecvBuf.substr(0, index);
        _strRecvBuf.erase(0, index + strlen(CMD_SEPARATOR));
        if (!onCommandLine(line)) {
            shutdown(SockException(Err_other, "exit cmd"));
            return;
        }
    }
}

void AlarmSession::onError(const SockException &err) {
    WarnP(this) << err.what();
}

void AlarmSession::onManager() {
    if (_beatTicker.elapsedTime() > 1000 * SESSION_TIMEOUT_SEC) {
        shutdown(SockException(Err_timeout, "alarm session timeout"));
        return;
    }
}

inline bool AlarmSession::onCommandLine(const string &line) {
    //    DebugP(this) << line;

    // ping/pong
    if (line == "PI")
        sendCmd("PO");

    return true;
}

}/* namespace mediakit */
