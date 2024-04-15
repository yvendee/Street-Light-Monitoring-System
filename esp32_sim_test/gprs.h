#ifndef __GPRS_H__
#define __GPRS_H__

#include "sim800.h"

enum Protocol {
    CLOSED = 0,
    TCP    = 1,
    UDP    = 2,
};

/** GPRS class.
 *  used to realize GPRS communication
 */
#define MESSAGE_LENGTH  20

class GPRS: public SIM800
{
public:
    uint32_t _ip;
    char ip_string[20];

    /** Create GPRS instance
     *  @param baudRate default baud rate for Serial communication
     */
    GPRS(int baudRate = 9600):SIM800(baudRate){};

    /** Initialize GPRS module including SIM card check & signal strength
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int init(void);

    /** Join APN
     *  @return
     *        true if join APN successfully
     *        false if failed to join VPN
     */
    bool join(const char  *apn = 0, const char *userName = 0, const char *passWord = 0);


    /** Parse IP string
     *  @return
     *      IP in hex
     */
    uint32_t str_to_ip(const char* str);

    /** Get IP address
     *  @return
     *       IP string
     */
    char* getIPAddress();

    /** Check SIM card status
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int checkSIMStatus(void);

    /** Check network connectivity
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int networkCheck(void);

    /** Send text SMS
     *  @param  *number phone number which SMS will be sent to
     *  @param  *data   message that will be sent
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int sendSMS(char* number, char* data);

    /** Read SMS if receiving a SMS message
     *  @param  messageIndex index of the message
     *  @param  message buffer used to store SMS message
     *  @param  length length of the buffer
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int readSMS(int messageIndex, char *message, int length);

    /** Delete SMS message from SIM card
     *  @param  index index of the message to delete
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int deleteSMS(int index);

    /** Call a phone number
     *  @param  number the phone number to call
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int callUp(char* number);

    /** Answer an incoming call
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int answer(void);

    /** Build TCP connection
     *  @param  ip  IP address to connect to
     *  @param  port TCP server port number
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int connectTCP(const char* ip, int port);

    /** Send data to TCP server
     *  @param  data data to send to TCP server
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int sendTCPData(char* data);

    /** Close TCP connection
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int closeTCP(void);

    /** Close TCP service
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int shutTCP(void);
};
#endif
