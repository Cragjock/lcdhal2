
#ifndef _MCP23S17_H
#define _MCP23S17_H
#include <stdint.h>

#define WRITE_CMD 0
#define READ_CMD 1

#include "transport.h"      /// for base class


/*****************************************************
    Set-up to send data to the 74HC595 shift register
    on the I2CSPIbackpack from Adafruit using SPI
*****************************************************/
class Transport
{
    private:
        std::string transport_name;

    public:
        Transport();
        Transport(std::string t_name){transport_name=t_name;};
        inline std::string get_name() {return this->transport_name;};
        virtual ~Transport();

        virtual int device_write_swap(uint8_t command_reg, uint16_t data)=0;
        virtual int16_t device_read_swap(uint8_t command)=0;

        virtual int device_read_block(int reg_request, int rd_size, unsigned char* readbuffer)=0;
        virtual int device_write_block(int reg_request, int rd_size, unsigned char* readbuffer)=0;
        virtual int device_read(int reg_address)=0;
        virtual int device_write(uint8_t command_reg, uint8_t data)=0;
};

class SPIBus : public Transport
{
    protected:
        unsigned int i2cbus;
        unsigned int i2caddress;
        int ptrfile;
        char busfile[64];
        std::string spibus_name;

    public:
        SPIBus(unsigned int bus, unsigned int address);
        virtual ~SPIBus();

        /**
        * Returns a file descriptor for the SPI device through which the
        * MCP23S17 port expander can be accessed.
        ********************************************/
        virtual int SPI_open(int bus, int chip_select);



        virtual void closei2c();


        int32_t myI2C_read_byte(int file);
        int myI2C_write_data(int file, uint8_t command_reg, uint8_t data);
        int myI2C_write_byte(int file, uint8_t data);
        int32_t myI2C_read_data(int file, uint8_t command);

        int device_write_swap(uint8_t command_reg, uint16_t data);
        int16_t device_read_swap(uint8_t command);


        virtual int device_read(int reg_address);
        virtual int device_write(uint8_t command_reg, uint8_t data);



        /**
        * Writes an 8 bit value to the 595 latch.
        * data The data byte to be written.
        * fd The file descriptor for SPI
        */
        void SPI_write_reg(uint8_t data, int fd);

        /**
        * Writes a single bit to the register specified. Must also specify
        * which hardware address and file descriptor to use.
        * data The data to write.
        * fd The file descriptor for SPI
        */
        void SPI_write_bit(uint8_t data, uint8_t bit_num, int fd);
};

#endif // _MCP23S17_H
