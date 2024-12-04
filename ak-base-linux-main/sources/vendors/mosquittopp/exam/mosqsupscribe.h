#ifndef MOSQSUPSCRIBE_H
#define MOSQSUPSCRIBE_H

#include "../lib/cpp/mosquittopp.h"
//#include <mosquittopp.h>
#include <cstring>
#include <cstdio>

struct packMessages_t;
void init_packMsg(struct packMessages_t *pkgMsg);

class mosqSupscribe : public mosqpp::mosquittopp
{
public:
    mosqSupscribe(struct packMessages_t *pkgMsg, const char *id); //const char *id, const char *host, int port);
    ~mosqSupscribe();
    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void setPkgMsg(struct packMessages_t *mPkgMsg);
    struct packMessages_t *mPkgMsg;
};
int test_mymosqSubscribe();

#endif // MOSQSUPSCRIBE_H
