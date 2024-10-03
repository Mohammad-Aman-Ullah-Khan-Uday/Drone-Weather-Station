#include <avr/io.h>
#include <util/delay.h>


#define Temp_h 0x41
#define Temp_l 0x42
#define config 0x1A
#define smprt 0x19
#define pwr_man 0x6B

#define START 0x08       
#define REP_START 0x10          
#define SLA_R 0xD1       
#define SLA_W 0xD0

int16_t  temp = 0, desired_temp = 0;
unsigned char data = 0;
bool err = false;

void  error(){
// { Serial.println(TWSR, HEX);
  err = true;
  TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);  
  Serial.println("error");
}

void i2c_start(void)
{
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0);
  //  Serial.println(TWSR, HEX);//11
  if ((TWSR & 0xF8) != START) error();
  // Serial.println("start");
}

void i2c_rep_start(void)
{
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0);
  //  Serial.println(TWSR, HEX);//0x10
  if ((TWSR & 0xF0) != REP_START) error();
  // Serial.println("repeated start");
}

void mpu_write_add(unsigned char slave_add)
{
  TWDR = slave_add; // SLA+W
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0);
  //  Serial.println(TWSR, HEX);
  if ((TWSR & 0xF8) != 0x18 ) error();
  delay(50);
  // Serial.println("mpu add + write");
}

void mpu_read_add(unsigned char slave_add)
{
  TWDR = slave_add; // SLA+W
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0);
  //  Serial.println(TWSR, HEX);
  if ((TWSR & 0xF0) != 0x40 ) error();
  delay(50);
  // Serial.println("mpu add + read");
}

void rgst_write_add(unsigned char rgstr)
{
  TWDR = rgstr; // Register address
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0);
  //  Serial.println(TWSR, HEX); //0x20
  if ((TWSR & 0xF0) != 0x20) error();
  delay(50);
    // Serial.println("write register add");
}

void rgst_write_data(unsigned char value)
{
  TWDR = value; // Register value
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0);
  //  Serial.println(TWSR, HEX); //0x20
  if ((TWSR & 0xF8) != 0x28) error();
  delay(50);
    Serial.println("write register data");
}


int burst_read() {

  TWCR |= (1 << TWINT) | (1 << TWEN) | (1<<TWEA);
  while ((TWCR & (1 << TWINT)) == 0);
    // Serial.println(TWSR, HEX);
  if ((TWSR & 0xF8) != 0x50)error();
    // Serial.println("burst_read");
     delay(100);
  return TWDR; 
}

int single_read() {

  TWCR |= (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0);
    // Serial.println(TWSR, HEX);
  if ((TWSR & 0xF8) != 0x58)error();
    // Serial.println("single data");
     delay(100);
     return TWDR; 
}


void i2c_stop(void)
{
  TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
  // Serial.println("stop");
}

void i2c_init(void)
{
  TWSR |= 0x00;  //TWPS = 00
  TWBR = 72;
  TWCR |= (1 << TWEN);
  // Serial.println("init");
}

int register_read(uint8_t reg_add){
  i2c_start();  
  mpu_write_add(SLA_W);  
  rgst_write_add(reg_add);
  i2c_rep_start();  
  mpu_read_add(SLA_R); 
  data = single_read();
  i2c_stop();
     delay(100);
  return data;
}

int register_write(uint8_t reg_add, uint8_t value){
  i2c_start();  
  mpu_write_add(SLA_W);  
  rgst_write_add(reg_add); 
  rgst_write_data(value);
  i2c_stop();
     delay(100);
}

int main(void)
{
  init();
  Serial.begin(115200);
  i2c_init();
  register_write(config,0x00); 
  register_write(pwr_man,0x00);
  register_write(smprt,0x00);

   while(1){       
     
     temp = register_read(Temp_l);
     temp = temp|(register_read(Temp_h)<<8);    
     temp = (temp/340)+36.53;
     Serial.print("actual temp:");
     Serial.print(temp);
     
     if(Serial.available()){
        desired_temp = Serial.parseInt();
      }
      Serial.print("  ");
      Serial.print("desired temp:");  
        
      Serial.print(desired_temp);
      if(desired_temp == 0){
         Serial.print("  ");  
         Serial.println("enter temp:");  
      }
      else if (temp < desired_temp){
         Serial.print("  ");  
        Serial.println("heater on");
      }
      else if(temp > desired_temp){
               Serial.print("  ");  
              Serial.println("fan on"); 
      }
      else{
        Serial.print("  "); 
        Serial.println("temp reached");

      }
     
      
    }   
   
  
}