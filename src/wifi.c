
#include "wifi.h"
#include "conf.h"
#include "stm32f0_discovery.h"
#include <string.h>
#include "conio.h"
#include "main.h"
#include "shell.h"

///bool wifi_connected = false;
char CID[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
int8_t cid = -1;

void wifi_intit_pins()
{
    /* GPIOD Periph clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_WIFI_RESET, ENABLE);

    /* Configure pins in output pushpull mode */
    GPIO_InitTypeDef GPIO_InitStructure_WiFiReset;
    GPIO_InitStructure_WiFiReset.GPIO_Pin = WIFI_MOD_RESET_PIN;
    GPIO_InitStructure_WiFiReset.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure_WiFiReset.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure_WiFiReset.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure_WiFiReset.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(WIFI_MOD_RESET_PORT, &GPIO_InitStructure_WiFiReset);
    
    GPIO_SetBits(WIFI_MOD_RESET_PORT, WIFI_MOD_RESET_PIN);
    
    /*
     * Detect boot done by:
     * - Serial 2 WiFi APP message using UART
     * - GPIO28 going high with SPI
     */

//    while(GPIO_ReadInputDataBit(WIFI_MOD_DEDECT_BOOT_PORT,WIFI_MOD_DEDECT_BOOT_PIN)== Bit_SET)
//    {
//        //Wait while WiFi module is booting
//    }


}

void reset_wifi_module()
{

    // Unexpected reset. Possible Warm Boot(Possibly reasons:
    // external reset or Low Battery)

    GPIO_ResetBits(WIFI_MOD_RESET_PORT, WIFI_MOD_RESET_PIN);
    delay_ms(100);
    GPIO_SetBits(WIFI_MOD_RESET_PORT, WIFI_MOD_RESET_PIN);
    delay_ms(5000);
//    while(GPIO_ReadInputDataBit(WIFI_MOD_DEDECT_BOOT_PORT,WIFI_MOD_DEDECT_BOOT_PIN)== Bit_SET)
//    {
//        //Wait while WiFi module is booting
//    }

}

void receive(char *expected)
{
    signed char c;
    uint16_t idx = 0;
    while (expected[idx] != 0 && !serial_rb_empty(&rxbuf))
    {
        c = serial_rb_read(&rxbuf);
        if (c != expected[idx])
        {
            // wrong data received, red and yellow led are on
            STM_EVAL_LEDOff(LED3);
            while (1) {}
        }
        idx += 1;
    }
    if((expected[idx] !=0) && serial_rb_empty(&rxbuf))
    {
        // no data received, red and green led are on
        STM_EVAL_LEDOff(LED4);
        while (1) {}
    }
}

void send_and_check(char *cmd, uint32_t sleeptime, char *expected)
{
    delay_ms(500);
    cio_printf("%s", cmd);
    delay_ms(sleeptime);
    receive(expected);
}

void wifi_init_ap_mode()
{

    STM_EVAL_LEDOn(LED3);
    STM_EVAL_LEDOn(LED4);

    //send_and_check("AT+RESET\n",3000,"AT+RESET\n\xff\n\rAPP Reset-APP SW Reset\r\n");
    cio_printf("%s", "AT+RESET\n");
    delay_ms(3000);

    while (!serial_rb_empty(&rxbuf)) { serial_rb_read(&rxbuf);} // clear receive buffer

    send_and_check("AT+WD\n", 1000, "AT+WD\n" "\r\nOK\r\n");
    send_and_check("AT+WSEC=8\n", 1000, "AT+WSEC=8\n" "\r\nOK\r\n");
    send_and_check("AT+WPAPSK=" WIFI_NAME "," WIFI_PASSWORD "\n", 1000,
                   "AT+WPAPSK=" WIFI_NAME "," WIFI_PASSWORD "\n\nComputing PSK from SSID and PassPhrase...\r\n");
    delay_ms(10000);
    receive("\r\nOK\r\n");
    send_and_check("AT+NSET=192.168.1.1,255.255.255.0,192.168.1.1\n", 1000,
                   "AT+NSET=192.168.1.1,255.255.255.0,192.168.1.1\n" "\r\nOK\r\n");
    send_and_check("AT+WM=2\n", 1000, "AT+WM=2\n" "\r\nOK\r\n");
    send_and_check("AT+WA=" WIFI_NAME ",,11\n", 5000,
                   "AT+WA=" WIFI_NAME ",,11\n\n    IP              SubNet         Gateway   \r\n 192.168.1.1: 255.255.255.0: 19"
                   "2.168.1.1\r\nOK\r\n");

    send_and_check("AT+DHCPSRVR=1\n", 1000, "AT+DHCPSRVR=1\n" "\r\nOK\r\n");
    send_and_check("AT+DNS=1,wifi.jann.cc\n", 1000, "AT+DNS=1,wifi.jann.cc\n" "\r\nOK\r\n");
    send_and_check("AT+NSTCP=2000\n", 1000, "AT+NSTCP=2000\n\r\nCONNECT 0\r\n\r\nOK\r\n");

    delay_ms(1000);

    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED4);

    //wifi_connected = true; // just a hack, need to check for
    rx_lines_count = 0;
    // connect message '\r\nCONNECT 0 2 192.168.1.3 54020\r\n'

}

uint8_t get_wifi_msg(char *buffer, int bufferLength, int8_t *cid)
{
    static char initialized = false;
    static char *p;
    enum {DEFAULT, COMMAND, CID};
    static char state = DEFAULT;
    
    if(!initialized)
    {
        *cid = -1;
        state = DEFAULT;
        p = buffer;
        *p = '\0';
        initialized = true;
    }
    
    while (!serial_rb_empty(&rxbuf))
    {
        char c = serial_rb_read(&rxbuf);
        
        switch (c)
        {
        case '\n' :
        case '\r' :
            initialized = false;
            return shell_str_len(buffer);
            break;
        case '\0' :
            initialized = false;
            return 0;
            break;
        }
        
        switch (state)
        {
        case DEFAULT :
            switch (c)
            {
            case '\x1b' :
                state = COMMAND;
                break;
            default : 
                if (p < buffer + bufferLength - 1 && c >= ' ' && c < 127)
                { 
                    *p++ = c;
                    *p = '\0';
                }
                else
                {
                    buffer = "command too long";
                    initialized = false;
                    return shell_str_len(buffer);
                }
                break;
            }
            break;
        case COMMAND :
            switch (c)
            {
            case 'S' :
                state = CID;
                break;
            case 'E' :
                *p = '\0';
                initialized = false;
                return shell_str_len(buffer);
            default :
                // throw away
                break;
            }
            break;
        case CID :
            *cid = c;
            state = DEFAULT;
            break; 
        }
    }
    return 0;
}
