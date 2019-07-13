# Smartarm

Remote bomb receptor and defuser with various (harmless) electrical components.

## Languages / Frameworks used

* C (Embedded)

## Purpose / Goals

* Develop a mobile app in MIT App Inventor that allows the user to arm the bomb via communication using the HC-05 Bluetooth Module
* Include visual and audial feedback on the app that shows if the bomb was armed, if it was defused, and if it has detonated
* Implement USART on the Atmega1284 microcontrollers that allow them to both communicate with each other, and with the HC-05

* Create a bomb on a breadboard using harmless EE components including:
```
2 x Atmega1284
1x LCD Display (HD44780)
1x Keypad
1x 7 Segment Display (IS132)
3 x LED
1 x Hobby Motor (ROB-11696)
```

* Allow for defusal of the bomb using the keypad connected to the the Atmega1284's and a specific keycode

### Installation

* Mac OS X
* Ubuntu
* Windows (if applicable)
* [Ateml Studio](https://www.microchip.com/mplab/avr-support/atmel-studio-7)
* [Pinout in Report](https://docs.google.com/document/d/1WL5iKJL9xsOxJTR1RYp9X0D2OAbXhJGG5hFqgCO2c2k/edit#)

### Getting started

![Flowchart](https://user-images.githubusercontent.com/14877762/61165324-bf891e80-a4d3-11e9-8f38-11b14fd5cc3f.png)


## Usage

One area I wanted to explore with the software and hardware of this project is the implementation of a lock system. Instead of using the app to arm the bomb, it could be used to arm a lock (e.g. on a door).

## Demo

![Pinout](https://user-images.githubusercontent.com/14877762/61165424-3246c980-a4d5-11e9-89b3-d6a8e1db1aef.png)

![Image](https://user-images.githubusercontent.com/14877762/61165427-4094e580-a4d5-11e9-9bfe-b4dea6b91e2b.png)


## Team

* Adrian Tran

## Report

[Google Doc](https://docs.google.com/document/d/1WL5iKJL9xsOxJTR1RYp9X0D2OAbXhJGG5hFqgCO2c2k/edit#)

## Errors and bugs

If something is not behaving intuitively, it is a bug and should be reported.
Report it here by creating an issue: https://github.com/adrianmoo2/SmartArm/issues

Help us fix the problem as quickly as possible by following [Mozilla's guidelines for reporting bugs.](https://developer.mozilla.org/en-US/docs/Mozilla/QA/Bug_writing_guidelines#General_Outline_of_a_Bug_Report)

## Patches and pull requests

Your patches are welcome. Here's our suggested workflow:
 
* Fork the project.
* Make your feature addition or bug fix.
* Send us a pull request with a description of your work. Bonus points for topic branches!

## Copyright and attribution

Copyright (c) 2019 Adrian Tran. Released under the [MIT License](https://github.com/adrianmoo2/SmartArm/blob/master/LICENSE).
