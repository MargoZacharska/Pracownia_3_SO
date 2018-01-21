#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include "sched.h"
#include "pthread.h"
#include "semaphore.h"
#include "unistd.h"
#include <vector>


using namespace std;


#define KUCHARZ "kucharz"
#define MYSLIWY "mysliwy"
#define POLANY 10
#define ZWIERZYNA 50


class ConcurrentResource
{
private:

	pthread_mutex_t	mutex;

public:

	ConcurrentResource()
	{
		resource = 0;
		pthread_mutex_init(&mutex, NULL);
	}

	~ConcurrentResource();

	volatile int resource;
	void add(int x);
	int get();
	bool TakeOneIfExists();
	bool CheckIfExists();
};


struct DaneWatku{
	bool (*akcja)();
	bool zyje;
	std::string rola;
};


ConcurrentResource::~ConcurrentResource()
{
	pthread_mutex_destroy(&mutex);
}


int roll_K6_dice()
{
	int x;
	x = rand() % 6;
	return x;
}


void ConcurrentResource::add(int x)
{
	pthread_mutex_lock(&mutex);
	resource += x;
	pthread_mutex_unlock(&mutex);
}


int ConcurrentResource::get()
{
	int value;
	pthread_mutex_lock(&mutex);
	value = resource;
	pthread_mutex_unlock(&mutex);
	return value;
}


bool ConcurrentResource::TakeOneIfExists()
{
	bool result = false;
	pthread_mutex_lock(&mutex);
	if (resource > 0)
	{
		resource--;
		result = true;
	}
	pthread_mutex_unlock(&mutex);
	return result;
}


bool ConcurrentResource::CheckIfExists(){
	bool result = false;
	pthread_mutex_lock(&mutex);
	result = resource > 0;
	pthread_mutex_unlock(&mutex);
	return result;
}


ConcurrentResource jedzenie;
ConcurrentResource zwierzyna;
ConcurrentResource liczbaAktorow;
ConcurrentResource polany[POLANY];


sem_t poczatekNocy;
sem_t poczatekDnia;


bool watekMysliwego(){
	int hunterLack = roll_K6_dice();
	int animalLack = roll_K6_dice();

	int polana = rand() % POLANY;
	if (hunterLack > animalLack){
		if (polany[polana].TakeOneIfExists()){
			zwierzyna.add(1);
		}
	}
	if (animalLack >= hunterLack + 3){
		return polany[polana].CheckIfExists();	
	}
	return jedzenie.TakeOneIfExists();
}


void *watekOgolny(void *_dane){
	DaneWatku *dane = (DaneWatku*)_dane;

	do{
		sem_wait(&poczatekNocy);
		sem_wait(&poczatekDnia);
	} while (dane->akcja());

	liczbaAktorow.add(-1);
	dane->zyje = false;
	sem_wait(&poczatekNocy);
	sem_wait(&poczatekDnia);

	pthread_exit(NULL);
	return NULL;
}


bool watekKucharza(){
	if (zwierzyna.TakeOneIfExists()){
		int n = roll_K6_dice();
		jedzenie.add(n);
	}
	return jedzenie.TakeOneIfExists();
}


void dodajAktora(vector<std::pair<pthread_t*, DaneWatku*> > &vec, bool akcja(), string rola){

		pthread_t* t = new pthread_t;
		DaneWatku* d = new DaneWatku;
		std::pair<pthread_t*, DaneWatku*> p;
		p.first = t;
		p.second = d;
		d->akcja = akcja;
		d->rola = rola;
		d->zyje = true;
		pthread_create(t, NULL, watekOgolny, (void*)d);

		vec.push_back(p);
	}


int policzWystapienia(vector<std::pair<pthread_t*, DaneWatku*>> aktorzy, string rola)
{
	int c = 0;
	for (vector<std::pair<pthread_t*, DaneWatku*>>::iterator i = aktorzy.begin(); i < aktorzy.end(); i++)
	{
		if (i->second->zyje && i->second->rola == rola)
			c++;
	}
	return c;
}


int main(int argc, char *argv[])
{
	srand(time(NULL));


	if (argc<5){
		return 0;
	}
	
	
	int liczba_mysliwych = atoi(argv[1]);
	int liczba_kucharzy = atoi(argv[2]);
	int liczba_zwierzyny = atoi(argv[3]);
	int liczba_jedzenia = atoi(argv[4]);


	int calkowita_liczba_watkow = liczba_kucharzy + liczba_mysliwych;
	liczbaAktorow.resource = calkowita_liczba_watkow;


	sem_init(&poczatekDnia, 0, 0);
	sem_init(&poczatekNocy, 0, 0);


	vector<std::pair<pthread_t*, DaneWatku*> > aktorzy;


	for (int j = 0; j < liczba_mysliwych; j++){
		dodajAktora(aktorzy, watekMysliwego, MYSLIWY);
	}


	for (int j = 0; j < liczba_kucharzy; j++){
		dodajAktora(aktorzy, watekKucharza, KUCHARZ);
	}


	jedzenie.add(liczba_jedzenia);
	zwierzyna.add(liczba_zwierzyny);


	int calkowita_liczba_watkow_tmp;


	for (int i = 0; i < 25; i++)
	{
		int tmp = 0;


		for (int i = 0; i < calkowita_liczba_watkow; i++){
			sem_post(&poczatekNocy);
		}


		do{
			sem_getvalue(&poczatekNocy, &tmp);
			usleep(500*1000);
		} while (tmp != 0);


		calkowita_liczba_watkow_tmp = liczbaAktorow.resource;
		
		
		cout << "Obecnie w osadzie mieszka:"
			<< "\nCalkowita liczba osadnikow: \t" << calkowita_liczba_watkow_tmp
			<< "\nkucharzy:  \t" << policzWystapienia(aktorzy, KUCHARZ)
			<< "\nmysliwych: \t" << policzWystapienia(aktorzy, MYSLIWY)
			<< "\nzwierzyny: \t" << zwierzyna.resource
			<< "\njedzenia:  \t" << jedzenie.resource;
			if (calkowita_liczba_watkow_tmp == 0)
			{
				cout << "\nOsada wyginela\n";
				break;
			}
			cout  << "\n-------------------------------------\n";


		for (int i = 0; i < calkowita_liczba_watkow; i++){
			sem_post(&poczatekDnia);
		}


		calkowita_liczba_watkow = calkowita_liczba_watkow_tmp;
		
		
		for (int i = 0; i < ZWIERZYNA; i++){
			polany[rand() % POLANY].resource++;
		}


		do{
			sem_getvalue(&poczatekDnia, &tmp);
			usleep(500*1000);
		} while (tmp != 0);

	}

	return 0;
}
