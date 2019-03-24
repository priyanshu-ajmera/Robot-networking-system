//Complete code for Transmit and Receive from 2 Lego using 2 XBee

#include <XBee.h>
#include <time.h>
#include <stdlib.h>
#include <Wire.h>
#include <Servo.h>

XBee xbee = XBee();
XBeeAddress64 ROUTER1 = XBeeAddress64(0x0013A200,0x4147FE2A);
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
XBeeAddress64 ROUTER5 = XBeeAddress64(0x0013A200,0x414E65AE);

uint8_t data[4]={1,0,0,0};
byte read_byte = 0x00;
int byte_count = 0;
uint8_t count = 0;
uint8_t receivedData[4];
byte* dataPointer;
uint8_t dist = 0;

void setup()
{
  Serial.begin(9600);           // We will spit it back out on the serial line.
  Wire.begin(0x0A);             // Start I2C on Address 0x0A
  xbee.setSerial(Serial);
  Serial.println("StartingUp!");
  
}

void loop()
{
  Wire.onReceive(receiveI2C);   // Receive Event from Master
  delay(180);
  transmitXBee();
  delay(50);
  receiveXBee();
  delay(180);
  Wire.onRequest(requestEvent); // Sending information back to the NXT! 
}

// When data is received, this function is called.
void receiveI2C(int bytesIn)
{
  while(Wire.available()) // loop through all but the last
  {
    read_byte = Wire.read();     // Receive the incoming byte
    data[byte_count] = read_byte;
    byte_count++;
    Serial.print(read_byte);          // print the incoming byte as a character on the Serial line.
  }
  Serial.println();          // print the incoming byte
  //Serial.print("Data from I2C: ");          // print the incoming byte
  /*for(uint8_t j=0;j<byte_count;j++)
  {
    Serial.print(data[j]);
  }*/
  Serial.println();
  //Serial.println("TEST");
  byte_count = '\0';
}

void transmitXBee()
{
  
  ZBTxRequest zbTx5 = ZBTxRequest(ROUTER5, data, sizeof(data));
  //Serial.println("Start Transmitting");
  xbee.send(zbTx5);
  Serial.println();
  //Serial.println("To Router 5");
}

void receiveXBee()
{
  xbee.readPacket(200);
  //Serial.println("Reading for Packet");
  if(xbee.getResponse().isAvailable())
  {
    //Serial.print("Packet Available: ");
    //Serial.println(count);
    //delay(500);
    if(xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
    {
      xbee.getResponse().getZBRxResponse(rx);
      //delay(500);
      /*
       if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
            // the sender got an ACK
            Serial.println("got something");
        } else {
            // we got it (obviously) but sender didn't get an ACK
            Serial.println("got nothing");
        }
      */
      dataPointer = reinterpret_cast<byte *>(receivedData);
      for(int n =0; n < rx.getDataLength(); n++)
      {
        *dataPointer = rx.getData(n);
        dataPointer += 1;
      }
      //Serial.print("Packet Received: ");
        for (byte n = 0; n < 4; n++) 
        {
          Serial.print(receivedData[n]);
        }
      Serial.println();
      //Serial.print("count: ");
      //count++;
      //Serial.println(count);
      //delay(500);
      dist = sqrt((data[1] - receivedData[1])^2 + (data[3] - receivedData[3])^2);
      Serial.println(dist);
    }
    //else
    //Serial.println("checking packet");// added
  }
  else if(xbee.getResponse().isError())
  {
    Serial.print("Error Reading Packet. Error Code: ");
    Serial.println(xbee.getResponse().getErrorCode());
    //delay(500);
  }
  else
    Serial.println("No data received"); 
}

// Based on the read_byte value we will return either Dexter or Indusries.
// This can be expanded to have 256 different registers that can be updated and accesed
// when the master calls for them.

void requestEvent()
{
  // We’re going to send a fixed response. However,
  // you can use a switch case here and send different responses
  // based on a register system.
  if(receivedData != '\0')
  {
   /* for(byte n = 0; n < 4; n++)
    {
      Wire.write(receivedData[n]); // respond with message of 10 bytes
    }*/
    Wire.write(dist);
  }
  else
  {
    Wire.write(","); // respond with message of 10 bytes
  } // as expected by master
}