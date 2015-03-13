// ==============================================================================
//	heavybox.c
//
//	pd-Interface to [ 11h11 | heavybox ]
//  Adapted by: Patrick Sebastien Coulombe
//	Website:	http://www.workinprogress.ca/guitare-a-crayon
//
//	Original Author:	Michael Egger
//	Copyright:	2007 [ a n y m a ]
//	Website:	http://gnusb.sourceforge.net/
//
//	License:	GNU GPL 2.0 www.gnu.org
//	Version:	2009-04-11
// ==============================================================================
// ==============================================================================

#include "m_pd.h"
#include <usb.h> //http://libusb-win32.sourceforge.net
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pthread.h"


// ==============================================================================
// Constants
// ------------------------------------------------------------------------------
#define USBDEV_SHARED_VENDOR    	0x16c0  /* VOTI */
#define USBDEV_SHARED_PRODUCT   	0x05dc  /* Obdev's free shared PID */
#define USB_CFG_VENDOR_NAME			"workinprogress.ca"
#define USB_CFG_DEVICE_NAME			"heavybox"
#define OUTLETS 					8
#define USBREPLYBUFFER 				13

unsigned char buffer[USBREPLYBUFFER]; //accessible everywhere

// ==============================================================================
// Our External's Memory structure
// ------------------------------------------------------------------------------
typedef struct _heavybox				// defines our object's internal variables for each instance in a patch
{
	t_object 		p_ob;					// object header - ALL pd external MUST begin with this...
	usb_dev_handle	*dev_handle;			// handle to the heavybox usb device
	void 			*outlets[OUTLETS];		// handle to the objects outlets
	int				x_verbose;
	pthread_attr_t 	heavybox_thread_attr;
	pthread_t    	x_threadid;
} t_heavybox;

void *heavybox_class;					// global pointer to the object class - so pd can reference the object

// ==============================================================================
// Function Prototypes
// ------------------------------------------------------------------------------
void *heavybox_new(t_symbol *s);
void heavybox_assist(t_heavybox *x, void *b, long m, long a, char *s);
static int usbGetStringAscii(usb_dev_handle *dev, int ndex, int langid, char *buf, int buflen);
void find_device(t_heavybox *x);


// =============================================================================
// Threading
// ------------------------------------------------------------------------------
static void *usb_thread_read(void *w)
{
	int nBytes;
	t_heavybox *x = (t_heavybox*) w;
	unsigned char bufferIn[USBREPLYBUFFER];

	while(1) {
		pthread_testcancel();

		nBytes = usb_control_msg(x->dev_handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, 0, 0, 0, (char *)bufferIn, sizeof(bufferIn), 5000);

		// analog
		outlet_float(x->outlets[0], ((uint16_t)bufferIn[0] << 8) | bufferIn[1]);
		outlet_float(x->outlets[1], ((uint16_t)bufferIn[2] << 8) | bufferIn[3]);
		outlet_float(x->outlets[2], ((uint16_t)bufferIn[4] << 8) | bufferIn[5]);
		outlet_float(x->outlets[3], ((uint16_t)bufferIn[6] << 8) | bufferIn[7]);
		outlet_float(x->outlets[4], ((uint16_t)bufferIn[8] << 8) | bufferIn[9]);
		// digital
		outlet_float(x->outlets[5], bufferIn[10]);
		outlet_float(x->outlets[6], bufferIn[11]);
		outlet_float(x->outlets[7], bufferIn[12]);

		//if(x->x_verbose)post("thread read %i bytes", nBytes);

	}
	return 0;
}

static void usb_thread_start(t_heavybox *x) {
	// create the worker thread
    if(pthread_attr_init(&x->heavybox_thread_attr) < 0)
	{
       error("heavybox: could not launch receive thread");
       return;
    }
    if(pthread_attr_setdetachstate(&x->heavybox_thread_attr, PTHREAD_CREATE_DETACHED) < 0)
	{
       error("heavybox: could not launch receive thread");
       return;
    }
    if(pthread_create(&x->x_threadid, &x->heavybox_thread_attr, usb_thread_read, x) < 0)
		{
       error("heavybox: could not launch receive thread");
       return;
    }
    else
    {
       if(x->x_verbose)post("heavybox: thread %d launched", (int)x->x_threadid );
    }
}

//--------------------------------------------------------------------------
// - Object creation and setup
//--------------------------------------------------------------------------
int heavybox_setup(void)
{
	heavybox_class = class_new ( gensym("heavybox"),(t_newmethod)heavybox_new, 0, sizeof(t_heavybox), 	CLASS_DEFAULT,0);
	post("bald-approved heavybox version 0.2",0);
	return 1;
}

//--------------------------------------------------------------------------
void *heavybox_new(t_symbol *s)		// s = optional argument typed into object box (A_SYM) -- defaults to 0 if no args are typed
{
	t_heavybox *x;									// local variable (pointer to a t_heavybox data structure)
	x = (t_heavybox *)pd_new(heavybox_class);			 // create a new instance of this object
    x->x_verbose = 0;
	x->dev_handle = NULL;
	find_device(x);
	
	int i;
	// create outlets and assign it to our outlet variable in the instance's data structure
	for (i=0; i < OUTLETS; i++) {
		x->outlets[i] = outlet_new(&x->p_ob, &s_float);
	}
	if (x->dev_handle) {
		usb_thread_start(x); //start polling the device
	}

	return x; // return a reference to the object instance
}

//--------------------------------------------------------------------------
// - Object destruction
//--------------------------------------------------------------------------
void heavybox_free(t_heavybox *x)
{
	if (x->dev_handle) usb_close(x->dev_handle);
		while(pthread_cancel(x->x_threadid) < 0)
			if(x->x_verbose)post("heavybox: killing thread\n");
		if(x->x_verbose)post("heavybox: thread canceled\n");
}

//--------------------------------------------------------------------------
// - USB Utility Functions
//--------------------------------------------------------------------------
static int  usbGetStringAscii(usb_dev_handle *dev, int ndex, int langid, char *buf, int buflen)
{
char    asciibuffer[256];
int     rval, i;

    if((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR, (USB_DT_STRING << 8) + ndex, langid, asciibuffer, sizeof(asciibuffer), 1000)) < 0)
        return rval;
    if(asciibuffer[1] != USB_DT_STRING)
        return 0;
    if((unsigned char)asciibuffer[0] < rval)
        rval = (unsigned char)asciibuffer[0];
    rval /= 2;
    /* lossy conversion to ISO Latin1 */
    for(i=1;i<rval;i++){
        if(i > buflen)  /* destination buffer overflow */
            break;
        buf[i-1] = asciibuffer[2 * i];
        if(asciibuffer[2 * i + 1] != 0)  /* outside of ISO Latin1 range */
            buf[i-1] = '?';
    }
    buf[i-1] = 0;
    return i-1;
}

//--------------------------------------------------------------------------
void find_device(t_heavybox *x) {
	usb_dev_handle      *handle = NULL;
	struct usb_bus      *bus;
	struct usb_device   *dev;

	usb_init();
	usb_find_busses();
    usb_find_devices();
	 for(bus=usb_busses; bus; bus=bus->next){
        for(dev=bus->devices; dev; dev=dev->next){
            if(dev->descriptor.idVendor == USBDEV_SHARED_VENDOR && dev->descriptor.idProduct == USBDEV_SHARED_PRODUCT){
                char    string[256];
                int     len;
                handle = usb_open(dev); /* we need to open the device in order to query strings */
                if(!handle){
                    error ("Warning: cannot open USB device: %s", usb_strerror());
                    continue;
                }
                /* now find out whether the device actually is heavybox */
                len = usbGetStringAscii(handle, dev->descriptor.iManufacturer, 0x0409, string, sizeof(string));
                if(len < 0){
                    post("heavybox: warning: cannot query manufacturer for device: %s", usb_strerror());
                    goto skipDevice;
                }

                if(strcmp(string, USB_CFG_VENDOR_NAME) != 0)
                    goto skipDevice;
                len = usbGetStringAscii(handle, dev->descriptor.iProduct, 0x0409, string, sizeof(string));
                if(len < 0){
                    post("heavybox: warning: cannot query product for device: %s", usb_strerror());
                    goto skipDevice;
                }
                if(strcmp(string, USB_CFG_DEVICE_NAME) == 0)
                    break;
				skipDevice:
                usb_close(handle);
                handle = NULL;
            }
        }
        if(handle)
            break;
    }

    if(!handle){
        post("Could not find USB device workinprogress.ca/heavybox");
		x->dev_handle = NULL;
	} else {
		 x->dev_handle = handle;
		 post("Found USB device workinprogress.ca/heavybox");
	}
}
