#include <iostream>
#include <fstream>
#include <string>

#define NUMBER_OF_CUSTOMERS 20
#define NUMBER_OF_RESOURCES 4

int available[NUMBER_OF_RESOURCES] = { 11,10,12,13 };
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = { 0 };
int max[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = { 0 };
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = { 0 };

int request_resources(int, int, int, int, int);
void release_resources(int,int,int,int,int);
void calculate_need();
void fill_maximum();
void display_menu();
void print_all();

int main()
{
	fill_maximum();
	display_menu();
    
	return 0;
}

void calculate_need()
{
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
		for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
		{
			need[i][j] = max[i][j] - allocation[i][j];
		}
	}
}

int request_resources(int customer_num, int r1, int r2, int r3, int r4)
{
	// Set work = available
	int work[NUMBER_OF_RESOURCES];
	memcpy(work, available, sizeof(work));

	//Subtract request from available to test against other customer needs
	work[0] -= r1;
	work[1] -= r2;
	work[2] -= r3;
	work[3] -= r4;


	bool finish[NUMBER_OF_CUSTOMERS]{ false };
	calculate_need();

	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
		for (int j = 0; j < NUMBER_OF_CUSTOMERS; j++) //j = customer number
		{
			if (finish[j] == false && need[j][0] <= work[0] && need[j][1] <= work[1] && need[j][2] <= work[2] && need[j][3] <= work[3]) 
			{
				//if customer j has still not finished and his need is less than work (which is the amount of available + any other completed customers), mark finished as true and increase work with j's resources
				work[0] += allocation[j][0];
				work[1] += allocation[j][1];
				work[2] += allocation[j][2];
				work[3] += allocation[j][3];
				finish[j] = true;
			}
			
		}
	}

	//check to see if all of finished = true 
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
		if (finish[i] == false) 
		{ 
			std::cout << "Request denied. Customer " << i << " is not satisfied.\n";
			std::cout << "Available remains as: " << available[0] << "," << available[1] << "," << available[2] << "," << available[3] << "\n";
			return 0; 
		}
	}

	//return 1 if all customers passed test (finsh == true)
	std::cout << "Request granted\n";

	allocation[customer_num][0] += r1;
	allocation[customer_num][1] += r2;
	allocation[customer_num][2] += r3;
	allocation[customer_num][3] += r4;
	available[0] -= r1;
	available[1] -= r2;
	available[2] -= r3;
	available[3] -= r4;

	std::cout << "New available is: " << available[0] << "," << available[1] << "," << available[2] << "," << available[3] << "\n";

	return 1;
}

void fill_maximum()
{
	std::ifstream fin;
	fin.open("max.txt");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
		for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
		{
			fin >> max[i][j];
			fin.ignore();
		}
	}
}

void release_resources(int customer_num, int r1, int r2, int r3, int r4)
{
	if (allocation[customer_num][0] < r1 || allocation[customer_num][1] < r2 || allocation[customer_num][2] < r3 || allocation[customer_num][3] < r4)
	{
		std::cout << "Customer " << customer_num << " does not have enough resources to release that amount. Denied.\n";
		return;
	}

	//Decrease allocatoin of customer
	allocation[customer_num][0] -= r1;
	allocation[customer_num][1] -= r2;
	allocation[customer_num][2] -= r3;
	allocation[customer_num][3] -= r4;

	//Increase global amount of available by funds released 
	available[0] += r1;
	available[1] += r2;
	available[2] += r3;
	available[3] += r4;

	std::cout << "Resources released\nNew available is " << available[0] << "," << available[1] << "," << available[2] << "," << available[3] << "\n";
}

void display_menu()
{
	std::string user_input = "";
	int customer_number, r1, r2, r3, r4 = 0;

	do
	{
		std::cin >> user_input;
		
		if (user_input == "RQ")
		{
			std::cin >> customer_number >> r1 >> r2 >> r3 >> r4;
			request_resources(customer_number, r1, r2, r3, r4);
		}
		else if (user_input == "RL")
		{
			std::cin>> customer_number >> r1 >> r2 >> r3 >> r4;
			release_resources(customer_number, r1, r2, r3, r4);
		}
		else if (user_input == "*")
		{
			print_all();
		}

	} while (user_input != "q");
}

void print_all()
{
	std::cout << "Amount available is: (" << available[0] << "," << available[1] << "," << available[2] << "," << available[3] << ")\n";
	std::cout << "\nMaximum Array\n";

	for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
		for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
		{
			std::cout << max[i][j] << " ";
		}
		std::cout << "\n";
	}

	std::cout << "\nAllocation Array\n";

	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
		for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
		{
			std::cout << allocation[i][j] << " ";
		}
		std::cout << "\n";
	}

	std::cout << "\nNeed Array\n";

	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
		for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
		{
			std::cout << need[i][j] << " ";
		}
		std::cout << "\n";
	}
}