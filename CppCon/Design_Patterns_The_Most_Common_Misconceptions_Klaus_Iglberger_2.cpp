#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <fstream>

// --- DrawStrategy (template) ---
template <typename ConcreteShape>
class DrawStrategy {
public:
    virtual ~DrawStrategy() = default;
    virtual void draw(ConcreteShape const& shape) const = 0;
};

// --- Runtime polymorphic shape interface ---
class SHape {
public:
    virtual ~SHape() = default;
    virtual void draw() const = 0;
};

// --- Circle ---
class Circle : public SHape {
public:
    Circle(double r, std::unique_ptr<DrawStrategy<Circle>> ds)
        : radius{r}, drawer{std::move(ds)} {}

    double getRadius() const { return radius; }

    void draw() const override {
        if (drawer) drawer->draw(*this);
        else std::cout << "Circle: radius=" << radius << '\n';
    }

private:
    double radius;
    std::unique_ptr<DrawStrategy<Circle>> drawer;
};

// --- Square ---
class Square : public SHape {
public:
    Square(double s, std::unique_ptr<DrawStrategy<Square>> ds)
        : side{s}, drawer{std::move(ds)} {}

    double getSide() const { return side; }

    void draw() const override {
        if (drawer) drawer->draw(*this);
        else std::cout << "Square: side=" << side << '\n';
    }

private:
    double side;
    std::unique_ptr<DrawStrategy<Square>> drawer;
};

// alias for container of polymorphic shapes
using SHAPES = std::vector<std::unique_ptr<SHape>>;

// --- ShapesFactory interface ---
class ShapesFactory {
public:
    virtual ~ShapesFactory() = default;
    virtual SHAPES create(std::string_view filename) const = 0;
};

// helper to draw all shapes
void drawAllShapes(SHAPES const& shapes) {
    for (auto const& s : shapes) {
        s->draw();
    }
}

// --- Concrete Drawer implementing both strategies ---
class OpenGLDrawer : public DrawStrategy<Circle>, public DrawStrategy<Square> {
public:
    OpenGLDrawer() = default;

    void draw(Circle const& c) const override {
        std::cout << "[OpenGL] Circle radius = " << c.getRadius() << '\n';
    }

    void draw(Square const& s) const override {
        std::cout << "[OpenGL] Square side = " << s.getSide() << '\n';
    }
};

// --- Concrete factory that reads a simple file format ---
// File format (tokens): "circle <radius>" or "square <side>" per token
class YourShapesFactory : public ShapesFactory {
public:
    SHAPES create(std::string_view filename) const override {
        SHAPES shapes{};
        std::ifstream shape_file{std::string(filename)};
        if (!shape_file) {
            std::cerr << "Cannot open file: " << filename << '\n';
            return shapes;
        }

        std::string token;
        while (shape_file >> token) {
            if (token == "circle") {
                double radius;
                if (!(shape_file >> radius)) break;
                shapes.emplace_back(
                    std::make_unique<Circle>(radius, std::make_unique<OpenGLDrawer>())
                );
            }
            else if (token == "square") {
                double side;
                if (!(shape_file >> side)) break;
                shapes.emplace_back(
                    std::make_unique<Square>(side, std::make_unique<OpenGLDrawer>())
                );
            }
            else {
                // unknown token -> stop or skip; here we stop
                std::cerr << "Unknown token: " << token << '\n';
                break;
            }
        }
        return shapes;
    }
};

// --- Demo: create a small sample file and run the factory ---
int main() {
    // create a small sample file
    const char* fname = "shapes.txt";
    {
        std::ofstream out(fname);
        out << "circle 1.5\n";
        out << "square 2.0\n";
        out << "circle 3.14\n";
    }

    YourShapesFactory factory{};
    SHAPES shapes = factory.create(fname);
    drawAllShapes(shapes);

    return 0;
}
