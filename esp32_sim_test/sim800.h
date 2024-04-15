#ifndef __SIM800_H__
#define __SIM800_H__

#include "Arduino.h"
#include <HardwareSerial.h>

#define TRUE                    1
#define FALSE                   0

#define SIM800_POWER_PIN        9
#define SIM800_POWER_STATUS     12

#define UART_DEBUG

#ifdef UART_DEBUG
#define ERROR(x)            Serial.println(x)
#define DEBUG(x)            Serial.println(x);
#else
#define ERROR(x)
#define DEBUG(x)
#endif

#define DEFAULT_TIMEOUT     5

/** SIM800 class.
 *  Used for SIM800 communication. Attention: SIM800 module communicates with MCU over serial protocol.
 */
class SIM800
{

public:
    /** Create SIM800 Instance 
     *  @param baudRate baud rate of UART communication
     */
    SIM800(int baudRate = 9600): powerPin(SIM800_POWER_PIN) {
        pinMode(powerPin, OUTPUT);
        Serial2.begin(baudRate);
    };
    
    /** Power on SIM800
     */
    void preInit(void);
    
    /** Check if SIM800 readable
     */
    int checkReadable(void);
    
    /** Read from SIM800 module and save to buffer array
     *  @param  buffer  buffer array to save what read from SIM800 module
     *  @param  count   the maximal bytes number read from SIM800 module
     *  @param  timeOut time to wait for reading from SIM800 module 
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int readBuffer(char* buffer, int count, unsigned int timeOut = DEFAULT_TIMEOUT);

    
    /** Clean Buffer
     *  @param buffer   buffer to clean
     *  @param count    number of bytes to clean
     */
    void cleanBuffer(char* buffer, int count);
    
    /** Send AT command to SIM800 module
     *  @param cmd  command array which will be sent to GPRS module
     */
    void sendCmd(const char* cmd);

    /** Send "AT" to SIM800 module
     */
    int sendATTest(void);
    
    /** Send '0x1A' to SIM800 Module
     */
    void sendEndMark(void);
    
    /** Check SIM800 module response before timeout
     *  @param  *resp   correct response which SIM800 module will return
     *  @param  *timeout    waiting seconds till timeout
     *  @returns
     *      0 on success
     *      -1 on error
     */ 
    int waitForResp(const char* resp, unsigned timeout);

    /** Send AT command to GPRS module and wait for correct response
     *  @param  *cmd    AT command which will be sent to GPRS module
     *  @param  *resp   correct response which GPRS module will return
     *  @param  *timeout    waiting seconds till timeout
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int sendCmdAndWaitForResp(const char* cmd, const char *resp, unsigned timeout);


    /** Used for serial debug, you can specify tx and rx pin and then communicate with GPRS module with common AT commands
     */
    void serialDebug(void);
    
    int powerPin;

private:
    
};

#endif
