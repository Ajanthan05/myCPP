#pragma once

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <type_traits>
#include <chrono>
#include <utility>

template <typename T, typename M = std::mutex>
class guarded {
private:
    // --- Traits to detect shared/timed interface --- //
    template <typename U, typename = void>
    struct has_lock_shared : std::false_type {};

    template <typename U>
    struct has_lock_shared<U, std::void_t<decltype(std::declval<U&>().lock_shared())>> : std::true_type {};

    template <typename U, typename = void>
    struct has_try_lock_shared : std::false_type {};

    template <typename U>
    struct has_try_lock_shared<U, std::void_t<decltype(std::declval<U&>().try_lock_shared())>> : std::true_type {};

    template <typename U, typename = void>
    struct has_try_lock_for : std::false_type {};

    template <typename U>
    struct has_try_lock_for<U, std::void_t<decltype(std::declval<U&>().try_lock_for(std::declval<std::chrono::steady_clock::duration>()))>> : std::true_type {};

    template <typename U, typename = void>
    struct has_try_lock_until : std::false_type {};

    template <typename U>
    struct has_try_lock_until<U, std::void_t<decltype(std::declval<U&>().try_lock_until(std::declval<std::chrono::steady_clock::time_point>()))>> : std::true_type {};

public:
    // deleter for exclusive lock: holds a unique_lock<M>
    struct ExclusiveDeleter {
        using mutex_type = M;
        std::unique_lock<mutex_type> lock;

        ExclusiveDeleter() noexcept = default;
        explicit ExclusiveDeleter(std::unique_lock<mutex_type>&& l) noexcept : lock(std::move(l)) {}

        void operator()(T* /*ptr*/) noexcept {
            // don't delete the pointed-to object; this wrapper only controls the lock lifetime
            // unique_lock's destructor will release the mutex
        }
    };

    // deleter for shared lock: holds a shared_lock<M>
    template <typename Mutex = M>
    struct SharedDeleter {
        // only instantiated when shared locking exists; but shared_lock type requires C++17
        std::shared_lock<Mutex> lock;

        SharedDeleter() noexcept = default;
        explicit SharedDeleter(std::shared_lock<Mutex>&& l) noexcept : lock(std::move(l)) {}

        void operator()(const T* /*ptr*/) noexcept {
            // no deletion; shared_lock destructor releases shared lock
        }
    };

    using handle = std::unique_ptr<T, ExclusiveDeleter>;
    using const_handle = std::unique_ptr<const T, ExclusiveDeleter>; // for const T access (if you want)
    using shared_handle = std::conditional_t<has_lock_shared<M>::value,
                                             std::unique_ptr<const T, SharedDeleter<M>>,
                                             void>;

    // --- Construction --- //
    template <typename... Us>
    explicit guarded(Us&&... us)
        : m_obj(std::forward<Us>(us)...)
    {}

    // Delete copy/move to avoid surprising semantics
    guarded(const guarded&) = delete;
    guarded& operator=(const guarded&) = delete;
    guarded(guarded&&) = delete;
    guarded& operator=(guarded&&) = delete;

    ~guarded() = default;

    // --- Exclusive locks --- //
    handle lock() {
        std::unique_lock<M> lk(m_mutex);
        return handle(&m_obj, ExclusiveDeleter(std::move(lk)));
    }

    handle try_lock() {
        std::unique_lock<M> lk(m_mutex, std::try_to_lock);
        if (lk.owns_lock()) return handle(&m_obj, ExclusiveDeleter(std::move(lk)));
        return handle(nullptr, ExclusiveDeleter(std::move(lk)));
    }

    // timed exclusive: only enabled when the mutex supports try_lock_for/try_lock_until
    template <class Rep, class Period,
              typename = std::enable_if_t<has_try_lock_for<M>::value>>
    handle try_lock_for(const std::chrono::duration<Rep, Period>& rel_time) {
        std::unique_lock<M> lk(m_mutex, std::defer_lock);
        if (lk.try_lock_for(rel_time)) return handle(&m_obj, ExclusiveDeleter(std::move(lk)));
        return handle(nullptr, ExclusiveDeleter(std::move(lk)));
    }

    template <class Clock, class Duration,
              typename = std::enable_if_t<has_try_lock_until<M>::value>>
    handle try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
        std::unique_lock<M> lk(m_mutex, std::defer_lock);
        if (lk.try_lock_until(abs_time)) return handle(&m_obj, ExclusiveDeleter(std::move(lk)));
        return handle(nullptr, ExclusiveDeleter(std::move(lk)));
    }

    // --- Shared locks: only available if M supports shared locking --- //
    template <typename MM = M>
    std::enable_if_t<has_lock_shared<MM>::value, shared_handle>
    lock_shared() {
        std::shared_lock<MM> lk(m_mutex);
        return shared_handle(&m_obj, SharedDeleter<MM>(std::move(lk)));
    }

    template <typename MM = M>
    std::enable_if_t<has_try_lock_shared<MM>::value, shared_handle>
    try_lock_shared() {
        std::shared_lock<MM> lk(m_mutex, std::try_to_lock);
        if (lk.owns_lock()) return shared_handle(&m_obj, SharedDeleter<MM>(std::move(lk)));
        return shared_handle(nullptr, SharedDeleter<MM>(std::move(lk)));
    }

    template <typename MM = M, class Rep, class Period>
    std::enable_if_t<has_try_lock_for<MM>::value && has_try_lock_shared<MM>::value, shared_handle>
    try_lock_shared_for(const std::chrono::duration<Rep, Period>& rel_time) {
        std::shared_lock<MM> lk(m_mutex, std::defer_lock);
        if (lk.try_lock_for(rel_time)) return shared_handle(&m_obj, SharedDeleter<MM>(std::move(lk)));
        return shared_handle(nullptr, SharedDeleter<MM>(std::move(lk)));
    }

    template <typename MM = M, class Clock, class Duration>
    std::enable_if_t<has_try_lock_until<MM>::value && has_try_lock_shared<MM>::value, shared_handle>
    try_lock_shared_until(const std::chrono::time_point<Clock, Duration>& abs_time) {
        std::shared_lock<MM> lk(m_mutex, std::defer_lock);
        if (lk.try_lock_until(abs_time)) return shared_handle(&m_obj, SharedDeleter<MM>(std::move(lk)));
        return shared_handle(nullptr, SharedDeleter<MM>(std::move(lk)));
    }

    // convenience: access underlying object while holding no lock (unsafe, for advanced usage)
    T& unsafe_get() & noexcept { return m_obj; }
    const T& unsafe_get() const & noexcept { return m_obj; }

private:
    T m_obj;
    M m_mutex;
};




