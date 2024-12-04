#include "mosqpublish.h"
#include <cstring>

#define PUBLISH_TOPIC "my_public_topic"
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
struct packMessages_pub_t {
    char *client_id;
    char *host;
    char *topic;
    int devID;
    int port;
    int keepalive;
    char msgGet [100];
    char info_msg [250];
};
void init_packMsg_pub(struct packMessages_pub_t *pkgMsg) {
    pkgMsg->host = (char*)BROKER_ADDRESS;
    pkgMsg->client_id = (char*)CLIENT_ID;
    pkgMsg->topic =  (char*)MQTT_TOPIC;
    pkgMsg->devID = 1122; //TODO: getID device
    pkgMsg->port = MQTT_PORT;
    pkgMsg->keepalive = DEFAULT_KEEP_ALIVE;
    memset(pkgMsg->msgGet, 0, sizeof (pkgMsg->msgGet));
    memset(pkgMsg->info_msg, 0, sizeof (pkgMsg->info_msg));
}
mosqpublish::mosqpublish(struct packMessages_pub_t *pkgMsg, const char *id) : mosquittopp(id)
{
    mosqpp::lib_init();
    mPkgMsg = pkgMsg;
    connect_async(mPkgMsg->host,     // non blocking connection to broker request
    mPkgMsg->port,
    mPkgMsg->keepalive);
    loop_start();            // Start thread managing connection / publish / subscribe
};
mosqpublish::~mosqpublish() {
    loop_stop();                // Kill the thread
    mosqpp::lib_cleanup();    // Mosquitto library cleanup
};
bool mosqpublish::send_message(const  char * message)
{
// Send message - depending on QoS, mosquitto lib managed re-submission this the thread
//
// * NULL : Message Id (int *) this allow to latter get status of each message
// * topic : topic to be used
// * lenght of the message
// * message
// * qos (0,1,2)
// * retain (boolean) - indicates if message is retained on broker or not
// Should return MOSQ_ERR_SUCCESS
    int ret = publish(NULL,mPkgMsg->topic,strlen(message), message,1,false);
    return ( ret == MOSQ_ERR_SUCCESS );
}

//void mosqpublish::on_message(const struct mosquitto_message *message)
//{
//    /**/
//    char buf[51];
//    if(!strcmp(message->topic, "helloS")){
//        memset(buf, 0, 51*sizeof(char));
//        /* Copy N-1 bytes to ensure always 0 terminated. */
//        memcpy(buf, message->payload, 50*sizeof(char));
//        printf("%s",buf);
//    }
//}

void mosqpublish::on_disconnect(int rc) {
    #ifdef DEBUG
    std::cout << ">> myMosq - disconnection(" << rc << ")" << std::endl;
    #endif
}

void mosqpublish::on_connect(int rc)
{
    if ( rc == 0 ) {
        #ifdef DEBUG
        std::cout << ">> myMosq - connected with server" << std::endl;
        #endif
    } else {
        #ifdef DEBUG
        std::cout << ">> myMosq - Impossible to connect with server(" << rc << ")" << std::endl;
        #endif
    }
}

void mosqpublish::on_publish(int mid)
{
    #ifdef DEBUG
    std::cout << ">> myMosq - Message (" << mid << ") succeed to be published " << std::endl;
    #endif
}

int test_mymosqPublish(){
    class mosqpublish *myPub;
    struct packMessages_pub_t pkgSimple;
    init_packMsg_pub(&pkgSimple);

    mosqpp::lib_init();

    myPub = new mosqpublish(&pkgSimple, CLIENT_ID);
    myPub->send_message("hello world from devh");
    myPub->loop_start();

    mosqpp::lib_cleanup(); //TODO: not free this place

    return 0; // TODO: can return class pointer to free: myPub
}


