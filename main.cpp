#include "mbed.h"
#include "uLCD_4DGL.h"
#include "mbed_events.h"
#include <iostream>
#include <thread> 
#include <ctime> 

using namespace std::chrono; 

Timer t;
Thread thread; 
Thread changeThread;

EventQueue queue(32 * EVENTS_EVENT_SIZE);


uLCD_4DGL uLCD(D1, D0, D2);
InterruptIn pin_up(D5);
InterruptIn pin_down(D3);
InterruptIn pin_sel(D10);
AnalogOut sig(PA_4);  // D7
AnalogIn ain(PC_5);   // A0

float freqlist[4] = {0.125, 0.250, 0.500, 1.000};
float freq = freqlist[0];
int pos = 0;

double T = 1 / freq;
int range = (int)(8000 * freq);
int flat = 24000 - 2 * range;
double amp = 3 / 3.3;
double waitTime[4] = {1,2,3,4};

void U()
{
    if(pos!=3) pos++;
    uLCD.cls();
    uLCD.color(GREEN);
    uLCD.locate(0,0);
    uLCD.printf("%-6.3f\n", freqlist[pos]);
    ThisThread::sleep_for(100ms);
}

void DN()
{
    if(pos != 0) pos--;
    uLCD.cls();
    uLCD.color(GREEN);
    uLCD.locate(0,0);
    uLCD.printf("%-6.3f\n", freqlist[pos]);
    ThisThread::sleep_for(100ms);
}

void ST()
{
    freq = freqlist[pos];
    uLCD.cls();
    uLCD.color(RED);
    uLCD.locate(0,0);
    uLCD.printf("%-6.3f\n", freqlist[pos]);
    ThisThread::sleep_for(100ms);
    range = (int)(8000 * freq);
    flat = 24000 - 2 * range;
}

/*void sample(void)
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
} */

int main()
{
    uLCD.color(RED);
    uLCD.locate(0,0);
    uLCD.printf("%-6.3f\n", freqlist[pos]);
    ThisThread::sleep_for(100ms);

    changeThread.start(callback(&queue, &EventQueue::dispatch_forever));

    pin_up.rise(queue.event(&U));
    pin_down.rise(queue.event(&DN));
    pin_sel.rise(queue.event(&ST));
    
    // start thread for sample
    // thread.start(sample);
    while(1)
    {
        for(int i = 0; i < range; i++)
        {
            sig = i/(float)range*amp;
            wait_us(10);
        }
        for(int count = 0; count < flat; count++)
        {
            sig = range/(float)range*amp;
            wait_us(10);
        }
        for(int i = range; i > 0; i--)
        {
            sig = i/(float)range*amp;
            wait_us(10);
        }
    }
}
