Great question! 🔥 Let's break down **when you actually *need* dynamic allocation** (i.e., `new`, `make_unique`, etc.) instead of just using stack allocation.

---

## 🧠 Stack vs Heap Recap

| Feature          | **Stack Allocation**       | **Dynamic Allocation (Heap)**     |
|------------------|----------------------------|-----------------------------------|
| Lifetime         | Auto (ends with scope)     | Manual / Controlled               |
| Performance      | Super fast (just a pointer shift) | Slower (heap management)     |
| Size limit       | Limited (e.g. 1MB or so)    | Much bigger (heap size)           |
| Syntax           | `int x = 5;`               | `int* p = new int(5);`            |

---

## ✅ Use the Stack **(default choice)** when:

- The object size is small or moderate.
- Lifetime matches the current scope (function, block).
- You want speed and simplicity.

```cpp
void foo() {
    int local = 42; // Simple, fast, automatic
}
```

---

## ✅ Use the Heap **(dynamic allocation)** when:

### 1. **Large Objects**

The stack is small. If you allocate huge arrays or objects there, it can cause a **stack overflow**.

```cpp
// BAD: might crash with large arrays
// int bigArray[1000000]; 

// GOOD:
auto bigArray = std::make_unique<int[]>(1000000);
```

---

### 2. **Variable Lifetime / Ownership**

If the object needs to **outlive the current scope**:

```cpp
std::unique_ptr<MyClass> ptr;

void create() {
    ptr = std::make_unique<MyClass>(); // lives after this function ends
}
```

---

### 3. **Polymorphism / Abstract Classes**

You can’t store abstract classes or sliced polymorphic objects on the stack easily — use pointers.

```cpp
class Base { virtual void foo() = 0; };
class Derived : public Base { void foo() override {} };

std::unique_ptr<Base> obj = std::make_unique<Derived>();
```

---

### 4. **Resource Handles / RAII**

Need to wrap a raw pointer (file, socket, buffer) with cleanup logic? Dynamic allocation + smart pointers = clean and safe.

```cpp
std::unique_ptr<FILE, decltype(&fclose)> file(fopen("data.txt", "r"), fclose);
```

---

### 5. **Data structures needing dynamic memory internally**

Like:
- `std::vector`
- `std::map`
- `std::string`

Even though you just write:

```cpp
std::vector<int> v = {1, 2, 3};
```

Internally, this uses dynamic memory. So anytime:
- You don’t know the size at compile time
- It can grow/shrink dynamically

➡️ You’re relying on heap allocation under the hood.

---

### 6. **Multithreading / Shared Ownership**

When multiple parts of your program need **shared access** to the same resource:

```cpp
std::shared_ptr<Config> config = std::make_shared<Config>();
```

---

## 🚫 Avoid heap when:

- Object is small, short-lived, or non-polymorphic.
- Simpler code is preferred (stack is easier and faster).

---

## 🧠 Summary

| Use Stack When…                  | Use Heap (Dynamic Allocation) When…           |
|----------------------------------|-----------------------------------------------|
| Small, fixed-size objects        | Large or variable-sized objects               |
| Lifetime ≤ current scope         | Lifetime needs to extend beyond scope         |
| No polymorphism needed           | Polymorphic behavior is needed                |
| Code simplicity is preferred     | Flexible lifetime, ownership, or sharing      |

---

Want examples comparing stack vs heap versions of the same code, like building a tree or graph?