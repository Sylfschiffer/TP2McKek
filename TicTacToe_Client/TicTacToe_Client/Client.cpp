#include "Client.h"
#include <stdio.h>
//#include <iostream>
#include <fstream>

using namespace std;

//D�finit la grandeur du buffer
#define BUFF_SIZE 9

//D�finit le message d'erreur
#define PRINTERROR(s)	cout<<"\n"<<s<<":"<<WSAGetLastError()

Client::Client()
{
	nbPort = 1234;
	server = "127.0.0.1";
	flag = 0;
}

void Client::Test()
{
}

void Client::Connect()
{
	WORD wsVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	short numPort = 5214; //Mettre le num�ro de port � 5214
	int nRet;

	nRet = WSAStartup(wsVersionRequested, &wsaData);
	if (wsaData.wVersion != wsVersionRequested)
	{
		cout << "Mauvaise version";
		return;
	}

	char sizeBuff[BUFF_SIZE];

	cout << "Le client se connecte au serveur :" << server << " sur le port " << nbPort << endl;

	//Aller chercher le serveur
	LPHOSTENT lphostent;

	lphostent = gethostbyname(server);
	if (lphostent == NULL)
	{
		PRINTERROR("gethostbyname()");
		return;
	}

	socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socketClient == INVALID_SOCKET)
	{
		PRINTERROR("socket()");
		return;
	}

	//Remplir la structure d'adresse
	SOCKADDR_IN socAdrServeur;

	socAdrServeur.sin_family = AF_INET;
	socAdrServeur.sin_addr = *((LPIN_ADDR)*lphostent->h_addr_list);
	socAdrServeur.sin_port = htons(nbPort);

	// Le client se connecte au serveur


	nRet = connect(socketClient, (LPSOCKADDR)&socAdrServeur, sizeof(struct sockaddr));

	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("socket()");
		closesocket(socketClient);
		return;
	}
}

void Client::SendData(char* data)
{
	int nRet;
	nRet = send(Client::socketClient, data, BUFF_SIZE, 0);
	cout << "im sending: " << data << endl;

	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("send()");
		return;
	}
}

string Client::ReceiveData()
{
	char data[BUFF_SIZE];
	int nRet;
	nRet = recv(Client::socketClient, data, BUFF_SIZE,0);
	if (nRet != SOCKET_ERROR)
		cout << "I received : " << data << endl;
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("recv()");
		//return "error";
	}

	string returnValue(data);
	return returnValue;
}
