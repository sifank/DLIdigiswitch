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

#include "dli_digiswitch.h"
#include "config.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wformat-truncation="

// declare an auto pointer to DLIsw
static std::unique_ptr<DLIsw> dlidriver(new DLIsw());

///////////////////////////////////////////////////////////////
const char *DLIsw::getDefaultName()
{
    return "DLI DigiSwitch";
}

///////////////////////////////////////////////////////////////
DLIsw::DLIsw()
{
    setVersion(DLI_VERSION_MAJOR, DLI_VERSION_MINOR);
}

///////////////////////////////////////////////////////////////
//  Connect
///////////////////////////////////////////////////////////////
bool DLIsw::Connect()
{
    string version;
    if(switchVersion(version)) {
        VersionTP[0].setText(version);
        LOGF_INFO("DLI switch version: %s", version.c_str());

        SetTimer(POLLMS);
        string version;
        DEBUG(INDI::Logger::DBG_SESSION, "KOBS switches connected successfully.");
        return true;
    }
    else {
        defineText(&AuthsTP);
        return false;
    }
}

///////////////////////////////////////////////////////////////
//  Disconnect
///////////////////////////////////////////////////////////////
bool DLIsw::Disconnect()
{
    DEBUG(INDI::Logger::DBG_SESSION, "KOBS switches disconnected successfully.");
    return true;
}

///////////////////////////////////////////////////////////////
//  initProperties
///////////////////////////////////////////////////////////////
bool DLIsw::initProperties()
{
    // We init parent properties first
    INDI::DefaultDevice::initProperties();

    setDriverInterface(AUX_INTERFACE);
    addAuxControls();

    ////////////////
    //Options Tab
    ////////////////
    //Port Labels - must define before pulling out names below
    PortLabelsTP[OUT1].fill("PORTLABEL1", "Port 1", "Port 1");
    PortLabelsTP[OUT2].fill("PORTLABEL2", "Port 2", "Port 2");
    PortLabelsTP[OUT3].fill("PORTLABEL3", "Port 3", "Port 3");
    PortLabelsTP[OUT4].fill("PORTLABEL4", "Port 4", "Port 4");
    PortLabelsTP[OUT5].fill("PORTLABEL5", "Port 5", "Port 5");
    PortLabelsTP[OUT6].fill("PORTLABEL6", "Port 6", "Port 6");
    PortLabelsTP[OUT7].fill("PORTLABEL7", "Port 7", "Port 7");
    PortLabelsTP[OUT8].fill("PORTLABEL8", "Port 8", "Port 8");
    PortLabelsTP.fill(getDeviceName(), "PORTLABELS", "Labels", OPTIONS_TAB, IP_WO, 60, IPS_IDLE);

    // DLI hostname or IP
    AuthsTP[User].fill("USER", "User name", "NotSet");
    AuthsTP[Auth].fill("AUTH", "Auth", "NotSet");
    AuthsTP[HostNm].fill("HOSTNAME", "Hostname", "NotSet");
    AuthsTP.fill(getDeviceName(), "Host or IP", "Authorization", OPTIONS_TAB, IP_RW, 60, IPS_IDLE);

    ////////////////
    //MAIN Tab
    ////////////////

    // ALERT need to set the portlabels either from getProperties or from the device
    //       but, the auth fiels are not updated yet ...
    for (int i=0; i < POWER_N; i++) {
        string sName;
        switchGetName(i, sName);
        PortLabelsTP[i].setText(sName.c_str());
    }

    bool sStatus = false;
    // Port 1
    if(!DLIsw::switchStatus(OUT1, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl1SP[DON].fill("P1ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl1SP[DOFF].fill("P1OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl1SP[DCYCLE].fill("P1CYCLE", "Cycle", ISS_OFF);
    PortControl1SP.fill(getDeviceName(), "PORT_1", PortLabelsTP[OUT1].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Port 2
    if(!DLIsw::switchStatus(OUT2, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl2SP[DON].fill("P2ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl2SP[DOFF].fill("P2OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl2SP[DCYCLE].fill("P2CYCLE", "Cycle", ISS_OFF);
    PortControl2SP.fill(getDeviceName(), "PORT_2", PortLabelsTP[OUT2].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Port 3
    if(!DLIsw::switchStatus(OUT3, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl3SP[DON].fill("P3ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl3SP[DOFF].fill("P3OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl3SP[DCYCLE].fill("P3CYCLE", "Cycle", ISS_OFF);
    PortControl3SP.fill(getDeviceName(), "PORT_3", PortLabelsTP[OUT3].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Port 4
    if(!DLIsw::switchStatus(OUT4, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl4SP[DON].fill("P4ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl4SP[DOFF].fill("P4OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl4SP[DCYCLE].fill("P4CYCLE", "Cycle", ISS_OFF);
    PortControl4SP.fill(getDeviceName(), "PORT_4", PortLabelsTP[OUT4].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Port 5
    if(!DLIsw::switchStatus(OUT5, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl5SP[DON].fill("P5ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl5SP[DOFF].fill("P5OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl5SP[DCYCLE].fill("P5CYCLE", "Cycle", ISS_OFF);
    PortControl5SP.fill(getDeviceName(), "PORT_5", PortLabelsTP[OUT5].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Port 6
    if(!DLIsw::switchStatus(OUT6, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl6SP[DON].fill("P6ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl6SP[DOFF].fill("P6OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl6SP[DCYCLE].fill("P6CYCLE", "Cycle", ISS_OFF);
    PortControl6SP.fill(getDeviceName(), "PORT_6", PortLabelsTP[OUT6].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Port 7
    if(!DLIsw::switchStatus(OUT7, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl7SP[DON].fill("P7ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl7SP[DOFF].fill("P7OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl7SP[DCYCLE].fill("P7CYCLE", "Cycle", ISS_OFF);
    PortControl7SP.fill(getDeviceName(), "PORT_7", PortLabelsTP[OUT7].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Port 8
    if(!DLIsw::switchStatus(OUT8, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl8SP[DON].fill("P8ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl8SP[DOFF].fill("P8OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl8SP[DCYCLE].fill("P8CYCLE", "Cycle", ISS_OFF);
    PortControl8SP.fill(getDeviceName(), "PORT_8", PortLabelsTP[OUT8].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // All On/Off/Profile
    AllSP[DON].fill("ALL_ON", "All On", ISS_OFF);
    AllSP[DOFF].fill("ALL_OFF", "All Off", ISS_OFF);
    AllSP.fill(getDeviceName(), "ALL", "All Ports", MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // INFO Tab
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Beaver Firmware Version
    VersionTP[0].fill("CVERSION", "Controller", "");
    VersionTP.fill(getDeviceName(), "DLI_FIRMWARE", "DLI Version", INFO_TAB, IP_RO, 0, IPS_IDLE);
    return true;

}

///////////////////////////////////////////////////////////////
//  updateProperties
///////////////////////////////////////////////////////////////
bool DLIsw::updateProperties()
{
    // Call parent update properties first
    INDI::DefaultDevice::updateProperties();

    if (isConnected())
    {
        // We're connected, paint the tabs
        defineText(&VersionTP);
        defineSwitch(&PortControl1SP);
        defineSwitch(&PortControl2SP);
        defineSwitch(&PortControl3SP);
        defineSwitch(&PortControl4SP);
        defineSwitch(&PortControl5SP);
        defineSwitch(&PortControl6SP);
        defineSwitch(&PortControl7SP);
        defineSwitch(&PortControl8SP);
        defineText(&PortLabelsTP);
        defineText(&AuthsTP);
        defineSwitch(&AllSP);
    }
    else
    {
        // We're disconnected
        deleteProperty(VersionTP.getName());
        deleteProperty(PortControl1SP.getName());
        deleteProperty(PortControl2SP.getName());
        deleteProperty(PortControl3SP.getName());
        deleteProperty(PortControl4SP.getName());
        deleteProperty(PortControl5SP.getName());
        deleteProperty(PortControl6SP.getName());
        deleteProperty(PortControl7SP.getName());
        deleteProperty(PortControl8SP.getName());
        deleteProperty(PortLabelsTP.getName());
        deleteProperty(AllSP.getName());
        deleteProperty(AuthsTP.getName());
    }
    return true;
}

///////////////////////////////////////////////////////////////
//  NewSwitch
///////////////////////////////////////////////////////////////
bool DLIsw::ISNewSwitch (const char *dev, const char *name, ISState *states, char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        // Control Ports
        portChanged = false;

        // Port 1
        if (PortControl1SP.isNameMatch(name))
        {
            PortControl1SP.update(states, names, n);
            if (PortControl1SP[DON].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT1, DON))
                    LOG_ERROR("Could not turn port 7 on");
            }
            else if (PortControl1SP[DOFF].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT1, DOFF))
                    LOG_ERROR("Could not turn port 7 off");
            }
            else if (PortControl1SP[DCYCLE].getState() == ISS_ON) {
                if (!DLIsw::switchCycle(OUT1)) {
                    LOG_ERROR("Could not cycle port 7");
                }
                PortControl1SP[DOFF].setState(ISS_ON);
                PortControl1SP[DCYCLE].setState(ISS_OFF);
            }
            PortControl1SP.setState(IPS_OK);
            PortControl1SP.apply();
            portChanged = true;
        }

        // Port 2
        if (PortControl2SP.isNameMatch(name))
        {
            PortControl2SP.update(states, names, n);
            if (PortControl2SP[DON].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT2, DON))
                    LOG_ERROR("Could not turn port 7 on");
            }
            else if (PortControl2SP[DOFF].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT2, DOFF))
                    LOG_ERROR("Could not turn port 7 off");
            }
            else if (PortControl2SP[DCYCLE].getState() == ISS_ON) {
                if (!DLIsw::switchCycle(OUT2)) {
                    LOG_ERROR("Could not cycle port 7");
                }
                PortControl2SP[DOFF].setState(ISS_ON);
                PortControl2SP[DCYCLE].setState(ISS_OFF);
            }
            PortControl2SP.setState(IPS_OK);
            PortControl2SP.apply();
            portChanged = true;
        }

        // Port 3
        if (PortControl3SP.isNameMatch(name))
        {
            PortControl3SP.update(states, names, n);
            if (PortControl3SP[DON].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT3, DON))
                    LOG_ERROR("Could not turn port 7 on");
            }
            else if (PortControl3SP[DOFF].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT3, DOFF))
                    LOG_ERROR("Could not turn port 7 off");
            }
            else if (PortControl3SP[DCYCLE].getState() == ISS_ON) {
                if (!DLIsw::switchCycle(OUT3)) {
                    LOG_ERROR("Could not cycle port 7");
                }
                PortControl3SP[DOFF].setState(ISS_ON);
                PortControl3SP[DCYCLE].setState(ISS_OFF);
            }
            PortControl3SP.setState(IPS_OK);
            PortControl3SP.apply();
            portChanged = true;
        }

        // Port 4
        if (PortControl4SP.isNameMatch(name))
        {
            PortControl4SP.update(states, names, n);
            if (PortControl4SP[DON].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT4, DON))
                    LOG_ERROR("Could not turn port 7 on");
            }
            else if (PortControl4SP[DOFF].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT4, DOFF))
                    LOG_ERROR("Could not turn port 7 off");
            }
            else if (PortControl4SP[DCYCLE].getState() == ISS_ON) {
                if (!DLIsw::switchCycle(OUT4)) {
                    LOG_ERROR("Could not cycle port 7");
                }
                PortControl4SP[DOFF].setState(ISS_ON);
                PortControl4SP[DCYCLE].setState(ISS_OFF);
            }
            PortControl4SP.setState(IPS_OK);
            PortControl4SP.apply();
            portChanged = true;
        }

        // Port 5
        if (PortControl5SP.isNameMatch(name))
        {
            PortControl5SP.update(states, names, n);
            if (PortControl5SP[DON].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT5, DON))
                    LOG_ERROR("Could not turn port 7 on");
            }
            else if (PortControl5SP[DOFF].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT5, DOFF))
                    LOG_ERROR("Could not turn port 7 off");
            }
            else if (PortControl5SP[DCYCLE].getState() == ISS_ON) {
                if (!DLIsw::switchCycle(OUT5)) {
                    LOG_ERROR("Could not cycle port 7");
                }
                PortControl5SP[DOFF].setState(ISS_ON);
                PortControl5SP[DCYCLE].setState(ISS_OFF);
            }
            PortControl5SP.setState(IPS_OK);
            PortControl5SP.apply();
            portChanged = true;
        }

        // Port 6
        if (PortControl6SP.isNameMatch(name))
        {
            PortControl6SP.update(states, names, n);
            if (PortControl6SP[DON].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT6, DON))
                    LOG_ERROR("Could not turn port 7 on");
            }
            else if (PortControl6SP[DOFF].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT6, DOFF))
                    LOG_ERROR("Could not turn port 7 off");
            }
            else if (PortControl6SP[DCYCLE].getState() == ISS_ON) {
                if (!DLIsw::switchCycle(OUT6)) {
                    LOG_ERROR("Could not cycle port 7");
                }
                PortControl6SP[DOFF].setState(ISS_ON);
                PortControl6SP[DCYCLE].setState(ISS_OFF);
            }
            PortControl6SP.setState(IPS_OK);
            PortControl6SP.apply();
            portChanged = true;
        }

        // Port 7
        if (PortControl7SP.isNameMatch(name))
        {
            PortControl7SP.update(states, names, n);
            if (PortControl7SP[DON].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT7, DON))
                    LOG_ERROR("Could not turn port 7 on");
            }
            else if (PortControl7SP[DOFF].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT7, DOFF))
                    LOG_ERROR("Could not turn port 7 off");
            }
            else if (PortControl7SP[DCYCLE].getState() == ISS_ON) {
                if (!DLIsw::switchCycle(OUT7)) {
                    LOG_ERROR("Could not cycle port 7");
                }
                PortControl7SP[DOFF].setState(ISS_ON);
                PortControl7SP[DCYCLE].setState(ISS_OFF);
            }
            PortControl7SP.setState(IPS_OK);
            PortControl7SP.apply();
            portChanged = true;
        }

        // Port 8
        if (PortControl8SP.isNameMatch(name))
        {
            PortControl8SP.update(states, names, n);
            if (PortControl8SP[DON].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT8, DON))
                    LOG_ERROR("Could not turn port 8 on");
            }
            else if (PortControl8SP[DOFF].getState() == ISS_ON) {
                if (!DLIsw::switchControl(OUT8, DOFF))
                    LOG_ERROR("Could not turn port 8 off");
            }
            else if (PortControl8SP[DCYCLE].getState() == ISS_ON) {
                if (!DLIsw::switchCycle(OUT8)) {
                    LOG_ERROR("Could not cycle port 8");
                }
                PortControl8SP[DOFF].setState(ISS_ON);
                PortControl8SP[DCYCLE].setState(ISS_OFF);
            }
            PortControl8SP.setState(IPS_OK);
            PortControl8SP.apply();
            portChanged = true;
        }

        if (portChanged) {
            AllSP[DOFF].setState(ISS_OFF);
            AllSP[DON].setState(ISS_OFF);
            AllSP.setState(IPS_OK);
        }

        // All On
        if (AllSP.isNameMatch(name))
        {
            AllSP.update(states, names, n);
            
            // ALLON
            if(AllSP[DON].getState() == ISS_ON) {
                if (DLIsw::switchControl(OUT1, DON))
                    PortControl1SP[DON].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 1 on");

                if (DLIsw::switchControl(OUT2, DON))
                    PortControl2SP[DON].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 2 on");

                if (DLIsw::switchControl(OUT3, DON))
                    PortControl3SP[DON].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 3 on");

                if (DLIsw::switchControl(OUT4, DON))
                    PortControl4SP[DON].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 4 on");

                if (DLIsw::switchControl(OUT5, DON))
                    PortControl5SP[DON].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 5 on");

                if (DLIsw::switchControl(OUT6, DON))
                    PortControl6SP[DON].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 6 on");

                if (DLIsw::switchControl(OUT7, DON))
                    PortControl7SP[DON].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 7 on");

                if (DLIsw::switchControl(OUT8, DON))
                    PortControl8SP[DON].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 8 on");

                AllSP[DON].setState(ISS_ON);
                AllSP[DOFF].setState(ISS_OFF);
                AllSP.setState(IPS_OK);
                AllSP.apply();
            }
            
            // All Off
            else if(AllSP[DON].getState() == ISS_OFF) {
                if (DLIsw::switchControl(OUT1, DOFF))
                    PortControl1SP[DOFF].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 1 off");

                if (DLIsw::switchControl(OUT2, DOFF))
                    PortControl2SP[DOFF].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 2 off");

                if (DLIsw::switchControl(OUT3, DOFF))
                    PortControl3SP[DOFF].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 3 off");

                if (DLIsw::switchControl(OUT4, DOFF))
                    PortControl4SP[DOFF].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 4 off");

                if (DLIsw::switchControl(OUT5, DOFF))
                    PortControl5SP[DOFF].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 5 off");

                if (DLIsw::switchControl(OUT6, DOFF))
                    PortControl6SP[DOFF].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 6 off");

                if (DLIsw::switchControl(OUT7, DOFF))
                    PortControl7SP[DOFF].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 7 off");

                if (DLIsw::switchControl(OUT8, DOFF))
                    PortControl8SP[DOFF].setState(ISS_ON);
                else
                    LOG_ERROR("Could not turn port 8 off");

                
                AllSP[DOFF].setState(ISS_ON);
                AllSP[DON].setState(ISS_OFF);
                AllSP.setState(IPS_OK);
                AllSP.apply();
            }
        }
    }

    return INDI::DefaultDevice::ISNewSwitch (dev, name, states, names, n);
}

///////////////////////////////////////////////////////////////
//   NewText
///////////////////////////////////////////////////////////////
bool DLIsw::ISNewText (const char *dev, const char *name, char *texts[], char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        // Port names
        if (PortLabelsTP.isNameMatch(name))
        {
            PortLabelsTP.update(texts, names, n);
            if (!switchSetNames()) {
                LOG_ERROR("Problem setting names");
            }
            PortLabelsTP.setState(IPS_OK);
            PortLabelsTP.apply();
        }

        // Authentications
        if (AuthsTP.isNameMatch(name))
        {
            AuthsTP.update(texts, names, n);
            AuthsTP.setState(IPS_OK);
            AuthsTP.apply();

            if (!testConfig()) {
                AuthsTP.setState(IPS_ALERT);
                AuthsTP.apply();
                LOG_ERROR("You need to click 'Connect' on the Main Tab to continue");
            }
            else {
                for (int i=0; i < POWER_N; i++) {
                    string sName;
                    switchGetName(i, sName);
                    PortLabelsTP[i].setText(sName.c_str());
                }
                // ALERT if this was just set, need to tell user to click Connect on Main Tab
                saveConfig();
                // for security reasons, remove the auth fields if successful
                deleteProperty(AuthsTP.getName());
            }
        }
    }
    return INDI::DefaultDevice::ISNewText (dev, name, texts, names, n);
}

///////////////////////////////////////////////////////////////
//   Timer Hit
///////////////////////////////////////////////////////////////
void DLIsw::TimerHit()
{
    if(!isConnected())
        return;

    // If auth is not set, set all switches blank
    if(!testConfig()) {
        LOG_ERROR("Auth not set, resetting port status to off");
        for (int i=0; i < POWER_N; i++) {
            PortControl1SP[i].setState(ISS_OFF);
            PortControl2SP[i].setState(ISS_OFF);
            PortControl3SP[i].setState(ISS_OFF);
            PortControl4SP[i].setState(ISS_OFF);
            PortControl5SP[i].setState(ISS_OFF);
            PortControl6SP[i].setState(ISS_OFF);
            PortControl7SP[i].setState(ISS_OFF);
            PortControl8SP[i].setState(ISS_OFF);
        }
        PortControl1SP.apply();
        AllSP[DON].setState(ISS_OFF);
        AllSP[DOFF].setState(ISS_OFF);
        AllSP.apply();
        SetTimer(POLLMS);
        return;
    }

    // update switch status
    bool sStatus = false;
    if(!DLIsw::switchStatus(OUT1, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl1SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl1SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl1SP.apply();

    if(!DLIsw::switchStatus(OUT2, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl2SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl2SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl2SP.apply();

    if(!DLIsw::switchStatus(OUT3, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl3SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl3SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl3SP.apply();

    if(!DLIsw::switchStatus(OUT4, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl4SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl4SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl4SP.apply();

    if(!DLIsw::switchStatus(OUT5, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl5SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl5SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl5SP.apply();

    if(!DLIsw::switchStatus(OUT6, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl6SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl6SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl6SP.apply();

    if(!DLIsw::switchStatus(OUT7, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl7SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl7SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl7SP.apply();

    if(!DLIsw::switchStatus(OUT8, sStatus))
        LOG_ERROR("Problem getting status");
    PortControl8SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl8SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl8SP.apply();
    
    // and tell INDI to do this again ...
    SetTimer(POLLMS);
    return;
}

///////////////////////////////////////////////////////////////
// GetProperties
///////////////////////////////////////////////////////////////
void DLIsw::ISGetProperties(const char *dev)
{
    INDI::DefaultDevice::ISGetProperties(dev);
    loadConfig(true, AuthsTP.getName());
}

///////////////////////////////////////////////////////////////
// saveConfigItems
///////////////////////////////////////////////////////////////
bool DLIsw::saveConfigItems(FILE *fp)
{
    IUSaveConfigText(fp, &AuthsTP);
    return true;
}

//////////////////// Communicatiion Functions /////////////////////////////

///////////////////////////////////////////////////////////////
//   CURL write callback
///////////////////////////////////////////////////////////////
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

///////////////////////////////////////////////////////////////
//   remove "[] from strings returned
///////////////////////////////////////////////////////////////
void DLIsw::textConditioning(string &pName)
{
    pName.erase(std::remove(pName.begin(), pName.end(), '['), pName.end());
    pName.erase(std::remove(pName.begin(), pName.end(), ']'), pName.end());
    pName.erase(std::remove(pName.begin(), pName.end(), '"'), pName.end());
}

///////////////////////////////////////////////////////////////
//   Test if configured
///////////////////////////////////////////////////////////////
bool DLIsw::testConfig() {
    // test if auth fields have been set
    if ((strcmp(AuthsTP[User].getText(), "NotSet") == 0) ||
            (strcmp(AuthsTP[Auth].getText(), "NotSet") == 0) ||
            (strcmp(AuthsTP[HostNm].getText(), "NotSet") == 0)) {
        LOG_ERROR("You need to set the autherazations under the 'Options' tab!");
        return false;
    }

    // test if Digiswitch answers back
    string version;
    if(!switchVersion(version)) {
        LOG_ERROR("Problem with authentication:  reset the auth fields under the 'Options' tab.");
        return false;
    }

    VersionTP[0].setText(version);
    VersionTP.apply();

    // for security reasons, remove the auth fields if successful
    deleteProperty(AuthsTP.getName());
    return true;
}

///////////////////////////////////////////////////////////////
// get switch version
///////////////////////////////////////////////////////////////
bool DLIsw::switchVersion(string &version)
{
    CURL *curl;
    string readBuffer;

    snprintf(URL, 100, "http://%s:%s@%s/restapi/config/=version/", AuthsTP[User].getText(), AuthsTP[Auth].getText(), AuthsTP[HostNm].getText());

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &version);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res)
            return false;

        textConditioning(version);
        return true;
    }
    LOG_ERROR("Problem getting DLI version");
    return false;
}

///////////////////////////////////////////////////////////////
//   control switch (on/off)
///////////////////////////////////////////////////////////////
bool DLIsw::switchControl(int port, int control)
{
    CURL *curl;
    string readBuffer;
    string data = "value=true";
    struct curl_slist *list = NULL;

    if(!testConfig())
        return false;

    snprintf(URL, 100, "http://%s:%s@%s/restapi/relay/outlets/=%i/state/", AuthsTP[User].getText(), AuthsTP[Auth].getText(), AuthsTP[HostNm].getText(), port);
    LOGF_DEBUG("Set URL: %s", URL);

    if (control == DOFF)
        data = "value=false";
    else if (control == DON)
        data = "value=true";
    else {
        LOG_ERROR("Control must be either on or off");
        return false;
    }

    list = curl_slist_append(list, "X-CSRF: x");

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res) {
            LOGF_ERROR("CURL error, result from curl: %i\n", res);
            LOGF_ERROR("CURL returned: %s\n", readBuffer.c_str());
            return false;
        }
        return true;
    }
    LOG_ERROR("Problem controlling port");
    return false;
}

///////////////////////////////////////////////////////////////
//   cycle switch (works only if on)
///////////////////////////////////////////////////////////////
bool DLIsw::switchCycle(int port)
{
    CURL *curl;
    string readBuffer;
    string data = "";

    struct curl_slist *list = NULL;

    if(!testConfig())
        return false;

    snprintf(URL, 100, "http://%s:%s@%s/restapi/relay/outlets/=%i/cycle/", AuthsTP[User].getText(), AuthsTP[Auth].getText(), AuthsTP[HostNm].getText(), port);
    LOGF_DEBUG("Cycle URL: %s", URL);

    list = curl_slist_append(list, "X-CSRF: x");

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res) {
            LOGF_ERROR("CURL error, result from curl: %i\n", res);
            LOGF_ERROR("CURL returned: %s\n", readBuffer.c_str());
            return false;
        }
        return true;
    }
    LOG_ERROR("Problem cycling port");
    return false;
}

///////////////////////////////////////////////////////////////
// get switch status
///////////////////////////////////////////////////////////////
bool DLIsw::switchStatus(int port, bool &sStatus)
{
    CURL *curl;
    string readBuffer;

    if(!testConfig())
        return false;

    snprintf(URL, 100, "http://%s:%s@%s/restapi/relay/outlets/=%i/state/", AuthsTP[User].getText(), AuthsTP[Auth].getText(), AuthsTP[HostNm].getText(), port);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res) {
            LOGF_ERROR("CURL error, result from curl: %i\n", res);
            LOGF_ERROR("CURL returned: %s\n", readBuffer.c_str());
            return false;
        }

        if(readBuffer == "[true]") {
            LOGF_DEBUG("CURL result for %i is: on", port);
            sStatus = true;
            return true;
        }
        else if(readBuffer == "[false]") {
            LOGF_DEBUG("CURL result for %i is: off", port);
            sStatus = false;
            return true;
        }
    }
    LOG_ERROR("Error reading status");
    return false;
}

///////////////////////////////////////////////////////////////
// get switch name
///////////////////////////////////////////////////////////////
bool DLIsw::switchGetName(int port, string &pName)
{
    CURL *curl;

    // get out of here if auth is not set yet
    if(!testConfig())
        return false;

    snprintf(URL, 100, "http://%s:%s@%s/restapi/relay/outlets/=%i/name/", AuthsTP[User].getText(), AuthsTP[Auth].getText(), AuthsTP[HostNm].getText(), port);
    LOGF_DEBUG("CURL cmd:  %s", URL);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &pName);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res) {
            LOGF_ERROR("CURL error, result from curl: %i\n", res);
            LOGF_ERROR("CURL returned: %s\n", pName.c_str());
            return false;
        }
        textConditioning(pName);
        return true;
    }
    LOG_ERROR("Error reading port names");
    return false;
}

///////////////////////////////////////////////////////////////
//   set all port names
///////////////////////////////////////////////////////////////
bool DLIsw::switchSetNames()
{
    for (int i=0; i < POWER_N; i++) {
        CURL *curl;
        string readBuffer;
        char pName[50];
        struct curl_slist *list = NULL;

        if(!testConfig())
            return false;

        snprintf(URL, 100, "http://%s:%s@%s/restapi/relay/outlets/%i/name/", AuthsTP[User].getText(), AuthsTP[Auth].getText(), AuthsTP[HostNm].getText(), i);
        LOGF_DEBUG("Set URL: %s", URL);

        snprintf(pName, 50, "value=%s", PortLabelsTP[i].getText());
        LOGF_DEBUG("Setting port %i name to: %s, URL: %s", i, pName, URL);

        list = curl_slist_append(list, "X-CSRF: x");

        curl = curl_easy_init();
        if(!curl) {
            LOG_ERROR("Problem setting port name");
            return false;
        }
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pName);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res) {
            LOGF_ERROR("CURL error, result from curl: %i\n", res);
            LOGF_ERROR("CURL returned: %s\n", readBuffer.c_str());
            return false;
        }
    }
    //deleteProperty(PortLabelsTP.getName());
    //defineText(&PortLabelsTP);
    PortLabelsTP.setState(IPS_OK);
    PortLabelsTP.apply();
    return true;
}
