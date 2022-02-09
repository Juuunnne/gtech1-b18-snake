#include <iostream>
#include "snake.hpp"

using namespace std;

int main(int argc, char * argv[])
{
    Game game = Game();
    Game().Run();
    cout << "Game has terminated successfully" << endl;
    return 0;
}