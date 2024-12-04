#include "mosqsupscribe.h"

#define PUBLISH_TOPIC "my_subscribe_topic"
#define CLIENT_ID "Client_ID"
#define BROKER_ADDRESS "broker.hivemq.com"
#define MQTT_PORT 1883;
#define MQTT_TOPIC PUBLISH_TOPIC //just for test
#define MAX_PAYLOAD 50
#define DEFAULT_KEEP_ALIVE 60

#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

#define UNUSED(A) (void)(A)

struct packMessages_t {
    char *client_id;
    char *host;
    char *topic;
    int devID;
    int port;
    int keepalive;
    char msgGet [100];
    char info_msg [250];
};
void init_packMsg(struct packMessages_t *pkgMsg) {
    pkgMsg->host = (char*)BROKER_ADDRESS;
    pkgMsg->client_id = (char*)CLIENT_ID;
    pkgMsg->topic =  (char*)MQTT_TOPIC;
    pkgMsg->devID = 1122; //TODO: getID device
    pkgMsg->port = MQTT_PORT;
    pkgMsg->keepalive = DEFAULT_KEEP_ALIVE;
    memset(pkgMsg->msgGet, 0, sizeof (pkgMsg->msgGet));
    memset(pkgMsg->info_msg, 0, sizeof (pkgMsg->info_msg));
}



mosqSupscribe::mosqSupscribe(struct packMessages_t *pkgMsg, const char *id) : mosquittopp(id)
{
    mPkgMsg = pkgMsg;
    connect(mPkgMsg->host, mPkgMsg->port, mPkgMsg->keepalive);
}

mosqSupscribe::~mosqSupscribe()
{
}

void mosqSupscribe::setPkgMsg(struct packMessages_t *pkgMsg){
    mPkgMsg = pkgMsg;
}

void mosqSupscribe::on_connect(int rc)
{
    if (!rc)
    {
        #ifdef DEBUG
            std::cout << "Connected - code " << rc << std::endl;
        #endif
    }
}

void mosqSupscribe::on_subscribe(int mid, int qos_count, const int *granted_qos)
{

    int i;
    UNUSED(mid);
    bool have_subscription = false;
    #ifdef DEBUG
        std::cout << "on_subscribe" << std::endl;
    #endif
    /* In this example we only subscribe to a single topic at once, but a
     * SUBSCRIBE can contain many topics at once, so this is one way to check
     * them all. */
    for(i=0; i<qos_count; i++){
        if(granted_qos[i] <= 2){
            have_subscription = true;
        }
    }
    if(have_subscription == false){
        /* The broker rejected all of our subscriptions, we know we only sent
         * the one SUBSCRIBE, so there is no point remaining connected. */
        fprintf(stderr, "Error: All subscriptions rejected.\n");
    }
}

void mosqSupscribe::on_message(const struct mosquitto_message *message)
{
    /*overide*/
    /*this function is called loop*/
    int payload_size = MAX_PAYLOAD + 1;
    char buf[payload_size];
    memset(buf, 0, 51*sizeof(char));
    memcpy(buf, message->payload, MAX_PAYLOAD * sizeof(char));
    std::cout << buf << std::endl;
    if(!strcmp(message->topic, mPkgMsg->topic))
    {
        memset(buf, 0, 51*sizeof(char));
        /* Copy N-1 bytes to ensure always 0 terminated. */
        memcpy(buf, message->payload, MAX_PAYLOAD * sizeof(char));

        #ifdef DEBUG
            std::cout << buf << std::endl;
        #endif

    }
}

int test_mymosqSubscribe(){
    class mosqSupscribe *iot_client;
    int rc;
    struct packMessages_t pkgSimple;
    init_packMsg(&pkgSimple);
    mosqpp::lib_init();

    iot_client = new mosqSupscribe(&pkgSimple, CLIENT_ID);

    while(1)
    {
        rc = iot_client->loop();
        if (rc)
        {
            iot_client->reconnect();
        }
        else
            iot_client->subscribe(NULL, MQTT_TOPIC);
    }
//    rc = iot_client->loop_start();
//    if (rc)
//    {
//        iot_client->reconnect();
//    }
//    else
//        iot_client->subscribe(NULL, MQTT_TOPIC);
    mosqpp::lib_cleanup(); //TODO: not free this place

    return 0; // TODO: can return class pointer to free: iot_client
}
