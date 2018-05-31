#include <iostream>
#include "mylcd.h"
#include <chrono>		//chrono::milliseconds(1000); need scope chrono
#include <thread>		// for chrono sleep this_thread::sleep_for(chrono::seconds(1));
#include "pitime.h"

using namespace std;

    // NIST
    char NL = '\n';
    char EL = '\0';
    int k=0;
    char str_filenm[33];

    int (*device_open)(void);
    //char* (*alt_pitime)();
    int (*alt_pitime)(char*);
    void get_NIST(char*);
    // end NIST


    string sdceng = {0x53,0x74,0x65,0x76,0x65,0x6e};
    //const char sdceng[] = {0x53,0x74,0x65,0x76,0x65,0x6e};        // ok
    //char sdceng[] = {0x53,0x74,0x65,0x76,0x65,0x6e};        // ok
    //const char sdcjp[]={0xbd,0xc3,0xa8,0xb0,0xcc,0xde,0xdd};      // ok
    string japan = {0xbd,0xc3,0xa8,0xb0,0xcc,0xde,0xdd};

    // uint8_t sdceng[] ={0x53,0x74,0x65,0x76,0x65,0x6e};       // no good
    const char alpha[] ={"123456789a123456789b123456789c123456789d123456789e123456789f123456789g123456789h"};
    char buf[80];

int main()      /// I2C version
{

    LCD LI2C{4,20,false};
    lcddisplay myLCD(LI2C);
    this_thread::sleep_for(chrono::seconds(1));

     /// NIST
    vector<char>vNISTbuffer (100,0);
    char* NISTbuffer;
	NISTbuffer = new char[100]();       // () sets to all zeros
    ///int size_test = buf_pitime(NISTbuffer);
    int size_test = buf_pitime(&vNISTbuffer[0]);

    cout <<"===return size is: "<<size_test<<endl;
    if(size_test == 0)
    {
        cout<<"NIST no data,\nsize is "<<endl;
        myLCD.lcd_write("NIST no data,\nsize is ");
        myLCD<<size_test;
        this_thread::sleep_for(chrono::seconds(1));
        return 0;
    }

    /// vector<char> vecnist(100,0);
	/// strcpy(&vecnist[0], NISTbuffer);	/// STD reference page 278
	/// vecnist.shrink_to_fit();
	vNISTbuffer.resize(size_test);
	vNISTbuffer.shrink_to_fit();

    // string mynist(NISTbuffer);
    string mynist(vNISTbuffer.begin(), vNISTbuffer.end());
    cout << "The size of vNISTbuffer is " << vNISTbuffer.size() << " bytes.\n";

	string line_1 = mynist.substr(7, 8);
	string line_2 = mynist.substr(16, 8);

	size_t found;
	cout << line_1 << endl;
	cout << line_2 << endl;
	found = mynist.find("UTC");
	string line_3 = mynist.substr(found, 9);
	cout << line_3 << endl;
	cout << mynist << endl;

    myLCD.lcd_clear();
 	myLCD.lcd_set_cursor_address(0x00);
	myLCD.lcd_write(line_3.c_str());
	myLCD.lcd_write("Time");
	myLCD.lcd_set_cursor_address(0x40);
	myLCD.lcd_write(line_1.c_str());
	myLCD.lcd_set_cursor_address(0x14);
	myLCD.lcd_write(line_2.c_str());
	myLCD.lcd_set_cursor_address(0x54);
    sprintf(buf, "%c====== %c =======%c\n",LeftBM, MiddleBM,RightBM);
    myLCD.lcd_write(buf);
    this_thread::sleep_for(chrono::seconds(2));
    /// end NIST


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

    myLCD.lcd_clear();
    myLCD.lcd_set_cursor_address(0);
	myLCD<<"insertor check 2\n";
	myLCD<<SatLeftBM<<SatRightBM<<LeftBM<<MiddleBM<<RightBM<<HourGlassEmptyBM<<HourGlassFillingBM<<HourGlassFullBM<<"\n";

	this_thread::sleep_for(chrono::seconds(2));


    myLCD.lcd_clear();
    myLCD.lcd_set_cursor_address(0);
	myLCD<<"load bitmaps 1";
	myLCD.load_bmp_bank(1);
	myLCD.lcd_set_cursor_address(0x40);
	myLCD<<ToRightBM<<ToLeftBM
        <<ToUpBM<<ToDownBM
        <<PacOpenBM<<PacClosedBM<<HandBM<<CheckBM;
	this_thread::sleep_for(chrono::seconds(1));
	myLCD.load_bmp_bank(0);
	myLCD.lcd_set_cursor_address(0x54);
	myLCD<<SatLeftBM<<SatRightBM
        <<LeftBM<<MiddleBM<<RightBM
        <<HourGlassEmptyBM<<HourGlassFillingBM<<HourGlassFullBM;
	this_thread::sleep_for(chrono::seconds(1));

    return 0;
}


