# Hardware Notes

While this is simply a demonstration project, in theory this could be hooked up to control a real HVAC system.

We assume the HVAC system is controlled by three control lines: a heater element (such as a gas burner or an electric heating element), a cooling element (such as an air conditioner/compressor system), and a fan. The thermostat attempts to control this (theoretical) HVAC system using these three control lines.

## Software changes to connect to hardware

If you are interested in using this software to actually control a thermostat, first, **DON'T**, unless you know damned good and well what you're doing. Many modern HVAC systems do not work the way this thermostat system models an HVAC system. Further, you can blow up your HVAC system sky-high. 

And remember the license: I do not warrant any of this will work, which means you're on your own.

Second, if you are actually interested in making this work, you will need to make a few modifications to the software.

In the file `AdaThermostat.cpp`, you will need to make the following changes:

**Hook up a temperature sensor.** Around line 105 of the file, the current temperature field should be set to the actual temperature of the unit. This means you will need to find a temperature sensor and hook it up to the Arduino, and read the temperature in degrees. (At present, the code simply runs the temperature up and down according to the amount of elapsed time the unit has been on.)

**Turn on and off Arduino control lines** for your heater, air conditioner, and fan systems around line 195. The fields you need to use is `heat` to turn on a heating unit, `cool` to turn on the cooling system, and `fan` to turn on the fan.

Those control lines will then need to be hooked up to appropriate hardware to actually control your HVAC system. That discussion is beyond the scope of this article.

## Control Constants

There are a number of control constants stored in `AdaThermostat.h` which you may wish to adjust. Those constants are:

**MINCYCLE** prevents 'fast cycling' of the heater or air conditioner elements by setting an an amount of time that must elapse before the heater or air conditioner will be turned on or off. In other words, if the heater was just turned on, the unit will wait `MINCYCLE` time before turning the heater back off. 

**TEMPWINDOW** is the number of degrees above or below our target temperature before we turn on the air conditioner or heater. For example, if the heat is set to 68°, the temperature must fall to 68° - TEMPWINDOW before the heater actually turns on.

**FANRUNWINDOW** is the number of seconds the fan will continue to run after the heater or air conditioner has been turned off. The theory is that the heating element or air conditioning elements still contain excess heat or coolness--and they should be brought back to room temperature to prevent the unit from overheating. This also extracts the maximum amount of heat or cooling out of the unit.

## Bonus Points.

On power-up the unit starts with the internal clock and calendar set to midnight on January 1, 2017. The clock code is maintained in `AdaTime.cpp`, and uses an internal timer clock to maintain a real-time clock. Ideally this would be wired up to an external battery-backed real-time clock or time source.

# Licensing

Remember that all this software is licensed under the GNU GPL. This means if you intend to use this code in a commercial application, you must make the software available to anyone who purchases your air conditioner unit--and ideally you should also allow users to tinker with the thermostat software, by not disabling the programming pins on an Arduino.

If you are interested in licensing this code for commercial purposes and do not wish to provide the software or allow users to reprogram their thermostats, you can obtain a commercial license. You also need a commercial license if you wish to use parts of this software in a commercial application other than as an air conditioner control system.

If you wish to do this, or you wish to have development assistance modifying this code, please contact:

William Edward Woody          <br>
12605 Raven Ridge Rd          <br>
Raleigh, NC 27614             <br>
United States of America      <br>
woody@alumni.caltech.edu
