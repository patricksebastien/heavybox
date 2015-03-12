#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "atmega-adc.h"
#include "usbdrv.h"


// utils
#define sbi(var, mask)	((var) |= (unsigned char) (1 << mask))
#define cbi(var, mask)	((var) &= (unsigned char)~(1 << mask))

typedef unsigned char  u08;
typedef   signed char  s08;
typedef unsigned short u16;
typedef   signed short s16;

// global
static uchar usb_reply[13];

/* ------------------------------------------------------------------------- */
/* ------------------------------- BOOTLOADER ------------------------------ */
/* ------------------------------------------------------------------------- */
void (*jump_to_bootloader)(void) = 0x7000; __attribute__ ((unused))

void startBootloader(void) {

		TIMSK0 &= ~(1<<TOIE0);			// disable timer overflow

		cli();							// turn off interrupts
		wdt_disable();					// disable watchdog timer
		usbDeviceDisconnect(); 			// disconnect from USB bus

		cbi(ADCSRA, ADIE);				// disable ADC interrupts
		cbi(ADCSRA, ADEN);				// disable ADC (turn off ADC power)

		PORTA = 0;						// pull all pins low
		PORTB = 0;
		PORTC = 0;

		jump_to_bootloader();
}


// usbFunctionSetup
USB_PUBLIC uchar usbFunctionSetup(uchar data[8]) {

	usbRequest_t    *rq = (void *)data;
	usbMsgPtr = usb_reply;
	if(rq->bRequest == 0) { //POST
		return sizeof(usb_reply);
	}
	return 0;

}


// hardwareInit
static void hardwareInit(void)
{
	//USB
	uchar i, j;

	/* activate pull-ups except on USB lines */
	USB_CFG_IOPORT = (uchar) ~ ((1 << USB_CFG_DMINUS_BIT) | (1 << USB_CFG_DPLUS_BIT));

	/* all pins input except USB (-> USB reset) */
	#ifdef USB_CFG_PULLUP_IOPORT    /* use usbDeviceConnect()/usbDeviceDisconnect() if available */
	USBDDR = 0;        /* we do RESET by deactivating pullup */
	usbDeviceDisconnect();
	#else
	USBDDR = (1 << USB_CFG_DMINUS_BIT) | (1 << USB_CFG_DPLUS_BIT);
	#endif

	j = 0;
	while (--j) {        /* USB Reset by device only required on Watchdog Reset */
	i = 0;
	while (--i);    /* delay >10ms for USB reset */
	}

	#ifdef USB_CFG_PULLUP_IOPORT
	usbDeviceConnect();
	#else
	USBDDR = 0;        /*  remove USB reset condition */
	#endif

	// PORT F: ADC 0-7
	DDRF = 0x00;
	PORTF = 0x00;
	// PORT K: ADC 8-15
	DDRK = 0x00;
	PORTK = 0x00;

	// PORT C: INPUT / pull-up
	DDRC = 0x00;
	PORTC = 0xFF;

	// PORT J: INPUT / pull-up
	DDRJ = 0x00;
	PORTJ = 0xFF;

	// PORT A: INPUT / pull-up
	DDRA = 0x00;
	PORTA = 0xFF;

	// LED
	DDRD |= (1 << 6);
	DDRD |= (1 << 7);

	cbi(PORTD,6);
	cbi(PORTD,7);
}

// main
int main(void) {

	hardwareInit();
	usbInit();

	while(1) {

		usbPoll();
		for(i = 0; i < 5; i++) {
			uint16_t v = adc_read(ADC_PRESCALER_32, ADC_VREF_AVCC, i);
			if(i == 0) {
					usb_reply[0] = (uint8_t)((v & 0xFF00) >> 8); // 10 bits adc
					usb_reply[1] = (uint8_t)(v & 0x00FF);
			} else if(i == 1) {
				usb_reply[2] = (uint8_t)((v & 0xFF00) >> 8); // 10 bits adc
				usb_reply[3] = (uint8_t)(v & 0x00FF);
			} else if(i == 2) {
				usb_reply[4] = (uint8_t)((v & 0xFF00) >> 8); // 10 bits adc
				usb_reply[5] = (uint8_t)(v & 0x00FF);
			} else if(i == 3) {
				usb_reply[6] = (uint8_t)((v & 0xFF00) >> 8); // 10 bits adc
				usb_reply[7] = (uint8_t)(v & 0x00FF);
			} else if(i == 4) {
				usb_reply[8] = (uint8_t)((v & 0xFF00) >> 8); // 10 bits adc
				usb_reply[9] = (uint8_t)(v & 0x00FF);
			}
		}

		usb_reply[10] = PINC;
		usb_reply[11] = PINJ;
		usb_reply[12] = PINA;

	}
	return 0;
}
