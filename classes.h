#include <iostream> //заголовочний файл, ввід-вивід
using namespace std;
const int wall = 0, //константи, які використовуються у коді: wall - позначення стіни
		  pass = 1, //pass - позначення проходу в лабірнті
		  way = -1, //way - використовується для позначення непройденої клітинки або непрохідної клітинки
   title_size = 11, //розмір заголовку в меню 
 max_filename = 20; //максимальна довжина назви файлу, яку може ввести користувач
typedef struct point //структура через яку передаються точки, їх координати в масиві
{
		  int x;
		  int y;
	point* next; //вказівник на наступну точку(знадобиться в черзі)
} point;
class queue //клас черги для алгоритму пошуку вшир
{
	point* front; //перший елемент черги
	 point* rear; //останній елемент черги
public:
	queue();	
	void push(int x, int y);
	point* pop(void);
	~queue();
};
queue::queue() //конструктор; ініціалізує чергу
{
	front = NULL;
	rear = NULL;
}
void queue::push(int x, int y)//метод класу queue, ставить клітинки в чергу на перевірку 
{
	point* pt = new point; //виділяємо пам'ять для перевіряємої клітинки
				pt->x = x; //призначуємо координати 
				pt->y = y;
		  pt->next = NULL; //робимо елемент останнім в існуючій черзі

	if (front == NULL) //якщо це перший елемент в черзі
	{
		front = pt; //то елемент буде і початком і кінцем черги
		rear = pt;
	}

	else
	{
		rear->next = pt; //якщо не перший, то присвоюємо його як останній елемент черги
		rear = pt;
	}

}

point* queue::pop(void) //метод класу queue, видаляє елемент з черги, повертає адресу
{
	if (front == NULL) //якщо черги немає, повертає NULL
	{
		return NULL;
	}

	point* buf = front; //ініціалізуємо нову точку у разі якщо залишився один елемент
	
	if (front == rear) //якщо один елемент в черзі
	{
		front = NULL;
		rear = NULL;
		return buf; //front міняємо, щоб не втратити вказівник на цю чергу
	}

	else //якщо все за звичних умов, то присвоюємо наступному елементу попередній
	{
		front = front->next;
		return buf;
	}

}
queue::~queue() { //деструктор, очищує пам'ять виділену для черги
	point* next;

	for (; front != NULL; front = next)
	{
		next = front->next;
		delete front;
	}

}
class mazeinf //основний клас, на якому працює вся програма; містить всю інформацію про мапу лабіринта
{
	int** maze, //вказівник на 2-ох вимірний масив з мапою лабіринту maze
		 width, //ширина
		height; //висота
public:
	void read(FILE* f);
	void visual(void);
	void solve(void);
	~mazeinf();
};
void mazeinf::read(FILE* f) //метод класу mazeinf; зчитуємо шуканий файл
{
	height = 0;
    width = 0;
    	
    //нульова перевірка: перевірка попереднього зчитування
    	
    if(maze != NULL) {

    	for (int i = 0; i < height; i++)
    	{
           	delete[] maze[i];
       	}

        delete maze;
    	}

	   int num_c = 0; //змінна в яку записуться загальна кількість елементів мапи лабіринту для подальшої перевірки розмірності масиву
		   	 char ch; // змінна для считування кожної клітинки поелементно
	int ind_room = 0; //індикатор, чи є хоча б один шлях в лабіринті

	//перевірки

	//перша  перевірка: можливість реалізації пошуку шляху


	//друга перевірка: рівність рядків лабіринту

	while ((ch = getc(f))) //перевіряємо перший рядок, задаємо ширину
	{

		if (ch == '\n') // обробили перший рядок
		{
			height++; 
			break;
		}

		if (ch - '0'== wall) //перевірка наявності верхньої стінки
		{ 
			width++;
			num_c++;
		}

		else if (ch - '0' == pass)
		{
			cerr << "You have a leaky maze!" << "\a";
			exit(1);
		}

		//cerr - стандартний потік помилок

		else if (ch != '\r')
		{
			cerr << "The maze should consist only of 0 and 1" << "\a";
			exit(1);
		}

	}

	//третя перевірка: формальність та коректне задання мапи лабіринту
	
	while ((ch = getc(f)) != EOF) //друга частина зчитування, встановлення висоти лабіринту
	{ 

		if (ch == '\n') //ідентифікація наступного рядка
		{
			height++;

			if (num_c % width || num_c / width != height) //перевірка висоти лабіринту, чи є всі рядки рівними
			{
				cerr << "The maze is set incorrectly!" << "\a";
				exit(1);
			}

		}

		else if (ch != '0' && ch != '1' && ch != '\r')
		{
			cerr << "The maze should consist only of 0 and 1" << "\a";
			exit(1);

		}

		else if ((num_c % width == 0 || num_c % width == width - 1) && ch - '0' == pass) //перевірка бокових стінок
		{
			cerr << " You have a leaky maze!" << "\a";
			exit(1);

		}

		else if (ch != '\r')
		{
			num_c++;

			if (ind_room == 0 && ch - '0' == pass) //перевірка, чи лабіринт складається не лише з нулів
				ind_room = 1;

		}

	}

	//четверта перевірка: наявність шляху

	if (!ind_room)
	{
		cerr << "There are no space in the maze." << "\a";
		exit(1);
	}

	//п'ята перевірка: окрема обробка останнього рядка(після того як ми зустрічаємо ЕОF, ми перевіряємо останній рядок,
	// тому що він має складатись з нулів.)

	height++;
	fseek(f, -width, SEEK_END); // перенос поточного елементу на перший рядок до зчитування
	
	for (int i = 0; i < width; i++)
	{

		if (getc(f) != '0')
		{
			cerr << "You have a leaky maze!" << "\a";
			exit(1);
		}

	}

	maze = new int* [height]; //виділення пам'яті для масиву мапи лабіринту 

	for (int i = 0; i < height; i++)
	{
		maze[i] = new int[width]();
	}

	fseek(f, 0, SEEK_SET); //переводимо курсор на першу клітинку мапи лабіринта

	for (int i = 0; i < height; ++i)
	{

		for (int j = 0; j < width; ++j)
		{
			maze[i][j] = getc(f) - '0'; //переведення char в int
		}

		fseek(f, 2, SEEK_CUR); //пропускаєм символи переносу на новий рядок
	}

}

void mazeinf::visual(void) //метод класу mazeinf; графічно подає мапу лабіринта
{

	for (int i = 0; i < height; i++)
	{

		for (int j = 0; j < width; j++)
		{

			switch (maze[i][j])
			{
			case wall: cout << "██"; break; //стіни
			case pass: cout << "  "; break; //прохід 
			case way: cout << "\033[31m██\033[0m"; break; //найкоротший шлях(червоною лінією)
			}

		}

		cout << endl;
	}
	cout << endl;

}


void mazeinf::solve(void) //метод класу mazeinf; знаходить найкоротший шлях від початку до фінішу
{
	int 		 cur_x = 1, // поточні координати руху в мапі
				 cur_y = 1,
   		fin_x = height - 2, //шукана координати фінішу
		 fin_y = width - 2,
	stepway[height][width]; //тут записано кількість кроків які треба робити щоб досягти фінішу
	point* tmp; //поточна клітинка на перевірку
	queue q;

	for (int i = 0; i < height; ++i) //спочатку заповнюємо всі точки на -1 крок
	{

		for (int j = 0; j < width; ++j)
		{
			stepway[i][j] = -1;
		}

	}

	stepway[1][1] = 0; //початкова точка
	q.push(cur_x, cur_y); //подаємо точку в чергу

	while (cur_x != fin_x || cur_y != fin_y) //доки поточні координати не прийдуть до фінішних
	{
		tmp = q.pop(); //координати отримали, можемо видаляти з черги

		if (tmp == NULL) //випадок, якщо далі від точки шляху немає
		{
			cerr << "There is no way in labyrinth." << "\a" << endl;
			return;
		}

		cur_x = tmp->x; //передаємо дані для перевірки
		cur_y = tmp->y;
		delete tmp; //отримали значення, очищуємо пам'ять від непотребу

		if (stepway[cur_x][cur_y - 1] == -1 && maze[cur_x][cur_y - 1] == pass) //перевірка нижнього сусіднього елемента
		{
			q.push(cur_x, cur_y - 1); //якщо елемент не перевірявся і це прохід, додаємо в чергу
			stepway[cur_x][cur_y - 1] = stepway[cur_x][cur_y] + 1; //записуємо кількість зроблених кроків
		}
		//надалі алгоритм роботи схожий, тому буде записано який елемент з якої сторони перевіряється

		if (stepway[cur_x][cur_y + 1] == -1 && maze[cur_x][cur_y + 1] == pass) //перевірка верхнього сусіднього елемента
		{
			q.push(cur_x, cur_y + 1);
			stepway[cur_x][cur_y + 1] = stepway[cur_x][cur_y] + 1;
		}

		if (stepway[cur_x + 1][cur_y] == -1 && maze[cur_x + 1][cur_y] == pass)//перевірка правого сусіднього елемента
		{
			q.push(cur_x + 1, cur_y);
			stepway[cur_x + 1][cur_y] = stepway[cur_x][cur_y] + 1;
		}

		if (stepway[cur_x - 1][cur_y] == -1 && maze[cur_x - 1][cur_y] == pass)//перевірка лівого сусіднього елемента
		{
			q.push(cur_x - 1, cur_y);
			stepway[cur_x - 1][cur_y] = stepway[cur_x][cur_y] + 1;
		}

	}

	maze[cur_x][cur_y] = way; //прийшли до фінішу, позначили шляхом, щоб знати звідки прямувати

	while (cur_x != 1 || cur_y != 1) //починаємо відлік кроків від кінця до початку. Підбираємо найкоротший шлях
	{

		if (stepway[cur_x][cur_y - 1] == stepway[cur_x][cur_y] - 1) //якщо знизу елемент є шляхом до старту
		{
			maze[cur_x][cur_y - 1] = way; //позначаємо шляхом 
			cur_y--; //рухаємось вниз
		}

		else if (stepway[cur_x][cur_y + 1] == stepway[cur_x][cur_y] - 1) //якщо зверху елемент є шляхом до старту
		{
			maze[cur_x][cur_y + 1] = way;
			cur_y++; //рухаємось вгору
		}

		else if (stepway[cur_x - 1][cur_y] == stepway[cur_x][cur_y] - 1) //якщо зліва елемент є шляхом до старту
		{
			maze[cur_x - 1][cur_y] = way;
			cur_x--; //рухаємось вліво
		}

		else if (stepway[cur_x + 1][cur_y] == stepway[cur_x][cur_y] - 1) //якщо справа елемент є шляхом до старту
		{
			maze[cur_x + 1][cur_y] = way;
			cur_x++; //рухаємось вправо
		}

	}
}

mazeinf::~mazeinf() //деструктор; очищуємо пам'ять від мапи лабіринту maze
{ 
	for (int i = 0; i < height; i++)
	{
		delete[] maze[i];
	}

	delete maze;
}
