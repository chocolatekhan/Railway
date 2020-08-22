#include<iostream>
using namespace std;
#include "Ticket.h"
#include <fstream>
#include <string.h>

Ticket::Ticket()
{

}

Ticket::Ticket(string custName, string custNum, string dept, int deptPnt, int arrPnt, int tid, int seatCat, int passengers): deptInd(deptPnt), arrInd(arrPnt), trainID(tid), seatCategory(seatCat), seats(passengers)
{
    strncpy(departure, dept.c_str(), sizeof(departure));
    strncpy(name, custName.c_str(), sizeof(name));
    strncpy(number, custNum.c_str(), sizeof(number));
}

void Ticket::store()
{
    fstream openFile;
    openFile.open("\Tickets.dat", ios::app|ios::binary);
    if (!openFile)  //problem reading file; return to main menu
    {
        cout<<"  Unable to open file 'Tickets.dat'.\n"<<endl;
        cout<<"  ";
        system("PAUSE");
        return;
    }
    openFile.write((char*)this, sizeof(*this));
    openFile.close();
}

void Ticket::bookSeats()
{
    Station *tempStation = getStation(departure);
    for (int i=0; i<MAXTRAINS; i++)
    {
        Train *tempTrain = tempStation->getTrain(i);
        if (tempTrain->getID() == trainID)
        {
            for (int j=deptInd; j<arrInd; j++)  tempTrain->occupySeat(seatCategory, seats, j);
        }
    }
}

void Ticket::read(int mode, string custName, string custNum, int tid)
{
    ifstream openFile;
    openFile.open("\Tickets.dat", ios::in|ios::binary);
    if (!openFile && (mode == 1 || mode == 2))
    {
        cout<<"  Unable to open file 'Tickets.dat'.\n"<<endl;
        cout<<"  ";
        system("PAUSE");
        return;
    }
    else if (!openFile)
    {
        cout<<"  Unable to open file 'Stations.txt'.\n  Setup failed.\n  Exiting Program.\n"<<endl;
        exit(1);
    }
    openFile.read((char*)this, sizeof(*this));

    int noResultFlag = 1;
    while (!openFile.eof())
    {
        if (mode == 0)  this->bookSeats();
        if ((mode == 1 && name == custName && number == custNum) || (mode == 2 && trainID == tid))
        {
            string arrSt, deptTime, arrTime;
            float distance;
            Station *tempStation = getStation(departure);
            for (int i=0; i<MAXTRAINS; i++)
            {
                Train *tempTrain = tempStation->getTrain(i);
                if (tempTrain->getID() == trainID)
                {
                    arrSt = tempTrain->getStation(arrInd);
                    deptTime = tempTrain->getDeptTime(deptInd);
                    arrTime = tempTrain->getArrTime(arrInd);
                    distance = tempTrain->getDistanceBetween(deptInd, arrInd);
                }
            }

            string seatCat;
            if (seatCategory == 1)
            {
                seatCat = "Business";
                distance *= 2;
            }
            else if (seatCategory == 2) seatCat = "Economy";

            cout<<"  Name: "<<name<<"\n  Phone Number: "<<number<<"\n  Train ID: "<<trainID;
            cout<<"\n  Departure Point: "<<departure<<"          \tArrival Point: "<<arrSt;
            cout<<"\n  Departure Time: "<<deptTime<<"          \tArrival Time: "<<arrTime;
            cout<<"\n  Seat Category: "<<seatCat<<"\n  Total Seats: "<<seats;
            cout<<"\n  Total Cost: BDT "<<distance*seats<<"\n"<<endl;
            noResultFlag = 0;
        }
        openFile.read((char*)this, sizeof(*this));
    }
    if (noResultFlag)  cout<<"  No available bookings.\n"<<endl;
    openFile.close();
}
