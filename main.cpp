#include <iostream>
#include "mylcd.h"
#include <chrono>		//chrono::milliseconds(1000); need scope chrono
#include <thread>		// for chrono sleep this_thread::sleep_for(chrono::seconds(1));


using namespace std;

    string sdceng = {0x53,0x74,0x65,0x76,0x65,0x6e};
    //const char sdceng[] = {0x53,0x74,0x65,0x76,0x65,0x6e};        // ok
    //char sdceng[] = {0x53,0x74,0x65,0x76,0x65,0x6e};        // ok
    //const char sdcjp[]={0xbd,0xc3,0xa8,0xb0,0xcc,0xde,0xdd};      // ok
    string japan = {0xbd,0xc3,0xa8,0xb0,0xcc,0xde,0xdd};

    // uint8_t sdceng[] ={0x53,0x74,0x65,0x76,0x65,0x6e};       // no good
    const char alpha[] ={"123456789a123456789b123456789c123456789d123456789e123456789f123456789g123456789h"};

int main()
{
    LCD LI2C{4,20,false};
    lcddisplay myLCD(LI2C);

    myLCD.lcd_set_cursor_address(0);
    myLCD.lcd_write("hello world");

	myLCD.lcd_clear();
	myLCD<<"Is this working?";
	myLCD.lcd_set_cursor_address(0x40);
	myLCD.lcd_write(japan);
	myLCD.lcd_set_cursor_address(0x14);
	myLCD.lcd_write(sdceng);
	this_thread::sleep_for(chrono::seconds(1));

	myLCD.lcd_set_cursor_address(0x54);
	myLCD.lcd_write("yo YO yo");    /// const char*
	this_thread::sleep_for(chrono::seconds(1));

	string bmptest = {static_cast<char>(3)};

	myLCD.lcd_write(bmptest);           /// string

	this_thread::sleep_for(chrono::seconds(1));
	myLCD.lcd_set_cursor_address(0);
	char buf[80];
    sprintf(buf, "%c====== %c =======%c",LeftBM, MiddleBM,RightBM);
    myLCD.lcd_write(buf);
    this_thread::sleep_for(chrono::seconds(1));

    char bmp = SatLeftBM;
    char bmp1 = SatRightBM;
    char* steve = " steve";
    string clark = "clark";

    myLCD.lcd_clear();
    myLCD.display(SatLeftBM);
    myLCD.display(bmp1);
    myLCD.display(steve);
    myLCD.display(clark);
    this_thread::sleep_for(chrono::seconds(1));

    myLCD.lcd_clear();
	myLCD<<"insertor check\n";
	myLCD<<bmp<<bmp1<<"\n";
	myLCD<<SatLeftBM<<SatRightBM;
	this_thread::sleep_for(chrono::seconds(1));
	myLCD.lcd_clear();
	myLCD.lcd_write(alpha);
	this_thread::sleep_for(chrono::seconds(1));

    return 0;
}


