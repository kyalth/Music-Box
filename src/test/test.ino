//Test space

//Includes
#include <Adafruit_MCP4725.h> //DAC library

#include <Wire.h>  //I2C library

//#include <SPI.h>

#include <SD.h>  //Library to use SD card breakout

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

Adafruit_MCP4725 DAC;

//SD card pin definitons
#define CS  10    //chip select
#define DI  11    //MOSI
#define DO  12    //MISO
#define CLK  13   //SPI clock

//I2C DAC pin definitions
#define SDA  A4   //I2C Data line
#define SCL  A5   //I2C Clock line

//WAV reading definitions
#define BUFFERSIZE  256  //size of buffers used to read wav file in chucks

//This struct will hold information related to the WAV file that's being read.
typedef struct 
{
  int format;
  int sample_rate;
  int bits_per_sample;
}wave_format;
wave_format wave_info;

volatile unsigned char note=0;		//Holds the current voltage value to be sent to the AD5330.

unsigned char header[44];			//Holds the WAV file header
unsigned char buffer1[BUFFERSIZE], buffer2[BUFFERSIZE];	//Two cycling buffers which contain the WAV data.
char file_name[30];					//WAV file name.

char play_buffer=0;					//Keeps track of which buffer is currently being used
char new_buffer_ready=0;			//Flag used by 'Loop' code to tell the Interrupt that new data is ready in the buffer.
volatile unsigned int byte_count=0;	//Keeps track of the number of bytes read from the current buffer.
volatile char need_new_data=0;		//Flag used by Interrupt to tell 'Loop' code that a buffer is empty and needs to be refilled.

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("DAC...");
  DAC.begin(0x62);
  DAC.setVoltage((uint16_t)0,false);
  Serial.println("+");

  Serial.print("SD ...");
  pinMode(10, OUTPUT);     // change this to 53 on a mega

  if (!card.init(SPI_HALF_SPEED, CS)) {
    Serial.println("*");
    return;
  } else {
   Serial.println("+"); 
  }


  if (!volume.init(card)) {
    Serial.println("Partition...*");
    return;
  }


  
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}
/*
ISR (TIMER1_COMPA_vect)
{
	cli();
  //Check to see if we've read all of the data in the current buffer
  if(byte_count==BUFFERSIZE)
  {
    need_new_data=1;	//Set a flag to tell the 'loop' code to refill the current buffer.
    byte_count=0;		//Reset the count
	//Check to see if new data exists in the alternate buffer
	if(new_buffer_ready==1)
    {
	  //If new data is available, reassign the play buffer.
      if(play_buffer==0)play_buffer=1;
      else play_buffer=0;
    }
    else
    {
	  //If no new data is available then wait for it!
	sei();
	return;
}
}	

  //Find out which buffer is being used, and get data from it.
  if(play_buffer==0)note=buffer1[byte_count];
  else note=buffer2[byte_count];
  
  //Increase the byte_count since we've taken the current data.
  byte_count +=1;
 
  DAC.setVoltage((uint16_t)note,false);
sei();
}
*/
void loop()
{
 /* TCCR1A =0;
  TCCR1B =0x0A;
  TIMSK1 =0x02;
  OCR1AH =0x00;
  OCR1AL =0x5A;
*/
  DAC.setVoltage((uint16_t)10,false);
  //Serial.println("Sound");
  delayMicroseconds(2272);
  DAC.setVoltage((uint16_t)0,false);
  
}
