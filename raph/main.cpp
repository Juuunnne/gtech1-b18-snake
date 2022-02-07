#include "Game.hpp"
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
    Game game = Game();
    Game().Run();
    cout << "Game has terminated successfully" << endl;
    return 0;
}