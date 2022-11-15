#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#ifdef __linux__ 
#include <string.h>
#elif _WIN32
#include <windows.h>
#endif
int DE = 0, UA = 0, ES = 0, CH = 0;
//#include <time.h>

using namespace std;
struct Elem
{
    int  OwnerPoints;    // Очки господаря
    int  OppPoints;      // Очки гостя
    char Match[10];     // Рахунок
    char Name[20];      // Команда-господар
    char Opponent[20];  // Суперник
    Elem* left, * right, * parent;

    static void Score(string name, int score) {
        if (name == "Іспанія") ES += score;
        else if (name == "Німеччина") DE += score;
        else if (name == "Швейцарія") CH += score;
        else if (name == "Україна") UA += score;
    }

};


class Tree
{
    Elem* root;  // корінь
public:
    Tree();
    ~Tree();
    // вивід від зазначеного вузла
    void Print(Elem* Node);
    // пошук від зазначеного вузла
    Elem* Search(Elem* Node, char* key);
    // min від зазначеного вузла
    Elem* Min(Elem* Node);
    // max від зазначеного вузла
    Elem* Max(Elem* Node);
    // наступний для зазначеного вузла
    Elem* Next(Elem* Node);
    // попередній для зазначеного вузла
    Elem* Previous(Elem* Node);
    // вставка вузла
    void Insert(Elem* z);
    // вилучення гілки для зазначеного вузла,
    // 0 - вилучення всього дерева
    void Del(Elem* z = 0);
    // отримати корінь
    Elem* GetRoot();
};
Tree::Tree()
{
    root = NULL;
}
Tree :: ~Tree()
{
    Del();
}
// Рекурсивний обхід дерева
void Tree::Print(Elem* Node)
{
    if (Node != 0)
    {
        Print(Node->left);
        cout << Node->Name << Node->Match << Node->Opponent << endl;
        Print(Node->right);
    }
}
Elem* Tree::Search(Elem* Node, char* k)
{
    // Поки є вузли і ключі не збігаються
    while (Node != 0 && strcmp(k, Node->Name) != 0)
    {
        if (strcmp(k, Node->Name) < 0)
            Node = Node->left;
        else
            Node = Node->right;
    }
    return Node;
}
Elem* Tree::Min(Elem* Node)
{
    // Пошук найлівішого вузла
    if (Node != 0)
        while (Node->left != 0)
            Node = Node->left;
    return Node;
}
Elem* Tree::Max(Elem* Node)
{
    // Пошук найправішого вузла
    if (Node != 0)
        while (Node->right != 0)
        {
            Node = Node->right;
        }
    return Node;
}
Elem* Tree::Next(Elem* Node)
{
    Elem* y = 0;
    if (Node != 0)
    {
        // якщо є правий нащадок
        if (Node->right != 0)
            return Min(Node->right);
        // батько вузла
        y = Node->parent;
        // якщо Node не корінний і Node справа
        while (y != 0 && Node == y->right)
        {
            // Рухаємося вгору
            Node = y;
            y = y->parent;
        }
    }
    return y;
}
Elem* Tree::Previous(Elem* Node)
{
    Elem* y = 0;
    if (Node != 0)
    {
        // якщо є лівий нащадок
        if (Node->left != 0)
            return Max(Node->left);
        // батько вузла
        y = Node->parent;
        // якщо Node не корінний і Node зліва
        while (y != 0 && Node == y->left)
        {
            // Рухаємося вгору
            Node = y;
            y = y->parent;
        }
    }
    return y;
}
Elem* Tree::GetRoot()
{
    return root;
}
void Tree::Insert(Elem* z)
{
    // нащадків нема
    z->left = NULL;
    z->right = NULL;
    Elem* y = NULL;
    Elem* Node = root;
    // пошук місця
    while (Node != 0)
    {
        // майбутній батько
        y = Node;
        if (strcmp(z->Name, Node->Name) < 0)
            Node = Node->left;
        else
            Node = Node->right;
    }
    // заповнюємо батька
    z->parent = y;
    if (y == 0) // елемент перший (єдиний)
        root = z;
    // чий ключ більше?
    else if (strcmp(z->Name, y->Name) < 0)
        y->left = z;
    else
        y->right = z;
}
void Tree::Del(Elem* z)
{
    // вилучення
    if (z != 0)
    {
        Elem* Node, * y;
        // Якщо нема хоча б одного з нащадків
        if (z->left == 0 || z->right == 0)
            y = z;
        else
            y = Next(z);
        if (y->left != 0)
            Node = y->left;
        else
            Node = y->right;
        if (Node != 0)
            Node->parent = y->parent;
        // Чи вилучати кореневий вузол?
        if (y->parent == 0)
            root = Node;
        else if (y == y->parent->left)
            // зліва від батька?
            y->parent->left = Node;
        else
            // праворуч від батька?
            y->parent->right = Node;
        if (y != z)
        {
            // Копіювання даних вузла
            strcpy(z->Name, y->Name);
            strcpy(z->Opponent, y->Opponent);
            strcpy(z->Match, y->Match);
            z->OppPoints = y->OppPoints;
            z->OwnerPoints = y->OwnerPoints;
        }
        delete y;
    }
    else // вилучення всього дерева
        while (root != 0)
            Del(root);
}

Tree tournament;
void Game(char Commands[][20], int N)
{
    int i, j, k;
    int p1, p2; // Рахунок
    // Кожна команда грає з кожною іншою двічі: вдома і в гостях
    //int k;
    Elem* temp;
    for (k = 0; k < 2; k++)
        for (i = 0; i < N - 1; i++)
        {
            for (j = i + 1; j < N; j++)
            {
                temp = new Elem;
                if (k == 0)
                {
                    // 1 гра
                    strcpy(temp->Name, Commands[i]);
                    strcpy(temp->Opponent, Commands[j]);
                }
                else
                {
                    // 2 гра
                    strcpy(temp->Name, Commands[j]);
                    strcpy(temp->Opponent, Commands[i]);
                }
                p1 = rand() % 4;
                p2 = rand() % 3;
                if (p1 > p2)
                {
                    temp->OwnerPoints = 3;
                    temp->OppPoints = 0;
                }
                else if (p1 == p2)
                {
                    temp->OwnerPoints = 1;
                    temp->OppPoints = 1;
                }
                else
                {
                    temp->OwnerPoints = 0;
                    temp->OppPoints = 3;
                }
                // Запис рахунка
                sprintf(temp->Match, "% d :% d ", p1, p2);
                // Додавання запису
                temp->Score(temp->Name, p1);
                temp->Score(temp->Opponent, p2);
                tournament.Insert(temp);
            }
        }
}


void Winner_print() {
    if (CH >= UA && CH >= ES && CH >= DE)
    {//1
        cout << "Перше місце з рахунком " << CH << " посіла Швейцарія\n";

        if (UA >= ES && UA >= DE)
        {
            cout << "Друге місце з рахунком " << UA << " посіла Україна\n";

            if (ES >= DE)
            {
                cout << "Третє місце з рахунком " << ES << " посіла Іспанія\n";
                cout << "Четверте місце з рахунком " << DE << " посіла Німеччина\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << DE << " посіла Німеччина\n";
                cout << "Четверте місце з рахунком " << ES << " посіла Іспанія\n";
            }

        }
        else if (ES >= UA && ES >= DE)
        {
            cout << "Друге місце з рахунком " << ES << " посіла Іспанія\n";

            if (UA >= DE)
            {
                cout << "Третє місце з рахунком " << UA << " посіла Україна\n";
                cout << "Четверте місце з рахунком " << DE << " посіла Німеччина\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << DE << " посіла Німеччина\n";
                cout << "Четверте місце з рахунком " << UA << " посіла Україна\n";
            }

        }
        else if (DE >= ES && DE >= UA)
        {
            cout << "Друге місце з рахунком " << DE << " посіла Німеччина\n";

            if (UA >= ES)
            {
                cout << "Третє місце з рахунком " << UA << " посіла Україна\n";
                cout << "Четверте місце з рахунком " << ES << " посіла Іспанія\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << ES << " посіла Іспанія\n";
                cout << "Четверте місце з рахунком " << UA << " посіла Україна\n";
            }

        }

    }
    else if (UA >= CH && UA >= ES && UA >= DE)
    {//2
        cout << "Перше місце з рахунком " << UA << " посіла Україна\n";
        if (CH >= ES && CH >= DE)
        {
            cout << "Друге місце з рахунком " << CH << " посіла Швейцарія\n";

            if (ES >= DE)
            {
                cout << "Третє місце з рахунком " << ES << " посіла Іспанія\n";
                cout << "Четверте місце з рахунком " << DE << " посіла Німеччина\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << DE << " посіла Німеччина\n";
                cout << "Четверте місце з рахунком " << ES << " посіла Іспанія\n";
            }

        }
        else if (ES >= CH && ES >= DE)
        {
            cout << "Друге місце з рахунком " << ES << " посіла Іспанія\n";

            if (CH >= DE)
            {
                cout << "Третє місце з рахунком " << CH << " посіла Швейцарія\n";
                cout << "Четверте місце з рахунком " << DE << " посіла Німеччина\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << DE << " посіла Німеччина\n";
                cout << "Четверте місце з рахунком " << CH << " посіла Швейцарія\n";
            }

        }
        else if (DE >= ES && DE >= CH)
        {
            cout << "Друге місце з рахунком " << DE << " посіла Німеччина\n";

            if (CH > ES)
            {
                cout << "Третє місце з рахунком " << CH << " посіла Швейцарія\n";
                cout << "Четверте місце з рахунком " << ES << " посіла Іспанія\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << ES << " посіла Іспанія\n";
                cout << "Четверте місце з рахунком " << CH << " посіла Швейцарія\n";
            }

        }

    }

//3
    else if (ES >= UA && ES >= CH && ES >= DE)
    {
        cout << "Перше місце з рахунком " << ES << " посіла Іспанія\n";
        if (CH >= UA && CH >= DE)
        {
            cout << "Друге місце з рахунком " << CH << " посіла Швейцарія\n";

            if (UA >= DE)
            {
                cout << "Третє місце з рахунком " << UA << " посіла Україна\n";
                cout << "Четверте місце з рахунком " << DE << " посіла Німеччина\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << DE << " посіла Німеччина\n";
                cout << "Четверте місце з рахунком " << UA << " посіла Україна\n";
            }

        }
        else if (UA >= CH && UA >= DE)
        {
            cout << "Друге місце з рахунком " << UA << " посіла Україна\n";

            if (CH >= DE)
            {
                cout << "Третє місце з рахунком " << CH << " посіла Швейцарія\n";
                cout << "Четверте місце з рахунком " << DE << " посіла Німеччина\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << DE << " посіла Німеччина\n";
                cout << "Четверте місце з рахунком " << CH << " посіла Швейцарія\n";
            }

        }
        else if (DE >= UA && DE >= CH)
        {
            cout << "Друге місце з рахунком " << DE << " посіла Німеччина\n";

            if (CH >= UA)
            {
                cout << "Третє місце з рахунком " << CH << " посіла Швейцарія\n";
                cout << "Четверте місце з рахунком " << UA << " посіла Україна\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << UA << " посіла Україна\n";
                cout << "Четверте місце з рахунком " << CH << " посіла Швейцарія\n";
            }

        }

    }//4
    else if (DE >= UA && DE >= ES && DE >= CH)
    {
        cout << "Перше місце з рахунком " << DE << " посіла Німеччина\n";
        if (CH >= UA && CH >= ES)
        {
            cout << "Друге місце з рахунком " << CH << " посіла Швейцарія\n";

            if (UA >= ES)
            {
                cout << "Третє місце з рахунком " << UA << " посіла Україна\n";
                cout << "Четверте місце з рахунком " << ES << " посіла Іспанія\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << ES << " посіла Іспанія\n";
                cout << "Четверте місце з рахунком " << UA << " посіла Україна\n";
            }

        }
        else if (UA >= CH && UA >= ES)
        {
            cout << "Друге місце з рахунком " << UA << " посіла Україна\n";

            if (CH >= ES)
            {
                cout << "Третє місце з рахунком " << CH << " посіла Швейцарія\n";
                cout << "Четверте місце з рахунком " << ES << " посіла Іспанія\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << ES << " посіла Іспанія\n";
                cout << "Четверте місце з рахунком " << CH << " посіла Швейцарія\n";
            }

        }
        else if (ES >= UA && ES >= CH)
        {
            cout << "Друге місце з рахунком " << ES << " посіла Іспанія\n";

            if (CH >= UA)
            {
                cout << "Третє місце з рахунком " << CH << " посіла Швейцарія\n";
                cout << "Четверте місце з рахунком " << UA << " посіла Україна\n";
            }
            else
            {
                cout << "Третє місце з рахунком " << UA << " посіла Україна\n";
                cout << "Четверте місце з рахунком " << CH << " посіла Швейцарія\n";
            }

        }
    }
}
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(0));
    const int N = 4;
    char Commands[4][20] =
    {
    "Іспанія",
    "Німеччина",
    "Швейцарія",
    "Україна"
    };
    // Гра
    Game(Commands, N);
    // Висновок
    tournament.Print(tournament.GetRoot());

    Winner_print();
}