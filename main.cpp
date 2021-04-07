#include "mbed.h"
#include "uLCD_4DGL.h"
#include <iostream>
#include <thread> 
#include <ctime> 

using namespace std::chrono; 

Timer t;
Thread thread; 

uLCD_4DGL uLCD(D1, D0, D2);
DigitalIn pin_up(D5);
DigitalIn pin_down(D3);
DigitalIn pin_sel(D10);
AnalogOut sig(PA_4);  // D7
AnalogIn ain(PC_5);   // A0

float freqlist[4] = {0.125, 0.250, 0.500, 1.000};
float ADCdata[500]; // record sample data

void sample(void)
{
    for(int i = 0; i < 500; i++)
    {
        ADCdata[i] = ain;
        ThisThread::sleep_for(2000ms/500);
    }
    for(int i = 0; i < 500; i++)
    {
        printf("%f\r\n", ADCdata[i]);
    }
}

int main()
{
    float freq = freqlist[0];
    int pos = 0;

    while(1)
    {
        if(pin_up.read() && pos!=3) pos++;
        else if(pin_down.read() && pos!=0) pos--;
        else if(pin_sel.read())
        {
            freq = freqlist[pos];
            break;
        }
        uLCD.locate(0,0);
        uLCD.printf("%-6.3f\n", freqlist[pos]);
        ThisThread::sleep_for(100ms);
    }
    uLCD.locate(0,1);
    uLCD.printf("SELECT %.3f Hz\n", freq);

    // setting
    double T = 1 / freq;
    double amp = 3 / 3.3;
    double uptime = T / 10;
    double downtime = T / 10 * 9;
    
    // start thread for sample
    thread.start(sample);

    // main loop
    while(1)
    {
        for(float i = 0; i < amp; i += amp / (uptime/0.0000237))
        {
            sig = i;
        }
        for(float i = amp; i > 0; i -= amp / (downtime/0.0000237))
        {
            sig = i;
        }
    }
    thread.join();
}
/*
    int while_add = 0;
    float T = 240.0f;
    int j = 1;
    float i = 0.0f;
    char store = 0;

    uLCD.printf("\nPlease Select the Frequency\n");
    uLCD.color(RED);
    uLCD.locate(0, 6);
    uLCD.printf("%5d Hz", freq);

    while(store == 0) {
        if (pin_up == 1) {
            if (freq > 1 && freq <= 8) freq /= 2;
            if (freq == 1) {
                uLCD.locate(0, 6);
                uLCD.printf("%5d", freq);
            } else {
                uLCD.locate(0, 6);
                uLCD.printf("1/%5d", freq);
            }  
        }
        if (pin_down == 1) {
            if (freq >= 1 && freq < 8) freq *= 2;
            if (freq == 1) {
                uLCD.locate(0, 6);
                uLCD.printf("%5d", freq);
            } else {
                uLCD.locate(0, 6);
                uLCD.printf("1/%5d", freq);
            }  
        }
        if (pin_sel == 1) {
            uLCD.locate(0, 6);
            uLCD.printf("\nConfirm.");
            //T = 1000.0 / freq;
            store = 1;
        }
    }

    while (store == 1){
        if (freq == 1) {
            if (i <= 80) sig = i / 80.0f / 1.1f;
            if ((i > 80) && (i <= 160)) sig = 1 / 1.1f;
            if ((i > 160) && (i <= 240)) sig = (240 - i) / 80.0f / 1.1f; 
            if (i > 240) i = 0.0f;
            wait_us(T * 10 - 13);    //
        } else if (freq == 2) {
            if (i <= 40) sig = i / 40.0f / 1.1f;
            if ((i > 40) && (i <= 200)) sig = 1 / 1.1f;
            if ((i > 200) && (i <= 240)) sig = (240 - i) / 40.0f / 1.1f; 
            if (i > 240) i = 0.0f;
            wait_us(T * 10 - 13);    //
        } else if (freq == 4) {
            if (i <= 20) sig = i / 20.0f / 1.1f;
            if ((i > 20) && (i <= 220)) sig = 1 / 1.1f;
            if ((i > 220) && (i <= 240)) sig = (240 - i) / 20.0f / 1.1f; 
            if (i > 240) i = 0.0f;
            wait_us(T * 10 - 13);    //
        } else {
            if (i <= 10) sig = i / 10.0f / 1.1f;
            if ((i > 10) && (i <= 230)) sig = 1 / 1.1f;
            if ((i > 230) && (i <= 240)) sig = (240 - i) / 10.0f / 1.1f; 
            if (i > 240) i = 0.0f;
            wait_us(T * 10 - 13);    //
        }  
        
        //if (store == 1) {
            while_add++;
            if (j == 500) {
               for(int k = 0; k < 500; k++) {
                    printf("%lf\r\n", ADCdata[k]);
                }
                j = 1;
                store = 0;
                while_add = 0;
            }  
            else if (while_add % (100 * freq / 500) == 0) {
               ADCdata[j - 1] = ain;
               j++;
            }         
        //}
        i++;
    }
} */