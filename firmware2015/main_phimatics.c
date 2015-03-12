#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "atmega-adc.h"
#include "usbdrv.h"

#include "../../include/phimatics_cmds.h" // common for firmware & externals

// ==============================================================================
// Useful define
// ------------------------------------------------------------------------------
#define setInput(ddr,pin) ((ddr) &= ~(1<<(pin)))
#define setOutput(ddr, pin) ((ddr) |= (1 << (pin)))
#define setLow(port, pin) ((port) &= ~(1 << (pin)))
#define setHigh(port, pin) ((port) |= (1 << (pin)))
#define invBit(port, pin) ((port) ^= (1 << (pin)))
#define getValue(port, pin) ((port) & (1 << (pin)))
#define pulse(port, pin) do {                       \
                           setHigh((port), (pin));  \
                           setLow((port), (pin));   \
                         } while (0)


// ==============================================================================
// Globals
// ------------------------------------------------------------------------------
void setAllOff(void);
void setAllOn(void);
void setAllLed(void);
void setOutLed(void);
void setMidLed(void);
void setInLed(void);
void setBlueLed(void);
void setLed(void);
void tlc5947Update(void);

// V-USB
uint8_t mode;
uint8_t usb_reply[18]; // ADC 10bits
uint8_t datenbuffer[3]; // 3 bytes - channel, 4bits pwm, 8 bits rest of pwm
uint8_t currentPosition, bytesRemaining;

// TLC5947
#define SCKDDR DDRB
#define SCKPORT PORTB
#define SCKPIN PB5

#define SINDDR DDRB
#define SINPORT PORTB
#define SINPIN PB3

#define CSDDR DDRB
#define CSPORT PORTB
#define CSPIN PB2

#define XLATDDR DDRD
#define XLATPORT PORTD
#define XLATPIN PD6

#define BLANKDDR DDRD
#define BLANKPORT PORTD
#define BLANKPIN PD7

//no operation assembly instruction
#define nop() __asm__ __volatile__("nop")

//while loop to pause if data is still sending
#define spi_wait() while (!(SPSR & (1<<SPIF))) {}

uint8_t nbChannel = 48;
uint16_t pwmbuffer[48];
uint8_t need2Update = 0;


// ------------------------------------------------------------------------------
// - usbFunctionSetup
// - see: http://vusb.wikidot.com/driver-api
// ------------------------------------------------------------------------------
usbMsgLen_t usbFunctionSetup(uchar data[8]) {
    usbRequest_t    *rq = (void *)data;

    if(rq->bRequest == PHIMATICS_CMD_POLL) {
        usbMsgLen_t len = sizeof(usb_reply); // we return up to 64 bytes
        if(len > rq->wLength.word)           // if the host requests less than we have
            len = rq->wLength.word;          // return only the amount requested
        usbMsgPtr = usb_reply;                  // tell driver where the buffer starts
        return len;

        
    } else if(rq->bRequest == PHIMATICS_CMD_ALLOFF) {
        setAllOff();

    } else if(rq->bRequest == PHIMATICS_CMD_ALLON) {
        setAllOn();

 	} else {
 		mode = rq->bRequest;
        currentPosition = 0;                // initialize position index
        bytesRemaining = rq->wLength.word;  // store the amount of data requested
        if(bytesRemaining > sizeof(datenbuffer)) // limit to buffer size
            bytesRemaining = sizeof(datenbuffer);
        return USB_NO_MSG;        // tell driver to use usbFunctionWrite
    }

    return 0;
}

//do not forget to set #define USB_CFG_IMPLEMENT_FN_WRITE 1 in usbconfig.h
uchar usbFunctionWrite(uchar *data, uchar len)
{
    uchar i;
    if(len > bytesRemaining)                // if this is the last incomplete chunk
        len = bytesRemaining;               // limit to the amount we can store
    bytesRemaining -= len;
    for(i = 0; i < len; i++)
        datenbuffer[currentPosition++] = data[i];

    switch(mode) {
    	case PHIMATICS_CMD_ALLLED:
    		setAllLed();
    	break;
    	case PHIMATICS_CMD_LED:
    		setLed();
    	break;
        case PHIMATICS_CMD_OUT:
            setOutLed();
        break;
        case PHIMATICS_CMD_MID:
            setMidLed();
        break;
        case PHIMATICS_CMD_IN:
            setInLed();
        break;
        case PHIMATICS_CMD_BLUE:
            setBlueLed();
        break;
    }

    return bytesRemaining == 0;             // return 1 if we have all data
}

// ------------------------------------------------------------------------------
// Handling Pure Data USB calls
// ------------------------------------------------------------------------------
void setAllOff(void) {
    setHigh(BLANKPORT, BLANKPIN);
}

void setAllOn(void) {
    setLow(BLANKPORT, BLANKPIN);
}

void setAllLed(void) {
    uint8_t c;
    uint16_t pwm = datenbuffer[0] << 8 | datenbuffer[1]; // because were using 2 full bytes for 12bits...

	for (c = 0; c <= 47; c++) {
            pwmbuffer[c] = pwm; 
    }
    //TLC5947_SetAllGS(pwm,0xFFFF);
    tlc5947Update();
}
/*
void TLC5947_SetAllGS(uint16_t value, uint16_t mask)
{
    uint8_t tmp1 = (value >> 4);
    uint8_t tmp2 = (value << 4);
    
    uint8_t i=0;
    uint8_t shift=0;

    // Each loop iteration writes 24-bits (2 channels)
    // Total number of loop iterations = 24 (2x24=48 channels)
    // Note: The lower indexes of pwmbuffer contain the higher channel numbers.
    // In other words, Channel 48 is shifted into the cascaded TLC5947s first.
    do
    {
        if((i==9)||(i==18)||(i==27)||(i==36)||(i==45)||(i==54)||(i==63))
            shift++;

        // Bits: 11 10 09 08 07 06 05 04  
        if((0x01<<shift)&mask)
            pwmbuffer[i] = tmp1;
        i++;

        // Bits: 03 02 01 00 11 10 09 08
        if((0x01<<shift)&mask)
            pwmbuffer[i] = tmp2;
                
        if((i==4)||(i==13)||(i==22)||(i==31)||(i==40)||(i==49)||(i==58)||(i==67))
            shift++;
            
        if((0x01<<shift)&mask)
            pwmbuffer[i] |= (tmp1>>4);
        i++;

        // Bits: 07 06 05 04 03 02 01 00
        if((0x01<<shift)&mask)
            pwmbuffer[i] = (uint8_t)value;
        i++;
    } while (i < 48);
}
*/
void setOutLed(void) {
    uint8_t c;
    uint16_t pwm = datenbuffer[0] << 8 | datenbuffer[1];
    
    for (c = 12; c <= 23; c++) {
            pwmbuffer[c] = pwm; 
    }
    tlc5947Update();
}

void setMidLed(void) {
    uint8_t c;
    uint16_t pwm = datenbuffer[0] << 8 | datenbuffer[1];
    
    for (c = 24; c <= 35; c++) {
            pwmbuffer[c] = pwm; 
    }
    tlc5947Update();
}

void setInLed(void) {
    uint8_t c;
    uint16_t pwm = datenbuffer[0] << 8 | datenbuffer[1];
    
    for (c = 36; c <= 47; c++) {
	
            pwmbuffer[c] = pwm; 
    }
    tlc5947Update();
}

void setBlueLed(void) {
    uint8_t c;
    uint16_t pwm = datenbuffer[0] << 8 | datenbuffer[1];

	for (c = 0; c <= 11; c++) {
            pwmbuffer[c] = pwm; 
    }
    tlc5947Update();
}

void setLed(void) {
	uint8_t channel = datenbuffer[0]; // full byte for channel
	uint16_t pwm = datenbuffer[1] << 8 | datenbuffer[2]; // because were using 2 full bytes for 12bits...
	
    pwmbuffer[channel] = pwm;
    tlc5947Update();
}

/*---------------------------------------------------------------------------*/
/* Configuration                                                             */
/*---------------------------------------------------------------------------*/
void hardwareInit(void)
{
	uchar i, j;

	/* activate pull-ups except on USB lines */
	USB_CFG_IOPORT = (uchar) ~ ((1 << USB_CFG_DMINUS_BIT) | (1 << USB_CFG_DPLUS_BIT));
	
	/* all pins input except USB (-> USB reset) */
	#ifdef USB_CFG_PULLUP_IOPORT	/* use usbDeviceConnect()/usbDeviceDisconnect() if available */
		USBDDR = 0;		/* we do RESET by deactivating pullup */
		usbDeviceDisconnect();
	#else
		USBDDR = (1 << USB_CFG_DMINUS_BIT) | (1 << USB_CFG_DPLUS_BIT);
	#endif

	j = 0;
	while (--j) {		/* USB Reset by device only required on Watchdog Reset */
		i = 0;
		while (--i);	/* delay >10ms for USB reset */
	}
	
	#ifdef USB_CFG_PULLUP_IOPORT
		usbDeviceConnect();
	#else
		USBDDR = 0;		/*  remove USB reset condition */
	#endif

	//only for HiCu
    //DDRD = 0b11010000; // set debug led

    setInput(DDRB, PB0);
    setHigh(PORTB, PB0);
    setInput(DDRB, PB1);
    setHigh(PORTB, PB1);
    setInput(DDRB, PB4);
    setHigh(PORTB, PB4);
    
    setInput(DDRD, PD0);
    setHigh(PORTD, PD0);
    setInput(DDRD, PD1);
    setHigh(PORTD, PD1);
    setInput(DDRD, PD4);
    setHigh(PORTD, PD4);
    setInput(DDRD, PD5);
    setHigh(PORTD, PD5);
}

/*---------------------------------------------------------------------------*/
/* TLC5947                                                                   */
/*---------------------------------------------------------------------------*/
void tlc5947Init(void)
{
	//set port output
	setOutput(SINDDR, SINPIN);
	setOutput(SCKDDR, SCKPIN);
	setOutput(CSDDR, CSPIN);
	setOutput(XLATDDR, XLATPIN);
	setOutput(BLANKDDR, BLANKPIN);
	nop();

	//pin initialize low
	setLow(SINPORT, SINPIN);
	setLow(SCKPORT, SCKPIN);
	setLow(CSPORT, CSPIN);
	nop();
	
	//enable SPI master mode
	SPCR = (1 << SPE) | (1 << MSTR);

	//enable double speed SPI
	SPSR = (1 << SPI2X);

	SPDR = 0;
	spi_wait();
	
	sei();
	nop();
}

void tlc5947Update(void)
{
	need2Update = 1;
}

void tlc5947UpdateSPI(void)
{
	uint8_t i;

	//display off
	//setHigh(BLANKPORT, BLANKPIN);

    /*
	uint8_t j;
	uint8_t k = 0;
	uint8_t eightbytesSpi[72];
    for(j = 0; j < 48; j++) {
        eightbytesSpi[k++] = pwmbuffer[j] >> 4;
        eightbytesSpi[k++] = (pwmbuffer[j] << 4) + (pwmbuffer[j + 1] >> 8);
        eightbytesSpi[k++] = pwmbuffer[j + 1];
        j++;
    }
    
    for(i = 0; i < 72; i++) {
        spi_wait();
		SPDR = (char) (eightbytesSpi[i]);
        
    }
	*/

	for (i = 0; i < nbChannel; i += 2)
	{
		spi_wait();
		SPDR = (char) (pwmbuffer[i] >> 4);
		spi_wait();
		SPDR = (char) (((pwmbuffer[i] << 4) & 0xF0) + ((pwmbuffer[i+1] >> 8) & 0xF));
		spi_wait();
		SPDR = (char) (pwmbuffer[i+1]);
	}
	
	//latch data
	setLow(XLATPORT, XLATPIN);
	
	//wait a bit (can be lowered to 1?)
	_delay_us(5);

	//read shift register data into grayscale reg
	setHigh(XLATPORT, XLATPIN);
	
	//display on
	//setLow(BLANKPORT, BLANKPIN);
 
}

// ==============================================================================
// - main
// ------------------------------------------------------------------------------
int main(void)
{
	hardwareInit();
	usbInit();
	tlc5947Init();
	setLow(BLANKPORT, BLANKPIN);

	//setOutput(SCKDDR, PB0);
    uint8_t i;
	uint8_t j;

	while (1)
	{
		usbPoll();
		
		if(need2Update) {
			tlc5947UpdateSPI();
			need2Update = 0;
		}
        
		// 8 analogs (0-7)
        for(i = 0; i < 8; i++) {
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
			} else if(i == 5) {
				usb_reply[10] = (uint8_t)((v & 0xFF00) >> 8); // 10 bits adc
                usb_reply[11] = (uint8_t)(v & 0x00FF);
			} else if(i == 6) {
				usb_reply[12] = (uint8_t)((v & 0xFF00) >> 8); // 10 bits adc
                usb_reply[13] = (uint8_t)(v & 0x00FF);
			} else if(i == 7) {
				usb_reply[14] = (uint8_t)((v & 0xFF00) >> 8); // 10 bits adc
                usb_reply[15] = (uint8_t)(v & 0x00FF);
			}
        }
        // 7 switch
        usb_reply[16] = PINB & 0b00010011;
        usb_reply[17] = PIND & 0b00110011;

        // test fading
        /*
        uint16_t i;
        uint8_t j;
        for(i = 0; i < 4095; i++) {
			for(j = 0; j < nbChannel; j++) {
				pwmbuffer[j] = i;
			}
			_delay_ms(2005);
			tlc5947UpdateSPI();
		}
		*/
    }
    
    return 0;
}
