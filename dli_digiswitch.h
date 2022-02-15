/*
    DLI DigiSwitch Controller

    Copyright (C) 2022 Sifan Kahale (sifan.kahale@gmail.com)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/
#include <defaultdevice.h>  // ALERT not in new format
#include <curl/curl.h>
#include <memory>
#include <indipropertytext.h>
#include <indipropertyswitch.h>
#include <indipropertynumber.h>

using namespace std;

class DLIsw : public INDI::DefaultDevice
{
public:    
    DLIsw();
    virtual ~DLIsw() = default;
    virtual bool initProperties() override;
    virtual const char *getDefaultName() override;
    virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n) override;
    virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n) override;
    virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;
    virtual void ISGetProperties(const char *dev) override;
    virtual bool updateProperties() override;
    virtual void TimerHit() override;
    
protected:
    virtual bool saveConfigItems(FILE *fp) override;

private:
    virtual bool   Connect() override;
    virtual bool   Disconnect() override;

    bool testConfig();
    bool switchVersion(string &version);
    bool switchStatus(int port, bool &sStatus);
    bool switchControl(int port, int control);
    bool switchCycle(int port);
    bool switchGetName(int port, string &pName);
    bool switchSetName(int port, const char* pName);
    void textConditioning(string &pName);
    void paintPortSwitches();
    bool curlRead(char* curlCmd, string &curRes);
    bool curlWrite(char* curlCmd, string &data);
    bool switchGetCycleDelay(int &cycleDelay);
    bool switchSetDelay(float delay);
    enum
    {
        DON,
        DOFF,
        DCYCLE,
        DCntl_N,
    };
    INDI::PropertySwitch PortControl1SP {DCntl_N};
    INDI::PropertySwitch PortControl2SP {DCntl_N};
    INDI::PropertySwitch PortControl3SP {DCntl_N};
    INDI::PropertySwitch PortControl4SP {DCntl_N};
    INDI::PropertySwitch PortControl5SP {DCntl_N};
    INDI::PropertySwitch PortControl6SP {DCntl_N};
    INDI::PropertySwitch PortControl7SP {DCntl_N};
    INDI::PropertySwitch PortControl8SP {DCntl_N};
    INDI::PropertySwitch AllSP {DCntl_N};

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
    INDI::PropertyText PortLabelsTP {POWER_N};

    INDI::PropertyText VersionTP {1};

    enum
    {
        User,
        Auth,
        HostNm,
        Auths_N,
    };
    INDI::PropertyText AuthsTP {Auths_N};
    INDI::PropertyNumber CycleDelayNP{1};
    
    char portLabel[MAXINDILABEL];
    CURLcode res;
    bool portChanged=false;
};

