#include <variant>
#include <iostream>
#include <string>

struct Disconnected {};
struct Connecting { int retries = 0; };
struct Connected { std::string ip; };
struct Error { std::string message; };


using TCPState = std::variant<Disconnected, Connecting, Connected, Error>;

void handle_state(TCPState& state) {
    std::visit([&](auto&& s) {
        using T = std::decay_t<decltype(s)>;
        if constexpr (std::is_same_v<T, Disconnected>) {
            std::cout << "State: Disconnected. Trying to connect...\n";
            state = Connecting{};
        } else if constexpr (std::is_same_v<T, Connecting>) {
            std::cout << "State: Connecting (retries = " << s.retries << ")\n";
            if (s.retries > 2)
                state = Error{"Failed to connect after 3 attempts"};
            else
                state = Connecting{s.retries + 1}; // retry
        } else if constexpr (std::is_same_v<T, Connected>) {
            std::cout << "State: Connected to " << s.ip << "\n";
        } else if constexpr (std::is_same_v<T, Error>) {
            std::cout << "Error: " << s.message << "\n";
        }
    }, state);
}

int main() {
    TCPState state = Disconnected{};

    for (int i = 0; i < 5; ++i) {
        handle_state(state);
        if (std::holds_alternative<Connecting>(state) && std::get<Connecting>(state).retries == 2) {
            state = Connected{"192.168.1.1"};
        }
    }
}
