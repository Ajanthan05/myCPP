#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <unordered_map>


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

class Jthread {
public:
    Jthread() :
    
    ~Jthread() {
        t.join();
    }

private:
    std::thread t;

};

class car {
    car(int fueal=10) : fueal()

    const int fueal;
}

void Thread() {
    std::thread t1(std::ref(Car))
}


int main() {

}
// ShoppingCart