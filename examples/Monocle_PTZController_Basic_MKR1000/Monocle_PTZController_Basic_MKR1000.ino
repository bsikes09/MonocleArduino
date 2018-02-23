/*
 **********************************************************************
 *             __  __  ___  _  _  ___   ___ _    ___
 *            |  \/  |/ _ \| \| |/ _ \ / __| |  | __|
 *            | |\/| | (_) | .` | (_) | (__| |__| _|
 *            |_|  |_|\___/|_|\_|\___/ \___|____|___|
 *
 * -------------------------------------------------------------------
 *                  MONOCLE PTZ CONTROLLER (BASIC)
 * -------------------------------------------------------------------
 *
 *  This project implements a Wireless PTZ camera controller using
 *  a small two-axis joystick with an OLED (128x64) display for status
 *  and menu access.  This project connects to a local Monocle
 *  Gateway service which provides direct communication with your
 *  ONVIF supported network cameras and the Monocle hosted services.
 *
 *  When used in conjunction with the Monocle Alexa Smart Home Skill,
 *  this PTZ controller will automatically switch to the current/
 *  active camera as last selected on your Amazon Echo Show, Echo
 *  Spot or FireTV device.
 *
 *  Author:   Robert Savage
 *  Date:     2018-02-18
 *  Website:  http://monoclecam.com
 *
 * -------------------------------------------------------------------
 *        COPYRIGHT SHADEBLUE, LLC @ 2018, ALL RIGHTS RESERVED
 * -------------------------------------------------------------------
 *
 **********************************************************************
 */

/**
 * ------------------------------------------------------------------------
 * PROJECT ABSTRACT
 * ------------------------------------------------------------------------
 * This project builds a fully functional wireless two axis PTZ (pan-tilt-zoom)
 * controller for your ONVIF supported network (IP) cameras.  This project
 * integrates with the Monocle platform and Monocle Alexa Smart Home skill
 * to actively and automatically control the last camera selected via your
 * Amazon Alexa-video enabled device such as Echo Show, Echo Spot or Fire TV.
 * In additional to direct PTZ control, you also have access from the
 * controller to recall the camera's home position or any of 1 to 9 camera
 * presets.
 *
 * This project uses a potentiometer based two-axis joystick to control
 * the cameras.  This joystick controller enables us to support multiple
 * PTZ speeds based on the relative position of the joystick.  We implement
 * only one stage of speed control due to the limited range of this joystick.
 * If you are looking for a thre-axis joystick with a wider range of motion,
 * please see the "Monocle_PTZController_Deluxe_MKR1000" sample project.
 * (Please see the Bill--Of-Materials section for more information
 * on this joystick controller.)
 */

/**
 * ------------------------------------------------------------------------
 * PROJECT DOCUMENTATION
 * ------------------------------------------------------------------------
 *
 * This project was developed and contributed as part of the
 *   "The Alexa and Arduino Smart Home Challenge"
 * https://www.hackster.io/contests/alexasmarthome
 *
 * You can find the fully documented project here:
 * https://www.hackster.io/monocle/monocle-view-control-ip-cameras-with-alexa-arduino-32bdd3
 */

 /**
  * ------------------------------------------------------------------------
  * MONOCLE CLOUD SERVICE
  * ------------------------------------------------------------------------
  *
  * This project is used in conjunction with the Monocle cloud service.
  * The cloud service integrates the Monocle Alexa smart home skill as
  * well as a provide a central location for you to configure your
  * network of IP cameras.  You will need to sign up and create an
  * account on MonocleCam and configure your netowrk cameras to use this
  * service.
  *
  * https://monoclecam.com | https://monocle.cam (not .com)
  */

/**
 * ------------------------------------------------------------------------
 * ALEXA SKILL
 * ------------------------------------------------------------------------
 *
 * This project is used in conjunction with the Monocle Alexa Smart Home
 * skill.  The PTZ controller can dynamically control the last selected
 * camera requested on your Alexa video-enabled device such as Echo Spot,
 * Echo Show or Fire TV.
 *
 * http://amzn.to/2CvKAuA
 */

/**
 * ------------------------------------------------------------------------
 * BILL OF MATERIALS
 * ------------------------------------------------------------------------
 *
 * This project was built using the following components:
 *
 *  1 @  Arduino MKR1000 (WiFI)
 *       https://store.arduino.cc/usa/arduino-mkr1000
 *       $34.99 USD
 *
 *  1 @  MakerFocus 2pcs I2C OLED Display Module 0.91 inch (128x32)
 *       http://amzn.to/2BBvqXX
 *       $10.99 USD (for 2 pieces)  <AMAZON PRIME>
 *
 *  1 @  Adafruit Analog 2-axis Thumb Joystick with Select Button
 *       http://amzn.to/2HoXlea
 *       $12.95 USD  <AMAZON PRIME>
 *
 *  1 @  Serpac A21 ABS Plastic Enclosure, 4-1/4" Length x 2.60" Width x 1-1/8" Height, Black
 *       http://amzn.to/2ETORgP
 *       $6.82 USD <AMAZON PRIME>
 *
 *  1 @  10-wire Ribbon Cable (Hook up wire)
 *       http://amzn.to/2ECzv0N
 *       $6.98 USD  <AMAZON PRIME>
 *
 *  OPTIONAL COMPONENTS:
 *
 *  1 @  Adafruit Lithium Ion Polymer Battery 3.7V 1200mAh
 *       http://amzn.to/2EDDZV2
 *       $15.88 USD  <AMAZON PRIME>
 *
 *  1 @  Micro USB Regulated Power Supply (5VDC 1A)
 *       http://amzn.to/2HqFFyT
 *       $7.99 USD  <AMAZON PRIME>
 *
 * (NOTE: prices listed are at the time of this writing: 2018-02-18)
 */

 /**
  * ------------------------------------------------------------------------
  * PROJECT DEPENDENCIES
  * ------------------------------------------------------------------------
  *
  * This project makes use of additional thrid-party Arduino libraries.
  * The following libraries must be installed to compile this project:
  *
  * - ArduinoHttpClient
  *   https://github.com/arduino-libraries/ArduinoHttpClient
  *   (used for web-socket communication)
  *
  * - ArduinoJson
  *   https://arduinojson.org
  *   (used for decoding communication messages)
  *
  * - Bounce2
  *   https://github.com/thomasfredericks/Bounce2
  *   (used for debouncing the joystick button)
  *
  * - Adafruit_GFX
  *   https://github.com/adafruit/Adafruit_SSD1306
  *   (used for OLED display)
  *
  * - Adafruit_SSD1306
  *   https://github.com/adafruit/Adafruit-GFX-Library
  *   (used for OLED display)
  *
  * - Arduino-MenuSystem
  *   https://github.com/jonblack/arduino-menusystem
  *   (used for menu navigation on OLED display)
  */

/**
 * ------------------------------------------------------------------------
 * PROGRAM LIBRARIES
 * ------------------------------------------------------------------------
 */

/* REQUIRED FOR WIRELESS NETWORK ON ARDUINO MKR1000 */
#include <WiFiClient.h>
#include <WiFi101.h>

/* REQUIRED FOR MONOCLE GATEWAY CLIENT */
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

/* REQUIRED FOR MONOCLE PTZ JOYSTICK */
#include <Bounce2.h>

/* REQUIRED FOR MONOCLE OLED */
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* REQUIRED FOR MONOCOLE MENU */
// @see https://github.com/jonblack/arduino-menusystem
#include <MenuSystem.h>

/* MONOCLE IMPLEMENTATION MODULES */
#include <MonocleGatewayClient.h>
#include <MonoclePTZJoystick.h>
#include <MonocleOLED.h>
#include <MonocleMenu.h>
#include <MonocleOLEDMenuRenderer.h>

/* YOUR PRIVATE INFORMATION */
#include "private.h"


/**
 * ------------------------------------------------------------------------
 * PROGRAM CONSTANTS
 * ------------------------------------------------------------------------
 */

/* PROGRAM NAME AND VERSION */
#define PROGRAM_NAME    "Monocle PTZ Controller (Basic)"
#define PROGRAM_VERSION "0.0.1"

/* JOYSTICK CONTROLLER PINS */
#define PIN_PAN     A5  // analog input pin 5 (PAN)  <X-AXIS>
#define PIN_TILT    A6  // analog input pin 6 (TILT) <Y-AXIS>
#define PIN_BUTTON  0   // joystick center (select) button

/* IF YOUR PTZ JOYSTICK IS WORKING BACKWARDS, YOU CAN INVERT EACH AXIS HERE */
#define INVERT_PAN_AXIS   false
#define INVERT_TILT_AXIS  false

/* CONFIGURE THE THRESHOLD FOR EACH AXIS STATE: LOW, MED, and HIGH */
#define JOYSTICK_THRESHOLD_LOW   500   // using 12 bit ADC, range is -2048 to +2048 where center is 0

/* DEFINE THE ADC RESOLUTION FOR JOYSTICK ANALOG PINS */
#define ADC_RESOLUTION 12

/* DEFINE THE DEBOUNCE INTERVAL FOR THE JOYSTICK BUTTON */
#define BUTTON_DEBOUNCE_INTERVAL 5 // milliseconds

/* DEFINE THE OLED MODULE SIZE (128x32 and 128x64 are supported) */
#define OLED_WIDTH  128
#define OLED_HEIGHT 32


/**
 * ------------------------------------------------------------------------
 * PROGRAM COMPILE TIME VALIDATION
 * ------------------------------------------------------------------------
 */

/* VERIFY THAT THE UNDERLYING ADAFRUIT SSD1306 LIBRARY IS CONFIGURED PROPERLY FOR A 128x64 OLED DISPLAY */
#if (SSD1306_LCDHEIGHT != OLED_HEIGHT)
// if you encounter this error, you need to open the "Adafruit_SSD1306.h"
// source file in the Arduino libfaries and find the "SSD1306_128_32"
// compiler directive and uncomment it and comment out any existing others
// for other OLED display resolutions
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


/**
 * ------------------------------------------------------------------------
 * PROGRAM VARIABLES
 * ------------------------------------------------------------------------
 */

 /* WIRELESS NETWORK SSID AND PASSWORD ARE STORED IN 'private.h' */
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASS;

// local connection state tracking variable
bool zoomActive = false;

// wifi client; needed for Monocle Gateway Client
WiFiClient wifi;

// create Monocle Gateway Client, PTZ Joystick, OLED and Menu instances
MonocleGatewayClient monocle = MonocleGatewayClient(wifi, MONOCLE_GATEWAY_ADDRESS, MONOCLE_GATEWAY_PORT);
MonoclePTZJoystick joystick = MonoclePTZJoystick();
MonocleOLED display = MonocleOLED(OLED_WIDTH, OLED_HEIGHT);
MonocleOLEDMenuRenderer renderer = MonocleOLEDMenuRenderer(&display);
MonocleMenu menu(renderer);

/**
 * ------------------------------------------------------------------------
 * PROGRAM INITIALIZATION
 * ------------------------------------------------------------------------
 */
void setup() {
  // define default UART baud rate
  // @see: https://www.arduino.cc/en/serial/begin
  Serial.begin(115200);

  // initialize OLED display and show boot logoand wait message
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.init();

  delay(2000); // <-- this is an artificial delay to see this data in the debug console when using Arduino IDE
  Serial.println("================================================");
  Serial.print(" PROGRAM: ");
  Serial.println(PROGRAM_NAME);
  Serial.print(" VERSION: ");
  Serial.println(PROGRAM_VERSION);
  Serial.println("================================================");

  // configure pins as input pins
  // @see: https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
  pinMode(PIN_PAN,    INPUT);
  pinMode(PIN_TILT,   INPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // configure the analog resoution and reference voltage (ADC)
  // @see: https://www.arduino.cc/reference/en/language/functions/analog-io/analogreference/
  // @see: https://www.arduino.cc/en/Reference.AnalogReadResolution
  analogReadResolution(ADC_RESOLUTION);
  analogReference(AR_DEFAULT);

  // configure joystick pins and the ADC resolution for each
  // (most likly same resulution for all pins)
  joystick.setupPan(PIN_PAN, ADC_RESOLUTION);
  joystick.setupTilt(PIN_TILT, ADC_RESOLUTION);

  // configure the joystick button and debounce interval
  joystick.setupButton(PIN_BUTTON, BUTTON_DEBOUNCE_INTERVAL);

  // optionally configure axis inversion
  joystick.invertPanAxis(INVERT_PAN_AXIS);
  joystick.invertTiltAxis(INVERT_TILT_AXIS);

  // configute joystick thresholds for the LOW state only (MED and HIGH are not used with this joystick)
  joystick.setPanThresholdLow(JOYSTICK_THRESHOLD_LOW);
  joystick.setTiltThresholdLow(JOYSTICK_THRESHOLD_LOW);

  // register for PTZ event and button press event callbacks
  joystick.onPTZ(&joystickPTZChangeHandler);
  joystick.onButtonPress(&joystickButtonPressHandler);

  // We will disable the multistate nature of the PTZ
  // joystick.  With multistate disabled, only the low
  // threshold of each axis is evaluated and events
  // dispatched.  This means that we will only receive
  // a single event for each PTZ axis directional change
  // rather than the variable states we get by default.
  // We do this because of the limited rage of this
  // particular joystick controller.  Only a single
  // speed is reliable for this joystick hardware.
  joystick.disableMultistate(true);

  // we will handle custom zooming logic in this
  // program, so lets add the zoom menu to the menu list
  menu.addZoomMenu();

  // register for Menu event callbacks
  menu.onActivate(&menuActivateHandler);
  menu.onDeactivate(&menuDeactivateHandler);
  menu.onHome(&menuHomeHandler);
  menu.onZoom(&menuZoomHandler);
  menu.onPreset(&menuPresetHandler);

  // display connecting status on OLED
  display.clearText(false);
  display.printLine1("Connecting to WiFi ..", false);
  display.printLine2(ssid);

  // let the user know that we are attempting to connect to the wireless network
  Serial.print("Connecting to wirless network: ");
  Serial.println(ssid);

  // start the connection to the user's wireless access point
  // @see: https://www.arduino.cc/en/Reference/WiFiBegin
  WiFi.begin(ssid, password);

  // wait until the wireless network connection has been established
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  // print something the let the user know we are still working
  }

  // let the user know we have successfully connected to the network
  IPAddress myIP = WiFi.localIP();
  String ip_address = String(myIP[0])+"."+String(myIP[1])+"."+String(myIP[2])+"."+String(myIP[3]);
  Serial.println("Successfully connected to wireless network:");
  Serial.print(" - SSID       : ");
  Serial.println(WiFi.SSID());
  Serial.print(" - IP Address : ");
  Serial.println(ip_address);
  Serial.println("================================================");

  // display connected status on OLED
  display.printText("WiFi Connected", ip_address);
}

/**
 * DISPLAY CAMERA INFO
 * ----------------------------------------------
 * This method is called to restore the current
 * camera information and control GUI to the
 * OLED display.
 */
void displayCameraInfo(){
  display.printText("    CAMERA READY!", "  (click for menu)");
}

/**
 * JOYSTICK PTZ CHANGE CALLBACK
 * ----------------------------------------------
 * This callback handler is called when any of the
 * three axis on the PTZ joystick change states.
 * The 'pan', 'tilt' and 'zoom' parameters
 * provided will contain one of the following
 * state values:
 *
 *  STATE  SPEED   PAN     TILT    ZOOM
 *  --------------------------------------
 *   -3    HIGH    LEFT    DOWN    IN
 *   -2    MED     LEFT    DOWN    IN
 *   -1    LOW     LEFT    DOWN    IN
 *    0    OFF     NONE    NONE    NONE
 *    1    LOW     RIGHT   UP      OUT
 *    2    MED     RIGHT   UP      OUT
 *    3    HIGH    RIGHT   UP      OUT
 *
 * NOTE: If the 'multistate' nature is disabled on
 *       the PTZ cjoystick, then only the LOW
 *       thresholds will be evaluated and only
 *       LOW states will be evented.
 */
void joystickPTZChangeHandler(int pan, int tilt, int zoom){

  // use the joystick inputs to handle menu
  // navigation control when menu is active
  if(menu.isActive()){
    if(tilt < 0) menu.next();
    else if(tilt > 0) menu.prev();
    else if(pan < 0) menu.back();
    return;
  }

  // while zoom mode is active, then translate any tilt actions to zoom
  if(zoomActive == true){
    zoom = tilt;
    tilt = 0;
    pan = 0;
  }

  // send instruction to the Monocle gateway client to perform the PTZ movement
  // (we multiply each axis by two to send a request for mediam speed rather than low speed)
  monocle.ptz(pan*2, tilt*2, zoom*2);

  // build a display string of the current  PTZ action(s)
  String info = "";
  if(pan > 0) info+=" RIGHT";
  if(pan < 0) info+=" LEFT";
  if(tilt > 0) info+=" UP";
  if(tilt < 0) info+=" DOWN";
  if(zoom > 0) info+=" IN";
  if(zoom < 0) info+=" OUT";
  display.printLine4(info);

  // enable this debugging info if you need to calibrate your josystick thresholds
  //  Serial.print("PAN=");
  //  Serial.print(joystick.panValue());
  //  Serial.print(" * ");
  //  Serial.print(joystick.panState());
  //  Serial.print("; TILT=");
  //  Serial.print(joystick.tiltValue());
  //  Serial.print(" * ");
  //  Serial.print(joystick.tiltState());
  //  Serial.print("; ZOOM=");
  //  Serial.print(joystick.zoomValue());
  //  Serial.print(" * ");
  //  Serial.println(joystick.zoomState());
}

/**
 * JOYSTICK BUTTON PRESS CALLBACK
 * ----------------------------------------------
 * This callback handler is called whenever
 * the PTZ joystick button is pressed
 */
void joystickButtonPressHandler(){

  // if the zoom mode is active, then disable it
  if(zoomActive == true){
    zoomActive = false;
    displayCameraInfo();
    return;
  }

  // if the menu is not currently active, then activate it
  if(!menu.isActive()){
    menu.activate();
  }
  // if the menu is already active, then pass the
  // select command to the menu system
  else{
    menu.select();
  }
}

/**
 * MENU SYSTEM ACTIVATION CALLBACK
 * ----------------------------------------------
 * This callback handler is called whenever
 * the menu system is activated.
 */
void menuActivateHandler(){
}

/**
 * MENU SYSTEM DEACTIVATION CALLBACK
 * ----------------------------------------------
 * This callback handler is called whenever
 * the menu system is de-activated.
 */
void menuDeactivateHandler(){
  // restore the current camera to the OLED display
  displayCameraInfo();
}

/**
 * MENU SYSTEM HOME CALLBACK
 * ----------------------------------------------
 * This callback handler is called whenever
 * the 'Home' items is selected in the menu system.
 */
void menuHomeHandler(){
  // send instruction to the Monocle gateway client
  // to perform the PTZ goto home position action.
  monocle.home();
}

/**
 * MENU SYSTEM ZOOM CALLBACK
 * ----------------------------------------------
 * This callback handler is called whenever
 * the 'Zoom' item is selected in the menu system.
 */
void menuZoomHandler(){
  zoomActive = true;
  display.printText("ZOOM  (click to exit)", " - UP to ZOOM IN", " - DOWN to ZOOM OUT");
}

/**
 * MENU SYSTEM PRESET CALLBACK
 * ----------------------------------------------
 * This callback handler is called whenever
 * one of the (1-9) 'Preset' items is selected in
 * the menu system.
 */
void menuPresetHandler(const int preset){
  // send instruction to the Monocle gateway client
  // to perform the PTZ preset recall action.
  monocle.preset(preset);
}


/**
 * ------------------------------------------------------------------------
 * PROGRAM MAINLINE (LOOP)
 * ------------------------------------------------------------------------
 */
void loop() {
  // display connecting status on OLED
  display.printText("Connecting to Gateway", MONOCLE_GATEWAY_ADDRESS);

  // let the user know we are going to attempt a connection to the Monocle Gateway
  Serial.println("Connecting to Monocle Gateway");

  // attmept to connect to the Monocle Gateway now
  monocle.begin();

  // let the user know we are connected to the Monocle Gateway
  if (monocle.connected()) {
    Serial.println("Successfully connected to Monocle Gateway.");
    display.printText("Gateway Connected");
    delay(1000);
    displayCameraInfo();
  }

  // continuous loop while we are connected to the Monocle Gateway
  while (monocle.connected()) {

    // we must call the 'loop' function on the Monocle
    // client to service communication and raise events
    monocle.loop();

    // we must call the 'loop' function on the joystick
    // class to service inputs and raise events
    joystick.loop();

    // we must call the 'loop' function on the menu
    // class to service the display and raise events
    menu.loop();
  }

  // let the user know that we are now disconnected from the Monocle Gateway
  Serial.println("Disconnected from Monocle Gateway.");
  Serial.println("We will attempt to reconnect to the Monocle Gateway in 5 seconds.");
  display.printText("Gateway Disconnected", "Reconnecting in 5 sec");
  delay(5000); // wait 5 seconds before attempting to reconnect
}
