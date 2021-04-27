/*******************************************************************************
File:           KOBSswitch.h
Version:        20210424
Author:         Sifan
Desc:           INDI driver to control DLI 8 port power switch

Based on:
  Copyright(c) 2014 Radek Kaczorek  <rkaczorek AT gmail DOT com>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License version 2 as published by the Free Software Foundation.
 .
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.
 .
 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
*******************************************************************************/

/* Define Driver version */
#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#include <defaultdevice.h>
#include <curl/curl.h>

using namespace std;

class kobsRelay : public INDI::DefaultDevice
{
public:    
	kobsRelay();
	virtual ~kobsRelay() = default;    
    virtual bool initProperties() override;
    virtual const char *getDefaultName() override;
    virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n) override;
    virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n) override;
    virtual void ISGetProperties(const char *dev) override;
    virtual bool updateProperties() override;
    virtual void TimerHit() override;
    
protected:
	virtual bool saveConfigItems(FILE *fp);
    
private:
    
    virtual bool   Connect() override;
	virtual bool   Disconnect() override;
    
    //static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    int  switchSTAT(int port);
    int  switchCTL(int port, int control);
    
	 // Power
    enum
    {
        OUT1,
        OUT2,
        OUT3,
        OUT4,
        OUT5,
        OUT6,
        OUT7,
        OUT8,
        POWER_N,
    };
    ISwitch PortCtlS[POWER_N];
    ISwitchVectorProperty PortCtlSP;
    
    IText PortLabelsT[POWER_N];
    ITextVectorProperty PortLabelsTP;
    
    ILight PORTlightsL[POWER_N];
    ILightVectorProperty PORTlightsLP;
    
    bool PortLastState[POWER_N];
    
    enum
    {
        User,
        Auth,
        HostNm,
        Auths_N,
    };
    IText AuthsT[Auths_N];
    ITextVectorProperty AuthsTP;
    
    enum
    {
        Off,
        On,
        All_N,
    };
    ISwitch AllS[All_N];
    ISwitchVectorProperty AllSP;
    
    char portLabel[MAXINDILABEL];
    int  portRC;
    char URL[100];
    CURLcode res;
    bool HostError;
    bool UserError;
    bool AuthError;
    bool SwitchInit;
};

