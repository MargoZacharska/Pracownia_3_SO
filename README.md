Celem projektu jest rozwinięcie problemu producenta-konsumenta.


Założenia projektowe:

-Dla każdego osadnika jest tworzony dokładnie jeden wątek, który kończy swoje działanie dopiero w przypadku śmierci osadnika lub końca symulacji.

-Synchronizacja wątków - wątki kucharzy i myśliwych są tworzone jednorazowo na samym początku programu i są synchronizowane za pomocą semaforów, wątek (osadnik) po wykonanej pracy jest usypiany i czeka do następenego dnia.

-W przypadku następujących zdarzeń:
	- brak żywności - wątki, którym zabrakło pożywienia są unicestwiane
	- jeśli liczba wylosowana przez zwierzynę przy polowaniu wynosi >= n + 3, gdzie n - liczba wylosowana przez myśliwego - wątek myśliwego jest unicestwiany.
	
-Zwierzyna wybiera każdego dnia polanę, na której będzie przebywała, wątki myśliwych losują polanę, na którą mają się wybrać, każdy wątek może wylosować dowolną polanę, w przypadku gdy dwóch lub wiecej mysliwych trafia na tą samą polanę i jest ich więcej niż zwierzyny, to muszą o nią rywalizować.

-W przypadku upolowania zwierzyny, następnego dnia pojawia się nowa na jej miejsce.

-Zmiana cech zasobów:
	- upolowana zwierzyna, która nie zostanie zużyta przez wątek kucharza odpowiednio szybko - zostaje usunieta ze zbioru zasobów
	- jednostki pożywienia, które nie zostaną zużyte przez osadników w ciągu kilku dni  - zostają usuniete ze zbioru zasobów
	
-Przyjmowanie do osady nowych osadników - w przypadku gdy conajmniej połowa myśliwych zakończy polowanie sukcesem oraz conajmniej połowa kucharzy coś ugotuje - do osady jest przyjmowany jeden osadnik, aby osada mogła się rozwijać.
