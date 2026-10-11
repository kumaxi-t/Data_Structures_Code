#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>


std::vector<int> PRIMES = {7, 17, 37, 79, 163, 331, 673, 1361, 2729, 5471};

template<typename K, typename V>
struct HashNode {
  K key;
  V value;
  HashNode* next;

  HashNode(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
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
class HashMap {
private:
  std::vector<HashNode<K, V>*> table;
  size_t num_elements;
  HashFunc<K> hash_func;

  size_t GetBucketIndex(const K& key) const {
    return hash_func(key) % table.size();
  }

public:

  HashMap() {
    table.resize(7, nullptr);
    num_elements = 0;
  }

  // 插入
  void Insert(const K& k, const V& v) {
    CheckAndRehash();
    size_t idx = GetBucketIndex(k);
    HashNode<K, V>* cur = table[idx];
    while(cur != nullptr) {
      // 之前已经插入了，这次只是修改
      if(cur->key == k) {
        cur->value = v;
        return ;
      }
      cur = cur->next;
    }
    // 否则就是没插过
    num_elements++;
    HashNode<K, V>* new_node = new HashNode<K, V>(k, v);
    new_node->next = table[idx];
    table[idx] = new_node;
  }

  // 查找
  V* Find(const K& k) const {
    size_t idx = GetBucketIndex(k);
    HashNode<K, V>* cur = table[idx];
    while(cur != nullptr) {
      if(cur->key == k) {
        return &(cur->value);
      }
      cur = cur->next;
    }
    return nullptr;
  }

  // 删除
  bool Erase(const K& k) {
    size_t idx = GetBucketIndex(k);
    HashNode<K, V>* cur = table[idx];
    HashNode<K, V>* pre = nullptr;
    while(cur != nullptr) {
      if(cur->key == k) {
        if(pre == nullptr) {
          table[idx] = cur->next;
        } else {
          pre->next = cur->next; 
        }
        num_elements--;
        delete cur;
        return true;
      }
      pre = cur;
      cur = cur->next;
    }
    return false;
  }

  // 清除
  void Clear() {
    for(size_t i = 0; i < table.size(); i++) {
      HashNode<K, V>* cur = table[i];
      while(cur != nullptr) {
        HashNode<K, V>* nxt = cur->next;
        delete cur;
        cur = nxt;
      }
      table[i] = nullptr;
    }
    num_elements = 0;
  }

  ~HashMap() {
    Clear();
  }

  // 检查平衡因子是否到达阈值并扩容
  void CheckAndRehash() {
    double a = (double)num_elements / table.size();
    if(a < 0.75) return ;
    // 否则就需要扩容
    // 创建新表把之前的元素都搬过去再覆盖原表
    auto it = upper_bound(PRIMES.begin(), PRIMES.end(), table.size());

    // 扩容达上限了
    if(it == PRIMES.end()) return ;

    int new_capacity = *it;
    std::vector<HashNode<K, V>*> new_table(new_capacity, nullptr);
    for(size_t i = 0; i < table.size(); i++) {
      HashNode<K, V>* cur = table[i];
      while(cur != nullptr) {
        HashNode<K, V>* nxt = cur->next;
        size_t idx = hash_func(cur->key) % new_capacity;
        cur->next = new_table[idx];
        new_table[idx] = cur;
        cur = nxt;
      }
      table[i] = nullptr;
    }
    table = std::move(new_table);
  }




  void PrintTable() const {
    std::cout << "[状态] 元素数量: " << num_elements 
              << " | 当前容量: " << table.size() 
              << " | 装填因子: " << (double)num_elements / table.size() << "\n";
    for (size_t i = 0; i < table.size(); ++i) {
      std::cout << "  Slot [" << i << "]: ";
      HashNode<K, V>* cur = table[i];
      while (cur) {
        std::cout << "{" << cur->key << ": " << cur->value << "} -> ";
        cur = cur->next;
      }
      std::cout << "nullptr\n";
    }
    std::cout << "----------------------------------------\n";
  }
};







int main() {
    HashMap<std::string, int> my_map;

    std::cout << ">>> 1. 连续插入 5 个元素（当前容量 7，未触发扩容）\n";
    my_map.Insert("apple", 10);
    my_map.Insert("banana", 20);
    my_map.Insert("cherry", 30);
    my_map.Insert("date", 40);
    my_map.Insert("fig", 50);
    my_map.PrintTable();

    std::cout << ">>> 2. 插入第 6 个元素（达到 6/7 >= 0.75，触发 Rehash 扩容至 17）\n";
    my_map.Insert("grape", 60);
    my_map.PrintTable();

    std::cout << ">>> 3. 查找测试\n";
    int* val = my_map.Find("banana");
    if (val) {
        std::cout << "Find('banana') 成功，数值为: " << *val << "\n\n";
    }

    std::cout << ">>> 4. 覆盖修改测试（把 apple 从 10 改为 99）\n";
    my_map.Insert("apple", 99);
    my_map.PrintTable();

    std::cout << ">>> 5. 删除测试（删除 cherry）\n";
    my_map.Erase("cherry");
    my_map.PrintTable();

    return 0;
}


