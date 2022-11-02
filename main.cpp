#include <iostream>

#include <vector>

#include <queue>

using namespace std;

int mode = 1;
const int N = 3;     // 3 By 3
const int T = N * N; // 3*3 = 9 for this example
int nodecounter = 0;
int longgest_size = 0;
int solution_key[T] = {1, 2, 3, 4, 5, 6, 7, 8, 0}; // Defult solution.

int testarray1[T] = {1, 2, 3, 4, 5, 6, 7, 8, 0}; // Solution depth 0  //
int testarray2[T] = {1, 2, 3, 4, 5, 6, 7, 0, 8}; // Solution depth 1  //
int testarray3[T] = {1, 2, 3, 4, 5, 6, 0, 7, 8}; // Solution depth 2  //
int testarray4[T] = {1, 3, 6, 5, 0, 2, 4, 7, 8}; // Solution depth 8  //
int testarray5[T] = {1, 3, 6, 5, 0, 7, 4, 8, 2}; // Solution depth 12  //
int testarray6[T] = {1, 6, 7, 5, 0, 3, 4, 8, 2}; // Solution depth 16  //
int testarray7[T] = {7, 1, 2, 4, 8, 5, 6, 3, 0}; // Solution depth 20  //
int testarray8[T] = {0, 7, 2, 4, 6, 1, 3, 5, 8}; // Solution depth 24  //
int testarray9[T] = {1, 2, 3, 4, 5, 6, 7, 7, 0}; // No Solution

// OWN-TEST-CASE
int testarray10[T] = {1, 2, 3, 4, 0, 5, 6, 7, 8}; // Solution depth 14?  //

struct node {
  int depth = 0;   // Or is this 0? FIX ME
  int board[N][N]; // Use array to repesent the board of the 8-puzzle
  int x, y;        // record the position of 0.
  int peviousoperation = 0;
  int cost = 0;
};

vector<node> wholeTree;

int misplaced(int *array, node *a);
int manhattanD(int *solution, node *a);

class Compare { // Override  // Reference 1
public:
  bool operator()(node a, node b) {
    if ((a.depth) + (a.cost) >= (b.depth) + (b.cost)) {
      return true; // a bigger than b, we want the small one
    }
    return false;
  }
};

void expandLegalMoveNode(node *a);
node expand_legalmove(int i, node a);
void setBoardTo(int *array, node *a) { // Set the board to the pass in array
  int index = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      a->board[i][j] = array[index];
      if (array[index] == 0) {
        a->x = i;
        a->y = j;
      }
      if (index < T) {
        index++; // Prevent segmentation fault
      }
    }
  }
}
void boardCopy(node *a,
               node *b) { // Copy a into b; Set the board to the pass in array

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      b->board[i][j] = a->board[i][j];
    }
  }

  b->x = a->x;
  b->y = a->y;
}
void printBoard(
    node a) { // accept any board that pass in, can be call anywhere.
  std::cout << "\n";
  for (int i = 0; i < N; i++) {
    std::cout << "[";
    for (int j = 0; j < N; j++) {
      std::cout << a.board[i][j];
      if (j != N - 1) {
        std::cout << "  ";
      }
    }
    std::cout << "]";
    std::cout << "\n";
  }
  cout << "With the Depth Of " << a.depth << "\n";
  std::cout << "\n";
}
void printBoard(
    node *a) { // accept any board that pass in as *, can be call anywhere.

  cout << "Depth: " << a->depth << "\n";
  std::cout << "\n";
  for (int i = 0; i < N; i++) {
    std::cout << "[";
    for (int j = 0; j < N; j++) {
      std::cout << a->board[i][j];
      if (j != N - 1) {
        std::cout << "  ";
      }
    }
    std::cout << "]";
    std::cout << "\n";
  }
  std::cout << "\n";
}
bool checkmate(int *solution, node a) {
  int index = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (a.board[i][j] != solution[index]) {
        return false;
      }
      index++;
    }
  }
  return true;
}
bool checkmate(int *solution, node *a) {
  int index = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (a->board[i][j] != solution[index]) {
        return false;
      }
      index++;
    }
  }
  return true;
}
void expandLegalMoveNode(node *a) {
  for (int i = 1; i <= 4; i++) {
    expand_legalmove(i, *a);
  }
}
bool ezcheck(int pevious_move, int i, node a) {
  if (i > 4 || i < 1) {
    return false; // Move not possible, option wrong
  }
  if (i == 1) { // moving to left
    if (a.y == 0 ||
        pevious_move ==
            2) { // not allowing to undo the pervious movement (it is pointless)
      return false;
    }
  }
  if (i == 2) { // moving to right
    if (a.y == N - 1 || pevious_move == 1) {
      return false;
    }
  }
  if (i == 3) { // moving to UP
    if (a.x == 0 || pevious_move == 4) {
      return false;
    }
  }
  if (i == 4) { // moving to DOWN
    if (a.x == N - 1 || pevious_move == 3) {
      return false;
    }
  }
  return true;
}
node expand_legalmove(int i, node a) {

  // This is keep unmodify because user can play
  // instead of the search function only
  // 1 is left
  // 2 is right
  // 3 is up
  // 4 is down

  struct node newNode;
  newNode.depth = a.depth + 1;
  boardCopy(&a, &newNode);

  if (i == 1) { // moving to left
    newNode.board[newNode.x][newNode.y] =
        newNode.board[newNode.x][newNode.y - 1];
    newNode.board[newNode.x][newNode.y - 1] = 0;
    newNode.y = newNode.y - 1;
    newNode.peviousoperation = 1;
  }

  if (i == 2) { // moving to right
    newNode.board[newNode.x][newNode.y] =
        newNode.board[newNode.x][newNode.y + 1];
    newNode.board[newNode.x][newNode.y + 1] = 0;
    newNode.y = newNode.y + 1;
    newNode.peviousoperation = 2;
  }

  if (i == 3) { // moving to UP
    newNode.board[newNode.x][newNode.y] =
        newNode.board[newNode.x - 1][newNode.y];
    newNode.board[newNode.x - 1][newNode.y] = 0;
    newNode.x = newNode.x - 1;
    newNode.peviousoperation = 3;
  }

  if (i == 4) { // moving to DOWN
    newNode.board[newNode.x][newNode.y] =
        newNode.board[newNode.x + 1][newNode.y];
    newNode.board[newNode.x + 1][newNode.y] = 0;
    newNode.x = newNode.x + 1;
    newNode.peviousoperation = 4;
  }
  return newNode;
}

bool equal_check(node *a, node *b) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (a->board[i][j] != b->board[i][j]) {
        return false;
      }
    }
  }
  return true;
}

bool check_databae(node *b) {
  for (node a : wholeTree) {
    if (equal_check(&a, b)) {
      return false; // when this node is already in database.
    }
  }
  return true; // this is a new node.
}

//==================================================================
// Need Misplaced
int misplaced(int *array, node *a) {
  int counter = 0;
  int index = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (a->board[i][j] != array[index]) {
        if (array[index] != 0) { // Do not include the blank
          counter++;
        }
      }
      index++;
    }
  }
  return counter;
}
// Need Manhattan Distance heuristic
int manhattanD(int *solution, node *a) {
  // Mdist = |x2 - x1| + |y2-y1|
  int Solutionboard[N][N];
  int index = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      Solutionboard[i][j] = solution[index];
      if (index < T) {
        index++; // Prevent segmentation fault
      }
    }
  }

  int counter = 0;
  bool flag = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (a->board[i][j] != Solutionboard[i][j] && a->board[i][j] != 0) {
        for (int a1 = 0; a1 < N; a1++) {
          for (int b1 = 0; b1 < N; b1++) {
            if (a->board[i][j] == Solutionboard[a1][b1]) {
              counter += (abs(a1 - i) + abs(b1 - j));
              flag = 1;
              break;
            }
            if (flag) {
              break;
            }
          }
          if (flag) {
            flag = 0; // reset flag
            break;
          }
        }
      }
    }
  }
  // cout << "Manhattan Distance: " << counter << endl;
  return counter;
  // return 1;
}

int general_search(int *array, int option) {
  wholeTree.clear();
  nodecounter = 0;
  longgest_size = 0;

  struct node newNode;
  struct node current;
  int found_flag = 0;
  setBoardTo(array, &newNode);
  newNode.depth = 0;

  priority_queue<node, vector<node>, Compare> max_queue;

  if (option == 1) {
    newNode.cost = -newNode.depth + 1;
  }
  if (option == 2) {
    newNode.cost = misplaced(solution_key, &newNode);
  }
  if (option == 3) {
    newNode.cost = manhattanD(solution_key, &newNode);
  }
  max_queue.push(newNode);
  wholeTree.push_back(newNode);

  while (1) {
    if (max_queue.empty()) {
      cout << "There's no solution." << endl;
      return -1;
    }

    if (found_flag || checkmate(solution_key, max_queue.top())) {
      struct node temp2 = max_queue.top();
      cout << "==============================================================="
           << endl;
      cout << "The solution is with a g(n) = " << temp2.depth
           << " and h(n) = " << temp2.cost << ". " << endl;
      printBoard(temp2);
      cout << "Solution depth was: " << temp2.depth << endl;
      cout << "Number of nodes expanded: " << nodecounter << endl;
      cout << "Max queue size: " << longgest_size << endl;
      cout << "==============================================================="
           << endl;
      return 1; // Get out from while 1 Loop
    } else {
      current = max_queue.top(); // Store the current node

      cout << "The best state to expand with a g(n) = " << current.depth
           << " and h(n) = " << current.cost << " is... "
           << "g(n)+h(n)= " << current.depth + current.cost << endl;
      printBoard(current);

      max_queue.pop(); // Pop it because it might get reorder
    }

    struct node temp;

    // Pick the best one
    for (int i = 1; i <= 4; i++) { // See what we can do with the current node
      if (ezcheck(current.peviousoperation, i,
                  current)) {                // if movement is legal, then ....
        temp = expand_legalmove(i, current); // expand this
        // printBoard(&temp);
        if (option == 1) {
          temp.cost = -temp.depth + 1;
          // cout << "Using Option1" << endl;
        }
        if (option == 2) {
          temp.cost = misplaced(solution_key, &temp);
          // cout << "Using Option2" << endl;
        }
        if (option == 3) {
          temp.cost = manhattanD(solution_key, &temp);
          // cout << "Using Option3" << endl;
        }
        // cout << "Cost+Depth = "<< temp.cost + temp.depth << endl;
        if (1) { // check_databae(&temp)
          // cout << "That pushed into the Q!" << endl;
          wholeTree.push_back(temp);
          max_queue.push(temp); // push it into the queue
          nodecounter++;
          if (longgest_size < max_queue.size()) {
            longgest_size = max_queue.size(); // update size
          }
        }
      }
    }
  }
  return -1;
}

int main() {

  int userarray[T]; // Solution depth 8  //
  int option = 3;   // defult the best
  string no_use;
  int flag_input = 0;
  int test_option = 0;
  int test_cases = 0;
  cout << endl << endl;
  std::cout << "Select your options" << endl << endl;
  cout << "1) Uniform Cost Search\n2) A * with the Misplaced Tile "
          "heuristic.\n3) A * with the Manhattan Distance heuristic\n";
  cin >> option;

  if (option < 0 || option > 3) {
    option = 3;
  }

  std::cout << "Enter 1 to enter pre-defined test cases (12 of them) OR 2 to "
               "enter user input puzzle."
            << endl;
  cin >> test_option;

  while (test_option == 2 && flag_input == 0) {
    cout << "==============================================================="
         << endl;
    cout << endl << endl << endl << endl;
    cout << "Enter your inital state (As array, left to right, top to "
            "bottom), "
            "each time with an enter key"
         << endl
         << endl;
    for (int i = 0; i < T; i++) {
      cin >> userarray[i];
    }

    int index = 0;

    for (int i = 0; i < N; i++) {
      cout << "[";
      for (int j = 0; j < N; j++) {
        cout << userarray[index];
        if (j != N - 1) {
          cout << ", ";
        }
        index++;
      }
      cout << "] " << endl;
    }

    // Some easy validation of input can be done here.

    cout << "Was that correct puzzle? (1 to begin; 0 to re-enter) " << endl;
    cin >> flag_input;
    cout << "==============================================================="
         << endl;
  }

  while (test_option == 1 && flag_input == 0) {
    cout << "==============================================================="
         << endl;
    cout << endl << endl << endl << endl;
    cout << "You won't be able to select which test cases, but you can view "
            "them one by one"
         << endl
         << endl;
    cout << "Enter anything when you ready." << endl;
    cin >> no_use;
    general_search(testarray1, option);
    cout << "Enter anything for the next test cases" << endl;
    cin >> no_use;
    general_search(testarray2, option);
    cout << "Enter anything for the next test cases" << endl;
    cin >> no_use;
    general_search(testarray3, option);
    cout << "Enter anything for the next test cases" << endl;
    cin >> no_use;
    general_search(testarray4, option);
    cout << "Enter anything for the next test cases" << endl;
    cin >> no_use;
    general_search(testarray5, option);
    cout << "Enter anything for the next test cases" << endl;
    cin >> no_use;
    general_search(testarray6, option);
    cout << "Enter anything for the next test cases" << endl;
    cin >> no_use;
    general_search(testarray7, option);
    cout << "Enter anything for the next test cases" << endl;
    cin >> no_use;
    general_search(testarray8, option);
    cout << "Enter anything for the next test cases" << endl;
    cin >> no_use;
    general_search(testarray10, option);
    cout << "The last one has no solution, it might take very long for it to "
            "run, Entere? (Y to enter; anything else would quit the program)"
         << endl;
    cin >> no_use;
    if (no_use == "Y") {
      general_search(testarray9, option);
    }

    cout << "New Options? (0 to exsit; 1-3 to choose options) " << endl;
    cin >> flag_input;
    if (flag_input == 0) {
			cout << "BYE BYE" << endl;
      return 0;
    } else {
      option = flag_input;
      flag_input = 0;
    }
    cout << "==============================================================="
         << endl;
  }

  general_search(userarray, option);

  return 0;
}

// Reference 1
// This is my first time using a priority queue,
// I know I need to give define some comparison function for it to do the
// comparison but I have no idea where to start with. This StackOverflow post
// teaches me that I should overwrite the operator function.

// Disclamier1
// Some output messages are 1:1 copies from the sample report.
// This is because I am unsure what output is expected. And I like EK’s output
// format.
