#include <RFIDReader.h>

/* TODO: Use usb_put_packet() to transmit data to USB HID,
using USB_HID_ENDPONT for the endpoint and the payload size
needs to match USB_CONFIG_HID_TX_SIZE. Use usb_get_packet()
to read incomming data, using USB_HID_ENDPOINT for the
endpoint. usb_enumerated() can be used to see if connected to
a host and ready to communicate. */
/*********************** usb_debug_task()*****************************/
//
// When called periodically, displays debugging information over serial
// to display enumeration and connection states.  Also lights LED2 and LED3
// based upon enumeration and connection status.
void usb_debug_task(void)
{
   static int8 last_connected;
   static int8 last_enumerated;
   int8 new_connected;
   int8 new_enumerated;
   
   new_connected = usb_attached(); // kiem tra xem device da duoc gan vao cac bus chua.no duoc goi lien tuc boi usb_task()
   new_enumerated = usb_enumerated();// thuc hien 1 quy trinh de ket noi device voi host

   if (new_connected && !last_connected)
      uart_printf("\r\n\nUSB connected, waiting for enumaration...");
   if (!new_connected && last_connected)
      uart_printf("\r\n\nUSB disconnected, waiting for connection...");
   if (new_enumerated && !last_enumerated)
      uart_printf("\r\n\nUSB enumerated by PC/HOST");
   if (!new_enumerated && last_enumerated)
      uart_printf("\r\n\nUSB unenumerated by PC/HOST, waiting for enumeration...");

   last_connected = new_connected;
   last_enumerated = new_enumerated;
}
void main()
{
   setup_adc_ports(HW_ADC_PORTS);
  setup_adc(HW_ADC_CONFIG);
  set_adc_channel(HW_ADC_CHANNEL);
   output_low(PIN_C6);
   output_low(PIN_C7);
   set_tris_a(0x00);
   uart_printf("Init USB \r\n");
 //  usb_init(); // ham khoi tao ket noi phan cung no se tao ra mot vong lap vo han de ket noi device toi cac bus nhung khong co nghia la no da duoc host chap nhan ket noi
   usb_init_cs();// ham nay khoi tao ket noi doi cho den khi co usb ket noi toi host
  
   uart_printf("Init RFID \r\n");
   LOW_A0;
   LOW_A1;
   rf_powerDown(); 
   delay_ms(100);
   HIGH_A0;
   HIGH_A1;
   HIGH_A3;
   rf_init();        
   rf_powerUp();         
   delay_ms(200);
   LOW_A0;
   LOW_A1;
   LOW_A3;
   uart_printf("RFID Setting success\r\n");
   while(TRUE)
   {
      usb_task();
      usb_debug_task();
      if(read_4102(inbuffer))
      {
         disable_interrupts(GLOBAL);
         HIGH_A3;
         HIGH_A0;
         LOW_A1;
         size = sizeof(inbuffer);
         uart_printf("Data RFID:\r\n");
         uart_printf("%u-%u-%u-%u-%u\r\n",inbuffer[0],inbuffer[1],inbuffer[2],inbuffer[3],inbuffer[4]); 
         if(usb_enumerated())
         {
            if(usb_put_packet(USB_HID_ENDPOINT, inbuffer, size, USB_DTS_TOGGLE))
            {
               uart_printf("Send data\r\n");
            }         
         }
         delay_ms(500);
         LOW_A3;
         enable_interrupts(GLOBAL);
      }
      else
      {
        HIGH_A1;
        LOW_A0;
      }
     
   };

}
