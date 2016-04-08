# gertcomm
Demo of communicating Gertboards

## How it works
This demo utilizes 3 Raspberry Pis, which are connected to a Gertboard Extention board each. The Pis have the following tasks:
* one Pi acts as input-device, using the three buttons of its Gertboard
* one Pi acts as a sender, which generates a LED-pattern, displaying it and sending the pattern to the output
* one Pi acts as output, which receives the pattern from the sender and outputs it

## What is does
The buttons from the input-Pi are transferred over three cables to the sender-Pi. Whenever there is a button pressed (indicated by a LOW-signal over the cable), the sender will switch the mode. There are three modes:
* binary count
* nightrider-mode
* initial mode (switchting the LEDs on, one after the other, then switching them off)

The connection between receiver and sender are two pins, one acting as clock-signal, which toggles for every bit sent. There is no phase-shift implemented between clock and data-pin. A frame begins with 13 LOWs, followed by one HIGH, followed by the twelve data-bits. With this frame-gap it is possible to regain the synchronization even if the clock or data pins are temporarily disconnected. 

## How is may be used
This demo was used at the Chemnitzer Linux Tage 2016 as a showcase. 

Deploy a binary per Raspberry Pi. I have put them in /opt/clt/. Then create a init.d-skript in /etc/init.d, which starts the application and enter the following into /etc/rc.local:

    /etc/init.d/clt start
    
Then the application on this Pi will be started on boot, so there is no display needed. 

## Wiring
Be sure to connect the mass-pins of the three gertboards.

### Input:
* GP17 -- B12
* GP23 -- B3
* GP24 -- B2
* GP25 -- B1

### Sender
* GP9-B12
* GP10 -- B11
* GP11 -- B10
* GP14 -- B09
* GP15 -- B08
* GP17 -- B07
* GP18 -- B06
* GP21 -- B05
* GP22 -- B04
* GP23 -- B03
* GP24 -- B02
* GP25 -- B01

### Output
* GP9-B12
* GP10 -- B11
* GP11 -- B10
* GP14 -- B09
* GP15 -- B08
* GP17 -- B07
* GP18 -- B06
* GP21 -- B05
* GP22 -- B04
* GP23 -- B03
* GP24 -- B02
* GP25 -- B01
 
### Between Input and Sender
* GP0 (input) -- GP4 (sender)
* GP1 -- GP4
* GP4 -- GP8
 
### Between Output and Sender
* GP0 (output) -- GP0 (sender)
* GP1 -- GP1
