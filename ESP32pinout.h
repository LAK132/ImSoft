//SPI 2
const uint8_t HSPICLK = 14;
const uint8_t HSPICS0 = 15;
const uint8_t HSPIMISO = 12;    //D2 for SD mode
const uint8_t HSPIMOSI = 13;    //D3 for SD mode
const uint8_t HSPIHD = 4;       //D1 for SD mode
const uint8_t HSPIWP = 2;       //D0 for SD mode

//SPI 3 (default)
const uint8_t VSPICLK = 18;
const uint8_t VSPICS0 = 5;
const uint8_t VSPIMISO = 19;
const uint8_t VSPIMOSI = 23;
const uint8_t VSPIHD = 21;
const uint8_t VSPIWP = 22;

//Ultra-Low Noise Analog Pre-Amp
const uint8_t SENSORVP = 36;
const uint8_t SENSORVN = 39;

//Analog to Digital Converter (12bit SAR)
const uint8_t ADC1CH4 = 32;
const uint8_t ADC1CH5 = 33;

const uint8_t ADC1CH6 = 34;         //34 to 39 are in only and have no pulldown or pullup
const uint8_t ADC1CH7 = 35;         //https://esp32.com/viewtopic.php?f=12&t=2278 
const uint8_t ADC1CH0 = SENSORVP;
const uint8_t ADC1CH3 = SENSORVN;

const uint8_t ADC2CH0 = HSPIHD;
const uint8_t ADC2CH1 = 0; 
const uint8_t ADC2CH2 = HSPIWP; 
const uint8_t ADC2CH3 = HSPICS0; 
const uint8_t ADC2CH4 = HSPIMOSI; 
const uint8_t ADC2CH5 = HSPIMISO; 
const uint8_t ADC2CH6 = HSPICLK; 
const uint8_t ADC2CH7 = 27; 
const uint8_t ADC2CH8 = 25;
const uint8_t ADC2CH9 = 26;

//Capacitive Touch Sensors
const uint8_t TOUCH0 = ADC2CH0; //HSPIHD
const uint8_t TOUCH1 = ADC2CH1;
const uint8_t TOUCH2 = ADC2CH2; //HSPIWP
const uint8_t TOUCH3 = ADC2CH3; //HSPICS0
const uint8_t TOUCH4 = ADC2CH4; //HSPIMOSI
const uint8_t TOUCH5 = ADC2CH5; //HSPIMISO
const uint8_t TOUCH6 = ADC2CH6; //HSPICLK
const uint8_t TOUCH7 = ADC2CH7;
const uint8_t TOUCH8 = ADC1CH5;
const uint8_t TOUCH9 = ADC1CH4;

//Hardware Serial
const uint8_t TXD0 = 1;     //UART 0 HardwareSerial(0) default
const uint8_t RXD0 = 3;
const uint8_t TXD1 = 10;    //UART 1 HardwareSerial(1)
const uint8_t RXD1 = 9;
const uint8_t TXD2 = 17;    //UART 2 HardwareSerial(2)
const uint8_t RXD2 = 16;

//Digital to Analog Converter (8bit) (already defined in ESP32 files)
// const uint8_t DAC1 = ADC2CH8;
// const uint8_t DAC2 = ADC2CH9;

//Left over IO
const uint8_t LEDPIN = RXD2;
const uint8_t BOOTBTN = ADC2CH1;

//ESP-WROOM-32 onboard SDIO flash memory https://www.sdcard.org/cht/downloads/pls/simplified_specs/archive/partE1_100.pdf
//Must support SPI mode, pinout suggests support for SD 4bit mode, possibly supports SD 1bit mode
                                //  SD 4bit,    SD 1bit,              SPI
const uint8_t FLASHCLK = 6;     //  Clock ---------------------------------------
const uint8_t FLASHCMD = 11;    //  Command Line (CMD),               Data Input
const uint8_t FLASHSD0 = 7;     //  DATA0,      DATA,                 Data Output
const uint8_t FLASHSD1 = 8;     //  DATA1,      Interupt (IRQ)-------------------
const uint8_t FLASHSD2 = 9;     //  DATA2,      Read Wait (optional), NC
const uint8_t FLASHSD3 = 10;    //  DATA3,      NC,                   Card Select

//Usable IO
const uint8_t PIN0 =    BOOTBTN;    //TOUCH1 //ADC2CH1
const uint8_t PIN25 =   DAC1;       //ADC2CH8
const uint8_t PIN26 =   DAC2;       //ADC2CH9
const uint8_t PIN27 =   TOUCH7;     //ADC2CH7
const uint8_t PIN32 =   TOUCH9;     //ADC1CH4
const uint8_t PIN33 =   TOUCH8;     //ADC1CH5
const uint8_t PIN34 =   ADC1CH6;    
const uint8_t PIN35 =   ADC1CH7;    
const uint8_t PIN36 =   SENSORVP;   //ADC1CH0
const uint8_t PIN39 =   SENSORVN;   //ADC1CH3

#ifdef ENABLE_SERIAL_PINOUT
const uint8_t PIN1 =    TXD0;
const uint8_t PIN3 =    RXD0;
const uint8_t PIN16 =   RXD2; 
const uint8_t PIN17 =   TXD2;
#endif

#ifdef ENABLE_FLASH_PINOUT
const uint8_t PIN6 =    FLASHCLK;
const uint8_t PIN7 =    FLASHSD0;
const uint8_t PIN8 =    FLASHSD1;
const uint8_t PIN9 =    FLASHSD2;
const uint8_t PIN10 =   FLASHSD3;
const uint8_t PIN11 =   FLASHCMD;
#endif

#if defined(ENABLE_HSPI_PINOUT) || defined(ENABLE_SPI_PINOUT)
const uint8_t PIN14 =   HSPICLK;    //ADC2CH6
const uint8_t PIN15 =   HSPICS0;    //ADC2CH3
const uint8_t PIN12 =   HSPIMISO;   //ADC2CH5
const uint8_t PIN13 =   HSPIMOSI;   //ADC2CH4
#endif
const uint8_t PIN2 =    HSPIWP;     //ADC2CH2
const uint8_t PIN4 =    HSPIHD;     //ADC2CH0

#if defined(ENABLE_VSPI_PINOUT) || defined(ENABLE_SPI_PINOUT)
const uint8_t PIN18 =   VSPICLK;
const uint8_t PIN5 =    VSPICS0;
const uint8_t PIN19 =   VSPIMISO;
const uint8_t PIN23 =   VSPIMOSI;
#endif
const uint8_t PIN22 =   VSPIWP;
const uint8_t PIN21 =   VSPIHD;