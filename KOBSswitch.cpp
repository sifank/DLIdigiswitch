/*******************************************************************************
File:           KOBSswitch.cpp
Version:        20210424
Author:         Sifan
Desc:           INDI driver to control DLI 8 port power switch

Based on:
Copyright(c) 2015-2020 Radek Kaczorek  <rkaczorek AT gmail DOT com>

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

#include "KOBSswitch.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wformat-truncation="

// declare an auto pointer to kobsRelay
static std::unique_ptr<kobsRelay> KOBSrelay(new kobsRelay());

void ISPoll(void *p);

/***************************************************************/
/*    INDI declarations                                        */
/***************************************************************/
void ISInit()
{
	static int isInit = 0;

	if (isInit == 1)
		return;
	if(KOBSrelay.get() == 0)
	{
		isInit = 1;
		KOBSrelay.reset(new kobsRelay());
	}
}

/***************************************************************/
void ISGetProperties(const char *dev)
{
        ISInit();
        KOBSrelay->ISGetProperties(dev);
}

/***************************************************************/
void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int num)
{
        ISInit();
        KOBSrelay->ISNewSwitch(dev, name, states, names, num);
}

/***************************************************************/
void ISNewText(	const char *dev, const char *name, char *texts[], char *names[], int num)
{
        ISInit();
        KOBSrelay->ISNewText(dev, name, texts, names, num);
}

/***************************************************************/
void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int num)
{
        ISInit();
        KOBSrelay->ISNewNumber(dev, name, values, names, num);
}

/***************************************************************/
void ISNewBLOB(const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[],
               char *formats[], char *names[], int n)
{
    KOBSrelay->ISNewBLOB(dev, name, sizes, blobsizes, blobs, formats, names, n);
}

/***************************************************************/
kobsRelay::kobsRelay()
{
	setVersion(VERSION_MAJOR,VERSION_MINOR);
}

/***************************************************************/
/*    CURL write callback                                      */
/***************************************************************/
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void ISSnoopDevice(XMLEle *root)
{
    KOBSrelay->ISSnoopDevice(root);
}

/***************************************************************/
/*    Switch control                                           */
/***************************************************************/
int kobsRelay::switchCTL(int port, int control)
{ 
  CURL *curl;
  string readBuffer;
  string data = "value=true";
  struct curl_slist *list = NULL;

  char *user = AuthsT[User].text;
  char *auth = AuthsT[Auth].text;  
  char *host = AuthsT[HostNm].text;
  
  if(strcmp(user, "NotSet") == 0) {
      if(!UserError) {
        LOG_ERROR("You need to set the username under the 'Options' tab!");
        UserError = true;
      }
      return IPS_ALERT;
  }
  
  if(strcmp(auth, "NotSet") == 0) {
      if(!AuthError) {
        LOG_ERROR("You need to set the authorization under the 'Options' tab!");
        AuthError = true;
      }
      return IPS_ALERT;
  }
  
  if(strcmp(host, "NotSet") == 0) {
      if(!HostError) {
        LOG_ERROR("You need to set the hostname under the 'Options' tab!");
        HostError = true;
      }
      return IPS_ALERT;
  }
  
  snprintf(URL, 100, "http://%s:%s@%s/restapi/relay/outlets/=%i/state/", user, auth, host, port);
  
  if (control == 0)
          data = "value=false";
  else if (control == 1)
          data = "value=true";
  else {
          LOG_ERROR("Control must be either on or off");
          return IPS_ALERT;
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
    if (res) {
        LOGF_ERROR("CURL error, result from curl: %i\n", res);
        LOGF_ERROR("CURL returned: %s\n", readBuffer.c_str());
        return IPS_ALERT;
    }

    curl_easy_cleanup(curl);
  }

    return IPS_OK;    
}

/***************************************************************/
/*  switch status                                              */
/***************************************************************/
int kobsRelay::switchSTAT(int port)
{    
  CURL *curl;
  string readBuffer;
  char *user = AuthsT[User].text;
  char *auth = AuthsT[Auth].text;  
  char *host = AuthsT[HostNm].text;
  
  if(strcmp(user, "NotSet") == 0) {
      //LOG_ERROR("user not set");
      if(!UserError) {
        LOG_ERROR("Make sure you set the Authorization fields under the 'Options' tab!");
        UserError = true;
      }
      return 3;
  }
  
  if(strcmp(auth, "NotSet") == 0) {
      if(!AuthError) {
        LOG_ERROR("Make sure you set the Authorization fields under the 'Options' tab!");
        AuthError = true;
      }
      return 3;
  }
  
  if(strcmp(host, "NotSet") == 0) {
      if(!HostError) {
        LOG_ERROR("Make sure you set the Authorization fields the 'Options' tab!");
        HostError = true;
      }
      return 3;
  }
  
  snprintf(URL, 100, "http://%s:%s@%s/restapi/relay/outlets/=%i/state/", user, auth, host, port);

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
    curl_easy_setopt(curl, CURLOPT_URL, URL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if(readBuffer == "[true]") {
        //LOGF_INFO("CURL result for %i is: on", port);
        return 1;
    }
    else if(readBuffer == "[false]") {
        //LOGF_INFO("CURL result for %i is: off", port);
        return 0;
    }
  }
  
  LOGF_ERROR("Error reading status: %s", readBuffer);

  return 3;
   
}

/***************************************************************/
/*   Connect                                                   */
/***************************************************************/
bool kobsRelay::Connect()
{
	DEBUG(INDI::Logger::DBG_SESSION, "KOBS switches connected successfully.");
    
    for (int i=0; i < POWER_N; i++)
            PortLastState[i] = switchSTAT(i);
    
    HostError = false;
    UserError = false;
    AuthError = false;
    SwitchInit = false;
    
    POLLMS = 1000;
    SetTimer(POLLMS);
	return true;
}

/***************************************************************/
/*   Disconnect                                                */
/***************************************************************/
bool kobsRelay::Disconnect()
{
	DEBUG(INDI::Logger::DBG_SESSION, "KOBS switches disconnected successfully.");
	return true;
}

/***************************************************************/
const char * kobsRelay::getDefaultName()
{
        return (char *)"KOBS Power Switches";
}

/***************************************************************/
/*   initProperties                                            */
/***************************************************************/
bool kobsRelay::initProperties()
{
	// We init parent properties first
	INDI::DefaultDevice::initProperties();
    setDriverInterface(AUX_INTERFACE);
    addAuxControls();
    
    /***************/
    /* Options Tab */
    /***************/
    //Port Labels - must define before pulling out names below
    IUFillText(&PortLabelsT[OUT1], "PORTLABEL1", "Port 1", "Port 1");
    IUFillText(&PortLabelsT[OUT2], "PORTLABEL2", "Port 2", "Port 2");
    IUFillText(&PortLabelsT[OUT3], "PORTLABEL3", "Port 3", "Port 3");
    IUFillText(&PortLabelsT[OUT4], "PORTLABEL4", "Port 4", "Port 4");
    IUFillText(&PortLabelsT[OUT5], "PORTLABEL5", "Port 5", "Port 5");
    IUFillText(&PortLabelsT[OUT6], "PORTLABEL6", "Port 6", "Port 6");
    IUFillText(&PortLabelsT[OUT7], "PORTLABEL7", "Port 7", "Port 7");
    IUFillText(&PortLabelsT[OUT8], "PORTLABEL8", "Port 8", "Port 8");
    IUFillTextVector(&PortLabelsTP, PortLabelsT, POWER_N, getDeviceName(), "PORTLABELS", "Labels", OPTIONS_TAB, IP_WO, 60, IPS_IDLE);
    
    // DLI hostname or IP
    IUFillText(&AuthsT[User], "USER", "User name", "NotSet");
    IUFillText(&AuthsT[Auth], "AUTH", "Auth", "NotSet");
    IUFillText(&AuthsT[HostNm], "HOSTNAME", "Hostname", "NotSet");
    IUFillTextVector(&AuthsTP, AuthsT, Auths_N, getDeviceName(), "Host or IP", "Authorization", OPTIONS_TAB, IP_RW, 60, IPS_IDLE);
    
    /***************/
    /* MAIN Tab    */
    /***************/
    // Port 1
    memset(portLabel, 0, MAXINDILABEL);
    portRC = IUGetConfigText(getDeviceName(), PortLabelsTP.name, PortLabelsT[OUT1].name, portLabel, MAXINDILABEL);
    IUFillSwitch(&PortCtlS[OUT1], "CPORT1", portRC == -1 ? "Port 1" : portLabel, kobsRelay::switchSTAT(OUT1) ? ISS_ON : ISS_OFF);
    IUFillLight(&PORTlightsL[OUT1], "LPORT1", portRC == -1 ? "Port 1" : portLabel, kobsRelay::switchSTAT(OUT1) ? IPS_OK : IPS_ALERT);
    
    // Port 2
    memset(portLabel, 0, MAXINDILABEL);
    portRC = IUGetConfigText(getDeviceName(), PortLabelsTP.name, PortLabelsT[OUT2].name, portLabel, MAXINDILABEL);
    IUFillSwitch(&PortCtlS[OUT2], "CPORT2", portRC == -1 ? "Port 2" : portLabel, kobsRelay::switchSTAT(OUT2) ? ISS_ON : ISS_OFF);
    IUFillLight(&PORTlightsL[OUT2], "LPORT2", portRC == -1 ? "Port 2" : portLabel, kobsRelay::switchSTAT(OUT2) ? IPS_OK : IPS_ALERT);
    
    // Port 3
    memset(portLabel, 0, MAXINDILABEL);
    portRC = IUGetConfigText(getDeviceName(), PortLabelsTP.name, PortLabelsT[OUT3].name, portLabel, MAXINDILABEL);
    IUFillSwitch(&PortCtlS[OUT3], "CPORT3", portRC == -1 ? "Port 3" : portLabel, kobsRelay::switchSTAT(OUT3) ? ISS_ON : ISS_OFF);
    IUFillLight(&PORTlightsL[OUT3], "LPORT3", portRC == -1 ? "Port 3" : portLabel, kobsRelay::switchSTAT(OUT3) ? IPS_OK : IPS_ALERT);
    
    // Port 4
    memset(portLabel, 0, MAXINDILABEL);
    portRC = IUGetConfigText(getDeviceName(), PortLabelsTP.name, PortLabelsT[OUT4].name, portLabel, MAXINDILABEL);
    IUFillSwitch(&PortCtlS[OUT4], "CPORT4", portRC == -1 ? "Port 4" : portLabel, kobsRelay::switchSTAT(OUT4) ? ISS_ON : ISS_OFF);
    IUFillLight(&PORTlightsL[OUT4], "LPORT4", portRC == -1 ? "Port 4" : portLabel, kobsRelay::switchSTAT(OUT4) ? IPS_OK : IPS_ALERT);
    
    // Port 5
    memset(portLabel, 0, MAXINDILABEL);
    portRC = IUGetConfigText(getDeviceName(), PortLabelsTP.name, PortLabelsT[OUT5].name, portLabel, MAXINDILABEL);
    IUFillSwitch(&PortCtlS[OUT5], "CPORT5", portRC == -1 ? "Port 5" : portLabel, kobsRelay::switchSTAT(OUT5) ? ISS_ON : ISS_OFF);
    IUFillLight(&PORTlightsL[OUT5], "LPORT5", portRC == -1 ? "Port 5" : portLabel, kobsRelay::switchSTAT(OUT5) ? IPS_OK : IPS_ALERT);
    
    // Port 6
    memset(portLabel, 0, MAXINDILABEL);
    portRC = IUGetConfigText(getDeviceName(), PortLabelsTP.name, PortLabelsT[OUT6].name, portLabel, MAXINDILABEL);
    IUFillSwitch(&PortCtlS[OUT6], "CPORT6", portRC == -1 ? "Port 6" : portLabel, kobsRelay::switchSTAT(OUT6) ? ISS_ON : ISS_OFF);
    IUFillLight(&PORTlightsL[OUT6], "LPORT6", portRC == -1 ? "Port 6" : portLabel, kobsRelay::switchSTAT(OUT6) ? IPS_OK : IPS_ALERT);
    
    // Port 7
    memset(portLabel, 0, MAXINDILABEL);
    portRC = IUGetConfigText(getDeviceName(), PortLabelsTP.name, PortLabelsT[OUT7].name, portLabel, MAXINDILABEL);
    IUFillSwitch(&PortCtlS[OUT7], "CPORT7", portRC == -1 ? "Port 7" : portLabel, kobsRelay::switchSTAT(OUT7) ? ISS_ON : ISS_OFF);
    IUFillLight(&PORTlightsL[OUT7], "LPORT7", portRC == -1 ? "Port 7" : portLabel, kobsRelay::switchSTAT(OUT7) ? IPS_OK : IPS_ALERT);
    
    // Port 8
    memset(portLabel, 0, MAXINDILABEL);
    portRC = IUGetConfigText(getDeviceName(), PortLabelsTP.name, PortLabelsT[OUT8].name, portLabel, MAXINDILABEL);
    IUFillSwitch(&PortCtlS[OUT8], "CPORT8", portRC == -1 ? "Port 8" : portLabel, kobsRelay::switchSTAT(OUT8) ? ISS_ON : ISS_OFF);
    IUFillLight(&PORTlightsL[OUT8], "LPORT8", portRC == -1 ? "Port 8" : portLabel, kobsRelay::switchSTAT(OUT8) ? IPS_OK : IPS_ALERT);
    
    // Set vector properties
    IUFillLightVector(&PORTlightsLP, PORTlightsL, POWER_N, getDeviceName(), "PORT_LIGHTS", "Status", MAIN_CONTROL_TAB, IPS_OK);
    IUFillSwitchVector(&PortCtlSP, PortCtlS, POWER_N, getDeviceName(), "PORT_ENABLES", "Power", MAIN_CONTROL_TAB, IP_RW, ISR_NOFMANY, 60, IPS_OK);
    
    // All On/Off/Profile
    IUFillSwitch(&AllS[On], "ALL_ON", "All On", ISS_OFF);
    IUFillSwitch(&AllS[Off], "ALL_OFF", "All Off", ISS_OFF);
    IUFillSwitchVector(&AllSP, AllS, All_N, getDeviceName(), "ALL", "All Ports", MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);
    

	return true;
}

/***************************************************************/
/*   updateProperties                                          */
/***************************************************************/
bool kobsRelay::updateProperties()
{
	// Call parent update properties first
	INDI::DefaultDevice::updateProperties();

	if (isConnected())
	{
        // We're connected, paint the tabs
        defineLight(&PORTlightsLP);
		defineSwitch(&PortCtlSP);
        defineSwitch(&AllSP);
        defineText(&AuthsTP);
        defineText(&PortLabelsTP);
	}
	else
	{
		// We're disconnected
		deleteProperty(PortCtlSP.name);
        deleteProperty(PortLabelsTP.name);
        deleteProperty(AllSP.name);
        deleteProperty(AuthsTP.name);
        deleteProperty(PORTlightsLP.name);
	}
	return true;
}

/***************************************************************/
/*   NewSwitch                                                 */
/***************************************************************/
bool kobsRelay::ISNewSwitch (const char *dev, const char *name, ISState *states, char *names[], int n)
{
	// first we check if it's for our device
	if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
	{
        // turn ports on or off
        if (strcmp(name, PortCtlSP.name) == 0)
        {
            IUUpdateSwitch(&PortCtlSP, states, names, n);
        
            for (int i=0; i < POWER_N; i++) {
                if(PortCtlS[i].s != PortLastState[i]) {
                    PortCtlS[i].s ?  kobsRelay::switchCTL(i, On) : kobsRelay::switchCTL(i, Off);
                    PORTlightsL[i].s = PortCtlS[i].s ? IPS_OK : IPS_ALERT;
                    PortLastState[i] = PortCtlS[i].s;
                }
            }
            
            AllS[Off].s = ISS_OFF;
            AllS[On].s = ISS_OFF;
            AllSP.s = IPS_OK;
            
            IDSetSwitch(&AllSP, nullptr);
            IDSetLight(&PORTlightsLP, nullptr);
            IDSetSwitch(&PortCtlSP, nullptr);
            return true;
        }
	}
	
	// All On
        if (strcmp(name, AllSP.name) == 0)
        {
            IUUpdateSwitch(&AllSP, states, names, n);
            
            // ALLON
            if(IUFindOnSwitchIndex(&AllSP) == On) 
            {
                //for loop to set all to on
                for (int i=0; i < POWER_N; i++) {
                    PortCtlS[i].s = ISS_ON;
                    kobsRelay::switchCTL(i, On);
                    PORTlightsL[i].s = IPS_OK;
                }
                
                AllS[On].s = ISS_ON;
                AllS[Off].s = ISS_OFF;
                AllSP.s = IPS_OK;
                
                IDSetLight(&PORTlightsLP, nullptr);
                IDSetSwitch(&PortCtlSP, nullptr);
                IDSetSwitch(&AllSP, nullptr);
                return true;
            }
            
            // All Off
            else if(IUFindOnSwitchIndex(&AllSP) == Off)
            {
                //for loop to set all to on
                for (int i=0; i < POWER_N; i++) {
                    PortCtlS[i].s = ISS_OFF;
                    kobsRelay::switchCTL(i, Off);
                    PORTlightsL[i].s = IPS_ALERT;
                }
                
                IUResetSwitch(&PortCtlSP);
                
                AllS[Off].s = ISS_ON;
                AllS[On].s = ISS_OFF;
                AllSP.s = IPS_OK;
                
                IDSetLight(&PORTlightsLP, nullptr);
                IDSetSwitch(&PortCtlSP, nullptr);
                IDSetSwitch(&AllSP, nullptr);
                return true;
            }
            
        }
        
	return INDI::DefaultDevice::ISNewSwitch (dev, name, states, names, n);
}

/***************************************************************/
/*    NewText                                                  */
/***************************************************************/
bool kobsRelay::ISNewText (const char *dev, const char *name, char *texts[], char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        // Port names
        if (strcmp(name, PortLabelsTP.name) == 0)
        {
            IUUpdateText(&PortLabelsTP, texts, names, n);
            PortLabelsTP.s = IPS_OK;
            IDSetText(&PortLabelsTP, nullptr);
            saveConfig();
            LOG_INFO("Power port labels saved. INDI must be restarted for the labels to take effect.");
            return true;
        }

        // Hostname
        if (strcmp(name, AuthsTP.name) == 0)
        {
            IUUpdateText(&AuthsTP, texts, names, n);
            PortLabelsTP.s = IPS_OK;
            IDSetText(&AuthsTP, nullptr);
            //LOG_INFO("Hostname saved. INDI must be restarted for the name to take effect.");
            saveConfig();
            return true;
        }
    }
	return INDI::DefaultDevice::ISNewText (dev, name, texts, names, n);
}

/***************************************************************/
/*    Timer Hit                                                */
/***************************************************************/
void kobsRelay::TimerHit()
{
    if(!isConnected())
        return;
    
    // If auth is not set (error from switchSTAT) set main status's to red
    if(kobsRelay::switchSTAT(0) == 3) {
        PORTlightsLP.s = IPS_ALERT;
        PortCtlSP.s = IPS_ALERT;
        AllSP.s = IPS_ALERT;
        for (int i=0; i < POWER_N; i++) {
            PORTlightsL[i].s = IPS_IDLE;
            PortCtlS[i].s = ISS_OFF;
        }
        
        IDSetLight(&PORTlightsLP, nullptr);
        IDSetSwitch(&PortCtlSP, nullptr);
        IDSetSwitch(&AllSP, nullptr);
        SetTimer(POLLMS);
        return;
    }
    
    // update port lights
    for (int i=0; i < POWER_N; i++) {
        if(kobsRelay::switchSTAT(i)) {
            PORTlightsL[i].s = IPS_OK;
        }
        else {
            PORTlightsL[i].s = IPS_ALERT;
        }
    }
    PORTlightsLP.s = IPS_OK;
    IDSetLight(&PORTlightsLP, nullptr);
    
    // initially set switches to their status
    if(!SwitchInit) {
        for (int i=0; i < POWER_N; i++)
            PortCtlS[i].s = kobsRelay::switchSTAT(i) ? ISS_ON : ISS_OFF;
        
        IDSetSwitch(&PortCtlSP, nullptr);
        SwitchInit = true;
    }
    
    // and tell INDI to do this again ...
    SetTimer(POLLMS);
    return;
}

/***************************************************************/
/*  GetProperties                                              */
/***************************************************************/
void kobsRelay::ISGetProperties(const char *dev)
{
	INDI::DefaultDevice::ISGetProperties(dev);
    loadConfig(true, PortLabelsTP.name);
    loadConfig(true, AuthsTP.name);
}

/***************************************************************/
/*  saveConfigItems                                            */
/***************************************************************/
bool kobsRelay::saveConfigItems(FILE *fp)
{
	IUSaveConfigText(fp, &PortLabelsTP);
    IUSaveConfigText(fp, &AuthsTP);
	return true;
}
