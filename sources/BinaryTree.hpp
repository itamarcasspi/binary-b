#pragma once
#include <iostream>





using namespace std;
namespace ariel
{
    template <typename T>
    class BinaryTree
    {
    private:
        struct Node
        {
            T data;
            Node *father_node, *left_node, *right_node;
            int node_lvl;
            Node() : left_node(nullptr), right_node(nullptr), father_node(nullptr), node_lvl(0) {}
            Node(const T &v) : data(v), left_node(nullptr), right_node(nullptr), father_node(nullptr), node_lvl(0) {}
            
        };

        //binary treeר
        Node *root;
        Node *node_finder(Node *node, T data)
        {
            if (node->data == data)
            {
                return node;
            }
            Node *temp = nullptr;
            if (temp == nullptr && node->left_node != nullptr)
            {
                temp = node_finder(node->left_node, data);
            }
            if (temp == nullptr && node->right_node != nullptr)
            {
                temp = node_finder(node->right_node, data);
            }
            return temp;
        }

        void node_copier(Node *output, Node *input)
        {
            if (input->left_node != nullptr)
            {
                output->left_node = new Node(input->left_node->data);
                output->left_node->father_node = output;
                output->left_node->node_lvl = output->node_lvl + 1;
                node_copier(output->left_node, input->left_node);
            }

            if (input->right_node != nullptr)
            {
                output->right_node = new Node(input->right_node->data);
                output->right_node->father_node = output;
                output->right_node->node_lvl = output->node_lvl + 1;
                node_copier(output->right_node, input->right_node);
            }
        }
        //recursively deletes nodes branching from a node (the root)
        void node_deleter(Node *root)
        {
            if (root != nullptr)
            {
                node_deleter(root->left_node);
                node_deleter(root->right_node);
            }
            delete root;
        }

    public:
        BinaryTree() : root(nullptr) {}
        BinaryTree(const BinaryTree &tree)
        {
            root = new Node(tree.root->data);
            node_copier(root, tree.root);
        }
        BinaryTree(BinaryTree &&tree) 
        {
            root = tree.root;
            tree.root = nullptr;
        }

        ~BinaryTree() { node_deleter(root); };
        BinaryTree &operator=(const BinaryTree<T> &tree)
        {
            if (this != &tree)
            {
                node_deleter(root);
                root = new Node(tree.root->data);
                node_copier(root, tree.root);
            }
            return *this;
        }
        BinaryTree &operator=(BinaryTree<T> &&tree) 
        {
            if (this != &tree)
            {

                node_deleter(root);

                root = tree.root;
                tree.root = nullptr;
            }
            return *this;
        }
        BinaryTree &add_root(const T v)
        {
            if (root != nullptr)
            {
                root->data = v;
            }
            else
            {
                root = new Node(v);
            }
            return *this;
        }
        BinaryTree &add_left(T father, T son)
        {
            if (root == nullptr)
            {
                throw "Error: root node doesnt exist (add_left)";
            }
            Node *temp = node_finder(root, father);
            if (temp != nullptr)
            {
                if (temp->left_node != nullptr)
                {
                    temp->left_node->data = son;
                }
                else
                {
                    temp->left_node = new Node(son);
                    temp->left_node->father_node = temp;
                }
                return *this;
            }
            throw "Error: father node has not been ";
        }
        BinaryTree &add_right(T father, T son)
        {
            if (root == nullptr)
            {
                throw "Error: root node doesnt exist (add_right)";
            }
            Node *temp = node_finder(root, father);
            if (temp != nullptr)
            {
                if (temp->right_node != nullptr)
                {
                    temp->right_node->data = son;
                }
                else
                {
                    temp->right_node = new Node(son);
                    temp->right_node->father_node = temp;
                    temp->right_node->node_lvl = temp->node_lvl + 1;
                }
                return *this;
            }
            throw "Error: father node does not exist in the tree (add_right)";
        }
        //inorder by default
        class iterator
        {
        protected:
            Node *runner;

        public:
            iterator(Node *root = nullptr) : runner(root)
            {
                if (root != nullptr)
                {
                    while (this->runner->left_node != nullptr)
                    {
                        this->runner = this->runner->left_node;
                    }
                }
            }

            T &operator*()
            {
                return runner->data;
            }

            T *operator->()
            {
                return &(runner->data);
            }

            bool operator==(const iterator &it)
            {
                return runner == it.runner;
            }

            bool operator!=(const iterator &tree)
            {
                return runner != tree.runner;
            }
            iterator &operator++()
            {
                if (runner != nullptr)
                {
                    if (runner->right_node != nullptr)
                    {
                        runner = runner->right_node;
                        while (runner->left_node != nullptr)
                        {
                            runner = runner->left_node;
                        }
                    }
                    else
                    {
                        while (runner->father_node != nullptr && runner == runner->father_node->right_node)
                        {
                            runner = runner->father_node;
                        }
                        runner = runner->father_node;
                    }
                }
                return *this;
            }
            iterator operator++(int)
            {
                const iterator i = *this;
                ++*this;
                return i;
            }
        };
        class inorder_iterator : public iterator
        {
        public:
            inorder_iterator(Node *root = nullptr)
            {
                this->runner = root;
                if (root != nullptr)
                {
                    while (this->runner->left_node != nullptr)
                    {
                        this->runner = this->runner->left_node;
                    }
                }
            }

            inorder_iterator &operator++()
            {

                if (this->runner != nullptr)
                {
                    if (this->runner->right_node != nullptr)
                    {

                        this->runner = this->runner->right_node;
                        while (this->runner->left_node != nullptr)
                        {
                            this->runner = this->runner->left_node;
                        }
                    }
                    else
                    {
                        while (this->runner->father_node != nullptr && this->runner == this->runner->father_node->right_node)
                        {
                            this->runner = this->runner->father_node;
                        }
                        this->runner = this->runner->father_node;
                    }
                }
                return *this;
            }
            inorder_iterator operator++(int)
            {
                const inorder_iterator i = *this;
                ++*this;
                return i;
            }
        };

        class preorder_iterator : public iterator
        {

        public:
            preorder_iterator(Node *root = nullptr)
            {
                this->runner = root;
            }
            preorder_iterator &operator++()
            {
                if (this->runner->left_node != nullptr)
                {
                    this->runner = this->runner->left_node;
                }
                else if (this->runner->right_node != nullptr)
                {
                    this->runner = this->runner->right_node;
                }
                else
                {
                    //as long as the father node exist and runner is a right son of some node
                    while (this->runner->father_node != nullptr && this->runner->father_node->right_node == nullptr)
                    {
                        this->runner = this->runner->father_node;
                        while (this->runner->father_node != nullptr && (this->runner == this->runner->father_node->right_node || this->runner->father_node->right_node == nullptr))
                        {
                            this->runner = this->runner->father_node;
                        }
                    }
                    while (this->runner->father_node != nullptr && this->runner->father_node->right_node == this->runner)
                    {
                        this->runner = this->runner->father_node;
                        while (this->runner->father_node != nullptr && this->runner->father_node->right_node == nullptr)
                        {
                            this->runner = this->runner->father_node;
                        }
                    }
                    if (this->runner->father_node == nullptr)
                    {
                        this->runner = nullptr;
                    }
                    else
                    {
                        this->runner = this->runner->father_node->right_node;
                    }
                }

                return *this;
            }

            preorder_iterator operator++(int)
            {
                preorder_iterator i = *this;
                ++*this;
                return i;
            }
        };

        class postorder_iterator : public iterator
        {

        public:
            postorder_iterator(Node *root = nullptr)
            {
                this->runner = root;
                if (root != nullptr)
                {
                    while (this->runner->left_node != nullptr || this->runner->right_node != nullptr)
                    {
                        if (this->runner->left_node != nullptr)
                        {
                            this->runner = this->runner->left_node;
                        }
                        else
                        {
                            this->runner = this->runner->right_node;
                        }
                    }
                }
            }

            postorder_iterator &operator++()
            {
                if (this->runner != nullptr)
                {
                    Node *tmp = this->runner;
                    if (this->runner->father_node != nullptr)
                    {
                        this->runner = this->runner->father_node;
                        if (this->runner->right_node != nullptr && this->runner->right_node != tmp)
                        {
                            this->runner = this->runner->right_node;
                            while (this->runner->left_node != nullptr || this->runner->right_node != nullptr)
                            {
                                if (this->runner->left_node != nullptr)
                                {
                                    this->runner = this->runner->left_node;
                                }
                                else
                                {
                                    this->runner = this->runner->right_node;
                                }
                            }
                        }
                    }
                    else
                    {
                        this->runner = this->runner->father_node;
                    }
                }
                return *this;
            }
            postorder_iterator operator++(int)
            {
                postorder_iterator i = *this;
                ++*this;
                return i;
            }
        };

        auto begin()
        {
            return begin_inorder();
        }

        auto end()
        {
            return end_inorder();
        }

        auto begin_inorder()
        {
            return inorder_iterator{root};
        }

        auto end_inorder()
        {
            return inorder_iterator{};
        }

        auto begin_preorder()
        {
            return preorder_iterator{root};
        }

        auto end_preorder()
        {
            return preorder_iterator{};
        }

        auto begin_postorder()
        {
            return postorder_iterator{root};
        }

        auto end_postorder()
        {
            return postorder_iterator{};
        }

        

        // friend std::ostream &operator<<(std::ostream &ostream, const BinaryTree &tree)
        // {
        // }

    
        };
    }