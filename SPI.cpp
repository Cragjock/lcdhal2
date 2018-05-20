#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "SPI.h"


/************************************************************
* Adafruit 74HC595 backpack SPI Latch port looks like:
* +---------+----+----+----+--------------------+
* | 7       | 6  | 5  | 4  | 3  | 2 | 1  | 0  |
* +---------+----+----+----+--------------------+
* | bklight | db4|db5 |db6 |db7 | E | RS | N/A|
* +---------+----+----+----+--------------------+
*    NOTE: DB4-7 are in backwards order, need to flip

// ==================================================
* Adafruit backpack I2C MCP23008 port looks like:
* +---------+-----+----+----+--------------------+
* | GP7     | GP6 |GP5 |GP4 |GP3 |GP2 |GP1 |GP0  |
* +---------+-----+----+----+--------------------+
* | bklight | db7 |db6 |db5 |db4 | E  | RS | N/A|
* +---------+-----+----+----+--------------------+
*
***********************************************************/

static const uint8_t spi_mode = 0;
static const uint8_t spi_bpw = 8; // bits per word
static const uint32_t spi_speed = 10000000; // 10MHz
static const uint16_t spi_delay = 0;
static const char * spidev[2][2] = {
    {"/dev/spidev0.0", "/dev/spidev0.1"},
    {"/dev/spidev1.0", "/dev/spidev1.1"},
};

/// int SPI_open(int bus, int chip_select)
SPIBus::SPIBus(unsigned int bus, unsigned int address) : Transport()
{
    this->ptrfile = address;
    this->i2cbus = bus;
    this->spibus_name = "SPIbus";


    openi2c();

}

int SPIBus:: openi2c()
{
    int fd = this->ptrfile;
    int bus = this->i2cbus;
    int chip_select = this->address;

    if ((fd = open(spidev[bus][chip_select], O_RDWR)) < 0)
        {
            fprintf(stderr,"SPI_open: ERROR Could not open SPI device (%s).\n",spidev[bus][chip_select]);
            return -1;
        }
    if (ioctl(fd, SPI_IOC_WR_MODE, &spi_mode) < 0)
        {
            fprintf(stderr, "SPI_open: ERROR Could not set SPI mode.\n");
            close(fd);
            return -1;
        }
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bpw) < 0)
        {
            fprintf(stderr, "SPI_open: ERROR Could not set SPI bits per word.\n");
            close(fd);
            return -1;
        }
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0)
        {
            fprintf(stderr, "SPI_open: ERROR Could not set SPI speed.\n");
            close(fd);
            return -1;
        }

    return fd;
}

SPIBus::~SPIBus()
{
    closei2c();
}

void SPIBus::closei2c()
{
	::close(this->ptrfile);
	this->ptrfile = -1;
}


int SPIBus::device_read(int reg_address)
{



}


///void SPI_write_reg(uint8_t data, int fd)
int SPIBus::device_write(uint8_t command_reg, uint8_t data)
{
    uint8_t tx_buf[1] = {data};
    uint8_t rx_buf[sizeof tx_buf];

    struct spi_ioc_transfer spi;
    memset (&spi, 0, sizeof(spi));
    spi.tx_buf = (unsigned long) tx_buf;
    spi.rx_buf = (unsigned long) rx_buf;
    spi.len = sizeof tx_buf;
    spi.delay_usecs = spi_delay;
    spi.speed_hz = spi_speed;
    spi.bits_per_word = spi_bpw;

    /***  do the SPI transaction   ***/
    if ((ioctl(fd, SPI_IOC_MESSAGE(1), &spi) < 0))
    {
        fprintf(stderr,
                "SPI_write_reg: There was a error during the SPI "
                "transaction.\n");
    }
    set_latch(data);
    //printf("Latch is set to 0x%x\n",data);
    return 0;

}





uint8_t get_latch(void)
{
    return latched;
}
uint8_t set_latch(uint8_t latch)
{
    latched = latch;
    return latched;

}




/*******************************************************************/
int SPI_open(int bus, int chip_select)
{
    int fd;

    if ((fd = open(spidev[bus][chip_select], O_RDWR)) < 0)
        {
            fprintf(stderr,"SPI_open: ERROR Could not open SPI device (%s).\n",spidev[bus][chip_select]);
            return -1;
        }
    if (ioctl(fd, SPI_IOC_WR_MODE, &spi_mode) < 0)
        {
            fprintf(stderr, "SPI_open: ERROR Could not set SPI mode.\n");
            close(fd);
            return -1;
        }
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bpw) < 0)
        {
            fprintf(stderr, "SPI_open: ERROR Could not set SPI bits per word.\n");
            close(fd);
            return -1;
        }
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0)
        {
            fprintf(stderr, "SPI_open: ERROR Could not set SPI speed.\n");
            close(fd);
            return -1;
        }

    return fd;
}

/*** this will do a read and write *********************************/
void SPI_write_reg(uint8_t data, int fd)
{
    uint8_t tx_buf[1] = {data};
    uint8_t rx_buf[sizeof tx_buf];

    struct spi_ioc_transfer spi;
    memset (&spi, 0, sizeof(spi));
    spi.tx_buf = (unsigned long) tx_buf;
    spi.rx_buf = (unsigned long) rx_buf;
    spi.len = sizeof tx_buf;
    spi.delay_usecs = spi_delay;
    spi.speed_hz = spi_speed;
    spi.bits_per_word = spi_bpw;

    /***  do the SPI transaction   ***/
    if ((ioctl(fd, SPI_IOC_MESSAGE(1), &spi) < 0))
    {
        fprintf(stderr,
                "SPI_write_reg: There was a error during the SPI "
                "transaction.\n");
    }
    set_latch(data);
    //printf("Latch is set to 0x%x\n",data);
}

/***sample : mcp23s17_write_bit(state, PIN_RS, GPIO, hw_addr, mcp23s17_fd);
* data in should be a 1 SET or 0 CLEAR
***/
void SPI_write_bit(uint8_t data, uint8_t bit_num, int fd)
{
    uint8_t reg_data = get_latch(); // should be last latched value

    if (data)
    {
        reg_data |= 1 << bit_num; // set
    } else
    {
        reg_data &= 0xff ^ (1 << bit_num); // clear
    }
    return SPI_write_reg(reg_data, fd); // do it and SPI_write will update latched data
}
