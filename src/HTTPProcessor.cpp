#include "HTTPProcessor.h"
#include <fstream>
#include "ServerSocket.h"
#include "Socket.h"
#include "SocketHandler.h"

#include <stdlib.h>
#include <string>
#include <unistd.h>

#include "Kernel.h"
#include "TokenProxyListener.h"
#include "Config.h"

#include "Responder.h"

#include "Utils.h"
#include "StringTokenizer.h"
#include "Stream.h"
#include "websocket.h"
#include <iostream>

using namespace std;

string templateContents;

class HTTPListener : public SocketListener, Responder
{
public:
    HTTPListener(Socket *s)
    {

        client = s;
        int n = ++numClients;
        while (n > 0)
        {
            char ch = (char)(n % 10) + '0';
            uid = ch + uid;
            n /= 10;
        }
        uid = "Web Client " + uid;
        SocketHandler::addSocket(uid, client);
    }
    virtual ~HTTPListener()
    {
    }
    void recv(string &s);
    void disconnected(const string &)
    {
        //	Needed here as well, for cases when recv() doesn't get called...
        SocketHandler::removeSocket(uid);
        --HTTPListener::numClients;
    }
    Socket *getSocket()
    {
        return client;
    }
    void connected()
    {
        cout << "Client connected" << endl;
    }
    string respond(Match *, PElement e, const string &)
    {
        if (e->getNamespace() == "http")
        {
            string tag = e->getTagname();
            if (tag == "quit")
            {
                serverRunning = false;	//	Natural quit
            }
        }
        return "";
    }
private:
    Socket *client;
    string uid;
    static int numClients;
};

string HTTPProcessor::process(Match *, PElement, Responder *, const string &)
{
    /*	string name = Kernel::process(m, e->getChild("name"), r, id);
    	int port = atoi(Kernel::process(m, e->getChild("port"), r, id).c_str());

    	ServerSocket *server = new ServerSocket(port);
    	SocketHandler::addSocket(name, server);
    	(new HTTPServer(server))->setUID(name);*/
    return "starting web server from AIML is currently unsupported";
}

HTTPServer::HTTPServer()
{
    server = new ServerSocket(httpConfig.port);
    SocketHandler::addSocket("Web Server", server);
    ifstream fin(httpConfig.templateFile.c_str());
    if (fin.is_open())
    {
        string line;
        while (!fin.eof())
        {
            getline(fin, line);
            templateContents += line + "\n";
        }
        fin.close();
    }
    server->setServerListener(this);
    if (server->init())
    {
        char str[1024];
        sprintf(str, "Starting up server (listening on port %d)\n", httpConfig.port);
        getStream("Console")->Write(str);
    }	//	No else - will display a shutdown message via server->init()
}

void HTTPServer::shutdown(const string &msg)
{
    cout << "Shutting down server: " << msg << endl;
    string err = "Shutting down server: " + msg + "\n";
    getStream("Console")->Write(err.c_str());
}

void HTTPServer::awaitingClient(Socket* socket)
{
    //cout << "HTTPServer::awaitingClient(" << socket->getSD() <<  ")" << endl << flush;
    socket->setListener(new HTTPListener(socket));
//	socket->process();
}

int HTTPListener::numClients = 0;

void HTTPListener::recv(string &s)
{
    string id = client->getPeerName();
    client->getPeerName();
    int sd=client->getSD();
    if(!WebSocketServer::isWS(sd))
    {
        return;
    }
    string response = Kernel::respond(s, id, this);

    WebSocketServer::sendMessage(sd, response);
}
