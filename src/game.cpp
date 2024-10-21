#include "board.cpp"


int main(int argc, char * argv[])
{
  Board B = Board();

  int input1;
  std::cin >> input1;  
  int input2;
  std::cin >> input2;
  std::vector<std::pair<int,int>> moves = B.getValidMoves(input1,input2);
  for (int i = 0; i < moves.size(); i++) {
    std::cout << moves[i].first << " " << moves[i].second << std::endl;
  }


  return 0;
}