# DLI Digiswitch
INDI DLI switch driver




Lunatico NexDome Beaver INDI Driver
=========================================
This package provides the INDI driver for Lunatico NexDome Beaver controller.

NexDome is a classic observatory dome with more than 100 degrees of shutter opening, allowing you to point your telescope to the zenith while remaining shielded from stray light and wind. The 2.2 meter inside diameter provides plenty of space to operate up to a 14” Schmidt Cass or a refractor 1400mm long.

The driver is compatible with Beaver firmware version 1.1.1 or higher.

INDI NexDome Beaver driver is compatible with libindi >= v1.8.2

NOTES
=====
- First you need to set the authentication parameters under the 'Options' tab
  - These need to match what you setup in the DLI switch
- There is a slight delay between setting a switch and the indicator light

*Read the Release Notes*

INSTALL
=======
- Reference the INSTALL document.

FEATURES
========


How to Use
==========

You can use the driver in any INDI-compatible client (such as KStars or Xephem) - just select 
DLI digiswitch from the Auxiliary list

To run the driver from the command line:

$ indiserver indi_dli_digiswitch # plus other needed drivers (telescope, camera, etc)

or using the indiwebserver.

You can then connect to the driver from any client, the default port is 7624.
If you're using KStars, the drivers will be automatically listed in KStars' Device Manager.

Before you Start
================

- 

OPERATIONS
==========

Options Tab
-----------

![Options Tab](Assets/OptionsTab-initial.jpeg)

You can enable or disable automatic shutter opening and closing when dome goes to into or out of the Park position. After changing these settings, click 'Save configuration' button to save the driver settings.

Mount Policy: Mount policy can be either set to Ignore Telescope (default) or Telescope Locks. When the policy is set to Ignore Telescope then the dome can park/unpark regardless of the mount parking state. When it is set Telescope locks, this disallows the dome from parking when telescope is unparked.  This might be important if you telescope has to be parked so as not to interfere with the dome parking.

![Options Tab](Assets/OptionsTab-authenticated.jpeg)


Main Tab
--------

![Main Tab](Assets/MainTab.jpeg)

Shutter Open/Close will only show up if the Shutter controller is up and communicating with the Rotator controller.

Move Relative buttons are meant to start/stop motion.  However, instead they move the dome by the amount set by the relative amount set in t he field above.  As a result, you need to press the CW or CCW button twice (once to start, once to stop).

Absolute Position will move the dome AZ relative to what you set the Home Offset to, which if set according to the instruction in the Rotator Tab, this will be from true north.

Dome and Shutter Status fields will display any errors, idle, moving, etc.

The Shutter Volts field displays the current voltage of the shutter battery.  In combination with the Safe Voltage on the Shutter tab, this can trigger the rotator controller to go into safety mode and call for the shutter to close.
- Shutter Volts and Status fields will not show if the shutter controller is off or not in communication.

General Info Tab
----------------

![General Info Tab](Assets/GeneralInfoTab.jpeg)


ISSUES
============
- Reference the Release Notes (above)



