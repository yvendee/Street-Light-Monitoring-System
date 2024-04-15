#include "sim800.h"

void SIM800::preInit(void)
{
    pinMode(SIM800_POWER_STATUS, INPUT);
    delay(10);
    if (LOW == digitalRead(SIM800_POWER_STATUS))
    {
        if (sendATTest() != 0)
        {
            delay(800);
            digitalWrite(powerPin, HIGH);
            delay(200);
            digitalWrite(powerPin, LOW);
            delay(2000);
            digitalWrite(powerPin, HIGH);
            delay(3000);
        }
        while (sendATTest() != 0);
    }
    else
    {
        Serial.println("Power check failed!");
    }
}

int SIM800::checkReadable(void)
{
    return Serial2.available();
}

int SIM800::readBuffer(char *buffer, int count, unsigned int timeOut)
{
    int i = 0;
    unsigned long timerStart, timerEnd;
    timerStart = millis();
    while (1)
    {
        while (Serial2.available())
        {
            char c = Serial2.read();
            if (c == '\r' || c == '\n')
                c = '$';
            buffer[i++] = c;
            if (i > count - 1)
                break;
        }
        if (i > count - 1)
            break;
        timerEnd = millis();
        if (timerEnd - timerStart > 1000 * timeOut)
        {
            break;
        }
    }
    delay(500);
    while (Serial2.available())
    { // display the other thing..
        Serial2.read();
    }
    return 0;
}

void SIM800::cleanBuffer(char *buffer, int count)
{
    for (int i = 0; i < count; i++)
    {
        buffer[i] = '\0';
    }
}

void SIM800::sendCmd(const char *cmd)
{
    Serial2.write(cmd);
}

int SIM800::sendATTest(void)
{
    int ret = sendCmdAndWaitForResp("AT\r\n", "OK", DEFAULT_TIMEOUT);
    return ret;
}

int SIM800::waitForResp(const char *resp, unsigned int timeout)
{
    int len = strlen(resp);
    int sum = 0;
    unsigned long timerStart, timerEnd;
    timerStart = millis();

    while (1)
    {
        if (Serial2.available())
        {
            char c = Serial2.read();
            sum = (c == resp[sum]) ? sum + 1 : 0;
            if (sum == len)
                break;
        }
        timerEnd = millis();
        if (timerEnd - timerStart > 1000 * timeout)
        {
            return -1;
        }
    }

    while (Serial2.available())
    {
        Serial2.read();
    }

    return 0;
}

void SIM800::sendEndMark(void)
{
    Serial2.println((char)26);
}

int SIM800::sendCmdAndWaitForResp(const char *cmd, const char *resp, unsigned timeout)
{
    sendCmd(cmd);
    return waitForResp(resp, timeout);
}

void SIM800::serialDebug(void)
{
    while (1)
    {
        if (Serial2.available())
        {
            Serial.write(Serial2.read());
        }
        if (Serial.available())
        {
            Serial2.write(Serial.read());
        }
    }
}
