/*
Баранников Степан Алексеевич
M8O-201Б

Вариант 2: Квадрат, Стек, Список.

Создать шаблон динамической коллекцию, согласно варианту задания:
1. Коллекция должна быть реализована с помощью умных указателей (std::shared_ptr, std::weak_ptr).
(Опционально использование std::unique_ptr;)

2. В качестве параметра шаблона коллекция должна принимать тип данных;

3. Коллекция должна содержать метод доступа:
    o Стек – pop, push, top;
    o Очередь – pop, push, top;
    o Список, Динамический массив – доступ к элементу по оператору [];

4. Реализовать аллокатор, который выделяет фиксированный размер памяти (количество блоков памяти –
является параметром шаблона аллокатора). Внутри аллокатор должен хранить указатель на
используемый блок памяти и динамическую коллекцию указателей на свободные блоки. Динамическая
коллекция должна соответствовать варианту задания (Динамический массив, Список, Стек, Очередь);

5. Коллекция должна использовать аллокатор для выделеления и освобождения памяти для своих
элементов.

6. Аллокатор должен быть совместим с контейнерами std::map и std::list (опционально – vector).

7. Реализовать программу, которая:
    o Позволяет вводить с клавиатуры фигуры (с типом int в качестве параметра шаблона фигуры) и добавлять в коллекцию использующую аллокатор;
    o Позволяет удалять элемент из коллекции по номеру элемента;
    o Выводит на экран введенные фигуры c помощью std::for_each;
*/


#include <iostream>
#include <string>
#include <exception>
#include "Stack.h"
#include "Shape.h"
#include "Allocator.h"
using namespace std;

struct stack_empty : public std::exception
{
    const char* what() const throw ()
    {
        return "The stack is currently empty";
    }
};

void PrintStack(const Stack< Figure<int>, Allocator<Figure<int>, 30> >& stack)
{
    int num = 1;
    for_each(stack.begin(), stack.end(), [&num](auto& e)
    {
        cout << num << ". " << e << endl;
        ++num;
    });
}

int GetNum(void)
{
    int k;
    bool got = false;
    while (!got)
    {
        cin >> k;
        if (!cin.good())
        {
            cout << "Bad input ignored:  " << flush;
            cin.clear();
            cin.ignore(256, '\n');
        }
        else got = true;
    }
    return k;
}

void print_menu(){
	cout
	  << "\n1. Add figure\n"
	    << "2. Print all elements\n"
	    << "3. Amount of figures, which have area less than\n"
	    << "4. Print this menu\n"
	    << "5. Delete element\n"
	    << "6. Delete all elements\n"
	    << "0. Exit\n"
	    << flush;
}

int main()
{
    Stack< Figure<int>, Allocator<Figure<int>, 30> > FigureStack;
    Figure<int> buff;
    int k = 1;
    size_t num;
    double MaxArea;
    print_menu();
    do
    {
        cout << "---------------\nMenu choice: ";
        try
        {
            k = GetNum();
            if (!cin) throw runtime_error("Failed to get input");
            switch (k)
            {
            case 1:
                cin >> buff;
                FigureStack.push(buff);
                k = 1;
                break;
            case 2:
                if (FigureStack.empty()) throw stack_empty();
                PrintStack(FigureStack);
                break;
            case 3:
                if (FigureStack.empty()) throw stack_empty();
                cout << "Max area: ";
                cin >> MaxArea;
                cout << "The amount of Figures that are equal to or lower than " << MaxArea 
                     << " is "
                     << count_if( FigureStack.begin(), FigureStack.end(), [&MaxArea](auto& sqr) -> bool
                        {
                            return ( !sqr.Abstract && (sqr.area < MaxArea || DoubleEqual(sqr.area, MaxArea)) );
                        }
                    ) << endl;
                break;
            case 4:
                print_menu();
                break;
            case 5:
                if (FigureStack.empty()) throw stack_empty();
                cout << "Number of the Figure: ";
                num = GetNum();
                if (num < 1 || num > FigureStack.size()) throw runtime_error("Incorrect index");
                FigureStack.erase(num - 1);
                break;
            case 6:
                FigureStack.erase();
                cout << "Stack erased" << endl;
                break;
            case 0:
                break;
            default:
                cout << "No such number" << endl;
            }

        }
        catch (stack_empty &err)
        {
            cout << err.what() << endl;
        }
        catch (runtime_error &err)
        {
            cerr << err.what() << endl;
        }
        catch (logic_error &err)
        {
            cerr << "In stack: " << err.what() << endl;
        }
        catch (bad_alloc &err)
        {
            cerr << "In allocator: " << err.what() << endl;
        }
        catch (...)
        {
            cerr << "Exception thrown, but undefined. Please open an issue at GitHub with steps to reproduce ." << endl;
        }
    } while (k);
    return 0;
}
