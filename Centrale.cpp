// Autor: Eduard Ulasenka (gr. 2I1, 15L)
// Prowadzacy: Piotr Witonski

// Project 3-1201

#include "Centrale.h"

using namespace std;

ofstream fout("test.txt");

extern int number_of_providers;
extern int number_of_lines;
extern int number_of_abonents;
extern int local_cost;
extern int global_cost;
extern int sms_cost;
extern int fax_cost;
extern int alarm_cost;
extern int answerphone_cost;
extern int num_of_all_lines;
extern size_t ch;

extern vector<Lines *> linesO;
extern vector<Service*> serviceO;
extern vector<Global_Calls*> global_callsO;
extern vector<Local_Calls*> local_callsO;

Abonent::Abonent(int numberG, int creditG, bool statusG)
{
	number = numberG;
	credit = creditG;
	status = statusG;
}

Abonent& Abonent::operator =(Abonent &a)
{
	number = a.number;
	credit = a.credit;
	status = a.status;
	return *this;
}

void Provider::set_abonents(void)
{
	for (int j = 0; j < number_of_abonents; j++)
	{
		cout << "-> Abonent: " << j << "Has been added to the base of central " << this->unique_number
			<< endl;
		fout << "Abonent: " << j << "Has been added to the base of central " << this->unique_number
			<< endl;
		try
		{
			AbonentO.push_back(new Abonent(j, 0));
			if (!(*AbonentO.back()).get_status()) throw false;
		}
		catch (bool s)
		{
			cout << "[WARNING] Caught an exception -- value is: " << s << endl;
			cout << "	Trying to fix this..." << endl;
			cout << "	Changing the abonent's status on true..." << endl;
			(*AbonentO.back()).change_status(1);
			cout << "	Succeed." << endl;
		}
	}
}

Provider::Provider(int i) :unique_number(i){}

int Service::find_line_true(int n)
{
	int i = 0;
	for (i=0; i < num_of_all_lines; i++)
	{
		//cout << "n == " << n << " i == " << i << "operator 1 == " << (*linesO[i]).operator_1 << " operator 2 == " << (*linesO[i]).operator_2 << " status " << (*linesO[i]).status << endl;
		if (((*linesO[i]).operator_1 == n && (*linesO[i]).operator_2 == n) && (*linesO[i]).status != false)
		{
			(*linesO[i]).status = false;
			return 1;
		}
		else continue;
	}
	if (i == num_of_all_lines)
	{
		cout << "[ERROR] There is no line available, please try again later." << endl;
		fout << "[ERROR] There is no line available, please try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Service();
		return -1;
	}
	return 0;
}

void Service::find_line_false(int n)
{
	for (int i = 0; i < num_of_all_lines; i++)
	{
		if ((((*linesO[i]).operator_1 == n) && ((*linesO[i]).operator_2 == n)) && ((*linesO[i]).status == 0))
		{
			(*linesO[i]).status = 1;
			cout << "-> Line:   " << (*linesO[i]).operator_1 << (*linesO[i]).operator_2 << " is available since now." << endl;
			cout << "---" << endl;
			fout << "-> Line:   " << (*linesO[i]).operator_1 << (*linesO[i]).operator_2 << " is available since now." << endl;
			fout << "---" << endl;
			break;
		}
		else continue;
	}
}

void Service::change_call_time(void)
{
	this->call_time--;
	if (this->call_time == -1)
	{
		(*abonent_1).change_status(1);
		cout << "-> Abonent: " << (*abonent_1).get_number() << " is available since now." << endl;
		cout << "---" << endl;
		fout << "-> Abonent: " << (*abonent_1).get_number() << " is available since now." << endl;
		fout << "---" << endl;
		this->find_line_false(unique_num);
		this->~Service();
	}
}

Service::Service(Abonent * abonent_1, int unique_num, int call_time)
{
	this->abonent_1 = abonent_1;
	this->unique_num = unique_num;
	this->call_time = call_time;
}

Service::~Service()
{
	for (size_t i = 0; i < serviceO.size(); i++)
	{
		if (serviceO[i] == this)
		{
			serviceO.erase(serviceO.begin() + i);
			ch--; // cofnij w petli
			break;
		}
		else continue;
	}
}

void Service::alarm(void)
{
	if ((*abonent_1).get_status() == false)
	{
		cout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
			<< " - Trying to turn on alarm." << endl;
		cout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
			<< " - Trying to turn on alarm." << endl;
		fout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Service();
	}

	else
	{
		cout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
			<< " - Trying to turn on alarm." << endl;
		fout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
			<< " - Trying to turn on alarm." << endl;

		if ((this->find_line_true(unique_num)) == 1)
		{
			(*abonent_1).change_status(false);
			(*abonent_1).change_credit(alarm_cost*call_time);

			cout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - ALARM (" << call_time << " min.)" << endl;
			cout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			cout << "-------------------------------------------------------------------------------" << endl;
			fout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - ALARM (" << call_time << " min.)" << endl;
			fout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			fout << "-------------------------------------------------------------------------------" << endl;
		}

	}
}

void Service::answerphone(void)
{
	if ((*abonent_1).get_status() == false)
	{
		cout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
			<< " - Turning on answerphone." << endl;
		cout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
			<< " - Turning on answerphone." << endl;
		fout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Service();
	}

	else
	{
		cout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
			<< " - Turning on answerphone." << endl;
		fout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
			<< " - Turning on answerphone." << endl;
		
		if ((this->find_line_true(unique_num)) == 1)
		{
			(*abonent_1).change_status(false);
			(*abonent_1).change_credit(answerphone_cost*call_time);

			cout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - ANSWERPHONE (" << call_time << " min.)" << endl;
			cout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			cout << "-------------------------------------------------------------------------------" << endl;
			fout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - ANSWERPHONE (" << call_time << " min.)" << endl;
			fout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			fout << "-------------------------------------------------------------------------------" << endl;
		}
	}
}

void Global_Calls::change_call_time(void)
{
	this->call_time--;
	if (this->call_time == -1)
	{
		(*abonent_1).change_status(1);
		(*abonent_2).change_status(1);
		cout << "-> Abonent: " << (*abonent_1).get_number() << " is available since now." << endl;
		cout << "-> Abonent: " << (*abonent_2).get_number() << " is available since now." << endl;
		cout << "---" << endl;
		fout << "-> Abonent: " << (*abonent_1).get_number() << " is available since now." << endl;
		fout << "-> Abonent: " << (*abonent_2).get_number() << " is available since now." << endl;
		fout << "---" << endl;
		this->find_line_false(unique_num, unique_num_2);
		this->~Global_Calls();
	}
}

Global_Calls::Global_Calls(Abonent * abonent_1, int unique_num, int call_time, Abonent * abonent_2, int unique_num_2): Service(
	abonent_1, unique_num, call_time)
{
	this->abonent_2 = abonent_2;
	this->unique_num_2 = unique_num_2;
}

Global_Calls::~Global_Calls()
{
	for (size_t i = 0; i < global_callsO.size(); i++)
	{
		if (global_callsO[i] == this)
		{
			global_callsO.erase(global_callsO.begin() + i);
			ch--; // cofnij w petli
			break;
		}
		else continue;
	}
}

int Global_Calls::find_line_true(int n, int m)
{
	int i = 0;
	for (i=0; i < num_of_all_lines; i++)
	{
		//cout << "n == " << n << "m " << m << " i == " << i << "operator 1 == " << (*linesO[i]).operator_1 << " operator 2 == " << (*linesO[i]).operator_2 << " status " << (*linesO[i]).status << endl;
		if (  ( (((*linesO[i]).operator_1 == n) && ((*linesO[i]).operator_2 == m))
			|| (((*linesO[i]).operator_2 == n) && ((*linesO[i]).operator_1 == m)) )
			&& ((*linesO[i]).status != false))
		{
			(*linesO[i]).status = false;
			return 1;
			break;
		}
		else continue;
	}
	if (i == num_of_all_lines)
	{
		cout << "[ERROR] There is no line available, please try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] There is no line available, please try again later." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Global_Calls();
		return -1;
	}
	return 0;
}

void Global_Calls::find_line_false(int n, int m)
{
	for (int i = 0; i < num_of_all_lines; i++)
	{
		if (((((*linesO[i]).operator_1 == n) && ((*linesO[i]).operator_2 == m))
			|| (((*linesO[i]).operator_1 == m) && ((*linesO[i]).operator_2 == n)))
			&& ((*linesO[i]).status == 0))
		{
			(*linesO[i]).status = 1;
			cout << "-> Line:   " << (*linesO[i]).operator_1 << (*linesO[i]).operator_2 << " is available since now." << endl;
			cout << "---" << endl;
			fout << "-> Line:   " << (*linesO[i]).operator_1 << (*linesO[i]).operator_2 << " is available since now." << endl;
			fout << "---" << endl;
			break;
		}
		else continue;
	}
}

void Global_Calls::global_call(void)
{
	cout << "Operator (outgoing): " << unique_num << " Abonent (outgoing): " << (*abonent_1).get_number()
		<< "\n " << " - Trying to call Operator (incoming): " << unique_num_2 << " Abonent (incoming): "
		<< (*abonent_2).get_number() << endl;
	fout << "Operator (outgoing): " << unique_num << " Abonent (outgoing): " << (*abonent_1).get_number()
		<< "\n " << " - Trying to call Operator (incoming): " << unique_num_2 << " Abonent (incoming): "
		<< (*abonent_2).get_number() << endl;

	if ((*abonent_2).get_status() == false)
	{
		cout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Global_Calls();
	}
	else if ((*abonent_1).get_status() == false)
	{
		cout << "[ERROR] You can't have more than one conversation." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] You can't have more than one conversation." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Global_Calls();
	}
	else
	{
		if ((this->find_line_true(unique_num, unique_num_2)) == 1)
		{
			(*abonent_1).change_status(false);
			(*abonent_2).change_status(false);
			(*abonent_1).change_credit(global_cost*call_time);

			cout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - GLOBAL CALL (" << call_time << " min.)" << endl;
			cout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			cout << "-------------------------------------------------------------------------------" << endl;
			fout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - GLOBAL CALL (" << call_time << " min.)" << endl;
			fout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			fout << "-------------------------------------------------------------------------------" << endl;
		}
	}
}

void Global_Calls::global_fax(void)
{
	cout << "Operator (outgoing): " << unique_num << " Abonent (outgoing): " << (*abonent_1).get_number()
		<< "\n " << " - Trying to fax Operator (incoming): " << unique_num_2 << " Abonent (incoming): "
		<< (*abonent_2).get_number() << endl;
	fout << "Operator (outgoing): " << unique_num << " Abonent (outgoing): " << (*abonent_1).get_number()
		<< "\n " << " - Trying to fax Operator (incoming): " << unique_num_2 << " Abonent (incoming): "
		<< (*abonent_2).get_number() << endl;

	if ((*abonent_2).get_status() == false)
	{
		cout << "[ERROR] The abonent's number (" << (*abonent_2).get_number()
			<< ") you want to send fax is busy. Please, try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] The abonent's number (" << (*abonent_2).get_number()
			<< ") you want to send fax is busy. Please, try again later." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Global_Calls();
	}
	else if ((*abonent_1).get_status() == false)
	{
		cout << "[ERROR] You can't have more than one conversation." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "Operator (outgoing): " << unique_num << " Abonent (outgoing): " << (*abonent_1).get_number()
			<< "\n " << " - Trying to fax Operator (incoming): " << unique_num_2 << " Abonent (incoming): "
			<< (*abonent_2).get_number() << endl;
		fout << "[ERROR] You can't have more than one conversation." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Global_Calls();
	}

	else
	{
		if ((this->find_line_true(unique_num, unique_num_2)) == 1)
		{
			(*abonent_1).change_status(false);
			(*abonent_2).change_status(false);
			(*abonent_1).change_credit(fax_cost*call_time);

			cout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - GLOBAL FAX (" << call_time << " min.)" << endl;
			cout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			cout << "-------------------------------------------------------------------------------" << endl;
			fout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - GLOBAL FAX (" << call_time << " min.)" << endl;
			fout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			fout << "-------------------------------------------------------------------------------" << endl;
		}
	}
}

void Global_Calls::global_sms(void)
{
	cout << "Operator (outgoing): " << unique_num << " Abonent (outgoing): " << (*abonent_1).get_number()
		<< "\n " << " - Trying to sms Operator (incoming): " << unique_num_2 << " Abonent (incoming): "
		<< (*abonent_2).get_number() << endl;
	fout << "Operator (outgoing): " << unique_num << " Abonent (outgoing): " << (*abonent_1).get_number()
		<< "\n " << " - Trying to sms Operator (incoming): " << unique_num_2 << " Abonent (incoming): "
		<< (*abonent_2).get_number() << endl;
	
	if ((*abonent_2).get_status() == false)
	{
		cout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Global_Calls();
	}

	else if ((*abonent_1).get_status() == false)
	{
		cout << "[ERROR] You can't have more than one conversation." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] You can't have more than one conversation." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Global_Calls();
	}

	else
	{
		if ((this->find_line_true(unique_num, unique_num_2)) == 1)
		{
			(*abonent_1).change_credit(sms_cost*call_time);

			cout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - GLOBAL SMS (" << call_time << " qnt.)" << endl;
			cout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			cout << "-------------------------------------------------------------------------------" << endl;
			fout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - GLOBAL SMS (" << call_time << " qnt.)" << endl;
			fout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			fout << "-------------------------------------------------------------------------------" << endl;
		}
	}
}

int Local_Calls::find_line_true(int n)
{
	int i = 0;
	for (i=0; i < num_of_all_lines; i++)
	{
		//cout << "n == " << n << " i == " << i << "operator 1 == " << (*linesO[i]).operator_1 << " operator 2 == " << (*linesO[i]).operator_2 << " status " << (*linesO[i]).status << endl;
		if ((((*linesO[i]).operator_1 == n) && ((*linesO[i]).operator_2 == n)) && ((*linesO[i]).status != false))
		{
			(*linesO[i]).status = false;
			return 1;
			break;
		}
		else continue;
	}
	if (i == num_of_all_lines)
	{
		cout << "[ERROR] There is no line available, please try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] There is no line available, please try again later." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Local_Calls();
		return -1;
	}
	return 0;
}

Local_Calls::Local_Calls(Abonent * abonent_1, int unique_num, int call_time, Abonent * abonent_2) : Service(
	abonent_1, unique_num, call_time)
{
	this->abonent_2 = abonent_2;
}

Local_Calls::~Local_Calls()
{
	for (size_t i = 0; i < local_callsO.size(); i++)
	{
		if (local_callsO[i] == this)
		{
			local_callsO.erase(local_callsO.begin() + i);
			ch--; // cofnij w petli
			break;
		}
		else continue;
	}
}

void Local_Calls::change_call_time(void)
{
	this->call_time--;
	if (this->call_time == -1)
	{
		(*abonent_1).change_status(1);
		(*abonent_2).change_status(1);
		cout << "-> Abonent: " << (*abonent_1).get_number() << " is available since now." << endl;
		cout << "-> Abonent: " << (*abonent_2).get_number() << " is available since now." << endl;
		cout << "---" << endl;
		fout << "-> Abonent: " << (*abonent_1).get_number() << " is available since now." << endl;
		fout << "-> Abonent: " << (*abonent_2).get_number() << " is available since now." << endl;
		fout << "---" << endl;
		this->find_line_false(unique_num);
		this->~Local_Calls();
	}
}

void Local_Calls::local_call(void)
{
	cout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
		<< " - Trying to make a local call to abonent with number " << (*abonent_2).get_number() << endl;
	fout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
		<< " - Trying to make a local call to abonent with number " << (*abonent_2).get_number() << endl;

	if ((*abonent_2).get_status() == false)
	{
		cout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Local_Calls();
	}
	else if ((*abonent_1).get_status() == false)
	{
		cout << "[ERROR] You can't have more than one conversation." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] You can't have more than one conversation." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Local_Calls();
	}

	else
	{
		
		if ((this->find_line_true(unique_num)) == 1)
		{
			(*abonent_1).change_status(false);
			(*abonent_2).change_status(false);
			(*abonent_1).change_credit(local_cost*call_time);

			cout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - LOCAL CALL (" << call_time << " min.)" << endl;
			cout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			cout << "-------------------------------------------------------------------------------" << endl;
			fout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - LOCAL CALL (" << call_time << " min.)" << endl;
			fout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			fout << "-------------------------------------------------------------------------------" << endl;
		}
	}
}

void Local_Calls::local_sms(void)
{
	cout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
		<< " - Trying to send a local sms to abonent with number " << (*abonent_2).get_number() << endl;
	fout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
		<< " - Trying to send a local sms to abonent with number " << (*abonent_2).get_number() << endl;
	
	if ((*abonent_2).get_status() == false)
	{
		cout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] The abonent's number is busy. Please, try again later." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Local_Calls();
	}

	else if ((*abonent_1).get_status() == false)
	{
		cout << "[ERROR] You can't have more than one conversation." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] You can't have more than one conversation." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Local_Calls();
	}

	else
	{
		if ((this->find_line_true(unique_num)) == 1)
		{
			(*abonent_1).change_status(false);
			(*abonent_2).change_status(false);
			(*abonent_1).change_credit(sms_cost*call_time);

			cout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - LOCAL SMS (" << call_time << " qnt.)" << endl;
			cout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			cout << "-------------------------------------------------------------------------------" << endl;
			fout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - LOCAL SMS (" << call_time << " qnt.)" << endl;
			fout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			fout << "-------------------------------------------------------------------------------" << endl;
		}
	}
}

void Local_Calls::local_fax(void)
{
	cout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
		<< " - Trying to send a local fax to abonent with number " << (*abonent_2).get_number() << endl;
	fout << "Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
		<< " - Trying to send a local fax to abonent with number " << (*abonent_2).get_number() << endl;

	if ((*abonent_2).get_status() == false)
	{
		cout << "[ERROR] The abonent's number (" << (*abonent_2).get_number()
			<< ") you want to send fax is busy. Please, try again later." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] The abonent's number (" << (*abonent_2).get_number()
			<< ") you want to send fax is busy. Please, try again later." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Local_Calls();
	}
	else if ((*abonent_1).get_status() == false)
	{
		cout << "[ERROR] You can't have more than one conversation." << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		fout << "[ERROR] You can't have more than one conversation." << endl;
		fout << "-------------------------------------------------------------------------------" << endl;
		this->~Local_Calls();
	}

	else
	{
		if ((this->find_line_true(unique_num)) == 1)
		{
			(*abonent_1).change_status(false);
			(*abonent_2).change_status(false);
			(*abonent_1).change_credit(fax_cost*call_time);

			cout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - LOCAL FAX (" << call_time << " min.)" << endl;
			cout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			cout << "-------------------------------------------------------------------------------" << endl;
			fout << "[SUCCEED] Operator: " << unique_num << " Abonent: " << (*abonent_1).get_number()
				<< " - LOCAL FAX (" << call_time << " min.)" << endl;
			fout << "	Abonent's credit: " << (*abonent_1).get_credit() << " pln." << endl;
			fout << "-------------------------------------------------------------------------------" << endl;
		}
	}	
}

Lines& Lines::operator =(Lines &a)
{
	operator_1 = a.operator_1;
	operator_2 = a.operator_2;
	status = a.status;
	return *this;
}

Lines::Lines(int operatorG_1, int operatorG_2, bool statusG)
{
	operator_1 = operatorG_1;
	operator_2 = operatorG_2;
	status = statusG;
	cout << "Connection has been created " << operator_1 << operator_2 << endl;
	fout << "Connection has been created " << operator_1 << operator_2 << endl;

	try { if (!statusG) throw false; }
	catch (bool s)
	{ 
		cout << "[WARNING] Caught an exception -- value is: " << s << endl;
		cout << "	Trying to fix this..." << endl;
		cout << "	Changing the line's status on true..." << endl;
		statusG = 1;
		cout << "	Succeed." << endl;
	}
}