#include "Shape.h"
#include "Stack.h"
using namespace std;

int main()
{
    Stack<int> IntStack;

    cout << "Current stack address: " << IntStack.begin().value.lock().get() << endl;
    cout << "Attempting to perform pop..." << endl;

    try
    {
        IntStack.pop();
    }
    catch(std::logic_error &err)
    {
        cout << err.what() << endl;
    }

    
    cout << "Pushing 5..." << endl;
    IntStack.push(5);
    cout << "Current address " << IntStack.begin().value.lock().get() << " with size of " << IntStack.size() << " and top element " << IntStack.top() << endl;

    cout << "Pushing 1, 7, 7, 0, 1, 3..." << endl;
    IntStack.push(1).push(7).push(7).push(0).push(1).push(3);
    cout << "Current address " << IntStack.begin().value.lock().get() << " with size of " << IntStack.size() << " and top element " << IntStack.top() << endl;

    cout << "Poping one element..." << endl;
    IntStack.pop();
    cout << "Current address " << IntStack.begin().value.lock().get() << " with size of " << IntStack.size() << " and top element " << IntStack.top() << endl;
}

