#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h> 

#include <iostream>
#include <stdlib.h> 
#include <agentModel.h>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <netinet/in.h>

#include <pthread.h>
#include <fstream>


using namespace std;

int CreateSocket(int, char*);
void *listen(void *);


class Neighbor
{
    public:
	int socketReceive;
	int socketSend;
	int PORTS;
	int PORTR;
	double receivedOut;
	struct sockaddr_in address;
	char* IP;
};

class Receiver
{
	public:
    Neighbor neighbors[4];
	bool flag;
	int numberNbrs;
	char* ownIP;
};

int main(int argc,char* argv[])
{
    int numberNbrs = atoi(argv[1]);
    
    clock_t startTime;
    double duration;
    struct timespec req = {0};
    double pause = 0;
    double T = 0.02;
    double tSimulation = 50;
    double count = tSimulation/T;
	double timePassed = 0;
	
	struct sockaddr_in address[4] ;

	double sendInfo = 0;
	double u = 0;
	
	//create and initialize receiver class
	Receiver receiver;
	receiver.flag = true;
	receiver.numberNbrs =numberNbrs;
	for(int i = 0; i<4 ; i++)
	{
	        receiver.neighbors[i].PORTS =0;
	        receiver.neighbors[i].PORTR = 0;
	        receiver.neighbors[i].socketReceive = 0;
	        receiver.neighbors[i].socketSend = 0;
	        receiver.neighbors[i].receivedOut = 0;
	}
	
	char* file_name;
	
	
	if (argc < (3*numberNbrs)+4)
	{
	    cout<<"Not enough input arguments"<<endl;
	    return -1;
	}
	else
	{
	    receiver.ownIP = argv[2];
	    file_name = argv[(3*numberNbrs)+3];   
	    
	    for(int i = 0; i<numberNbrs ; i++)
	    {
	        receiver.neighbors[i].IP = argv[3*(i+1)];
	        receiver.neighbors[i].PORTS = atoi((argv[3*(i+1)+1]));
	        receiver.neighbors[i].PORTR = atoi((argv[3*(i+1)+2]));
	    }
	}
	
	
	pthread_t listeningThread;
	ofstream fileHandler;
	
	//create socket to send info
	for(int i = 0; i<numberNbrs ; i++)
	{
	    receiver.neighbors[i].socketSend = CreateSocket(receiver.neighbors[i].PORTS, receiver.ownIP);
	    
	    memset( &receiver.neighbors[i].address, 0, sizeof(receiver.neighbors[i].address) ) ;
	    receiver.neighbors[i].address.sin_family      = AF_INET ;
	    receiver.neighbors[i].address.sin_addr.s_addr = inet_addr(receiver.neighbors[i].IP) ;
	    receiver.neighbors[i].address.sin_port        = htons( receiver.neighbors[i].PORTS ) ;
	}
   
	//initialize agent and second thread
//	double numberReceived = 0;
    agentModel_initialize();
	if(pthread_create(&listeningThread, NULL, listen, &receiver) ) //begin seconf thread that listens and changed value of numberReceived
	{
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	
    cout << "Agent initialized" << endl;
	
	sendInfo = 1;
	for(int i = 0; i<numberNbrs ; i++)
	{
	    sendto( receiver.neighbors[i].socketSend, &sendInfo, sizeof(double), 0, (const sockaddr *)&receiver.neighbors[i].address, sizeof(receiver.neighbors[i].address) ) ;
	}
	while ((receiver.neighbors[0].socketSend+receiver.neighbors[1].socketSend+receiver.neighbors[2].socketSend+receiver.neighbors[3].socketSend) != numberNbrs)
	{	
	}
	for(int i = 0; i<numberNbrs ; i++)
	{
	   receiver.neighbors[i].receivedOut = 0;
	}
//	receiver.receivedN1 = 0; 
//	receiver.receivedN2 = 0;
	
	sendInfo = 0;
	//open output file and begin dynamics
	fileHandler.open(file_name);
    for(int i = 0; i < count; i++)
    {
		//start clock
		startTime = clock();
		
		u = -2*agentModel_Y.Output + receiver.neighbors[0].receivedOut + receiver.neighbors[1].receivedOut;
		//set as input value of the system numberReceived and executes one time step
        agentModel_U.Input = u;
		agentModel_step(1);
		
		//prints value of number received and of own calculated state
		//cout<< *receivedN1<< "*" <<*receivedN2<< "*" << agentModel_Y.Out1 <<"\n";
		//cout<< receiver.receivedN1<< "*" <<receiver.receivedN2<< "*" << agentModel_Y.Out1 <<"\n";
		
		//sends information of own state
		sendInfo = agentModel_Y.Output;		
		for(int i = 0; i<numberNbrs ; i++)
	    {
	     sendto( receiver.neighbors[i].socketSend, &sendInfo, sizeof(double), 0, (const sockaddr *)&receiver.neighbors[i].address, sizeof(receiver.neighbors[i].address) ) ;
	    }
		//log info on Output.txt: t numberReceived output
		timePassed = i*T;
		fileHandler << timePassed  << "\t"<< receiver.neighbors[0].receivedOut << "\t" <<receiver.neighbors[1].receivedOut << "\t" <<receiver.neighbors[2].receivedOut << "\t" << receiver.neighbors[2].receivedOut << "\t" << sendInfo << "\n";
		
		//if duration of loop smaller than T, waits until equal T
		duration = ( std::clock() - startTime ) / (double) CLOCKS_PER_SEC;
		pause = T-duration;
		if(pause >= 0)
		{
			usleep(pause*1000000);
		}
		else{
			cout<<"Sampling time exceeded"<<endl;
		}
	}
	
	//signalize end of simulation, close socket and file
	receiver.flag = false;
	for(int i = 0; i<numberNbrs ; i++)
    {
	   close( receiver.neighbors[i].socketSend );
	}
	fileHandler.close();
	
	//wait for end of second thread
	cout << "Waiting to stop listening" << endl;
	if(pthread_join(listeningThread, NULL)) 
	{
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}
	cout << "Agent Finilized" << endl;
	
	return 0 ;
}

//function to create socket, receives as input the disered port
int CreateSocket(int port, char* ownIP)
{
    // Create a socket
    int sock = socket( AF_INET, SOCK_DGRAM, 0 ) ;
    if( sock == -1 )
    {
       printf( "CreateSocket: socket failed" ) ;
       return -1 ;
    }

    int on = 1 ;
    if( setsockopt(sock,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on)) != 0 )
	{
		printf( "setsockopt failed" ) ;
	}
       
    //Sets local adress
    struct sockaddr_in myAddress ;

    memset( &myAddress, 0, sizeof(myAddress) ) ;
    myAddress.sin_family      = AF_INET ;
    myAddress.sin_addr.s_addr = inet_addr(ownIP) ;
    myAddress.sin_port        = htons( port ) ;
	
	//bind socket to local adress
    if( bind(sock,(struct sockaddr *)&myAddress,sizeof(myAddress)) == -1 )
    {
       printf( "CreateSocket: bind failed" ) ;
       close( sock ) ;
       return -1 ;
    }

    return sock ;
}

//function for listening thread
void *listen(void *mainReceiverVoid)
{
	int    size ;
	int received = 0;
	socklen_t length = 0;

	fd_set master;
	fd_set temp;
	Receiver *mainReceiver =(Receiver*) mainReceiverVoid; 
	
	//creates sockets
	for(int i = 0; i<mainReceiver->numberNbrs ; i++)
	{
	    mainReceiver->neighbors[i].socketReceive = CreateSocket(mainReceiver->neighbors[i].PORTR, mainReceiver->ownIP);
	    
	    //sets listening adress neighbors
	    memset( &mainReceiver->neighbors[i].address, 0, sizeof(mainReceiver->neighbors[i].address) ) ;
	    mainReceiver->neighbors[i].address.sin_family      = AF_INET ;
	    mainReceiver->neighbors[i].address.sin_addr.s_addr = inet_addr(mainReceiver->neighbors[i].IP) ;
    	mainReceiver->neighbors[i].address.sin_port        = htons(  mainReceiver->neighbors[i].PORTR ) ;
	}
	
	//sets timeout
	struct timeval tv;
	tv.tv_sec = 0;//timeout_in_seconds;
	tv.tv_usec = 500000;
	for(int i = 0; i++; i<mainReceiver->numberNbrs)
	{
	    setsockopt(mainReceiver->neighbors[i].socketReceive, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	}
	//set time out select
	struct timeval tvSelect;
	tvSelect.tv_sec = 1;
	tvSelect.tv_usec = 0 ;

	//clear master and temp, add socket to master set
	FD_ZERO(&master);
	FD_ZERO(&temp);
	for(int i = 0; i<mainReceiver->numberNbrs ; i++)
	{
	    FD_SET(mainReceiver->neighbors[i].socketReceive, &master);
	}

	
	//initializes listening
	cout<<"Listening begin"<<endl;
	// get maxfd
	int max1 = max(mainReceiver->neighbors[0].socketReceive,mainReceiver->neighbors[1].socketReceive);
	int max2 = max(mainReceiver->neighbors[2].socketReceive,mainReceiver->neighbors[3].socketReceive);
   int maxfdp1 = max(max1,max2) + 1;
   
	while( mainReceiver->flag)
	{
		temp = master;
		
		select(maxfdp1, &temp, NULL, NULL, &tvSelect);
		
		for(int i = 0; i++; i<mainReceiver->numberNbrs)
	    {
	        if(FD_ISSET(mainReceiver->neighbors[i].socketReceive,&temp))
		    {
		        length = sizeof(mainReceiver->neighbors[i].address ); 
		        size = recvfrom( mainReceiver->neighbors[i].socketReceive, &mainReceiver->neighbors[i].receivedOut, sizeof(double), 0, (struct sockaddr *)&mainReceiver->neighbors[i].address, &length ) ;
		    }
	    }
	    
		FD_ZERO(&temp);
	}
	   
	//end of listening, close socket
	cout<<"Listening ended"<<endl;
	for(int i = 0; i<mainReceiver->numberNbrs ; i++)
	{
	   close(mainReceiver->neighbors[i].socketReceive) ;
	}
	
	return NULL;
}
