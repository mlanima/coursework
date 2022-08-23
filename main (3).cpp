#include <iostream> //заголовочний файл, ввід-вивід
#include <cstdlib>  //заголовочний файл, функції стандартної бібліотеки С++
#include <ctime>    //заголовочний файл, робота з датою та часом
#include <limits>    //заголовочний файл, для перевірки правильності введення
#include "Classes.h"
using namespace std;
void phraseGen(void) //обирається фраза, якою заверше програма свою роботу
{
	int random;
	srand(time(NULL));
	random = rand() % 5 + 1;

	switch (random) {
	case 1:
		cout << "See you soon! Goodbye."; break;
	case 2:
		cout << "Have a nice day! Bye."; break;
	case 3:
		cout << "Eeny, meeny, miny, moe..."; break;
	case 4:
		cout << "Why can't we say 'P' without closing a mouth..."; break;
	case 5:
		cout << "Have you ever wondered about bitcoins..."; break;
	default:break;
	}

}

void menu(char* filename) //головне меню
{
	int decision; //вибір користувача

	for (int i = 0; i < title_size; i++) {
		cout << "· ";
	}; //далі здебільшого елементи графічного інтерфейсу

	cout << "\n";
	cout << "· M A Z E   G A M E ·" << endl;

	for (int i = 0; i < title_size; i++)
		cout << "· ";

	cout << "\n";
	cout << "-> Play Game(enter 1)" << endl;
	cout << "-> Exit(enter 0)" << endl;
	cin >> decision;

	while(cin.fail() || decision > 1 || decision < 0) //перевірка на правильність вводу
	{ 
    cin.clear();
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    cout << "Oops. I don't understand what you mean. Try again... \a"<<endl;
    cin >> decision;
	}
  	
	if (decision == 1) {
		cout << "Let's start! How is your mazemap file called? use .txt at the end" << endl;
		cin >> filename;
	}

	else 
	{
		phraseGen();
		exit(0);
	}
			
}

int main(void) //головна фунція
{
	mazeinf a;   //ініціалізуємо головний клас
	int ind = 1; //індикатор для зациклювання програми
	FILE* file;  //файл, який буде зчитаний
	char filename[max_filename]; //назва файлу зберігається тут
	do {

		menu(filename); //викликається  меню з варіантами вибору
		file = fopen(filename, "r"); //відкриваємо файл

		if (file == NULL) //якщо файла не існує
		{ 
			cout << "Opening error." << "\a" << endl;
			exit(1);
		}

		a.read(file);//зччитка з файлу
		a.solve();//алгоритм для знаходження найкоротшого шляху
		a.visual();//вивід лабіринту на екран
	} while (ind);
	
}