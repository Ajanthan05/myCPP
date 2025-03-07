#include <iostream>
using namespace std;

class IButton {
public:
    virtual void press() = 0;
};

class MacButton : public IButton {
public:
    void press() {
        cout << "Mack Button Pressed\n";
    }
};

class WinButton : public IButton {
public:
    void press() {
        cout << "Win Button Pressed\n";
    }
};


class ITextBox {
public:
    virtual void Print() = 0;
};

class MacTextBox : public ITextBox {
public:
    void Print() {
        cout << "Mack TextBox Pressed\n";
    }
};

class WinTextBox : public ITextBox {
public:
    void Print() {
        cout << "Win TextBox Pressed\n";
    }
};

class IFactory {
public:
    virtual IButton *CreateButton() = 0; 
    virtual ITextBox *CreateTextBox() = 0;
};

class MacFactory : public IFactory {
public: 
    IButton *CreateButton() {
        return new MacButton();
    }
    ITextBox *CreateTextBox() {
        return new MacTextBox();
    }
};

class WinFactory : public IFactory {
public: 
    IButton *CreateButton() {
        return new WinButton();
    }
    ITextBox *CreateTextBox() {
        return new WinTextBox();
    }
};

class GUIAbstractFactory {
public:

    static IFactory *CreateFactory(string osType) {
        if(osType == "Win") {
            return new WinFactory();
        }
        else if (osType == "Mac") {
            return new MacFactory();
        }
        else return new WinFactory();
    }
};

int main() {
    cout << "Enter the machine OS: \n";
    string osType;
    cin >> osType;

    IFactory *fact = GUIAbstractFactory::CreateFactory(osType);

    IButton *button = fact->CreateButton();
    button->press();

    ITextBox *textBox = fact->CreateTextBox();
    textBox->Print();

    return 0;
}