#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <unordered_map>
/*

class Product {
public:
    Product(std::string id, std::string name, int price) :
                id{id}, name{name}, price{price} {}

private:
    std::string id;
    std::string name;
    int price;
    // int availableCount;
};


struct ProductReturn {
    std::weak_ptr<Product> productPtr;
    bool available;

    operator bool() const { return available; }
    std::weak_ptr<Product> operator*() const { return productPtr; }
};


class Cart {

protected:
    std::vector<std::unique_ptr<Product>> product;
    // product.reserve(10);
    // pair<index, count>
    std::unordered_map<std::string, std::pair<int, int>> availableProdeuct;
};

class Catelog : public Cart {

public:
    void addProduct(std::string id, std::string name, int price, size_t numOfProduct) {
        auto findId = availableProdeuct.find(id);
        if (findId != availableProdeuct.end()) {
            availableProdeuct[id].second += numOfProduct;
        }
        else {   // Adding New Product
            product.emplace_back(std::make_unique<Product>(Product{id, name, price}));
            availableProdeuct.emplace(id, std::pair<int, int>(product.size(), numOfProduct));
        }
    }

    ProductReturn findProduct(std::string id) {
        auto findId = availableProdeuct.find(id);
        if (findId != availableProdeuct.end()) {
            
        }
    }

    void removeProduct(std::string id) {

    }


};

*/

class Product {
public:
    Product(std::string id, std::string name, int price)
        : id{id}, name{name}, price{price} {}

    void print() const {
        std::cout << "Product: " << id << " | " << name << " | $" << price << "\n";
    }

private:
    std::string id;
    std::string name;
    int price;
};

struct ProductReturn {
    std::weak_ptr<Product> productPtr;
    bool available;

    operator bool() const { return available; }
    std::shared_ptr<Product> operator*() const { return productPtr.lock(); }
};

class Cart {
protected:
    std::vector<std::shared_ptr<Product>> product;
    // map: id -> (index in vector, available count)
    std::unordered_map<std::string, std::pair<int, int>> availableProduct;
};

class Catalog : public Cart {
public:
    void addProduct(const std::string& id, const std::string& name, int price, size_t numOfProduct) {
        auto findId = availableProduct.find(id);
        if (findId != availableProduct.end()) {
            availableProduct[id].second += numOfProduct;
        } else {
            product.emplace_back(std::make_shared<Product>(id, name, price));
            availableProduct.emplace(id, std::make_pair(product.size() - 1, numOfProduct));
        }
    }

    ProductReturn findProduct(const std::string& id) {
        auto findId = availableProduct.find(id);
        if (findId != availableProduct.end() && findId->second.second > 0) {
            int index = findId->second.first;
            return {product[index], true};
        }
        return {std::weak_ptr<Product>(), false};
    }

    void removeProduct(const std::string& id) {
        auto it = availableProduct.find(id);
        if (it != availableProduct.end()) {
            if (it->second.second > 0) {
                it->second.second--;
                std::cout << "Removed one unit of " << id << ". Remaining: " << it->second.second << "\n";
            }
        }
    }
};

int main() {
    Catalog c;
    c.addProduct("P1", "Phone", 500, 3);
    c.addProduct("P2", "Laptop", 1000, 2);

    auto result = c.findProduct("P1");
    if (result) {
        auto p = *result;
        p->print();
    }

    c.removeProduct("P1");
    c.removeProduct("P1");
    c.removeProduct("P1");
    c.removeProduct("P1"); // No effect when count hits zero
}




// class Jthread {
// public:
//     Jthread() :
    
//     ~Jthread() {
//         t.join();
//     }

// private:
//     std::thread t;

// };

// class car {
//     car(int fueal=10) : fueal()

//     const int fueal;
// }

// void Thread() {
//     std::thread t1(std::ref(Car))
// }


// int main() {

// }
// ShoppingCart