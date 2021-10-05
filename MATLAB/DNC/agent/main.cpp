#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h> //UDP coomunication
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

//class neighbor that stores communication specifications
class Neighbor
{
    public:
	int socketReceive;
	int socketSend;
	int PORTS;
	int PORTR;
	double receivedOut;
	struct sockaddr_in addressSend;
	struct sockaddr_in addressReceive;
	char* IP;
};

//class receiver is passed to the secondary (receiving) thread
class Receiver
{
	public:
    Neighbor neighbors[4]; //size of array = #pis - 1
	bool flag;
	int numberNbrs;
	char* ownIP;
};

int main(int argc,char* argv[])
{
    int numberNbrs = atoi(argv[1]); //get number of neighbors from argument
 
	//initialization of simulation variables
    clock_t startTime;
    double duration;
    struct timespec req = {0};
    double pause = 0;
    double T = 0.02;
    double tSimulation = 50;
    double count = tSimulation/T;
    double timePassed = 0;
	double u = 0;
    
	//initialization of communication and output variables
    double sendInfo = 0;
    socklen_t length = 0;
	char* file_name;
	ofstream fileHandler;
	
	//create and initialize receiver class
	Receiver receiver;
	receiver.flag = true;
	receiver.numberNbrs = numberNbrs;
	for(int i = 0; i<numberNbrs ; i++)
	{
	        receiver.neighbors[i].PORTS =0;
	        receiver.neighbors[i].PORTR = 0;
	        receiver.neighbors[i].socketReceive = 0;
	        receiver.neighbors[i].socketSend = 0;
	        receiver.neighbors[i].receivedOut = 0;
	}
	
	
	//pass program arguments to designated variables
	if (argc != (3*numberNbrs)+4)
	{
	    cout<<"Wrong number of arguments"<<endl;
	    return -1;
	}
	else
	{
	    receiver.ownIP = argv[2];
	    file_name = argv[(3*numberNbrs)+3];   
	    
	    for(int i = 0; i<numberNbrs ; i++)
	    {
	        receiver.neighbors[i].IP = argv[3*(i+1)];
	        receiver.neighbors[i].PORTR = atoi((argv[3*(i+1)+1]));
	        receiver.neighbors[i].PORTS = atoi((argv[3*(i+1)+2]));
	    }
	}	
	
	//create send socket for all neighbors
	for(int i = 0; i<numberNbrs ; i++)
	{
	    receiver.neighbors[i].socketSend = CreateSocket(receiver.neighbors[i].PORTS, receiver.ownIP);
	    
	    memset( &receiver.neighbors[i].addressSend, 0, sizeof(receiver.neighbors[i].addressSend) ) ;
	    receiver.neighbors[i].addressSend.sin_family      = AF_INET ;
	    receiver.neighbors[i].addressSend.sin_addr.s_addr = inet_addr(receiver.neighbors[i].IP) ;
	    receiver.neighbors[i].addressSend.sin_port        = htons( receiver.neighbors[i].PORTS ) ;
	    cout << receiver.neighbors[i].socketSend << '\t' << receiver.neighbors[i].IP << '\t' << receiver.neighbors[i].PORTS << '\n';
	}
   
	//initialize second thread that listens and changed value of numberReceived from all neighbors
	pthread_t listeningThread;
    agentModel_initialize();
	if(pthread_create(&listeningThread, NULL, listen, &receiver) )
	{
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	
    cout << "Agent initialized" << endl;
	
	//open output file and begin simulation
	fileHandler.open(file_name);
    for(int i = 0; i < count; i++)
    {
		//start clock
		startTime = clock();
		
		//your code goes here


		
		//pass control variable as input to the plant
		agentModel_U.Input = u;
		//executes one simulation step
		agentModel_step(1);
		
		//prints output value of neighbors and own calculated output
		for(int i = 0; i<numberNbrs ; i++)
		{
		cout<< receiver.neighbors[0].receivedOut<<"\t";
		}
		cout<<agentModel_Y.Output <<"\n";
		
		//sends information of own state to neighbors
		sendInfo = agentModel_Y.Output;		
		for(int i = 0; i<numberNbrs ; i++)
		{
		length = sizeof(receiver.neighbors[i].addressSend ); 
		sendto( receiver.neighbors[i].socketSend, &sendInfo, sizeof(double), 0, (const sockaddr *)&receiver.neighbors[i].addressSend, sizeof(receiver.neighbors[i].addressSend) ) ;
		}
		
		//log info into output file
		timePassed = i*T;
		fileHandler<< timePassed << '\t'; //simulatio time
		for(int i = 0; i<numberNbrs ; i++)
		{
		fileHandler<< receiver.neighbors[0].receivedOut<<"\t"; //neighbors output
		}
		fileHandler<<agentModel_Y.Output <<"\n"; //own output
		
		//if duration of loop smaller than the sampling time T, waits until sampling time reached
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
	
	//signalize end of simulation, close sockets and file
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
	//initializations
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
	    
	    //sets listening adress from neighbors
	    memset( &mainReceiver->neighbors[i].addressReceive, 0, sizeof(mainReceiver->neighbors[i].addressReceive) ) ;
	    mainReceiver->neighbors[i].addressReceive.sin_family      = AF_INET ;
	    mainReceiver->neighbors[i].addressReceive.sin_addr.s_addr = inet_addr(mainReceiver->neighbors[i].IP) ;
	    mainReceiver->neighbors[i].addressReceive.sin_port        = htons(  mainReceiver->neighbors[i].PORTR ) ;
	    cout << mainReceiver->neighbors[i].socketReceive << '\t' << mainReceiver->neighbors[i].IP << '\t' << mainReceiver->neighbors[i].PORTR  << '\t' << mainReceiver->neighbors[i].receivedOut << '\n';
	}
	
	//sets timeout
	struct timeval tv;
	tv.tv_sec = 0;//timeout_in_seconds;
	tv.tv_usec = 500000;
	for(int i = 0; i<mainReceiver->numberNbrs ; i++)
	{
	    setsockopt(mainReceiver->neighbors[i].socketReceive, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	}
	//set time out of select function (line 295)
	struct timeval tvSelect;
	tvSelect.tv_sec = 1;
	tvSelect.tv_usec = 0 ;

	//clear master and temp set, add sockets to master set. This sets are used by the select function (line 295)
	FD_ZERO(&master);
	FD_ZERO(&temp);
	for(int i = 0; i<mainReceiver->numberNbrs ; i++)
	{
	    FD_SET(mainReceiver->neighbors[i].socketReceive, &master);
	}

	// get maxfd (necessary for select function, line 295)
	int max_temp = 0;
	for(int i = 0; i<mainReceiver->numberNbrs ; i++)
	{
	    max_temp = max(max_temp,mainReceiver->neighbors[i].socketReceive);
	}
	int maxfdp1 = max_temp + 1;
   
	//initializes listening
	cout<<"Listening begin"<<endl;
	while( mainReceiver->flag)
	{
		temp = master; 
		
		select(maxfdp1, &temp, NULL, NULL, &tvSelect); //receives a set of sockets and modify it to only contain the ones that are ready to be read from (have new message)
		
		for(int i = 0; i<mainReceiver->numberNbrs ; i++)
		{
	        if(FD_ISSET(mainReceiver->neighbors[i].socketReceive,&temp)) //checks if corresponding socket is in read to be read set
			{
				//receive information
		        length = sizeof(mainReceiver->neighbors[i].addressReceive ); 
		        size = recvfrom( mainReceiver->neighbors[i].socketReceive, &(mainReceiver->neighbors[i].receivedOut), sizeof(double), 0, (struct sockaddr *)&mainReceiver->neighbors[i].addressReceive, &length ) ;
		    }
		}
	    
		FD_ZERO(&temp); //clean temp set
	}
	   
	//end of listening, close sockets
	cout<<"Listening ended"<<endl;
	for(int i = 0; i<mainReceiver->numberNbrs ; i++)
	{
	   close(mainReceiver->neighbors[i].socketReceive) ;
	}
	
	return NULL;
}
