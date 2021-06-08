#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <stack>
#include <cctype>
#include <ctime>

const int BUF_SIZE = 30000;

class BfInterpreter {
private:
  char buffer[BUF_SIZE] = {0};
  std::vector <char> cmd;
  int pos = 0;
  int cmdInd = 0;
  std::stack <int> openSt;
public:
  BfInterpreter() = default;
  BfInterpreter(const BfInterpreter& obj) = delete;
  BfInterpreter& operator= (const BfInterpreter& obj) = delete;
  ~BfInterpreter() = default;
  BfInterpreter(std::string pathToSrc);
  void interpret();
};

BfInterpreter::BfInterpreter(std::string pathToSrc) {
  std::ifstream in(pathToSrc);

  if (!in) {
    throw "Unable to open file";
  }

  char c;
  while (!in.eof()) {
    in.get(c);
    if (isspace(c)) continue;
    cmd.push_back(c);
  }

  in.close();
}

void BfInterpreter::interpret() {
  while (pos >= 0 && pos < BUF_SIZE && cmdInd < cmd.size()) {
    switch (cmd[cmdInd]) {
    case '>':
      ++pos;
      break;
    case '<':
      --pos;
      break;
    case '+':
      ++buffer[pos];
      break;
    case '-':
      --buffer[pos];
      break;
    case '.':
      putchar(buffer[pos]);
      break;
    case ',':
      buffer[pos] = getchar();
      break;
    case '[':
      if (buffer[pos]) {
        openSt.push(cmdInd);
      } else {
        int balance = 1;
        while (cmdInd < cmd.size() && balance != 0) {
          ++cmdInd;
          balance += cmd[cmdInd] == '[' ? 1 : cmd[cmdInd] == ']' ? -1 : 0;
        }
      }
      break;
    case ']':
      if (openSt.empty()) {
        throw "Syntax error";
      } else {
        if (!buffer[pos]) {
          openSt.pop();
        } else {
          cmdInd = openSt.top();
        }
      }
      break;
    default:
      throw "Syntax error";
      break;
    }
    ++cmdInd;
  }
  
}


int main(int argc, char **argv) {
  BfInterpreter bf(argv[1]);
  auto start = clock();
  bf.interpret();
  auto end = clock();
  std::cout << (double)(end - start)/CLOCKS_PER_SEC<< "\n";
  return 0;
}