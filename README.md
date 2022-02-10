# DLI Digiswitch
INDI DLI switch driver

DEPENDENCIES:

- sudo apt install build-essential devscripts debhelper fakeroot cdbs software-properties-common cmake pkg-config libmysqlclient-dev
- sudo add-apt-repository ppa:mutlaqja/ppa 
- sudo apt install libindi-dev libnova-dev libz-dev libgsl-dev

ISSUES:
- Latest INDI release has an orphaned link:  /usr/lib/libindidriver.so points to nowhere!
  - To fix: remove it, use 'locate libindidriver.so' to find the most recent version and create a link to it.
  - Example: 
    - locate libindidriver.so 
    - sudo rm /usr/lib/libindidriver.so
    - sudo ln -s /usr/lib/x86_64-linux-gnu/libindidriver.so   /usr/lib/libindidriver.so

CONTENTS:

- INDI driver and xml files

INSTALLING:

In a work directory of your choosing

- git clone https://github.com/sifank/KOBSswitch.git
- cd [install_path/]KOBSswitch
- mkdir build; cd build
- cmake ../
- make clean; make
- sudo make install
- (restart INDI)

NOTES:
- First you need to set the authentication parameters under the 'Options' tab
  - These need to match what you setup in the DLI switch
- There is a slight delay between setting a switch and the indicator light
