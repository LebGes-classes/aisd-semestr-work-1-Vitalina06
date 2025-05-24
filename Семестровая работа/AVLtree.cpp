#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

template <typename T>
class AVLTree {
private:
    struct Node {
        T value;
        Node* left;
        Node* right;
        int height;
        
        Node(const T& val) : value(val), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    // Вспомогательные функции
    int getHeight(Node* node) const {
        return node ? node->height : 0;
    }

    void updateHeight(Node* node) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    int getBalance(Node* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    // Повороты для балансировки
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        
        updateHeight(x);
        updateHeight(y);
        
        return y;
    }

    // Балансировка узла
    Node* balance(Node* node) {
        updateHeight(node);
        int balanceFactor = getBalance(node);
        
        // Левое поддерево слишком высокое
        if (balanceFactor > 1) {
            if (getBalance(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        
        // Правое поддерево слишком высокое
        if (balanceFactor < -1) {
            if (getBalance(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        
        return node;
    }

    // Вставка значения в поддерево
    Node* insert(Node* node, const T& value) {
        if (!node) return new Node(value);
        
        if (value < node->value) {
            node->left = insert(node->left, value);
        } else if (node->value < value) {
            node->right = insert(node->right, value);
        } else {
            return node; // Дубликаты не допускаются
        }
        
        return balance(node);
    }

    // Поиск минимального значения в поддереве
    Node* findMin(Node* node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // Удаление узла с минимальным значением
    Node* removeMin(Node* node) {
        if (!node->left) {
            return node->right;
        }
        node->left = removeMin(node->left);
        return balance(node);
    }

    // Удаление значения из поддерева
    Node* remove(Node* node, const T& value) {
        if (!node) return nullptr;
        
        if (value < node->value) {
            node->left = remove(node->left, value);
        } else if (node->value < value) {
            node->right = remove(node->right, value);
        } else {
            Node* left = node->left;
            Node* right = node->right;
            delete node;
            
            if (!right) return left;
            
            Node* min = findMin(right);
            min->right = removeMin(right);
            min->left = left;
            return balance(min);
        }
        
        return balance(node);
    }

    // Очистка памяти
    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // Рекурсивный вывод дерева
    void printTree(Node* node, const std::string& prefix = "", bool isLeft = false) const {
        if (node) {
            std::cout << prefix;
            std::cout << (isLeft ? "├──" : "└──");
            std::cout << node->value << " (h:" << node->height << ", b:" << getBalance(node) << ")" << std::endl;

            printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
            printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
        }
    }

    // Обходы дерева
    void inOrder(Node* node, std::vector<T>& result) const {
        if (node) {
            inOrder(node->left, result);
            result.push_back(node->value);
            inOrder(node->right, result);
        }
    }

    void preOrder(Node* node, std::vector<T>& result) const {
        if (node) {
            result.push_back(node->value);
            preOrder(node->left, result);
            preOrder(node->right, result);
        }
    }

    void postOrder(Node* node, std::vector<T>& result) const {
        if (node) {
            postOrder(node->left, result);
            postOrder(node->right, result);
            result.push_back(node->value);
        }
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { clear(root); }

    // Основные операции
    void insert(const T& value) {
        root = insert(root, value);
    }

    void remove(const T& value) {
        root = remove(root, value);
    }

    bool contains(const T& value) const {
        Node* current = root;
        while (current) {
            if (value < current->value) {
                current = current->left;
            } else if (current->value < value) {
                current = current->right;
            } else {
                return true;
            }
        }
        return false;
    }

    bool empty() const {
        return root == nullptr;
    }

    // Вывод дерева
    void print() const {
        if (root) {
            std::cout << "АВЛ-дерево (h - высота, b - баланс):\n";
            printTree(root);
        } else {
            std::cout << "Дерево пусто\n";
        }
        std::cout << std::endl;
    }

    // Получение элементов в разных порядках
    std::vector<T> inOrderTraversal() const {
        std::vector<T> result;
        inOrder(root, result);
        return result;
    }

    std::vector<T> preOrderTraversal() const {
        std::vector<T> result;
        preOrder(root, result);
        return result;
    }

    std::vector<T> postOrderTraversal() const {
        std::vector<T> result;
        postOrder(root, result);
        return result;
    }
};

// Пример использования
int main() {
    AVLTree<int> tree;

    // Вставка элементов
    std::cout << "Вставляем элементы: 10, 20, 30, 40, 50, 25\n";
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);
    tree.print();

    // Проверка наличия элементов
    std::cout << "Содержит 30: " << (tree.contains(30) ? "да" : "нет") << "\n";
    std::cout << "Содержит 35: " << (tree.contains(35) ? "да" : "нет") << "\n\n";

    // Удаление элемента
    std::cout << "Удаляем 30\n";
    tree.remove(30);
    tree.print();

    // Обходы дерева
    std::cout << "In-order обход: ";
    for (int val : tree.inOrderTraversal()) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    // Еще операции
    std::cout << "Вставляем 15, 5, 35\n";
    tree.insert(15);
    tree.insert(5);
    tree.insert(35);
    tree.print();

    std::cout << "Pre-order обход: ";
    for (int val : tree.preOrderTraversal()) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    return 0;
}