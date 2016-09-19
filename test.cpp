// Autor: Eduard Ulasenka (gr. 2I1, 15L)
// Prowadzacy: Piotr Witonski

// Project 3-1201

#include "Centrale.h"

using namespace std;

extern ofstream fout;

int generate(void) // pzypadkowe wartosci dla zmiennych globalnych
{
	srand((unsigned int)time(NULL));
	return rand() % 3;
}

int number_of_providers = 2 + generate();
int number_of_lines = 1 + generate();
int number_of_abonents = 6 + generate();
int local_cost = 2 + generate();
int global_cost = 5 + generate();
int sms_cost = 1 + generate();
int fax_cost = 2 + generate();
int alarm_cost = 2 + generate();
int answerphone_cost = 1 + generate();
int num_of_all_lines;
size_t ch;

vector<Service*> serviceO; // aktywne uslugi
vector<Global_Calls*> global_callsO; // aktywne polaczenia globalne
vector<Local_Calls*> local_callsO; // aktywne polaczenia lokalne
vector<Lines *> linesO; // Przechowuje linie laczace operatorow

const int max_n = 50;
Provider * providerO[max_n];

void introduction_menu(void);
void create_service_obj(void);
void create_global_call_obj(void);
void create_local_call_obj(void);
int check(void);
int arithmetic_progression(int n){ return ((1+n) * n)/2; }

int main()
{
	introduction_menu();

	cout << "===============================================================================" << endl << endl;
	fout << "===============================================================================" << endl << endl;
	cout << "-->Starting the simulation:" << endl << endl;
	fout << "-->Starting the simulation:" << endl << endl;
	
	int choose_service;

	for (int i = 0; i <= 10 + rand() % 20; i++)
	{
		if (local_callsO.size() > 0) // odliczanie "czasu"
		{
			for (ch = 0; ch < local_callsO.size(); ch++)
				(*local_callsO[ch]).change_call_time();
		}
		if (global_callsO.size() > 0)
		{
			for (ch = 0; ch < global_callsO.size(); ch++)
				(*global_callsO[ch]).change_call_time();
		}
		if (serviceO.size() > 0)
		{
			for (ch = 0; ch < serviceO.size(); ch++)
				(*serviceO[ch]).change_call_time();
		}

		choose_service = 1 + rand() % 7; // losujemy usluge
		switch (choose_service)
		{
		case 1: // alarm (Service)
			create_service_obj();
			(*serviceO.back()).alarm();
			break;
		case 2: //answerphone (Service)
			create_service_obj();
			(*serviceO.back()).answerphone();
			break;
		case 3: //global_call(Global_Calls)
			create_global_call_obj();
			(*global_callsO.back()).global_call();
			break;
		case 4: //global_sms(Global_Calls)
			create_global_call_obj();
			(*global_callsO.back()).global_sms();
			break;
		case 5: //global_fax(Global_Calls)
			create_global_call_obj();
			(*global_callsO.back()).global_fax();
			break;
		case 6://local_call(Local_Calls)
			create_local_call_obj();
			(*local_callsO.back()).local_call();
			break;
		case 7://local_sms(Local_Calls)
			create_local_call_obj();
			(*local_callsO.back()).local_sms();
			break;
		case 8://local_fax(Local_Calls)
			create_local_call_obj();
			(*local_callsO.back()).local_fax();
			break;
		}//end of switch
	}//end of for
	cin.get();

	return 0;
}

void introduction_menu(void)
{
	int i = 0, j = 0;
	int choose_mode;

	cout << "=========================================================================" << endl;
	cout << "         ***Hello! It's an application, which will simulate\n"
		<< "           a work of phone operators and their abonents.***" << endl;
	cout << "=========================================================================" << endl << endl;

	cout << "-> Please, choose the mode: \n1 - Your own options. \n2 - Automatic simulation." << endl;
	choose_mode = check();
	if (choose_mode == 1)
	{
		cout << "Enter the number of providers:" << endl;
		cin >> number_of_providers;
		cout << "Enter the number of abonents:" << endl;
		cin >> number_of_abonents;
		cout << "Enter the number of connection lines:" << endl;
		cin >> number_of_lines;
		cout << endl;
	}

	cout << "-We got next situation:" << endl;
	fout << "=========================================================================" << endl;
	fout << "         ***Hello! It's an application, which will simulate\n"
		<< "           a work of phone operators and their abonents.***" << endl;
	fout << "=========================================================================" << endl << endl;
	fout << "-We got next situation:" << endl;
	cout << "->The number of providers is " << number_of_providers << endl;
	fout << "->The number of providers is " << number_of_providers << endl;
	cout << "->The number of connection lines is " << number_of_lines << endl;
	fout << "->The number of connection lines is " << number_of_lines << endl;
	cout << "->The number of abonents is " << number_of_abonents << endl << endl;
	fout << "->The number of abonents is " << number_of_abonents << endl << endl;

	cout << "->Today's prices:" << endl;
	cout << "	Local Call: " << local_cost << " pln." << endl;
	cout << "	Global Call: " << global_cost << " pln." << endl;
	cout << "	Sms: " << sms_cost << " pln." << endl;
	cout << "	Fax: " << fax_cost << " pln." << endl;
	cout << "	Alarm: " << alarm_cost << " pln." << endl;
	cout << "	Answerphone: " << answerphone_cost << " pln." << endl << endl;
	fout << "->Today's prices:" << endl;
	fout << "	Local Call: " << local_cost << " pln." << endl;
	fout << "	Global Call: " << global_cost << " pln." << endl;
	fout << "	Sms: " << sms_cost << " pln." << endl;
	fout << "	Fax: " << fax_cost << " pln." << endl;
	fout << "	Alarm: " << alarm_cost << " pln." << endl;
	fout << "	Answerphone: " << answerphone_cost << " pln." << endl << endl;

	cout << "Setting up providers and abonents." << endl;
	fout << "Setting up providers and abonents." << endl;
	for (i = 0; i < number_of_providers; i++)
	{
		providerO[i] = new Provider(i);
		cout << "Provider " << i << " has been created." << endl;
		fout << "Provider " << i << " has been created." << endl;
		(*providerO[i]).set_abonents();

	}

	cout << "\nSetting up connections." << endl;
	fout << "\nSetting up connections." << endl;

	num_of_all_lines = arithmetic_progression(number_of_providers)*number_of_lines;
	for (i = 0; i < number_of_providers; i++)
	{
		for (; j < number_of_providers; j++)
		{
			for (int k = 0; k < number_of_lines; k++)
			{
				linesO.push_back(new Lines(i, j));
			}
		}
		j = i + 1;

	}

	cout << "\nProviders, abonents and connections where created and set up successful." << endl << endl;
	fout << "\nProviders, abonents and connections where created and set up successful." << endl << endl;
}

void create_service_obj(void)
{
	int choose_provider, choose_call_time, choose_abonent;

	choose_provider = rand() % (number_of_providers - 1);
	choose_call_time = 1 + rand() % 4;
	choose_abonent = rand() % (number_of_abonents - 1);

	serviceO.push_back(new Service((*providerO[choose_provider]).return_abonent(choose_abonent),
		choose_provider, choose_call_time));
}
void create_global_call_obj(void)
{
	int choose_provider, choose_call_time, choose_abonent,
		choose_provider_2, choose_abonent_2;

	choose_provider = rand() % (number_of_providers - 1);
	while (1)
	{
		if (number_of_providers == 2) // warunek dodatkowy, bo czasami sie zapetla
		{
			choose_provider_2 = !choose_provider;
			break;
		}
		choose_provider_2 = rand() % (number_of_providers - 1);
		if (choose_provider_2 == choose_provider) continue;
		else break;
	}
	choose_call_time = 1 + rand() % 4;
	choose_abonent = rand() % (number_of_abonents - 1);
	choose_abonent_2 = rand() % (number_of_abonents - 1);

	global_callsO.push_back(new Global_Calls((*providerO[choose_provider]).return_abonent(choose_abonent),
		choose_provider, choose_call_time,
		(*providerO[choose_provider_2]).return_abonent(choose_abonent_2), choose_provider_2));
}
void create_local_call_obj(void)
{
	int choose_provider, choose_call_time, choose_abonent,
		choose_abonent_2;

	choose_provider = rand() % (number_of_providers - 1);
	choose_call_time = 1 + rand() % 4;
	choose_abonent = rand() % (number_of_abonents - 1);
	while (1)
	{
		if (number_of_abonents == 2)
		{
			choose_abonent_2 = !choose_abonent;
			break;
		}
		choose_abonent_2 = rand() % (number_of_abonents - 1);
		if (choose_abonent_2 == choose_abonent) continue;
		else break;
	}

	local_callsO.push_back(new Local_Calls((*providerO[choose_provider]).return_abonent(choose_abonent),
		choose_provider, choose_call_time,
		(*providerO[choose_provider]).return_abonent(choose_abonent_2)));
}

int check(void){  // sprawdza poprawnosc danych
	int choice;
	while (1) {
		cin >> choice;
		if (cin.peek() == '\n'){ // jesli wpisana liczba, to w buforze sie zostalo '\n'
			cin.get(); // clear bufor
			break;
		}
		else {
			cout << "[ERROR] It has to be integer value.\n";
			cout << "Try again:" << endl;
			cin.clear(); // reset all errors in stream
			while (cin.get() != '\n') { // clearing bufor
			}
		}
	}
	return choice;
}