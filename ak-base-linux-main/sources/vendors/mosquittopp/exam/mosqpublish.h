#ifndef MOSQPUBLISH_H
#define MOSQPUBLISH_H
#include "../lib/cpp/mosquittopp.h"
#include <cstring>
#include <cstdio>

struct packMessages_pub_t;
void init_packMsg_pub(struct packMessages_pub_t *pkgMsg);

class mosqpublish : public mosqpp::mosquittopp
{
public:
    struct packMessages_pub_t *mPkgMsg;
    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_publish(int mid);
public:
    mosqpublish(struct packMessages_pub_t *pkgMsg, const char *id);
    ~mosqpublish();
    bool send_message(const char * _message);
};

int test_mymosqPublish();

#endif // MOSQPUBLISH_H
