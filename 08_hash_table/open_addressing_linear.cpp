#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<int> PRIMES = {7, 17, 37, 79, 163, 331, 673, 1361, 2729, 5471};


enum class State {
  EMPTY,
  EXIST,
  DELETE
};

template<typename K, typename V>
struct HashEntry {
  K key;
  V value;
  State state;

  HashEntry(const K& k, const V& v) : key(k), value(v), state(State::EXIST) {}

  HashEntry() : state(State::EMPTY) {}
};


template<typename K>
struct HashFunc {
  size_t operator()(const K& key) const {
    return static_cast<size_t>(key);
  }
};

template<>
struct HashFunc<std::string> {
  size_t operator()(const std::string& str) const {
    // BKDR字符串哈希算法
    size_t hash = 0;
    for(auto &ch : str) {
      hash = hash * 131 + ch;
    }
    return hash;
  }
};

template<class K, class V>
class HashTable {
private:
  std::vector<HashEntry<K, V>> table;
  int num_elements;
  HashFunc<K> hash_func;

  size_t GetBucketIndex(const K& key) const {
    return hash_func(key) % table.size();
  }

public:
  HashTable() : num_elements(0) {
    table.resize(PRIMES[0]);
  }

  V* Find(const K& k) {
    size_t idx = GetBucketIndex(k);
    size_t cnt = 0;
    while(cnt < table.size() && table[idx].state != State::EMPTY) {
      if(table[idx].state == State::EXIST) {
        if(table[idx].key == k) {
          return &(table[idx].value);
        }
      }
      idx = (idx + 1) % table.size();
      cnt++;
    }
    return nullptr;
  }

  bool Erase(const K& k) {
    size_t idx = GetBucketIndex(k);
    size_t cnt = 0;
    while(cnt < table.size() && table[idx].state != State::EMPTY) {
      if(table[idx].state == State::EXIST) {
        if(table[idx].key == k) {
          table[idx].state = State::DELETE;
          return true;
        }
      }
      idx = (idx + 1) % table.size();
      cnt++;
    }
    num_elements--;
    return false;
  }


  void Insert(const K& k, const V& v) {
    CheckAndRehash();
    size_t idx = GetBucketIndex(k);
    size_t cnt = 0;
    int first_del_idx = -1;
    while(cnt < table.size()) {
      if(table[idx].state == State::EXIST && table[idx].key == k) {
        table[idx].value = v;
        return ;
      }
      if(table[idx].state == State::DELETE && first_del_idx == -1) {
        first_del_idx = idx;
      }
      if(table[idx].state == State::EMPTY) {
        break;
      }
      idx = (idx + 1) % table.size();
      cnt++;
    }
    size_t final_idx = (first_del_idx == -1 ? idx : first_del_idx);
    table[final_idx].key = k;
    table[final_idx].value = v;
    table[final_idx].state = State::EXIST;
    num_elements++;
    return ;
  }

  void CheckAndRehash() {
    double factor = (double)num_elements / table.size();
    if(factor < 0.70) return ;
    auto it = std::upper_bound(PRIMES.begin(), PRIMES.end(), table.size());
    if(it == PRIMES.end()) return ;
    int new_capacity = *it;
    std::vector<HashEntry<K, V>> new_table(new_capacity);
    for(size_t i = 0; i < table.size(); i++) {
      if(table[i].state != State::EXIST) continue;
      size_t new_idx = hash_func(table[i].key) % new_capacity;
      while(new_table[new_idx].state == State::EXIST) new_idx = (new_idx + 1) % new_capacity;
      new_table[new_idx].state = State::EXIST;
      new_table[new_idx].key = table[i].key;
      new_table[new_idx].value = table[i].value;
    }
    table = std::move(new_table);
  }

  void Clear() {
    for(size_t i = 0; i < table.size(); i++) {
      table[i] = HashEntry<K, V>();
    }
    num_elements = 0;
  }

  ~HashTable() {
    Clear();
  }

  // 观察槽位分布与状态机变化
  void PrintTable() const {
    std::cout << "[状态] 元素数量: " << num_elements 
              << " | 当前容量: " << table.size() 
              << " | 装填因子: " << (double)num_elements / table.size() << "\n";
    for (size_t i = 0; i < table.size(); ++i) {
      std::cout << "  Slot [" << i << "]: ";
      if (table[i].state == State::EMPTY) {
        std::cout << "[EMPTY]\n";
      } else if (table[i].state == State::DELETE) {
        std::cout << "[DELETE/TOMBSTONE] (曾为 key: " << table[i].key << ")\n";
      } else {
        std::cout << "[EXIST] {" << table[i].key << ": " << table[i].value << "}\n";
      }
    }
    std::cout << "----------------------------------------\n";
  }
};

int main() {
    HashTable<std::string, int> ht;

    std::cout << ">>> 1. 连续插入 4 个元素（初始容量 7）\n";
    ht.Insert("apple", 10);
    ht.Insert("banana", 20);
    ht.Insert("cherry", 30);
    ht.Insert("date", 40);
    ht.PrintTable();

    std::cout << ">>> 2. 测试删除（打上墓碑标记）\n";
    ht.Erase("banana");
    ht.PrintTable();

    std::cout << ">>> 3. 测试覆盖与新插入（观察墓碑是否被复用）\n";
    ht.Insert("banana", 200); // 重新插入已被删除的 key
    ht.Insert("fig", 50);
    ht.PrintTable();

    std::cout << ">>> 4. 插入触发扩容（阈值 >= 0.70，扩容至 17 并自动清理所有墓碑）\n";
    ht.Insert("grape", 60);
    ht.PrintTable();

    std::cout << ">>> 5. 查找测试\n";
    int* val = ht.Find("cherry");
    if (val) {
      std::cout << "查找 cherry 成功，值为: " << *val << "\n";
    } else {
      std::cout << "查找 cherry 失败\n";
    }

    return 0;
}