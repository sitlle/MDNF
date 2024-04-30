#include <bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include <utility>

using namespace sf;
using namespace std;

class Function {
 public:
    explicit Function(int input) {
        input_function_ = input;
    }
    vector<int> preobr(int k) {
        vector<int> res;
        while (input_function_) {
            res.push_back(int(input_function_ % 2));
            input_function_ /= 2;
        }
        while (res.size() != (1 << k)) {
            res.push_back(0);
        }
        reverse(res.begin(), res.end());
        return res;
    }
 private:
  long long input_function_;
};

class MakeTablet {
 public:
  explicit MakeTablet(vector<int> a, int n) {
      input_function_ = std::move(a);
      count_of_vars_ = n;
  }

  void resize() {
      table_.resize(input_function_.size(), vector<string> ((1 << count_of_vars_)));
      copy_of_table_.resize(input_function_.size(), vector<int> ((1 << count_of_vars_)));
      out_letters_.resize(table_[0].size());
      used_.resize(table_[0].size());
  }

  int cnt{}, layer{};

  void rec(vector<int>& e, int sz, int last = 1) {
      if (e.size() == sz) {
          for (auto &t : e) {
              table_[layer][cnt] += table_[layer][t];
              if (!used_[cnt]) {
                  out_letters_[cnt] += out_letters_[t];
              }
          }
          letters_number_[out_letters_[cnt]] = cnt;
          used_[cnt] = true;
          cnt++;
          return;
      }
      for (int i = last; i <= count_of_vars_; ++i) {
          e.push_back(i);
          rec(e, sz, i + 1);
          e.pop_back();
      }
  }

  void build() {
      resize();
      for (int i = 0; i < input_function_.size(); ++i) {
          table_[i][0] = char(input_function_[i] + '0');
      }
      int power = (1 << count_of_vars_);
      used_[0] = true;
      out_letters_[0] = "F()";
      for (int j = 1; j <= count_of_vars_; ++j) {
          out_letters_[j] = char('A' + j - 1);
          used_[j] = true;
          int i = 0, p = 0;
          while (i < input_function_.size()) {
              p %= power;
              if (p < power / 2) {
                  table_[i][j] = '0';
              } else {
                  table_[i][j] = '1';
              }
              p++;
              i++;
          }
          power >>= 1;
      }
      cnt = count_of_vars_ + 1;
      for (int sz = 2; sz <= count_of_vars_; ++sz) {
          int mx = 0;
          for (layer = 0; layer < input_function_.size(); ++layer) {
              int lt = cnt;
              vector<int> e;
              rec(e, sz);
              mx = cnt;
              cnt = lt;
          }
          cnt = mx;
      }
  }

  void delete_elements() {
      for (int i = 0; i < input_function_.size(); ++i) {
          for (int j = 0; j < table_[i].size(); ++j) {
              copy_of_table_[i][j] = 0;
          }
      }
      int sz_of_used = 0;
      for (int i = 0; i < input_function_.size(); ++i) {
          if (table_[i][0] == "1") {
              sz_of_used++;
          }
      }
      vector<unordered_map<string, int>> used(table_[0].size());
      for (int i = 0; i < input_function_.size(); ++i) {
          for (int j = 1; j < table_[i].size(); ++j) {
              if (table_[i][0] == "0") {
                  used[j][table_[i][j]] = 1;
                  copy_of_table_[i][j] = 1;
              }
          }
      }
      for (int j = 1; j < table_[0].size(); ++j) {
          for (int i = 0; i < input_function_.size(); ++i) {
              if (used[j][table_[i][j]] && !copy_of_table_[i][j]) {
                  copy_of_table_[i][j] = 2;
              }
          }
      }
      for (int i = 0; i < input_function_.size(); ++i) {
          if (table_[i][0] == "0") {
              continue;
          }
          vector<int> nums_stb;
          for (int j = 1; j < copy_of_table_[i].size(); ++j) {
              if (!copy_of_table_[i][j]) {
                  nums_stb.push_back(j);
              }
          }
          for (int k = 0; k < nums_stb.size(); ++k) {
              if (!copy_of_table_[i][nums_stb[k]]) {
                  for (int p = k + 1; p < nums_stb.size(); ++p) {
                      if (!copy_of_table_[i][nums_stb[p]]) {
                          unordered_map<char, char> e1, e2;
                          for (int t = 0; t < out_letters_[nums_stb[k]].size(); ++t) {
                              e1[out_letters_[nums_stb[k]][t]] = table_[i][nums_stb[k]][t];
                          }
                          for (int t = 0; t < out_letters_[nums_stb[p]].size(); ++t) {
                              e2[out_letters_[nums_stb[p]][t]] = table_[i][nums_stb[p]][t];
                          }
                          bool flag = true;
                          for (auto &t : e1) {
                              if (e2[t.first] != t.second) {
                                  flag = false;
                              }
                          }
                          if (flag) {
                              copy_of_table_[i][nums_stb[p]] = 3;
                          }
                      }
                  }
              }
          }
      }
      for (int i = 0; i < input_function_.size(); ++i) {
          if (table_[i][0] == "0") {
              continue;
          }
          nums.emplace_back();
          for (int j = 1; j < copy_of_table_[i].size(); ++j) {
              if (!copy_of_table_[i][j]) {
                  nums.back().emplace_back(i, j);
              }
          }
      }
      for (int i = 0; i < input_function_.size(); ++i) {
          if (table_[i][0] == "0") {
              copy_of_table_[i][0] = 1;
          }
      }
      cnt_nums_.resize(nums.size(), 0);
      used_str_.resize(table_[0].size(), false);
  }

  bool rv(vector<string>& e1, vector<string>& e2) {
      if (e1.size() != e2.size()) {
          return false;
      }
      bool flag = true;
      vector<bool> used(e2.size(), false);
      for (int i = 0; i < e1.size(); ++i) {
          for (int j = 0; j < e2.size(); ++j) {
              if (!used[j] && e1[i] == e2[j]) {
                  used[j] = true;
              }
          }
      }
      for (auto t : used) {
          flag &= t;
      }
      return flag;
  }

  void find_answers(multiset<pair<int, int>> e1, vector<map<string, int>> mp, int r = 0, int last = 0) {
      bool flagt = true;
      for (int i = 0; i < used_str_.size(); ++i) {
          if (table_[i][0] == "1" && !used_str_[i]) {
              flagt = false;
              break;
          }
      }
      if (last == nums.size()) {
          if (flagt) {
              int cnt_sz = 0;
              for (auto &t : e1) {
                  cnt_sz += (int) out_letters_[t.second].size();
              }
              if (cnt_sz < res_sz_) {
                  res_sz_ = cnt_sz;
                  res_.clear();
              }
              if (cnt_sz == res_sz_) {
                  vector<string> new_push;
                  for (auto &t : e1) {
                      string ee;
                      for (int j = 0; j < out_letters_[t.second].size(); ++j) {
                          if (table_[t.first][t.second][j] == '0') {
                              ee += "!";
                          }
                          ee += out_letters_[t.second][j];
                      }
                      new_push.push_back(ee);
                  }
                  bool flag = false;
                  for (int i = 0; i < res_.size(); ++i) {
                      flag |= rv(new_push, res_[i]);
                      if (flag) {
                          break;
                      }
                  }
                  if (!flag) {
                      res_.push_back(new_push);
                  }
              }
          }
          return;
      }
      if (table_[r][0] == "0") {
          find_answers(e1, mp, r + 1, last);
          return;
      }
      for (int i = 0; i < nums[last].size(); ++i) {
          int k = nums[last][i].second;
          if (mp[k][table_[nums[last][i].first][nums[last][i].second]] == 0) {
              e1.insert(nums[last][i]);
          }
          mp[nums[last][i].second][table_[nums[last][i].first][nums[last][i].second]]++;
          used_str_[r] = true;
          find_answers(e1, mp, r + 1, last + 1);
          used_[last] = false;
          mp[nums[last][i].second][table_[nums[last][i].first][nums[last][i].second]]--;
          if (mp[nums[last][i].second][table_[nums[last][i].first][nums[last][i].second]] == 0) {
              e1.erase(e1.find(nums[last][i]));
          }
      }
  }

  void print_res() {
      for (auto &t : res_) {
          for (auto &e : t) {
              cout << e << ' ';
          }
          cout << '\n';
      }
  }

  void TT() {
      build();
      delete_elements();
      multiset<pair<int, int>> e1;
      vector<map<string, int>> mp;
      mp.resize(table_[0].size());
      find_answers(e1, mp);
      print_res();
  }

  vector<vector<string>> Return_Table() {
      return table_;
  }

  vector<vector<int>> Return_Copy_Of_Table() {
      return copy_of_table_;
  }

  vector<string> Return_Out_Letters() {
      return out_letters_;
  }

  vector<vector<string>> Return_Out_Res() {
      return res_;
  }

 private:
  vector<int> input_function_ = {};
  vector<vector<string>> table_;
  vector<vector<pair<int, int>>> nums;
  vector<vector<int>> copy_of_table_;
  unordered_map<int, bool> used_nums;
  vector<int> cnt_nums_;
  vector<string> out_letters_;
  vector<bool> used_;
  vector<bool> used_str_;
  set<pair<int, int>> del_pairs;
  unordered_map<string, int> letters_number_;
  int res_sz_ = 40000;
  vector<vector<string>> res_;
  int count_of_vars_ = 0;
};

class FrontEnd {
 public:
  explicit FrontEnd() = default;

  static void update_num(Event Main_Project_Window_Event, string& r) {
      if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
          if (!r.empty()) {
              r.pop_back();
          }
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
          r += '0';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
          r += '1';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
          r += '2';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
          r += '3';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
          r += '4';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
          r += '5';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
          r += '6';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
          r += '7';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
          r += '8';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num9)) {
          r += '9';
      }
  }

  void Making_Text(Text& text, string e, pair<int, int> pos, int SZ, Color COLOR) {
      text.setFont(Main_Font_);
      text.setFillColor(COLOR);
      text.setPosition(float(pos.first), float(pos.second));
      text.setCharacterSize(SZ);
      text.setString(e);
  }


  static void DrawRectangle(RenderWindow& Main_Project_Window, int coord_x, int coord_y, int size_x, int size_y, Color COLOR) {
      RectangleShape Rectangle;
      Rectangle.setPosition(float(coord_x), float(coord_y));
      Rectangle.setSize(Vector2f(float(size_x), float(size_y)));
      Rectangle.setFillColor(COLOR);
      Main_Project_Window.draw(Rectangle);
  }

  static void Draw_Rectangles(RenderWindow& Main_Project_Window) {
      DrawRectangle(Main_Project_Window, MAIN_PROJECT_WINDOW_W_ - 300, 30, 300, 140, Color::White);
      DrawRectangle(Main_Project_Window, MAIN_PROJECT_WINDOW_W_ - 300, 230, 300, 140, Color::White);
      DrawRectangle(Main_Project_Window, MAIN_PROJECT_WINDOW_W_ - 300, MAIN_PROJECT_WINDOW_H_ - 600, 300, 100, Color::White);
      DrawRectangle(Main_Project_Window, MAIN_PROJECT_WINDOW_W_ - 300, MAIN_PROJECT_WINDOW_H_ - 400, 300, 100, Color::White);
      DrawRectangle(Main_Project_Window, 0, MAIN_PROJECT_WINDOW_H_ - 250, MAIN_PROJECT_WINDOW_W_ - 300, 250, Color::White);
  }

  void Print_Window_0(RenderWindow& Main_Project_Window) {
      string output = "Write F() number:";
      Text input_f;
      Making_Text(input_f, output, {MAIN_PROJECT_WINDOW_W_ - 300, 30}, 25, Color::Blue);
      Main_Project_Window.draw(input_f);
  }

  void Print_Window_0_TEXT(RenderWindow& Main_Project_Window) {
      Text input_f;
      Making_Text(input_f, input_number_n_, {MAIN_PROJECT_WINDOW_W_ - 300, 100}, 40, Color::Red);
      Main_Project_Window.draw(input_f);
  }

  void Print_Window_1(RenderWindow& Main_Project_Window) {
      string output = "Write count of vars:";
      Text input_vars;
      Making_Text(input_vars, output, {MAIN_PROJECT_WINDOW_W_ - 300, 230}, 25, Color::Blue);
      Main_Project_Window.draw(input_vars);
  }

  void Print_Window_1_TEXT(RenderWindow& Main_Project_Window) {
      Text input_f;
      Making_Text(input_f, input_number_k_, {MAIN_PROJECT_WINDOW_W_ - 300, 300}, 40, Color::Red);
      Main_Project_Window.draw(input_f);
  }

  void Print_Window_2(RenderWindow& Main_Project_Window) {
      string output = "Start algorithm";
      Text output_text;
      Making_Text(output_text, output, {MAIN_PROJECT_WINDOW_W_ - 275, MAIN_PROJECT_WINDOW_H_ - 600}, 30, Color::Magenta);
      Main_Project_Window.draw(output_text);
  }

  void Print_Window_3(RenderWindow& Main_Project_Window) {
      string output = "Delete numbers";
      Text output_text;
      Making_Text(output_text, output, {MAIN_PROJECT_WINDOW_W_ - 280, MAIN_PROJECT_WINDOW_H_ - 400}, 30, Color::Magenta);
      Main_Project_Window.draw(output_text);
  }

  void Print_Windows(RenderWindow& Main_Project_Window) {
      Print_Window_0(Main_Project_Window);
      Print_Window_0_TEXT(Main_Project_Window);
      Print_Window_1(Main_Project_Window);
      Print_Window_1_TEXT(Main_Project_Window);
      Print_Window_2(Main_Project_Window);
      Print_Window_3(Main_Project_Window);
  }

  void Which_Window() {
      if (mouse_pos_.x >= 0 && mouse_pos_.x <= MAIN_PROJECT_WINDOW_W_ - 300) {
          if (mouse_pos_.y >= MAIN_PROJECT_WINDOW_H_ - 250 && mouse_pos_.y <= MAIN_PROJECT_WINDOW_H_) {
              Window_ = 2;
              return;
          }
      }
      if (mouse_pos_.x >= MAIN_PROJECT_WINDOW_W_ - 300 && mouse_pos_.x <= MAIN_PROJECT_WINDOW_W_) {
          if (mouse_pos_.y >= 30 & mouse_pos_.y <= 170) {
              Window_ = 0;
              step_ = -1;
          } else if (mouse_pos_.y >= 230 && mouse_pos_.y <= 370) {
              Window_ = 1;
              step_ = -1;
          } else {
              Window_ = -1;
          }
      } else {
          Window_ = -1;
      }
  }



  void Print_Text(RenderWindow& Main_Project_Window, string e, int cnt_of_output, int sz, int sz1, int sz2, int i, int flag) {
      Text output;
      pair<int, int> pos;
      if (flag == 0) {
          pos = {(sz1 - cnt_of_output * sz) / 2 + i * sz1, (sz2 - sz) / 2};
      } else {
          pos = {(sz1 - sz) / 2, (sz2 - cnt_of_output * sz) / 2 + i * sz2};
      }
      Making_Text(output, e, pos, sz, Color::White);
      Main_Project_Window.draw(output);
  }

  void Print_Text_For_Table(RenderWindow& Main_Project_Window, string e, int cnt_of_output, int sz, int sz1, int sz2, int i, int j) {
      Text output;
      pair<int, int> pos = {(sz1 - cnt_of_output / 2 * sz) / 2 + j * sz1, (sz2 - sz) / 2 + i * sz2};
      Making_Text(output, e, pos, sz, Color::White);
      Main_Project_Window.draw(output);
  }

  static void Print_Colms(RenderWindow& Main_Project_Window, int sz1, int sz2) {
      int points_sz = (MAIN_PROJECT_WINDOW_W_ - 300) / (sz1);
      for (int x = 0; x <= MAIN_PROJECT_WINDOW_W_ - 300; x += points_sz) {
          sf::VertexArray lines(sf::LinesStrip, 2);
          lines[0].position = sf::Vector2f(float(x), 0);
          lines[1].position = sf::Vector2f(float(x), float((int)(int(MAIN_PROJECT_WINDOW_H_ - 300) / sz2) * sz2));
          Main_Project_Window.draw(lines);
      }
  }

  static void Print_Rows(RenderWindow& Main_Project_Window, int sz1, int sz2) {
      int points_sz = (MAIN_PROJECT_WINDOW_H_ - 300) / (sz1);
      for (int y = 0; y <= MAIN_PROJECT_WINDOW_H_ - 300; y += points_sz) {
          sf::VertexArray lines(sf::LinesStrip, 2);
          lines[0].position = sf::Vector2f(0, float(y));
          lines[1].position = sf::Vector2f(float((int)(int(MAIN_PROJECT_WINDOW_W_ - 300) / sz2) * sz2), float(y));
          Main_Project_Window.draw(lines);
      }
  }

  void Print_Out_Letters(RenderWindow& Main_Project_Window, vector<string>& out_letters, int sz1, int sz2, int k) {
      int font_sz = min(sz2, sz1 / k);
      for (int i = 0; i < out_letters.size(); ++i) {
          Print_Text(Main_Project_Window, out_letters[i],
                     (int)out_letters[i].size(), font_sz, sz1, sz2, i, 0);
      }
  }

  void Print_Function(RenderWindow& Main_Window_Project, vector<vector<string>>& table, int sz1, int sz2, int k) {
      int font_sz = min(sz2, sz1 / k);
      for (int i = 1; i <= table.size(); ++i) {
          Print_Text(Main_Window_Project, table[i - 1][0], 1, font_sz, sz1, sz2, i, 1);
      }
  }

  void Print_Out_Table(RenderWindow& Main_Window_Project, vector<vector<string>>& table, int sz1, int sz2, int k) {
      int font_sz = min(sz2, sz1 / k);
      for (int i = 0; i < table.size(); ++i) {
          for (int j = 1; j < table[i].size(); ++j) {
              Print_Text_For_Table(Main_Window_Project, table[i][j], (int)table[i][j].size(), font_sz, sz1, sz2, i + 1, j);
          }
      }
  }

  void Draw_Line(RenderWindow& Main_Project_Window, int first_x, int first_y, int second_x, int second_y, Color COLOR) {
      sf::VertexArray lines(sf::LinesStrip, 2);
      lines[0].position = sf::Vector2f(float(first_x), float(first_y));
      lines[1].position = sf::Vector2f(float(second_x), float(second_y));
      lines[0].color = COLOR;
      lines[1].color = COLOR;
      Main_Project_Window.draw(lines);
  }

  void Print_Res(RenderWindow& Main_Project_Window) {
      int pos_y = MAIN_PROJECT_WINDOW_H_ - 230;
      for (int i = start_; i < min((int)res_.size(), start_ + 3); ++i) {
          int pos_x = 20;
          for (int j = 0; j < res_[i].size(); ++j) {
              Text output;
              string output_str;
              map<char, int> used;
              for (int t = 0; t < res_[i][j].size(); t++) {
                  if (res_[i][j][t] == '!') {
                      t++;
                      output_str += res_[i][j][t];
                      used[res_[i][j][t]] = 1;
                  } else {
                      output_str += res_[i][j][t];
                      used[res_[i][j][t]] = 0;
                  }
              }
              Making_Text(output, output_str, {pos_x, pos_y}, 30, Color::Red);
              Main_Project_Window.draw(output);
              int k = pos_x;
              for (int t = 0; k + 19 <= pos_x + 19 * (int)output_str.size(); k += 19, t++) {
                  if (used[output_str[t]]) {
                      Draw_Line(Main_Project_Window, k, pos_y + 2, k + 12,
                                pos_y + 2, Color::Red);
                  }
              }
              pos_x = k;
              if (j != res_[i].size() - 1) {
                  pos_x += 19;
                  Text dd;
                  Making_Text(dd, "V", {pos_x, pos_y + 8}, 22, Color::Red);
                  Main_Project_Window.draw(dd);
                  pos_x += 35;
              }
          }
          pos_y += 50;
      }
  }

  void Print_Out_Copy_Of_Table(RenderWindow& Main_Window_Project, vector<vector<string>>& table,
                               vector<vector<int>>& copy_of_table, int sz1, int sz2, int k) {
      int font_sz = min(sz2, sz1 / k);
      int num = 0, ct = 0;
      for (int i = 0; i < copy_of_table.size(); ++i) {
          for (int j = 1; j < copy_of_table[i].size(); ++j) {
              num = max(num, copy_of_table[i][j]);
          }
      }
      for (int i = 0; i < copy_of_table.size(); ++i) {
          for (int j = 1; j < copy_of_table[i].size(); ++j) {
              Print_Text_For_Table(Main_Window_Project, table[i][j], (int) table[i][j].size(),
                                   font_sz, sz1, sz2, i + 1, j);
              if (step_ == 1) {
                  if (copy_of_table[i][j] == 1) {
                      ct = 1;
                      Draw_Line(Main_Window_Project,
                                j * sz1,
                                (i + 1) * sz2 + sz2 / 2,
                                ((int) (int(MAIN_PROJECT_WINDOW_W_ - 300) / sz2) * sz2),
                                (i + 1) * sz2 + sz2 / 2,
                                Color::Red);
                  }
              } else if (step_ == 2) {
                  if (copy_of_table[i][j] == 1) {
                      Draw_Line(Main_Window_Project,
                                j * sz1,
                                (i + 1) * sz2 + sz2 / 2,
                                ((int) (int(MAIN_PROJECT_WINDOW_W_ - 300) / sz2) * sz2),
                                (i + 1) * sz2 + sz2 / 2,
                                Color::Red);
                  } else if (copy_of_table[i][j] == 2) {
                      ct = 2;
                      Draw_Line(Main_Window_Project, j * sz1, (i + 1) * sz2,
                                (j + 1) * sz1, (i + 2) * sz2, Color::Green);
                      Draw_Line(Main_Window_Project, (j + 1) * sz1, (i + 1) * sz2,
                                j * sz1, (i + 2) * sz2, Color::Green);
                  }
              } else {
                  if (copy_of_table[i][j] == 1) {
                      Draw_Line(Main_Window_Project,
                                j * sz1,
                                (i + 1) * sz2 + sz2 / 2,
                                ((int) (int(MAIN_PROJECT_WINDOW_W_ - 300) / sz2) * sz2),
                                (i + 1) * sz2 + sz2 / 2,
                                Color::Red);
                  } else if (copy_of_table[i][j] == 2) {
                      Draw_Line(Main_Window_Project, j * sz1, (i + 1) * sz2,
                                (j + 1) * sz1, (i + 2) * sz2, Color::Green);
                      Draw_Line(Main_Window_Project, (j + 1) * sz1, (i + 1) * sz2,
                                j * sz1, (i + 2) * sz2, Color::Green);
                  } else if (copy_of_table[i][j] == 3) {
                      ct = 3;
                      Draw_Line(Main_Window_Project, j * sz1, (i + 1) * sz2,
                                (j + 1) * sz1, (i + 2) * sz2, Color::Blue);
                      Draw_Line(Main_Window_Project, (j + 1) * sz1, (i + 1) * sz2,
                                j * sz1, (i + 2) * sz2, Color::Blue);
                  }
              }
          }
      }
      if (num == ct) {
          Print_Res(Main_Window_Project);
      }
  }

  void Print_Table(RenderWindow& Main_Window_Project, int variant) {
      int n = stoi(input_number_n_);
      int k = stoi(input_number_k_);
      Function func(n);
      vector<int> print = func.preobr(k);
      MakeTablet Make(print, k);
      Make.build();
      vector<vector<string>> table = Make.Return_Table();
      Make.delete_elements();
      multiset<pair<int, int>> e1;
      vector<map<string, int>> mp;
      mp.resize(table[0].size());
      Make.find_answers(e1, mp);
      res_ = Make.Return_Out_Res();
      vector<vector<int>> copy_of_table = Make.Return_Copy_Of_Table();
      vector<string> out_letters = Make.Return_Out_Letters();
      Print_Colms(Main_Window_Project, (int)out_letters.size(), (int)table.size() + 1);
      Print_Rows(Main_Window_Project, (int)table.size() + 1, (int)out_letters.size());
      Print_Out_Letters(Main_Window_Project, out_letters, (MAIN_PROJECT_WINDOW_W_ - 300) / ((int)out_letters.size()),
          (MAIN_PROJECT_WINDOW_H_ - 300) / ((int)table.size() + 1), k);
      Print_Function(Main_Window_Project, table, (MAIN_PROJECT_WINDOW_W_ - 300) / ((int)out_letters.size()),
                     (MAIN_PROJECT_WINDOW_H_ - 300) / ((int)table.size() + 1), k);
      if (variant == 0) {
      Print_Out_Table(Main_Window_Project, table, (MAIN_PROJECT_WINDOW_W_ - 300) / ((int)out_letters.size()),
                      (MAIN_PROJECT_WINDOW_H_ - 300) / ((int)table.size() + 1), k);
      } else {
          Print_Out_Copy_Of_Table(Main_Window_Project, table, copy_of_table,
                                  (MAIN_PROJECT_WINDOW_W_ - 300) / ((int)out_letters.size()),
                          (MAIN_PROJECT_WINDOW_H_ - 300) / ((int)table.size() + 1), k);
      }
  }

  void Main_FrontEnd() {
      RenderWindow Main_Project_Window(VideoMode(MAIN_PROJECT_WINDOW_W_, MAIN_PROJECT_WINDOW_H_), "Zachemtakbolno");
      Main_Font_.loadFromFile("Romance.ttf");
      while (Main_Project_Window.isOpen()) {
          Event Main_Project_Window_Event{};
          while (Main_Project_Window.pollEvent(Main_Project_Window_Event)) {
              if (Main_Project_Window_Event.type == Event::Closed) {
                  Main_Project_Window.close();
              } else if (Main_Project_Window_Event.type == Event::MouseButtonPressed &&
                  Main_Project_Window_Event.mouseButton.button == Mouse::Left) {
                  mouse_pos_ = Mouse::getPosition(Main_Project_Window);
                  click_ = true;
              } else {
                  if (Window_ == -1) {
                      continue;
                  }
                  if (Window_ == 0) {
                      update_num(Main_Project_Window_Event, input_number_n_);
                  } else if (Window_ == 1) {
                      update_num(Main_Project_Window_Event, input_number_k_);
                  } else if (Window_ == 2) {
                      if (Main_Project_Window_Event.type == Event::KeyPressed &&
                          Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                          start_ = min((int)res_.size(), start_ + 1);
                      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
                          Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                          start_ = max(0, start_ - 1);
                      }
                  }
              }
          }
          if (click_) {
              Which_Window();
              if (mouse_pos_.x >= MAIN_PROJECT_WINDOW_W_ - 300 && mouse_pos_.x <= MAIN_PROJECT_WINDOW_W_) {
                  if (mouse_pos_.y >= MAIN_PROJECT_WINDOW_H_ - 600 && mouse_pos_.y <= MAIN_PROJECT_WINDOW_H_ - 500) {
                      step_ = 0;
                  } else if (mouse_pos_.y >= MAIN_PROJECT_WINDOW_H_ - 400 && mouse_pos_.y <= MAIN_PROJECT_WINDOW_H_ - 300) {
                      step_ = min(3, step_ + 1);
                  }
              }
              click_ = false;
          }
          Main_Project_Window.clear();
          Draw_Rectangles(Main_Project_Window);
          Print_Windows(Main_Project_Window);
          if (step_ != -1 && !input_number_n_.empty() && !input_number_k_.empty() && input_number_k_ >= "1") {
              Print_Table(Main_Project_Window, step_);
          }
          Main_Project_Window.display();
      }
  }

 private:
  string input_number_n_;
  string input_number_k_;
  int step_ = -1;
  Font Main_Font_;
  int Window_ = -1;
  static const int MAIN_PROJECT_WINDOW_H_ = 1080;
  static const int MAIN_PROJECT_WINDOW_W_ = 1920;
  bool click_ = false;
  bool print_table = false;
  Vector2i mouse_pos_;
  int last_num_ = -1;
  int start_ = 0;
  vector<vector<string>> res_;
};

int main() {
    FrontEnd Project;
    Project.Main_FrontEnd();
    return 0;
}
