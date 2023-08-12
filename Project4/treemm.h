#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <list>
#include <iostream>
#include <vector>
template <typename KeyType, typename ValueType>
class TreeMultimap
{
  private:
    struct Node
    {
        //Node constructor to have key and value
        Node(const KeyType& k, const ValueType& v)
        {
          m_key = k;
          m_values.push_back(v);
          left = nullptr;
            right = nullptr;
        }
        KeyType m_key;
        std::vector<ValueType> m_values;
        Node *left, *right;
    };
    Node* head;

    // find recursivily when there is more than one values for one key
    Node* recursive_find(Node* current, const KeyType& key) const
    {
      if (current == nullptr)
        return nullptr;
      else if(key == current->m_key)
        return current;
      else if (key < current->m_key)
        return recursive_find(current->left, key);
      else
        return recursive_find(current->right, key);
      return nullptr;
    }
    // delete all the nodes
    void FreeTree(Node *cur)
    {
      if(cur==nullptr)
        return;
      FreeTree(cur->left);
      FreeTree(cur->right);
      delete cur;
    }

  public:
    class Iterator
    {
      public:
        Iterator()
        {
            curr = nullptr;
          currentValueIndex = 0;
        }
        Iterator(Node* n):curr(n), currentValueIndex(0)
        {
          if(curr != nullptr && curr->m_values.empty())
          {
            advance();
          }
        }
        ValueType& get_value() const
        {
            return curr->m_values.at(currentValueIndex);
        }

        bool is_valid() const
        {
          return (curr != nullptr && currentValueIndex < curr->m_values.size());
        }

        void advance()
        {
          if(!is_valid())
            return;
          currentValueIndex++;
          if (currentValueIndex >= curr->m_values.size())
          {
            curr = nullptr;
          }
        }

      private:
        Node* curr;
        int currentValueIndex;
    };

    TreeMultimap()
    {
        head = nullptr;
    }

    ~TreeMultimap()
    {
      FreeTree(head);
    }

    void insert(const KeyType& k, const ValueType& v)
    {
        if (head == nullptr)
        {
            head = new Node(k, v);
          return;
        }
        Node *cur = head;
        while(true)
        {
          if (k == cur->m_key)
          {
            cur->m_values.push_back(v);
            return;
          }
            // trace using binary search tree -> O(logN)
          if (k < cur->m_key)
          {
            if (cur->left != nullptr)
            {
              cur = cur->left;
            }
            else
            {
              cur->left = new Node(k,v);
              return;
            }
            
          }
          else if(k > cur->m_key)
          {
            if (cur->right != nullptr)
            {
              cur = cur->right;
            }
            else
            {
              cur->right = new Node(k,v);
              return;
            }
          }
        }
    }

    Iterator find(const KeyType& key) const
    {
      Node* node = recursive_find(head, key);
      if (node != nullptr && node->m_key == key)
      {
        return Iterator(node);
      }
      else
        return Iterator(nullptr);
    }
};

#endif // TREEMULTIMAP_INCLUDED
