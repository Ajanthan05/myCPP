#include <iostream>
#include <memory>
#include <string>

// Base class for PDUs
class PDU {
public:
    virtual void process() = 0;
    virtual ~PDU() = default;
};

class PUSCH_PDU : public PDU {
public:
    void process() override { std::cout << "Processing PUSCH PDU\n"; }
};

class PUCCH_PDU : public PDU {
public:
    void process() override { std::cout << "Processing PUCCH PDU\n"; }
};

// Factory
class PDUFactory {
public:
    static std::unique_ptr<PDU> create(const std::string& type) {
        if (type == "pusch") return std::make_unique<PUSCH_PDU>();
        if (type == "pucch") return std::make_unique<PUCCH_PDU>();
        return nullptr;
    }
};
void Factory_Pattern() {
    auto pdu = PDUFactory::create("pusch");
    if (pdu) pdu->process();  // Output: Processing PUSCH PDU
}


// ------------------------------------- Builder Pattern ----------------------------
class PDU2 {
public:
    int sfn{};
    int slot{};
    std::string payload;

    void show() {
        std::cout << "PDU: sfn=" << sfn << " slot=" << slot
                  << " payload=" << payload << "\n";
    }
};

// Builder
class PDUBuilder {
    PDU2 pdu;
public:
    PDUBuilder& setSfn(int val) { pdu.sfn = val; return *this; }
    PDUBuilder& setSlot(int val) { pdu.slot = val; return *this; }
    PDUBuilder& setPayload(const std::string& val) { pdu.payload = val; return *this; }

    PDU2 build() { return pdu; }
};

void Builder_Pattern() {
    PDU2 pdu = PDUBuilder()
                .setSfn(10)
                .setSlot(5)
                .setPayload("Downlink Data")
                .build();

    pdu.show();  // Output: PDU: sfn=10 slot=5 payload=Downlink Data
}




// Product
class House {
public:
    std::string walls;
    std::string roof;
    bool hasGarage = false;
    bool hasPool = false;

    void show() {
        std::cout << "House with " << walls << " walls, " << roof
                  << " roof, garage=" << hasGarage
                  << ", pool=" << hasPool << "\n";
    }
};

// Builder Interface
/*  When an object has too many parameters (some optional, some mandatory).

When you want to create different representations of the same type of object (e.g., Car vs. Truck).

When construction logic would make the constructor messy.

class House {
public:
    House(int rooms, bool hasGarden, bool hasGarage, bool hasSwimmingPool, std::string roofType);
    // ... imagine 10+ parameters 😵
};
This leads to:

“Telescoping constructors” → many overloaded constructors.

Hard to read, hard to maintain.

🔹 Solution (Builder Pattern)
Encapsulate the construction process in a separate Builder object.

The client calls step-by-step methods to set parts.

Finally, call a build() (or getResult()) method to get the product.


🔹 Key Points

Product (House): the object we’re building.

Builder Interface: defines the steps (buildWalls, addGarage...).

Concrete Builder: implements the actual steps.

Director (optional): can orchestrate the steps if a standard sequence is needed.

Client: decides which steps to call.

🔹 When to Use

When constructors or static factories are getting ugly.

When you need different configurations of the same type.

When object creation should be readable and flexible.*/

/*
class HouseBuilder {
public:
    virtual ~HouseBuilder() = default;
    virtual HouseBuilder* buildWalls(const std::string& type) = 0;
    virtual HouseBuilder* buildRoof(const std::string& type) = 0;
    virtual HouseBuilder* addGarage() = 0;
    virtual HouseBuilder* addPool() = 0;
    virtual House build() = 0;
};

// Concrete Builder
class ConcreteHouseBuilder : public HouseBuilder {
private:
    House house;
public:
    HouseBuilder* buildWalls(const std::string& type) override {
        house.walls = type;
        return this;
    }
    HouseBuilder* buildRoof(const std::string& type) override {
        house.roof = type;
        return this;
    }
    HouseBuilder* addGarage() override {
        house.hasGarage = true;
        return this;
    }
    HouseBuilder* addPool() override {
        house.hasPool = true;
        return this;
    }
    House build() override {
        return house;
    }
};
*/

class HouseBuilder {
public:
    virtual ~HouseBuilder() = default;
    virtual HouseBuilder& buildWalls(const std::string& type) = 0;
    virtual HouseBuilder& buildRoof(const std::string& type) = 0;
    virtual HouseBuilder& addGarage() = 0;
    virtual HouseBuilder& addPool() = 0;
    virtual House build() = 0;
};

class ConcreteHouseBuilder : public HouseBuilder {
private:
    House house;
public:
    HouseBuilder& buildWalls(const std::string& type) override {
        house.walls = type;
        return *this;  // return reference to the current object
    }
    HouseBuilder& buildRoof(const std::string& type) override {
        house.roof = type;
        return *this;
    }
    HouseBuilder& addGarage() override {
        house.hasGarage = true;
        return *this;
    }
    HouseBuilder& addPool() override {
        house.hasPool = true;
        return *this;
    }
    House build() override {
        return house;
    }
};


// Client
void Builder_Pattern2() {
    ConcreteHouseBuilder builder;

    House luxuryHouse = builder
        .buildWalls("brick")
        .buildRoof("slate")
        .addGarage()
        .addPool()
        .build();

/* dot-chaining 
What is Dot Chaining?

“Dot chaining” (a.k.a. method chaining or fluent interface) is when each method 
returns the same object (or a reference to it), so you can immediately call another 
method on the result — instead of breaking it up into multiple statements.

builder.buildWalls("brick");
builder.buildRoof("slate");
builder.addGarage();
builder.addPool();
House luxuryHouse = builder.build();*/

    luxuryHouse.show();

    House simpleHouse = builder
        .buildWalls("wood")
        .buildRoof("tiles")
        .build();

    simpleHouse.show();
}

int main() {
    Builder_Pattern();

    Builder_Pattern2();
}
