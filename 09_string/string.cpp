#include <iostream>
#include <cstring>
#include <cassert>

namespace my_stl {


  class string {
  private:
    char* _str;
    size_t _size;
    size_t _capacity;
  public:
    // 无符号整数，这里为size_t 所能表示的最大正整数
    static const size_t npos = -1;

    typedef char* iterator;
    typedef const char* const_iterator;   

    size_t size() const {
      return _size;
    }
    size_t capacity() const {
      return _capacity;
    }

    string(const char* str = "") {
      _size =  strlen(str);
      _str = new char[_size + 1];
      for(size_t i = 0; i < _size; i++) {
        _str[i] = str[i];
      }
      _str[_size] = '\0';
      _capacity = _size;
    }

    string(const string& str) {
      _str = new char[str._capacity + 1];
      for(size_t i = 0; i < str.size(); i++) {
        _str[i] = str.c_str()[i];
      }
      _str[str._size] = '\0';
      _size = str.size();
      _capacity = str.capacity();
    }


    string& operator=(const string& str) {
      if(&str == this) return *this;
      delete [] _str;
      _str = new char[str._capacity + 1];
      for(size_t i = 0; i < str.size(); i++) {
        _str[i] = str.c_str()[i];
      }
      _str[str.size()] = '\0';
      _size = str.size();
      _capacity = str.capacity();
      return *this;
    }


    char& operator[](size_t pos) {
      assert(pos < _size);
      return _str[pos];
    }

    const char& operator[](size_t pos) const {
      assert(pos < _size);
      return _str[pos];
    }

    void reserve(size_t n) {
      if(n <= _capacity) return ;
      char* new_str = new char[n + 1];
      for(size_t i = 0; i < _size; i++) {
        new_str[i] = _str[i];
      }
      new_str[_size] = '\0';
      delete [] _str;
      _str = new_str;
      _capacity = n;
    }
    
    void resize(size_t n, char ch = '\0') {
      if(n <= _size) {
        _size = n;
        _str[_size] = '\0';
      } else {
        if(n > _capacity) {
          this->reserve(n);
        }
        for(size_t i = _size; i < n; i++) {
          _str[i] = ch;
        }
        _size = n;
        _str[_size] = '\0';
      }
    }

    void push_back(char ch) {
      if(_size == _capacity) {
        this->reserve(_capacity == 0 ? 4 : _capacity * 2);
      }
      _str[_size] = ch;
      _size++;
      _str[_size] = '\0';
    }

    void append(const char* str) {
      size_t len = strlen(str);
      if(_size + len > _capacity) {
        this->reserve(_size + len);
      }
      for(size_t i = 0; i < len; i++) {
        _str[_size + i] = str[i];
      }
      _size += len;
      _str[_size] = '\0';
    }

    string& insert(size_t pos, char ch) {
      assert(pos <= _size);
      if(_size == _capacity) this->reserve(_capacity == 0 ? 4 : _capacity * 2);
      for(size_t i = _size; i > pos; i--) {
        _str[i] = _str[i - 1]; 
      }
      _str[pos] = ch;
      _size++;
      _str[_size] = '\0';
      return *this;
    } 

    string& insert(size_t pos, const char* str) {
      assert(pos <= _size);
      size_t len = strlen(str);
      if(_size + len > _capacity) this->reserve(_size + len);
      for(size_t i = _size + len; i >= pos + len; i--) {
        _str[i] = _str[i - len];
      }
      for(size_t i = pos; i < pos + len; i++) {
        _str[i] = str[i - pos];
      }
      _size += len;
      _str[_size] = '\0';
      return *this;
    }


    string& erase(size_t pos, size_t len = npos) {
      assert(pos < _size);
      // 往后所有的字符都删了
      if(len == npos || len >= _size - pos) {
        _str[pos] = '\0';
        _size = pos;
      } 
      // 删除指定位置的字符
      else {
        for(size_t i = pos + len; i < _size; i++) {
          _str[i - len] = _str[i];
        }
        _size -= len;
        _str[_size] = '\0';
      }
      return *this;
    }

    size_t find(char ch, size_t pos = 0) const {
      for(size_t i = pos; i < _size; i++) {
        if(_str[i] == ch) return i;
      }
      return npos;
    }

    size_t find(const char* sub, size_t pos = 0) const {
      assert(pos < _size);
      const char* p = strstr(_str + pos, sub);
      if(p == nullptr) return npos;
      else return p - _str;
    }

    string substr(size_t pos = 0, size_t len = npos) const {
      assert(pos < _size);
      string res;
      if(len == npos || len >= _size - pos) {
        len = _size - pos;
      }
      for(size_t i = 0; i < len; i++) {
        res += _str[i + pos];
      }
      return res;
    }


    string& operator+=(const char ch) {
      this->push_back(ch);
      return *this;
    }

    string& operator+=(const char* str) {
      this->append(str);
      return *this;
    }



    iterator begin() { return _str; }
    iterator end() { return _str + _size; }
    const_iterator begin() const { return _str; }
    const_iterator end() const { return _str + _size; }



    void clear() {
      _str[0] = '\0';
      _size = 0;
    }


    const char* c_str() const {
      return _str;
    }



    ~string() {
      delete [] _str;
    }
  };

  std::ostream& operator<<(std::ostream& out, const string& s) {
    for(size_t i = 0; i < s.size(); i++) {
      out << s[i];
    }
    return out;
  }


  std::istream& operator>>(std::istream& in, string& s) {
    s.clear(); 
    char ch = in.get();
    while((ch == ' ' || ch == '\n') && in) ch = in.get();
    while(ch != ' ' && ch != '\n' && in) {
      s += ch;
      ch = in.get();
    }
    return in; 
  }


};
// end my_stl















int main() {
  // 1. 测试构造、深拷贝与赋值（三法则）
  my_stl::string s1("hello");
  my_stl::string s2(s1);
  my_stl::string s3;
  s3 = s2;
  s1[0] = 'H';
  std::cout << "s1: " << s1 << " | s2: " << s2 << " | s3: " << s3 << std::endl;

  // 2. 测试扩容与追加 (push_back, append, +=)
  s1 += ' ';
  s1 += "world!";
  std::cout << "After += : " << s1 << " (size: " << s1.size() << ", cap: " << s1.capacity() << ")" << std::endl;

  // 3. 测试头插、中间插入与删除 (insert, erase)
  s1.insert(0, "Say: ");      // 测试 pos = 0 头插字符串
  s1.insert(s1.size(), '~');  // 测试尾插字符
  std::cout << "After insert: " << s1 << std::endl;

  s1.erase(0, 5);             // 删掉开头的 "Say: "
  std::cout << "After erase : " << s1 << std::endl;

  // 4. 测试迭代器与范围 for
  std::cout << "Range-for   : ";
  for (char ch : s1) {
    std::cout << ch << ' ';
  }
  std::cout << std::endl;

  // 5. 测试查找与子串截取 (find, substr)
  size_t pos = s1.find("world");
  if (pos != my_stl::string::npos) {
    my_stl::string sub = s1.substr(pos, 5);
    std::cout << "Found substr: " << sub << " at pos " << pos << std::endl;
  }

  return 0;
}






