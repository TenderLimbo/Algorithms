#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>

using namespace std;


// Условие

// Найти вершины, через которые проходит нечётное число наибольших полупутей,
//  и удалить (правым удалением) ту из них, ключ которой наибольший. 
//  Если в дереве нет вершин, удовлетворяющих нужному свойству, 
//  то ничего удалять не требуется. 
//  Выполнить прямой (левый) обход полученного дерева.


struct Node {
  int key;
  int height = 0;
  int leaves_h = 0;
  int max_semipath_length = 0;
  int msl_semipath = 0;
  int a = 0;
  Node* left;
  Node* right;
  Node* parent;
};

class BinaryTreeSearch {
  Node* root;
  int msl;
  vector <int> del_value;
 public:
  BinaryTreeSearch() {
    root = nullptr;
    msl = 0;
  }
  Node* GetRoot() {
    return root;
  }
  int GetMSl() {
    return msl;
  }
  bool DelBalueIsEmpty() {
    return del_value.empty();
  }
  int GetDelValue() {
    return *max_element(del_value.begin(),del_value.end());
  }
  void AddNode(const int& value) {
    Node *node = new Node;
    node->right = nullptr;
    node->left = nullptr;
    node->key = value;
    if (root == nullptr) {
      root = node;
    } else {
      Node *temp = root;
      while (temp != nullptr) {
        node->parent = temp;
        if (value < temp->key) {
          temp = temp->left;
        } else if (value > temp->key) {
          temp = temp->right;
        }
        else break;
      }
      if (node->parent->key > value) {
        node->parent->left = node;
      } else if (node->parent->key < value) {
        node->parent->right = node;
      }
    }
  }
  void MarkB(Node *node) {
    if (node->max_semipath_length == msl) {
      if (node->right != nullptr && node->left == nullptr)
        node->msl_semipath = node->right->leaves_h;
      else if (node->right == nullptr && node->left != nullptr)
        node->msl_semipath = node->left->leaves_h;
      else if (node->right != nullptr && node->left != nullptr)
        node->msl_semipath = node->left->leaves_h*node->right->leaves_h;
      else if (node->right == nullptr && node->left == nullptr) {
        node->msl_semipath = 1;
      }
    }
  }
  void MarkA(Node* node) {
    if (node->right != nullptr && node->left == nullptr)
      node->right->a = node->a + node->msl_semipath;
    else if (node->right == nullptr && node->left != nullptr)
      node->left->a = node->a + node->msl_semipath;
    else if (node->right != nullptr && node->left != nullptr) {
      if (node->right->height > node->left->height) {
        node->right->a = node->a + node->msl_semipath;
        node->left->a = node->msl_semipath;
      }
      else if (node->right->height < node->left->height) {
        node->left->a = node->a + node->msl_semipath;
        node->right->a = node->msl_semipath;
      }
      else if (node->right->height == node->left->height) {
        node->left->a = node->msl_semipath + node->left->leaves_h * node->a/node->leaves_h;
        node->right->a = node->msl_semipath + node->right->leaves_h * node->a/node->leaves_h;
      }
    }
  }
  void CheckNode(Node *node) {
    if ((node->msl_semipath + node->a) % 2 == 1) {
      del_value.push_back(node->key);
    }
  }
  void Traversal(Node* r, ofstream& stream) {
    if (r != nullptr) {
      MarkB(r);
      MarkA(r);
      CheckNode(r);
      stream << r->key << " Height: " << r->height << ' '
      << "MSL : " << r->max_semipath_length << ' '
      <<" Leaves: " << r->leaves_h << ' '
      << " Msl semipath: " << r->msl_semipath << ' '
      << " From parent semipath: " << r->a << ' '
      << " All semipath: " << r->a + r->msl_semipath << ' ' << endl;
      Traversal(r->left,stream);
      Traversal(r->right,stream);
    }
  }

  void Print(Node* r, ofstream& stream) {
    if (r != nullptr) {
      stream << r->key << endl;
      Print(r->left,stream);
      Print(r->right,stream);
    }
  }

  Node* FindNode(const int& value) {
    if (root != nullptr) {
      Node* temp = root;
      while (temp != nullptr){
        if (value < temp->key) {
          temp = temp->left;
        } else if (value > temp->key) {
          temp = temp->right;
        }
        else return temp;
      }
    }
    return nullptr;
  }


  void DeleteNode(const int& value) {
    Node* node = FindNode(value);
    if (node == nullptr)
      return;
    if (node->left == nullptr && node->right != nullptr) {
      if (node == root)
        root = node->right;
      else {
        if (node->parent->key > value)
          node->parent->left = node->right;
        else
          node->parent->right = node->right;
      }
    }
    else if (node->right == nullptr && node->left != nullptr) {
      if (node == root)
        root = node->left;
      else {
        if (node->parent->key > value)
          node->parent->left = node->left;
        else
          node->parent->right = node->left;
      }
    }
    else if (node->right == nullptr && node->left == nullptr) {
      if (node->parent->key > value)
        node->parent->left = nullptr;
      else
        node->parent->right = nullptr;
    }
    else if (node->right->left == nullptr) {
      node->right->left = node->left;
      if (node == root) {
        root = node->right;
      }
      else {
        if (node->parent->key > value)
          node->parent->left = node->right;
        else
          node->parent->right = node->right;
      }
    }
    else if (node->right->left != nullptr) {
      Node* temp = node->right->left;
      while (temp->left != nullptr) {
        temp = temp->left;
      }
      temp->parent->left = temp->right;
      temp->left = node->left;
      temp->right = node->right;
      if (node == root) {
        root = temp;
      }
      else {
        if (node->parent->key > value)
          node->parent->left = temp;
        else
          node->parent->right = temp;
      }
    }
  }

  void MarkHeight(Node *node) {
    if (node->left == nullptr && node->right == nullptr) {
      node->height = 0;
      node->leaves_h = 1;
      node->max_semipath_length = 0;
    }
    else if(node->left == nullptr && node->right != nullptr) {
      node->height = node->right->height+1;
      node->leaves_h = node->right->leaves_h;
      node->max_semipath_length = node->height;
    }
    else if(node->left != nullptr && node->right == nullptr) {
      node->height = node->left->height+1;
      node->leaves_h = node->left->leaves_h;
      node->max_semipath_length = node->height;
    }
    else if (node->left != nullptr && node->right != nullptr) {
      node->height = max(node->right->height,node->left->height) + 1;
      if (node->right->height == node->left->height)
        node->leaves_h = node->right->leaves_h + node->left->leaves_h;
      else if (node->right->height > node->left->height)
        node->leaves_h = node->right->leaves_h;
      else if (node->right->height < node->left->height)
        node->leaves_h = node->left->leaves_h;
      node->max_semipath_length = node->right->height+node->left->height + 2;
    }
    if (msl < node->max_semipath_length)
      msl = node->max_semipath_length;
  }

  void PostOrderTraversal(Node* r) {
    if (r != nullptr) {
      PostOrderTraversal(r->left);
      PostOrderTraversal(r->right);
      MarkHeight(r);
    }
  }
};


int main() {
 // ios_base::sync_with_stdio(false);
  int k;
  ifstream stream("tst.in");
  BinaryTreeSearch bts;
  while (!stream.eof()) {
    stream >> k;
    bts.AddNode(k);
  }
  bts.PostOrderTraversal(bts.GetRoot());
  ofstream output("tst.out");
  if (output) {
    bts.Traversal(bts.GetRoot(), output);
    output << "MSL : " << bts.GetMSl() << endl;
    if (!bts.DelBalueIsEmpty()) {
      bts.DeleteNode(bts.GetDelValue());
      output << "Deleted value : " << bts.GetDelValue() << endl << endl;
    }
    bts.Print(bts.GetRoot(),output);
  }
  return 0;
}
