#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include "sched.h"
#include "pthread.h"
#include "semaphore.h"
#include "unistd.h"
#include <vector>

using namespace std;

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
};

struct DaneWatku{
	bool (*akcja)();
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


ConcurrentResource jedzenie;
ConcurrentResource zwierzyna;
ConcurrentResource liczbaAktorow;

sem_t poczatekNocy;
sem_t poczatekDnia;
//int sem_init(sem_t *sem, int pshared, unsigned int value);

bool watekMysliwego(){
	int hunterLack = roll_K6_dice();
	int animalLack = roll_K6_dice();

	if (hunterLack > animalLack){
		zwierzyna.add(1);
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

void dodajAktora(vector<std::pair<pthread_t*, DaneWatku*> > &vec, bool akcja()){

		pthread_t* t = new pthread_t;
		DaneWatku* d = new DaneWatku;
		std::pair<pthread_t*, DaneWatku*> p;
		p.first = t;
		p.second = d;
		d->akcja = akcja;
		pthread_create(t, NULL, watekOgolny, (void*)d);

		vec.push_back(p);
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
		dodajAktora(aktorzy, watekMysliwego);
	}

	for (int j = 0; j < liczba_kucharzy; j++){
		dodajAktora(aktorzy, watekKucharza);
	}

	jedzenie.add(liczba_jedzenia);
	zwierzyna.add(liczba_zwierzyny);

	int calkowita_liczba_watkow_tmp;

	for (int i = 0; i < 5; i++)
	{
		int tmp = 0;

		for (int i = 0; i < calkowita_liczba_watkow; i++){
			sem_post(&poczatekNocy);
		}

		do{
			sem_getvalue(&poczatekNocy, &tmp);
			usleep(500*1000);
		} while (tmp != 0);

		cout << "Obecnie w osadzie mieszka:"
			<< "\nkucharzy:  \t" << liczba_kucharzy
			<< "\nmysliwych: \t" << liczba_mysliwych
			<< "\nzwierzyny: \t" << zwierzyna.resource
			<< "\njedzenia:  \t" << jedzenie.resource
			<< "\n-------------------------------------\n";

		calkowita_liczba_watkow_tmp = liczbaAktorow.resource;
		for (int i = 0; i < calkowita_liczba_watkow; i++){
			sem_post(&poczatekDnia);
		}

		calkowita_liczba_watkow = calkowita_liczba_watkow_tmp;

		do{
			sem_getvalue(&poczatekDnia, &tmp);
			usleep(500*1000);
		} while (tmp != 0);

	}

	return 0;
}
