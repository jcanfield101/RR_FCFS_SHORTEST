//Jonathan Canfield
#include <iostream>
#include <fstream>
#include <list>
using namespace std;

//struct used for the input queue
struct records{
	int arriv, pid, cpu, priority;
};
//struct used for the ready queue
struct pcb{
	int pid, cpu, priority;
};
//struct used for the active process
struct active{
	int pid, cpu, priority, startTime, duration;
};
//Function to initialize the input queue with the data from the file
void initInputQ(const char*file,list<records>&inputQ){
	ifstream infile(file);										//open file
	records newRecord;											//create new struct for data
	//Check to make sure file was opened properly
	if(!infile){													
		cout << "There was a problem opening: " << file
			  << "." << endl;
	}
	//File is open. Read from file and store in input queue
	else{
		int a, x;
		while(!infile.eof()){
			for( x=0; x<4; x++ ){
				infile >> a;
				if( x==0 )
					newRecord.arriv = a;
				else if( x==1 )
					newRecord.pid = a;
				else if( x==2 )
					newRecord.cpu = a;
				else if( x==3 )
					newRecord.priority = a;
				else
					cout << "There was an error" << endl;
			}
			inputQ.push_back(newRecord);
		}
		infile.close();
	}
}

bool compare_cpu(const pcb& a, const pcb& b){
	if(a.cpu < b.cpu)
		return true;
	else
		return false;
}

void shortest(list<records> *inputQ){
	pcb newPcb;
	active activeP;
	bool activeProcess = false;
	list<pcb> readyQ;
	list<records>::iterator i;
	list<pcb>::iterator r;
	i = inputQ->begin();
	r = readyQ.begin();
	int clock = 0;
	do{
		//Process input queue
		while( clock == i->arriv ){
			newPcb.pid = i->pid;
			newPcb.cpu = i->cpu;
			newPcb.priority = i->priority;
			readyQ.push_back(newPcb);
			inputQ->pop_front();
			i = inputQ->begin();
		}
		//When there is no active process, a new one must be created.
		//The new active process is taken from the front of the
		//ready queue and adds the start time and duration counter
		//to a new struct.
		if( !activeProcess && !readyQ.empty() ){
			r = readyQ.begin();
			activeP.pid = r->pid;
			activeP.cpu = r->cpu;
			activeP.priority = r->priority;
			activeP.startTime = clock;
			activeP.duration = 0;
			activeProcess = true;
			//cout << "Made a new process" << endl;
		}
		clock++;
		if( activeProcess ){
			if( activeP.cpu > 0 ){
				activeP.cpu--;
				activeP.duration++;
			}
			if( activeP.cpu == 0 ){
				cout << "Process pid: " << activeP.pid
					<< " | Terminated: " << clock
					<< " time units." << endl;
				readyQ.pop_front();
				readyQ.sort(compare_cpu);
				activeProcess = false;
			}
		}
	
	} while( !readyQ.empty() );
}
//First Come First Served Scheduler
void fcfs(list<records> *inputQ){
	pcb newPcb;
	active activeP;
	bool activeProcess = false;
	list<pcb> readyQ;
	list<records>::iterator i;
	list<pcb>::iterator r;
	i = inputQ->begin();
	r = readyQ.begin();
	int clock = 0;
	do{
		//Process input queue
		while( clock == i->arriv ){
			newPcb.pid = i->pid;
			newPcb.cpu = i->cpu;
			newPcb.priority = i->priority;
			readyQ.push_back(newPcb);
			inputQ->pop_front();
			i = inputQ->begin();
		}
		//When there is no active process, a new one must be created.
		//The new active process is taken from the front of the
		//ready queue and adds the start time and duration counter
		//to a new struct.
		if( !activeProcess && !readyQ.empty() ){
			r = readyQ.begin();
			activeP.pid = r->pid;
			activeP.cpu = r->cpu;
			activeP.priority = r->priority;
			activeP.startTime = clock;
			activeP.duration = 0;
			activeProcess = true;
			//cout << "Made a new process" << endl;
		}
		clock++;
		if( activeProcess ){
			if( activeP.cpu > 0 ){
				activeP.cpu--;
				activeP.duration++;
			}
			if( activeP.cpu == 0 ){
				cout << "Process pid: " << activeP.pid
					<< " | Terminated: " << clock
					<< " time units." << endl;
				readyQ.pop_front();
				//sort readyQ here
				activeProcess = false;
			}
		}
	
	} while( !readyQ.empty() );
}
//Round Robin scheduler
void roundRobin(list<records> *inputQ, int quantum){
	int tmp;
	pcb newPcb;
	active activeP;
	list<pcb> readyQ;
	list<pcb>::iterator r;
	list<records>::iterator i;
	i = inputQ->begin();
	int clock = 0;
	bool activeProcess = false;
	//Simulation of Round Robbin
	do{
	
		while( i->arriv <= clock ){
			newPcb.pid = i->pid;
			newPcb.cpu = i->cpu;
			newPcb.priority = i->priority;
			readyQ.push_back(newPcb);
			inputQ->pop_front();
			i = inputQ->begin();
		}
		r = readyQ.begin();
		//create new active process from top of ready queue
		if( !activeProcess && !readyQ.empty() ){
			activeP.pid = r->pid;
			activeP.cpu = r->cpu;
			activeP.priority = r->priority;
			activeP.startTime = clock;
			activeP.duration = 0;
			activeProcess = true;
		}
		if(activeP.cpu < quantum)
			tmp = activeP.cpu;
		else
			tmp = quantum;
		clock += tmp;
		if( activeProcess){
			if( activeP.cpu > 0 ){
				tmp = quantum;
				if(activeP.cpu <= quantum)
					tmp = activeP.cpu;
				activeP.cpu -= tmp;
				r->cpu -= tmp;
				readyQ.splice(readyQ.end(), readyQ, readyQ.begin());
				activeProcess = false;
			}
			if(activeP.cpu == 0 ){
				cout << "Process pid: " << activeP.pid
			  		  << " | Terminated: " << clock
					  << " time units." << endl;
				readyQ.pop_back();		//deleting end to prevent the process with 0 left to be counted again.
				activeProcess = false;
			}
		}
	}while( !readyQ.empty() );
}

int main(){
	//Initialization of values.
	const char *fileName = "input.txt";		//file with data for testing
	int choice, quantum;
	pcb newPcb;								//struct for PCB
	active activeP;						    //struct for active process
	list<records> inputQ;				    //input queue
	list<pcb> readyQ;						//ready queue
	initInputQ(fileName,inputQ);		   //read data from file and initialize queue
	
	cout << "\n1. First Come First Served" << endl
		  << "2. Round Robin" << endl
		  << "3. Shortest" << endl
		  << "Please enter your choice of scheduler: ";
	cin >> choice;
	
	if( choice == 1 )
		fcfs(&inputQ);
	else if( choice == 2 ){
		cout << "Please enter time quantum: ";
		cin.ignore();
		cin >> quantum;
		roundRobin(&inputQ,quantum);
	}
	else if( choice == 3 ){
		shortest(&inputQ);
	}
	else
		cout << "Error: User entered incorrect response." << endl;
	
	return(0);
}

