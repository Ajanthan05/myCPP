#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

class Token {
public:
    static Token create() {
        return Token();
        /*is exactly the same (internally) as:
        Token t;    (because Token::create() just gives you a fresh Token.)*/
    }
    /*static Token create() {
        Token t;
        t.setup();  // some complex setup
        return t;
    }*/
    // Other members of Token if needed
};

class TokenPool {
    std::mutex mtx;
    std::vector<Token> tokens_;

public:
    Token getToken() {
        std::lock_guard<std::mutex> lock(mtx);
        if (tokens_.empty()) {
            tokens_.push_back(Token::create());
        }
        Token t = std::move(tokens_.back()); // Takes the last token, moves it into t (no copy! efficient).
        tokens_.pop_back();
        return t;
    }

    void returnToken(Token&& token) {
        std::lock_guard<std::mutex> lock(mtx);
        tokens_.push_back(std::move(token));
    }
};



// Assuming Token and TokenPool are defined as above

void worker(TokenPool& pool, int id) {
    Token t = pool.getToken();
    std::cout << "Thread " << id << " got a token." << std::endl;
    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    pool.returnToken(std::move(t));
    std::cout << "Thread " << id << " returned a token." << std::endl;
}

int main() {
    TokenPool pool;
    std::vector<std::thread> threads;

    // Launch 5 worker threads
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker, std::ref(pool), i);
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads finished." << std::endl;
    return 0;
}
