#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

const std::pair<int, int> BadIndex (-1, -1);

std::pair<int, int> find(std::vector<int>& matrix, int col, bool stepOnRow, int start, int element) {
  std::pair<int, int> result = BadIndex;
  auto& [r, c] = result;
  
  int stride = (stepOnRow) ? 1 : col;
  int max    = (stepOnRow) ? (start = start * col, start + col) : matrix.size();
  
  for (;  start < max; start += stride) {
    if (matrix[start] == element) {
      r = start / col;
      c = start % col;
      
      matrix[start] = -1;
      
      printf("Element : %X;\tPos %ix%i\n", element, r + 1, c + 1);
      break;
    }
  }
  
  return result;
}

std::vector<std::pair<int, int>> find_all(std::vector<int>& matrix, int col, bool stepOnRow, int start, int element) {
  std::vector<std::pair<int, int>> result;
  
  std::pair<int, int> tmp = find(matrix, col, stepOnRow, start, element);
  int* step = (stepOnRow) ? &tmp.first : &tmp.second;
  
  while (tmp != BadIndex) {
    result.push_back(tmp);
    tmp = find(matrix, col, stepOnRow, *step, element);
  }
  
  return result;
}

struct FileContent {
  int startRow {0};
  int columns {0};
  std::vector<int> inputCodes;
  std::vector<int> codeMatrix;
  
  int& operator[](std::pair<int, int> id) {
    auto& [r, c] = id;
    
    return codeMatrix[r * c];
  }
};
  
std::istream& operator>>(std::istream& is, FileContent& content) {
  std::string buffer;  
      
  is >> content.startRow;
  
  int size = 0;
  is >> size;
  content.inputCodes.resize(size);
  for (size_t i = 0; i < content.inputCodes.size(); ++i) {
    is >> buffer;
    content.inputCodes[i] = std::stoi(buffer, nullptr, 16);
  }
  
  is >> content.columns;
  content.codeMatrix.resize( std::pow(content.columns, 2) );
  for (size_t i = 0; i < content.codeMatrix.size(); ++i) {
    is >> buffer;
    content.codeMatrix[i] = std::stoi(buffer, nullptr, 16);
  }
  
  return is;
}
 
struct Node {
  int value;
  std::vector<Node> childs;
};

std::vector<std::vector<Node*>> __fastcall expandTree(Node& root) {
  std::vector<std::vector<Node*>> result;
  
  for ( auto& child : root.childs ) {
    auto tmp = expandTree(child);
    
    for (auto& vec : tmp) {
      result.push_back( { } );
      result.back().insert(result.back().end(), vec.begin(), vec.end());
    }
  }
  
  if (root.childs.size() == 0) {
    result.push_back( { &root } );
  }
  else {
    for(auto& t : result) {
      t.insert(t.begin(), &root);
    }
  }
  
  return result;
}

std::vector<Node> __fastcall getSequence(std::vector<int>& matrix, int columns, bool stepOnRow, int start, std::vector<int> codes) {
  static int I = -1;
  I++;
  
  std::vector<Node> result;
 
  auto locations = find_all(matrix, columns, stepOnRow, start, codes[I]);
  stepOnRow = !stepOnRow;
  
  for (auto& [r, c] : locations) {
    int* value = (stepOnRow) ? &r : &c;
    
    result.push_back( { *value, {} } );
    result.back().childs = getSequence(matrix, columns, stepOnRow, *value, codes);
  }
  
  I--;
  return result;
}

int main() {
  std::string inputFile = "b3.txt";
  
  std::ifstream inData(inputFile);
  
  if (!inData) {
    std::cout << "File '" << inputFile << "' didn't open\n";
    return 2;
  }
  
  FileContent file;
  inData >> file;
  inData.close();
  
  std::vector<Node> sequences = getSequence(file.codeMatrix, file.columns, true, file.startRow - 1, file.inputCodes);
  
  for(auto& tree : sequences) {
    for (auto& vectors : expandTree(tree)) {
      for (auto& node : vectors) {
        std::cout << (node->value + 1) << ' ';
      }
      std::cout << std::endl;
    }
  }
  
  return 0;
}
