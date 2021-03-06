/*
 * ChariotLib.h - Library for interfacing to Chariot IoT Shield
 *               
 * Created by George Wayne for Qualia Networks, Inc., 2016.
 * BSD license, all text above must be included in any redistribution.
 */
#ifndef CHARIOT_EP_LIBINCLUDED
#define CHARIOT_EP_LIBINCLUDED

#include <Arduino.h>
#include <Wire.h>    			// the Arduino I2C library
#include <SoftwareSerial.h>

#define EP_DEBUG			0
#define SerialMon			if(debug)Serial

#define UNO					1
#define LEONARDO			2
#define MEGA_DUE			3

/*
 * Determine Chariot-Arduino channel and event resource setup
 */
#if !defined(HAVE_HWSERIAL0) && defined(HAVE_HWSERIAL1)
    // LEONARDO Host  
    #define LEONARDO_HOST   1
    #define UNO_HOST    	0
    #define MEGA_DUE_HOST 	0
	#define RX_PIN			11
	#define TX_PIN			12//4 -- problem using pin 4?
	#define MAX_RESOURCES	6

#elif defined(HAVE_HWSERIAL0) && !defined(HAVE_HWSERIAL1)
    //# UNO Host
    #define LEONARDO_HOST   0
    #define UNO_HOST      	1
    #define MEGA_DUE_HOST 	0
	#define RX_PIN			11
	#define TX_PIN			12
	#define MAX_RESOURCES	4
	
#elif defined(HAVE_HWSERIAL3)
	// MEGA Host
    #define LEONARDO_HOST   0
    #define UNO_HOST    	0
    #define MEGA_DUE_HOST 	1
	#define MAX_RESOURCES	8	// the limit of Chariot 
    #define ChariotClient Serial3
#else
  #error Board type not supported by Chariot at this time--contact Qualia Networks Tech Support.
#endif

/*
 * Chariot HW parameters
 *  --events, serial, CoAP resource
 */
#define RSRC_EVENT_INT_PIN  	9  // initiate external event interrupt
#define CHARIOT_STATE_PIN   	8  // driven HIGH when Chariot is online
#define MAX_BUFLEN				64

#define	TMP275_ADDRESS			0x48
#define FAHRENHEIT    			1
#define CELSIUS       			2
#define KELVIN        			3

#define JSON          			50  // per CoAP RFC

#define LT            			1
#define GT            			2
#define EQ            			3

#define ON            			true
#define OFF           			false

#define LF            			10
#define CR            			13

#define MINUTES       			1
#define SECONDS       			2

class ChariotEPClass
{
  public:
    ChariotEPClass();
	~ChariotEPClass();
    boolean begin();
	int available();
	void process();
	int coapResponseGet(String& response);
	bool pinValParse(String& command, int *pin, int *value);
		
	int createResource(String& uri, uint8_t maxBufLen, String& attrib);
	int createResource(const __FlashStringHelper* uri, uint8_t maxBufLen, const __FlashStringHelper* attrib);
	
	bool triggerResourceEvent(int handle, String& event, bool signalChariot);
	
	void serialChariotCmd();
	void serialChariotCmdHelp();
	int getIdFromURI(String& uri);
	int setPutHandler(int handle, String * (*putCallback)(String& putCmd));
	float readTMP275(uint8_t units);
	uint8_t getArduinoModel();
	void enableDebugMsgs();
	void disableDebugMsgs();
	
  private:
	uint8_t arduinoType;
	bool chariotAvailable;
	uint8_t maxBufLen;
	bool 	debug;

	// Event resources--these are stored in Chariot
	int nextRsrcId;

	String rsrcURIs[MAX_RESOURCES];
	String rsrcATTRs[MAX_RESOURCES];
	String * (*putCallbacks[MAX_RESOURCES])(String& putCmd);

	uint8_t rsrcChariotBufSizes[MAX_RESOURCES];

	void digitalCommand(String& command);
	void analogCommand(String& command);
	void modeCommand(String& command);
	void chariotSignal(int pin);
	void chariotPrintResponse();
};

extern ChariotEPClass ChariotEP;   // the EndPoint object for Chariot
#if LEONARDO_HOST==1 || UNO_HOST==1
    extern SoftwareSerial ChariotClient;
#endif

#endif
