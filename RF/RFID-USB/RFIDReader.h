#include <18F2550.h>
#device ADC=16
// include lb USB
#include <pic18_usb.h>
#include <usb_desc_hid.h>
#include <usb.c>
#include <usbn960x.h>
// include lb RFID
#include <em4095.c>
#include <em4102.c>


#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES PLL5
#FUSES CPUDIV1
#FUSES HSPLL
#FUSES NOWDT                    //No Watch Dog Timer 
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale 
#FUSES NOPROTECT                //Code not protected from reading 
#FUSES BROWNOUT_NOSL            //Brownout enabled during operation, disabled during SLEEP 
#FUSES BROWNOUT                 //Reset when brownout detected 
#FUSES PUT                      //Power Up Timer 
#FUSES NOCPD                    //No EE protection 
#FUSES STVREN                   //Stack full/underflow will cause reset 
#FUSES NODEBUG                  //No Debug mode for ICD 
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O 
#FUSES NOWRT                    //Program memory not write protected 
#FUSES NOWRTD                   //Data EEPROM not write protected 
#FUSES IESO                     //Internal External Switch Over mode enabled 
#FUSES FCMEN                    //Fail-safe clock monitor enabled 
#FUSES PBADEN                   //PORTB pins are configured as analog input channels on RESET 
#FUSES NOWRTC                   //configuration not registers write protected 
#FUSES NOWRTB                   //Boot block not write protected 
#FUSES NOEBTR                   //Memory not protected from table reads 
#FUSES NOEBTRB                  //Boot block not protected from table reads 
#FUSES NOCPB                    //No Boot Block code protection 
#FUSES MCLR                     //Master Clear pin enabled 
#FUSES LPT1OSC                  //Timer1 configured for low-power operation 
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode) 

#use delay(clock=48000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)

#define USB_CONFIG_HID_TX_SIZE 5
#define USB_CONFIG_HID_RX_SIZE 5
#define USB_CONFIG_VID 0x04d8
#define USB_CONFIG_PID 0x0101
#define USB_HID_EP 1;

#define HW_ADC_CONFIG   ADC_CLOCK_INTERNAL
#define HW_ADC_CHANNEL  0
#define HW_ADC_PORTS    AN0
#define HW_INIT() setup_adc_ports(HW_ADC_PORTS); setup_comparator(NC_NC_NC_NC)
   
//#define USB_CON_SENSE_PIN 
// PIN_A0, PIN_A1 - LED
// PIN_A3 - sound

#define LED_ON(x) output_high(x)
#define LED_OFF(x) output_low(x)
#define HIGH_A0 output_high(PIN_A0)
#define HIGH_A1 output_high(PIN_A1)
#define HIGH_A3 output_high(PIN_A3)
#define LOW_A0 output_low(PIN_A0)
#define LOW_A1 output_low(PIN_A1)
#define LOW_A3 output_low(PIN_A3)
// define pin UART

#define uart_printf  printf

//function prototype
void usb_debug_task(void);
int8 outbuffer[USB_CONFIG_HID_TX_SIZE]; // buffer  out
int8 inbuffer[USB_CONFIG_HID_RX_SIZE]; // buffer in

//int8  data[]; 
int8 size, res;
