
#include "mylcd.h"

using namespace std;
using namespace HAL;


/******************************/
lcddisplay::lcddisplay()
{
	cout << "lcddisplay In constructor" << endl;
	myHAL=new halLCD;
}

lcddisplay::lcddisplay(LCD sLCD)
{
	cout << "lcddisplay In lcd constructor" << endl;
	myHAL=new halLCD;
	this->properties.rows=sLCD.rows;
	this->properties.column=sLCD.column;
	this->properties.is_color=sLCD.is_color;
	this->command_param = nullptr;
	//cout << "rows " << this->properties.rows<<endl;
	//cout << "column " << this->properties.column<<endl;
	//cout << "is_color " << this->properties.is_color<<endl;
}

/******************************/
lcddisplay::~lcddisplay()
{
    myHAL->~halLCD();
}

/******************************/
void lcddisplay::set_color(enum my_color thecolor)
{
    set_color(table[thecolor].cRED, table[thecolor].cGREEN, table[thecolor].cBLUE);
}

/******************************/
void lcddisplay:: set_color(uint8_t red, uint8_t green, uint8_t blue)
{
	myHAL->hal_set_color(red,green,blue);
}

/******************************/
void lcddisplay:: set_color(struct s_myColor theColors)
{
    myHAL->hal_set_color(theColors);
}

/******************************/
bool lcddisplay::load_bmp_bank(uint8_t bank_number)     /// command is 0xfe, 0xc0, bank_number (0-4)
{
    myHAL->hal_load_bmp_bank(bank_number);
	return true;
}

/******************************/
void lcddisplay::set_eeprom_bmp()       /// format to eeprom 0xfe, 0xc1, bankid(0-4), bmap_id(0-7), data 8bytes
{
    myHAL->hal_set_eeprom_bmp();
}

/******************************/
void lcddisplay::set_bmp()
{
    myHAL->hal_set_bmp();
}

/******************************/
void lcddisplay::lcd_set_cursor_address(uint8_t address)
{
    myHAL->halLCD_set_cursor_address(address);
}

/******************************/
void lcddisplay::lcd_clear(void)
{
	myHAL->halLCD_clear();
}

/******************************/
void lcddisplay::lcd_home(void)
{
	myHAL->halLCD_home();
}

/******************************/
bool lcddisplay::write_command(char command, int bsize, const vector<int> buffer)
{
    myHAL->hal_write_command(command, bsize, buffer);
    return true;
}

/******************************/
bool lcddisplay::write_command(char command)
{
    myHAL->hal_write_command(command);
    return true;
}

/******************************/
void lcddisplay::lcd_bitmap(int bmp_id)
{
	myHAL->halLCD_bitmap(bmp_id);
}

/******************************/
void lcddisplay::lcd_usb_splash(string mystring)
{
    myHAL->halLCD_usb_splash(mystring);
}
