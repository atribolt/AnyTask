#include <vector>
#include <numeric>
#include <iostream>
#include <algorithm>

using ii = std::pair<int, int>;
using vii = std::vector<ii>;

float length(ii i) {
  auto [x, y] = i;
  return sqrt(pow(x, 2) + pow(y, 2));
}

ii operator-(ii a, ii b) {
  auto [ax, ay] = a;
  auto [bx, by] = b;

  return { ax - bx, ay - by };
}

int main() {
  vii in = { {1,1}, {8,6}, {6,8}, {1,3} };
  std::vector<std::pair<ii, float>> len;

  for (size_t i = 0; i < in.size(); ++i) {
    for (size_t j = i + 1; j < in.size(); ++j) {
      len.push_back({ {i, j},  length(in[i] - in[j]) });
    }
  }

  std::sort(len.begin(), len.end(), [] (std::pair<ii, float> const& a, std::pair<ii, float> const& b) {
    return a.second < b.second;
  });

  float value = std::accumulate(len.begin(), len.begin() + (in.size() / 2), 0.f, [](float a, std::pair<ii, float> const& b) { return a + b.second; });

  std::cout << value << std::endl;
  return 0;
}
