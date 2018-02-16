//SPI 2
const uint8_t HSPICLK = 14;
const uint8_t HSPICS0 = 15;
const uint8_t HSPIMISO = 12;
const uint8_t HSPIMOSI = 13;
const uint8_t HSPIHD = 4;
const uint8_t HSPIWP = 2;

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
const uint8_t ADC1CH0 = SENSORVP;
const uint8_t ADC1CH2 = SENSORVN;
const uint8_t ADC1CH4 = 32; 
const uint8_t ADC1CH5 = 33; 
const uint8_t ADC1CH6 = 34;
const uint8_t ADC1CH7 = 35;
const uint8_t ADC2CH0 = 4;
const uint8_t ADC2CH1 = 0; 
const uint8_t ADC2CH2 = 2; 
const uint8_t ADC2CH3 = 15; 
const uint8_t ADC2CH4 = 13; 
const uint8_t ADC2CH5 = 12; 
const uint8_t ADC2CH6 = 14; 
const uint8_t ADC2CH7 = 27; 
const uint8_t ADC2CH8 = 25;
const uint8_t ADC2CH9 = 26;

//Capacitive Touch Sensors
const uint8_t TOUCH0 = ADC2CH1;
const uint8_t TOUCH1 = ADC2CH2;
const uint8_t TOUCH2 = ADC2CH3;
const uint8_t TOUCH3 = ADC2CH4;
const uint8_t TOUCH4 = ADC2CH5;
const uint8_t TOUCH5 = ADC2CH6;
const uint8_t TOUCH6 = ADC2CH7;
const uint8_t TOUCH8 = ADC1CH5;
const uint8_t TOUCH9 = ADC1CH4;

//ESP-WROOM-32 onboard SDIO flash memory https://www.sdcard.org/cht/downloads/pls/simplified_specs/archive/partE1_100.pdf
//Must support SPI mode, pinout suggests support for SD 4bit mode, possibly supports SD 1bit mode
                            //  SD 4bit,    SD 1bit,              SPI
const uint8_t FLASHCLK = 6;     //  Clock ---------------------------------------
const uint8_t FLASHCMD = 11;    //  Command Line (CMD),               Data Input
const uint8_t FLASHDATA0 = 7;   //  DATA0,      DATA,                 Data Output
const uint8_t FLASHDATA1 = 8;   //  DATA1,      Interupt (IRQ)-------------------
const uint8_t FLASHDATA2 = 9;   //  DATA2,      Read Wait (optional), NC
const uint8_t FLASHDATA3 = 10;  //  DATA3,      NC,                   Card Select

//Digital to Analog Converter (8bit)
//const uint8_t DAC1 = 25;
//onst uint8_t DAC2 = 26;

const uint8_t BOOTBTN = ADC2CH1;
const uint8_t LEDPIN = 16;