![1](https://user-images.githubusercontent.com/79573609/113851201-d7d3bd80-97cd-11eb-9380-ff4095cddaeb.JPG)
![2](https://user-images.githubusercontent.com/79573609/113851209-d86c5400-97cd-11eb-93d3-5873ef0506df.JPG)
Running & compile

Embedding in B_L4S5I_IOT01A

sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM -f

To check the output of ADC by screen

sudo screen /dev/ttyACM0

Run python FFT.py to analysis the wave

sudo python3 FFT.py

press three buttom to select frequency.

1.JPG : choose0.5

2.JPG : choose1.0
