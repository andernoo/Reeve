#include "Kernel.h"
#include "Utils.h"
#include "Config.h"

#include "SocketHandler.h"

#include "HTTPProcessor.h"
#include "Stream.h"
#include "Main.h"
#include "../version.h"
#include <csignal>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <strstream>
#include <iterator>
#include <algorithm>
#include <stdio.h>

using namespace std;

long totalTime = 0;

bool	serverRunning		= true;

Kernel 	*kernel				= NULL;

bool 	bootstrap			= true;
bool	convert				= false;
bool	debug				= false;
bool	trace				= false;
int		maxRecursiveDepth	= 32;
string	botName				= "Reeve";
string	connectPattern		= "CONNECT";
string	noMatchPattern		= "NOMATCH";
string	loopPattern			= "LOSTINLOOP";

HttpConfig			httpConfig;

void loadConfig();
void run();
void clean(string &);
void process(string &, int, string &, string &);

void signalHandler( int signum )
{
    cout << "Segmentation Fault (" << signum << ") received."<<endl;
    cleanup_reeve();
    exit(1);
}

void reeveSIGINT( int signum )
{
    cout << "Interrupt recieved."<<endl;
    cleanup_reeve();
    exit(1);
}

int init_reeve(int argc, char **argv)
{
    cout<<"initialising Reeve "<<AutoVersion::FULLVERSION_STRING<<endl;

    kernel = new Kernel();

    loadConfig();
    string aimlset="main";
    if (argc > 0)
    {
        string s;
        for(int i=0; i<argc; i++)
        {
            s = argv[i];
            if(s=="-aiml")
            {
                aimlset=argv[++i];
                i++;
            }
        }
    }
cout<<"AIMLSET="<<aimlset.c_str()<<endl;
    kernel->loadAIML(aimlset);

    char str[1024];
    sprintf(str, "Total time to startup: %dms\n",  totalTime );
    getStream("Console")->Write(str);

    return 0;
}

#ifndef GUI
int main(int argc, char **argv)
{
    signal(SIGSEGV, signalHandler);
    signal(SIGINT, reeveSIGINT);
    init_reeve(argc, argv);

    run_reeve();
    cleanup_reeve();

    cout << "\nReeve has been shutdown" << endl;
}
#endif

void clean(string &input)
{
    string::size_type ix = input.find_first_of(";\n\t\r");
    if (ix == string::npos)
    {
        return;
    }
    input = input.erase(ix);
    ix = input.find_last_not_of(" ");
    input = input.erase(ix + 1);
}

void loadConfig()
{
    ifstream fin("reeve.ini", ios::binary | ios::in);
    if (!fin.is_open())
    {
        return;
    }
    string line, section, subsection, property, value;
    int ircsection = -1;
    while (!fin.eof())
    {
        getline(fin, line);
        clean(line);
        if (line.length() > 0)
        {
            if (line[0] == '[')
            {
                section = line.substr(1, line.length() - 2);
                subsection = "";
            }
            else
            {
                string::size_type st = line.find("=");
                property = line.substr(0, st);
                value = line.substr(st + 1);
                process(section, ircsection, property, value);
            }
        }
    }
    fin.close();
}

void process(string &s, int ss, string &p, string &v)
{
    if (p == "MaxRecursiveDepth")
    {
        maxRecursiveDepth = atoi(v.c_str());
    }
    else if (p == "BootstrapEnabled")
    {
        bootstrap = v == "true";
    }
    else if (p == "BotName")
    {
        botName = v;
    }
    else if (p == "ConnectPattern")
    {
        connectPattern = v;
    }
    else if (p == "NoMatchPattern")
    {
        noMatchPattern = v;
    }
    else if (p == "LoopPattern")
    {
        loopPattern = v;
    }
    else if (p == "Enabled")
    {
        bool enabled = v == "true";
        if (s == "Web Server")
        {
            httpConfig.enabled = enabled;
        }
    }
    else if (p == "Port")
    {
        int port = atoi(v.c_str());
        if (s == "Web Server")
        {
            httpConfig.port = port;
        }
    }
    else if (p == "TemplateFile")
    {
        httpConfig.templateFile = v;
    }
    else if (p == "StartupPattern")
    {

        if (s == "Web Server")
        {
            httpConfig.startupPattern = v;
        }
    }
}

void run_reeve()
{
    //	The Web Server (if enabled) !!! CHANGE TO WEBSOCK
    if (httpConfig.enabled)
    {
        new HTTPServer();
    }

    //	Start the SocketHandler loop
    SocketHandler::runLoop();
    //	Once this loop has finished, bot will shutdown
}

void cleanup_reeve()
{
    serverRunning = false;
    getStream("Console")->Write("Please wait while Reeve exits...");
    if (kernel != NULL)
    {
        delete Kernel::predicates;
        getStream("Console")->Write("Saved predicates");
        delete kernel;
        getStream("Console")->Write("Engine stopped");
        kernel = NULL;
    }
}
