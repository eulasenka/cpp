// Autor: Eduard Ulasenka (gr. 2I1, 15L)
// Prowadzacy: Piotr Witonski

// Project 3-1201

#ifndef CENTRALE_H_
#define CENTRALE_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

class Abonent{
protected:
	int number;
	int credit;
	bool status;
public:
	Abonent(){}
	~Abonent(){}
	Abonent(int numberG, int creditG = 0, bool statusG = true);

	Abonent& operator =(Abonent &a);

	int get_number(void) { return number; }
	int get_credit(void) { return credit; }
	bool get_status(void) { return status; }
	void change_credit(int n) { credit += n; }
	void change_status(bool s) { status = s; }
};

class Provider
{
protected:
	int unique_number;
	vector<Abonent*> AbonentO;
public:
	Provider(int i);
	Provider(){}
	~Provider(){}

	void set_abonents(void);
	Abonent * return_abonent(int i){ return this->AbonentO[i]; }
};

class Service
{
protected:
	int unique_num; // Numer centrali
	int call_time;
	Abonent * abonent_1;
public:
	Service() {};
	~Service();
	Service(Abonent * abonent_1, int unique_num, int call_time);

	int find_line_true(int n); // Znadz wolna linie i ja zajmij
	void find_line_false(int n); // Zwolnij linie
	void change_call_time(void);

	void alarm(void);
	void answerphone(void);
};

class Global_Calls: public Service
{
protected:
	int unique_num_2;
	Abonent * abonent_2;
public:
	Global_Calls(){}
	~Global_Calls();
	Global_Calls(Abonent * abonent_1, int unique_num, int call_time, Abonent * abonent_2, int unique_num_2);

	int find_line_true(int n, int m);
	void find_line_false(int n, int m);
	void change_call_time(void);

	void global_call(void);
	void global_sms(void);
	void global_fax(void);
};

class Local_Calls : public Service
{
protected:
	Abonent * abonent_2;
public:
	Local_Calls(){}
	~Local_Calls();
	Local_Calls(Abonent * abonent_1, int unique_num, int call_time, Abonent * abonent_2);
	
	int find_line_true(int n);
	void change_call_time(void);

	void local_call(void);
	void local_sms(void);
	void local_fax(void);
};

class Lines
{
protected:
	int operator_1, operator_2;
	bool status;
public:
	Lines(){}
	~Lines(){}
	Lines(int operatorG_1, int operatorG_2, bool statusG = true);
	
	friend Service;
	friend Global_Calls;
	friend Local_Calls;

	Lines& operator =(Lines &a);
};

#endif