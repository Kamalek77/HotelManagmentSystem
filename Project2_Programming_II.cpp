#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstring>
#include<string>
#include<regex>
#include<ctime>
#include<stdio.h>
#include "sha1.hpp"

using namespace std;

struct date
{
	int day, month, year;
};
struct room
{
	int num;
	string address, type;
	double price;
	int number_of_options;
	string* option;//this field indicates if the room has additional features such as: free cancellation, free wifi, etc.
	date start_date;
	date end_date;
};
struct client
{
	int ID;
	string firstname, lastname, password, address, tel;
	room* r;
	char position;
};

string Hashed(string);

bool Error_Password(string);
bool Error_Email(string);
bool Error_Phone(string);
bool Admin(string[]);
bool Room_Availability(int);
bool Log_In(char&);

void Welcome_Message();
void Room_Managment();
void Sign_UP();
void Add_room();
void Modify_room();
void Delete_room();
void Temporary(int);
void reservation();
void Client_Managment();
void cancellation();


int Numofrooms(fstream&);

int main(void)
{
	srand(time(0));
	int choice;
	client customer;
	char p = 'B';
	
	Welcome_Message();

	do
	{
		cout << "1- Create an account \n2- Sign in.\n Enter : ";
		cin >> choice;
		if (choice != 1 && choice != 2)
			cout << "kindly enter a valid option !!" << endl;
	} while (choice != 1 && choice != 2);
	cin.ignore();
	switch (choice)
	{
	case 1:
	{
		Sign_UP();
	}

	do
	{
		cin >> choice;
		if (choice != 1 && choice != 0)
			cout << "Enter a valid value " << endl;
	} while (choice != 1 && choice != 0);
	cin.ignore();

	if (choice != 1)
		break;

	case 2:
	{
		if (Log_In(p))
		{
			p = toupper(p);
			
			switch (p)
			{
			case'A':
			{
				int keepgoing = 0;
				do
				{
					Room_Managment();
					cout << "\n Continue? (0-No/1-Yes) : ";
					cin >> keepgoing;
				} while (keepgoing != 0);
			}
			break;

			case 'C':
			{
				int keepgoing = 0;
				do
				{
					void Client_Managment();
					cout << "\n Continue? (0-No/1-Yes) : ";
					cin >> keepgoing;
				} while (keepgoing != 0);
			}
			break;

			default:
			{
				cout << "An error has occured plz contact provider ";//program should never arrive here
			}
			break;

			}
		}
		
	}
	
	return 0;

	}
}


	void Welcome_Message()
{
	cout << "Welcome to the Grand Hotel :) \n Our Priority is to insure Unforgetable moments ,comfort and security\n\n\n";
	cout <<endl<< "\n -------------------------------------------------------- \n \n";
}

string Hashed(string s)
{
	SHA1 checksum;
	checksum.update(s);
	const string hash = checksum.final();
	return hash;
}

bool Error_Email(string Email_address)
{
	const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	return regex_match(Email_address, pattern);

}

bool Error_Password(string password)
{
	int lower = 0, upper = 0, symbol = 0, number = 0, Error = 0;

	if (password.length() < 8)
	{
		cout << endl << "ERROR 101 : Password should be at least 8 characters long" << endl;//101
		Error++;
		cout << endl << "--------------------------------------------------------" << endl;
	}

	else
	{
		for (int i = 0; i < password.length(); i++)
		{
			if (password[i] >= 65 && password[i] <= 90)
				upper++;
			else if (password[i] >= 97 && password[i] <= 122)
				lower++;
			else if (password[i] >= 48 && password[i] <= 57)
				number++;
			else if ((password[i] >= 33 && password[i] <= 47) || (password[i] >= 58 && password[i] <= 64) || (password[i] >= 91 && password[i] <= 96) || (password[i] >= 123 && password[i] <= 126))
				symbol++;
		}
		if (upper == 0)
		{
			cout << endl << "ERROR 102 :Password must Include uppercase letters" << endl;//101
			Error++;
			cout << endl << "--------------------------------------------------------" << endl;
		}
		else if (lower == 0)
		{
			cout << endl << "ERROR 103 :Password must Include lowercase letters" << endl;//103
			Error++;
			cout << endl << "--------------------------------------------------------" << endl;
		}
		else if (number == 0)
		{
			cout << endl << "ERROR 104 :Password must Include Numbers" << endl;//104
			Error++;
			cout << endl << "--------------------------------------------------------" << endl;
		}
		else if (symbol == 0)
		{
			cout << endl << "ERROR 105 :Password must Include Symbols and special characters" << endl;//105
			Error++;
			cout << endl << "--------------------------------------------------------" << endl;
		}

	}

	if (Error == 0)
		return false;
	else
		return true;
}

bool Error_Phone(string number)
{
	if (number.length() != 8)
		return true;
	string str1(number.begin(), number.begin() + 2);
	if (str1 != "76" && str1 != "81" && str1 != "03" && str1 != "70" && str1 != "78")
		return true;
	for (int i = 0; i < number.length(); i++)
	{
		if (number[i] <= 48 && number[i] >= 57)
			return true;
	}
	return false;
}


bool Admin(string username[2])
{
	if (username[0] == "Grand" && username[1] == "Hotel")
		return true;
	return false;
}



void Sign_UP()
{
	fstream clients("client.csv", ios::in | ios::out | ios::app);
	string login[2];
	client customer;
	char log;
	int identifier;
	cout << "Welcome : \n Please enter the following information : \n ";
	cout << "Enter your firstname: ";
			
	customer.ID = time(0)%10000000 + rand()%100000;

	clients << customer.ID << ',';
	getline(cin, customer.firstname);
	cout << endl;
	clients << customer.firstname<< ',';
	cout << "Lastname: ";
	
	getline(cin, customer.lastname);
	cout << endl;
	clients << customer.lastname << ',';

	do
	{
		cout << "enter your password: ";

		getline(cin, customer.password);
		cout << endl;
	} while (Error_Password(customer.password));
	cout << endl;
	clients << Hashed(customer.password) << ',';
	do
	{
		cout << "enter Email Address : ";
		getline(cin, customer.address);

		if (!Error_Email(customer.address))
			cout << endl << "your email address isn't valid" << endl;
	} while (!Error_Email(customer.address));
	clients << customer.address << ',';
	do
	{
		cout << "enter phone number : +961 ";
		getline(cin, customer.tel);
		if (Error_Phone(customer.tel))
			cout << endl << "enter a valid lebanese phone number (include the first 0 if it exists)" << endl;
	} while (Error_Phone(customer.tel));
	clients << customer.tel << ',';
	cout << endl;
	do
	{
		cout << endl << "admin or client (a for admin and c for client): ";
		cin >> log;
		log=tolower(log);
		if (log != 'c' && log != 'a')
			cout << "kindly enter a valid value" << endl;
	} while (log != 'c' && log != 'a');
	cin.ignore();
	if (log == 'c')
		clients << 'C' << '\n';
	else
	{
		int i = 3;
		do 
		{
			cout << "\n \nSign in: \n \nUsername : ";
			getline(cin, login[0]);
			
			cout << "Password : ";
			getline(cin, login[1]);

			if (!Admin(login) && i!=1)
			{
				i--;
				cout << "The entered info are wrong you have " << i << "more trie(s)";
			}
		} while (i != 0 && !Admin(login));
		if (Admin(login))
		{
			cout << "welcome admin";
			clients << 'A' << '\n';
		}
		else
		{
			cout << "the entered info are wrong you will be assigned as a client " << endl;
			clients << 'C' << '\n';
		}
	}
	clients.close();
	cout << "\n Continue to log in? (1-yes,0-no) : ";
}

bool Log_In(char &c)
{
	fstream clients("client.csv", ios::in | ios::out | ios::app);
	client login, verification;
	int j = 0,k=3;
	
	bool email_found=false;
	
		cout << "Enter Your email : ";
		do
		{
			getline(cin, login.address);
			if (!Error_Email(login.address))
				cout << endl << "kindly enter a valid value" << endl;
		} while (!Error_Email(login.address));

		while (clients)
		{
			clients >> verification.ID;

			clients.ignore(INT_MAX, ',');

			getline(clients, verification.firstname, ',');

			getline(clients, verification.lastname, ',');

			getline(clients, verification.password, ',');

			getline(clients, verification.address, ',');

			getline(clients, verification.tel, ',');

			clients >> verification.position;

			clients.ignore(INT_MAX, '\n');

			for (int i = 0; i < login.address.length(); i++)
			{
				if (login.address[i] == verification.address[i])
					j++;
				else
				{
					j = 0;
					break;
				}
			}
		
			if (j != 0)
			{
				email_found = true;
				break;
			}
			else
				email_found = false;

		}

		if (!email_found)
			cout << "The entered email is not found please create an account" << endl;
		else
		{


			j = 0;
			do
			{
				cout << "Enter your password : ";
				getline(cin, login.password);
				login.password = Hashed(login.password);


				for (int i = 0; i < verification.password.length(); i++)
				{
					if (login.password[i] == verification.password[i])
						j++;
					else
					{
						j = 0;
						break;
					}
				}

				if (j == verification.password.length())
				{
					cout << endl << "sign in successful" << "\n Welcome back  " << verification.firstname << endl;
					break;
				}
				else
				{
					cout << endl << "Wrong Pass \n you have " << --k << " More tries \n" << endl;
				}
			} while (k != 0);
		}
	
	clients.close();
	if (k == 0)
	{
		cout << "The entered infos are wrong please sign again or create an account" << endl;
		return false;
	}
	else
	{
		c = verification.position;
		return true;
	}
	
}

void Room_Managment()
{
	int choice = 0;
	fstream rooms("room.csv", ios::in | ios::out | ios::app);
	//fstream temp("temporary.csv", ios::in | ios::out | ios::app);
	room hotel_room;
	cout << endl << "Welcome admin!!! " << endl;
	do
	{
		cout << "Would you like to : \n 1- Add Rooms \n 2- Modify rooms \n 3-Delete rooms \n\n\n Choice : ";
		cin >> choice;
	} while (choice < 1 || choice>3);

	switch (choice)
	{
	case 1:
	{
		Add_room();
	}break;

	case 2:
	{
		Modify_room();
	}break;

	case 3:
	{
		Delete_room();
	}break;

	default:
	{
		cout << "An error has occured plz contact provider ";//program should never arrive here
	}
	break;
	}
	//temp.close();
	//int status = remove("temporary.csv");
	rooms.close();
}

bool Room_Availability(int num)
{
	fstream rooms("room.csv", ios::in | ios::out | ios::app);
	int roomnumber;
	while (rooms)
	{
		rooms >> roomnumber;
		
		if (num == roomnumber)
		{
			return true;
			break;
		}
		rooms.ignore(INT_MAX, '\n');
	}
	return false;
}

void Add_room()
{
	room hotel_room;
	fstream rooms("room.csv", ios::in | ios::out | ios::app);
	
	do
	{
		cout << "Enter room number: ";
		cin >> hotel_room.num;
		if (Room_Availability(hotel_room.num))
			cout << "Room number : " << hotel_room.num << " already available \n";
	} while (Room_Availability(hotel_room.num));

	cin.ignore();

	cout << "Enter location : ";
	getline(cin, hotel_room.address);
	rooms << hotel_room.num << ',' << hotel_room.address << ',';

	cout << "Room type: \n1-Single room\n2-double room\n3-Business room\n4-Suite\n";
	cout << "Enter corresponding number : ";
	int num = 0;
	do
	{
		cin >> num;
		if (num < 1 || num>4)
			cout << "Enter a valid value\n";
	} while (num < 1 || num>4);
	cin.ignore();


	switch (num)
	{
	case 1:
	{
		hotel_room.type = "Single Room";
		rooms << hotel_room.type << ',';
	}break;
	case 2:
	{
		hotel_room.type = "Double Room";
		rooms << hotel_room.type << ',';
	}break;
	case 3:
	{
		hotel_room.type = "Business Room";
		rooms << hotel_room.type << ',';
	}break;
	case 4:
	{
		hotel_room.type = "Suite";
		rooms << hotel_room.type << ',';
	}break;
	default:
	{
		cout << "An error has occured plz contact provider ";//program should never arrive here
	}
	break;
	}

	do
	{
		cout << "Price (in $/night): ";
		cin >> hotel_room.price;
		if (hotel_room.price <= 0)
			cout << "\n kindly enter a valid value \n";
	} while (hotel_room.price <= 0);
	cin.ignore();
	rooms << hotel_room.price << ',';
	int choice1 = 0;
	cout << "Add additional features (1-yes/0-no) : ";
	cin >> choice1;
	if (choice1 == 1)
	{
		cout << "Additional features : \n \n2-NetFlix\n3-Free Cancelation\n4-Free parking\n5-Access to business longue\n\n6-High speed WI-FI";
		cout << endl << "How many features to add for room " << hotel_room.num << endl;
		do
		{
			cin >> hotel_room.number_of_options;
			if (hotel_room.number_of_options > 5 || hotel_room.number_of_options < 0)
				cout << endl << "Kindly enter a valid value \n" << endl;
		} while (hotel_room.number_of_options > 5 || hotel_room.number_of_options < 0);
		rooms << hotel_room.number_of_options << ',';
		hotel_room.option = new string[hotel_room.number_of_options];
		int* choice2 = new int[hotel_room.number_of_options];
		cin.ignore();
		cout << "Enter the number of The feature(s) : ";
		for (int i = 0; i < hotel_room.number_of_options; i++)
		{
			do
			{
				do
				{

					cin >> choice2[i];
					if (choice2[i] > 6 || choice2[i] < 1)
						cout << endl << "Kindly enter a valid value \n" << endl;
				} while (choice2[i] > 6 || choice2[i] < 1);
				if (choice2[i] == choice2[i - 1] || choice2[i] == choice2[i - 2] || choice2[i] == choice2[i - 3] || choice2[i] == choice2[i - 4] || choice2[i] == choice2[i - 5])
					cout << "Option already chosen \n Enter the number of The feature(s) : ";
			} while (choice2[i] == choice2[i - 1] || choice2[i] == choice2[i - 2] || choice2[i] == choice2[i - 3] || choice2[i] == choice2[i - 4] || choice2[i] == choice2[i - 5]);

			switch (choice2[i])
			{
			case 6:
				hotel_room.option[i] = "High speed WI-FI";
				break;
			case 2:
				hotel_room.option[i] = "Netflix";
				break;
			case 3:
				hotel_room.option[i] = "Free cancelation";
				break;
			case 4:
				hotel_room.option[i] = "Free parking";
				break;
			case 5:
				hotel_room.option[i] = "Access to Business longue";
				break;
			default:
				cout << "An error has occured plz contact provider ";//program should never arrive here
				break;
			}


			if (i == hotel_room.number_of_options - 1)
				rooms << hotel_room.option[i] << '\n';
			else
				rooms << hotel_room.option[i] << ',';


		}
	}
	rooms.close();

}


void Modify_room()
{
	room modify;
	int num;
	fstream rooms("room.csv", ios::in | ios::out | ios::app);
	fstream temp("temp.txt", ios::in | ios::out | ios::app);
	do {
		cout << "Which room would like to Modify : ";
		cin >> num;

		if (!Room_Availability(num))
			cout << "\n Room not Found \n " << endl;
	} while (!Room_Availability(num));

	while (rooms)
	{
		rooms >> modify.num;
		//cout << modify.num << endl;
		rooms.ignore(INT_MAX, ',');
		getline(rooms, modify.address, ',');
		//cout << modify.address << endl;
		getline(rooms, modify.type, ',');
		//cout << modify.type << endl;

		rooms >> modify.price;
		rooms.ignore(INT_MAX, ',');
		//	cout << modify.price << endl;
		rooms >> modify.number_of_options;
		//cout << modify.number_of_options<<endl;

		rooms.ignore(INT_MAX, ',');

		modify.option = new string[modify.number_of_options];
		for (int i = 0; i < modify.number_of_options; i++)
		{
			if (i == modify.number_of_options - 1)
				getline(rooms, modify.option[i], '\n');
			else
				getline(rooms, modify.option[i], ',');

		}


		if (num == modify.num)
			break;


	}

	cout << endl << "1- Number : " << modify.num << endl;
	cout << "2-Type : " << modify.type << endl;
	cout << "3- Location : " << modify.address << endl;
	cout << "4-Price : " << modify.price << endl;
	cout << "5- additional Features : \n";
	for (int i = 0; i < modify.number_of_options; i++)
		cout << "- " << modify.option[i] << endl;

	cout << endl << endl << "===========================================================" << endl << endl;

	int numb = 1;
	while (numb != 0)
	{
		do
		{
			cout << "What would you like to modify? (enter Corresponding number/ or zero for nothing) : ";
			cin >> numb;
			cin.ignore();
		} while (numb < 0 || numb>5);
		if (numb == 0)
			break;
		else if (numb == 1)
		{
			do
			{
				cout << endl << "What's the new Room number : ";
				cin >> modify.num;
				cin.ignore();
				if (Room_Availability(modify.num))
					cout << endl << "Cannot use this number because it might belong to another room";
			} while (Room_Availability(modify.num));
			//	cout << "modify.num=" << modify.num;
		}
		else if (numb == 2)
		{
			cout << "NEW Room type: \n1-Single room\n2-double room\n3-Business room\n4-Suite\n";
			cout << "Enter corresponding number : ";
			int num = 0;
			do
			{
				cin >> num;
				cin.ignore();
				if (num < 1 || num>4)
					cout << "Enter a valid value\n";
			} while (num < 1 || num>4);



			switch (num)
			{
			case 1:
			{
				modify.type = "Single Room";
				//rooms << hotel_room.type << ',';
			}break;
			case 2:
			{
				modify.type = "Double Room";
				//rooms << hotel_room.type << ',';
			}break;
			case 3:
			{
				modify.type = "Business Room";
				//rooms << hotel_room.type << ',';
			}break;
			case 4:
			{
				modify.type = "Suite";
				//rooms << hotel_room.type << ',';
			}break;
			default:
			{
				cout << "An error has occured plz contact provider ";//program should never arrive here
			}
			//cout << "modify.type=" << modify.type;
			break;
			}
		}
		else if (numb == 3)
		{
			cout << "Enter New location : ";
			getline(cin, modify.address);
			//rooms << hotel_room.num << ',' << hotel_room.address << ',';
			//cout << "modify.address=" << modify.address;
		}
		else if (numb == 4)
		{
			do {
				cout << "New Price (in $/night): ";
				cin >> modify.price;
				if (modify.price <= 0)
					cout << "\n kindly enter a valid value \n";
			} while (modify.price <= 0);
			cin.ignore();
			//cout << "modify.price=" << modify.price;
		}
		else if (numb == 5)
		{

			cout << "Change Additional features : \n \n2-NetFlix\n3-Free Cancelation\n4-Free parking\n5-Access to business longue\n\n6-High speed WI-FI";
			cout << endl << "How many features to add for room " << endl;
			do
			{
				cin >> modify.number_of_options;
				cin.ignore();
				if (modify.number_of_options > 5 || modify.number_of_options < 0)
					cout << endl << "Kindly enter a valid value \n" << endl;
			} while (modify.number_of_options > 5 || modify.number_of_options < 0);
			//	cout << modify.number_of_options << '\n';
			modify.option = new string[modify.number_of_options];
			int* choice2 = new int[modify.number_of_options];

			cout << "Enter the number of The feature(s) : ";
			for (int i = 0; i < modify.number_of_options; i++)
			{
				do
				{
					do
					{

						cin >> choice2[i];
						if (choice2[i] > 6 || choice2[i] < 1)
							cout << endl << "Kindly enter a valid value \n" << endl;
					} while (choice2[i] > 6 || choice2[i] < 1);
					if (choice2[i] == choice2[i - 1] || choice2[i] == choice2[i - 2] || choice2[i] == choice2[i - 3] || choice2[i] == choice2[i - 4] || choice2[i] == choice2[i - 5])
						cout << "Option already chosen \n Enter the number of The feature(s) : ";
				} while (choice2[i] == choice2[i - 1] || choice2[i] == choice2[i - 2] || choice2[i] == choice2[i - 3] || choice2[i] == choice2[i - 4] || choice2[i] == choice2[i - 5]);

				switch (choice2[i])
				{
				case 6:
					modify.option[i] = "High speed WI-FI";
					break;
				case 2:
					modify.option[i] = "Netflix";
					break;
				case 3:
					modify.option[i] = "Free cancelation";
					break;
				case 4:
					modify.option[i] = "Free parking";
					break;
				case 5:
					modify.option[i] = "Access to Business longue";
					break;
				default:
					cout << "An error has occured plz contact provider ";//program should never arrive here
					break;
				}
				
			}//for
		}//else if
		else
			cout << "An error has occured plz contact provider ";//program should never arrive here

	}//while


	temp << modify.num << '\t' << modify.address << '\t' << modify.type << '\t' << modify.price << '\t' << modify.number_of_options << '\t';

	///


	for (int i = 0; i < modify.number_of_options; i++)
	{
		temp << modify.option[i];
		if (i != (modify.number_of_options - 1))
			temp << '\t';
		else
			temp << '\n';
	}

	Temporary(num);

}



void Delete_room()
{
	int num;
	fstream rooms("room.csv", ios::in | ios::out | ios::app);
	
	room remove;
	cout << "\n Which room would you like to remove : ";
	do
	{
		cin >> num;

		if (!Room_Availability(num))
			cout << "\nROOM ENTERED NOT AVAILABLE \n" << endl;
	} while (!Room_Availability(num));

	while (rooms)
	{
		rooms >> remove.num;

		rooms.ignore(INT_MAX, ',');
		getline(rooms, remove.address, ',');

		getline(rooms, remove.type, ',');


		rooms >> remove.price;
		rooms.ignore(INT_MAX, ',');

		rooms >> remove.number_of_options;


		rooms.ignore(INT_MAX, ',');

		remove.option = new string[remove.number_of_options];
		for (int i = 0; i < remove.number_of_options; i++)
		{
			if (i == remove.number_of_options - 1)
				getline(rooms, remove.option[i], '\n');
			else
				getline(rooms, remove.option[i], ',');

		}


		if (num == remove.num)
			break;


	}

	cout << endl << "1- Number : " << remove.num << endl;
	cout << "2-Type : " << remove.type << endl;
	cout << "3- Location : " << remove.address << endl;
	cout << "4-Price : " << remove.price << endl;
	cout << "5- additional Features : \n";
	for (int i = 0; i < remove.number_of_options; i++)
		cout << "- " << remove.option[i] << endl;
	int temporary_int = remove.num;
	cout << endl << endl << "===========================================================" << endl << endl;
	char confirm = 0;
	do
	{
		cout << "\n Are you sure you want to delete room n" << remove.num << "? (y/n)";
		cin >> confirm;
		confirm = tolower(confirm);
		if (confirm != 'y' && confirm != 'n')
			cout << "\nkindly enter a valid option \n";
	} while (confirm != 'y' && confirm != 'n');

	cout << endl;
	switch (confirm)
	{
		case'y':
		{
			Temporary(remove.num);
		}
	}
}


void Temporary(int num) 
{
	fstream f;
	fstream temp;
	int nb;
	f.open("room.csv", ios::in);
	temp.open("temp.txt", ios::out);
	int size;
	
	string addR;
	string typeR;

	string priceR;
	while (f) {
		int numR;
		addR = "";
		typeR = "";
		priceR = "";
		
		f >> numR;
		f.ignore();
		getline(f, addR, ',');
		getline(f, typeR, ',');
		getline(f, priceR, ',');
		f >> size;
		f.ignore();
		string* info = new string[size];
		for (int i = 0; i < size; i++)
			getline(f, info[i], ',');
		if (num == numR)
			continue;
		
			temp << numR << "\t" << addR << "\t" << typeR << "\t" << priceR << "\t";
			for (int i = 0; i < size; i++)
			{
				temp << info[i];
				if (i == size - 1)
					temp << '\n';
				else
					temp << '\t';
			}
		
		f.ignore(INT_MAX, ',');
		f.ignore(INT_MAX, '\n');
	}
	f.close();
	temp.close();

	f.open("room.csv", ios::out);
	temp.open("temp.txt", ios::in);

	while (temp) {
		int numR;
		addR = "";
		typeR = "";
		priceR = "";
		
		temp >> numR;
		temp.ignore();
		getline(temp, addR, '\t');
		getline(temp, typeR, '\t');
		getline(temp, priceR, '\t');
		f >> size;
		f.ignore();
		string* info = new string[size];
		for (int i = 0; i < size; i++)
			getline(temp, info[i], '\t');
		if (num == numR)
			continue;
		else
		{
			f << numR << "," << addR << "," << typeR << "," << priceR << ",";
			for (int i = 0; i < size; i++)
			{
				f << info[i];
				if (i == size - 1)
					f << '\n';
				else
					f << ',';
			}
		}
		temp.ignore();
	}
	temp.close();
	f.close();
	remove("temp.txt");

}

void Client_Managment()
{
	int choice = 0;
	do
	{
		cout << "hello Client would you like to \n1-Reserve a room\n2-Cancel a reservation";
		cin >> choice;
		if (choice != 1 && choice != 2)
			cout << "\nKindly enter a valid value\n";
	} while (choice != 1 && choice != 2);


	switch (choice)
	{
	case 1:
	{
		reservation();
	}
	break;
	case 2:
	{
		cancellation();
	}
		break;
	default:
		cout << "Error";
		break;
	}

 }

void reservation()
{
	string fname, lname, address;
	date start, end;
	int room_num, startday, startmonth, startyear, eday, endmonth, endyear;
	cout << "Welcome!!" << endl;
	cout << "Enter your firstname: " << endl;
	getline(cin, fname);
	cout << "Enter your lastname: " << endl;
	getline(cin, lname);
	cout << "Enter the starting date(date,month,year)" << endl;
	do {
		cout << "Day: " << endl;
		cin >> start.day;
		if (start.day < 0 || start.day> 31)
			cout << "Enter a correct day" << endl;
	} while (start.day < 0 || start.day>31);
		do {
			cout << "Month: " << endl;
			cin >> start.month;
			if (start.month < 0 || start.month >31)
				cout << " Enter a correct month" << endl;
		} while (start.month < 0 || start.month>12);
		do {
			cout << "Year:" << endl;
			cin >> start.year;
			if (start.year >= 2022)
				cout << "Enter a correct year" << endl;
		} while (start.year >= 2022);
		cout << "Please enter the ending date (day,year,month)" << endl;
		do {
			cout << "Day:" << endl;
			cin >> end.day;
			if (end.day < 0 || end.day>31)
				cout << "Please enter a correct day" << endl;
		} while (end.day < 0 || end.day>31);
		do {
			cout << "Month:" << endl;
			cin >> end.month;
			if (end.month < 0 || end.month >31)
				cout << "Please enter a correct month " << endl;
		} while (end.day < 0 || end.month >12);
		do {
			cout << "Year:" << endl;
			cin >> end.year;
			if (end.year >= 2022)
				cout << "Please enter a correct year" << endl;
		} while (end.year >= 2022);

		fstream check;
		check.open("reservation.csv", ios::in);
		int size = Numofrooms(check);
		int* room = new int[size];
		int i = 0;
		while (check) {
			check >> room_num;
			check.ignore();
			getline(check, address, ',');
			check >> startday;
			check >> startmonth;
			check >> startyear;
			check >> eday;
			check >> endmonth;
			check >> endyear;
			check.ignore();
			if (start.day >= startday && start.month >= startmonth && start.year >= startyear && end.day <= eday && end.month <= endmonth && end.year <= endyear)
			{
				room[i] = room_num;
				i++;
			}
		}
		check.close();
		int Rnum;
		string add, type, ADDRESS, options;
		double price;
		cout << " The rooms you can reserve are " << endl;
		fstream f;
		f.open("Room.csv", ios::in);
		check.open("reservation.csv", ios::in);
		for (int j = 0; j < size; j++)
		{
			while (f) {
				address = " ";
				options = " ";
				f >> Rnum;
				f.ignore();
				getline(f, ADDRESS, ',');
				getline(f, type, ',');
				f >> price;
				f.ignore();
				getline(f, options, '\n');
				if (Rnum == room[j])
				{
					cout << Rnum << "\t" << address << "\t" << type << "\t" << price << "$\n";
					check<< Rnum << "," <<fname << "," << lname << "," << start.day << "-"<<start.month<<"-"<<start.year<<","<<end.day << "-" << end.month << "-" << end.year<<"\n";
				}
				else
					continue;
			}
		}
		check.close();
		f.close();
}




int Numofrooms(fstream& check)
{
	int i = 0;
	while (check)
	{
		i++;
	}
	return i;
}

void cancellation(string first, string last)
{
	int Room_num;
	char respond;
	cout << "Enter the room that you would to cancel" << endl;
	cin >> Room_num;
	cin.ignore();
	cout << "Does your room come with additional options (y/n)" << endl;
	do {
		cin >> respond;
		if (respond != 'y' && respond != 'Y' && respond != 'N' && respond != 'n')
			cout << "Please answer correctly" << endl;
	} while (respond != 'y' && respond != 'Y' && respond != 'N' && respond != 'n');
	cin.ignore();
	if (respond == 'y' || respond == 'Y')
	{
		int features, c = 0;
		cout << "How many features" << endl;
		cin >> features;
		string* Features = new string[features];
		for (int i = 0; i < features; i++) {
			cout << "enter features" << i + 1 << endl;
			getline(cin, Features[i]);
			if (Features[i] == " free cancellation" || Features[i] == "Free cancellation")
				c++;
		}
		if (c == 1) {
			cout << "Your cancellationis done." << endl;
			fstream reserv;
			reserv.open("Reservation.csv", ios::in);
			fstream temp;
			temp.open("temp.csv", ios::out);
			int num;
			string fn, ln, start, end;
			while (reserv) {
				fn = " ";
				ln = " ";
				reserv >> num;
				reserv.ignore();
				getline(reserv, fn, ',');
				getline(reserv, ln, ',');
				//getline(reserv, start, ',');
				getline(reserv, end, '\n');
				if (Room_num == num && fn == first && ln == last)
					continue;
				else 
					temp << num << "," << fn << "," << ln << "," << start << "," << end << "\n";
					
			}
			reserv.close();
			temp.close();
			temp.open("temp.csv", ios::in);
			reserv.open("Reservation.csv", ios::out || ios::app);
			while (temp) {
				fn = " ";
				ln = " ";
				temp >> num;
				temp.ignore();
				getline(temp, fn, ',');
				getline(temp, ln, ',');
				getline(temp, first, ',');
				getline(temp, last, ',');
				if (fn != " ")
					reserv << num << "," << fn << " ," << ln << "," << start << "," << end << "," << "\n";
			}
			temp.close();
			reserv.close();
			remove("temp.csv");

		}
	}
	else
		cout << "you can cancel your reservation" << endl;
}