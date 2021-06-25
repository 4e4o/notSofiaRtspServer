#ifndef ALARM_SESSION_H_
#define ALARM_SESSION_H_

#include <functional>
#include "Common/config.h"
#include "Util/TimeTicker.h"
#include "Network/TcpSession.h"

using namespace toolkit;

namespace mediakit {

class AlarmSession: public TcpSession {
  public:
    AlarmSession(const Socket::Ptr &_sock);
    virtual ~AlarmSession();

    void onRecv(const Buffer::Ptr &) override;
    void onError(const SockException &err) override;
    void onManager() override;

  private:
    inline bool onCommandLine(const string &);
    void onMotion(int id);
    void sendMotion(int id);
    void sendCmd(const string &);

    string _strRecvBuf;
    Ticker _beatTicker;
};

} /* namespace mediakit */

#endif /* ALARM_SESSION_H_ */
