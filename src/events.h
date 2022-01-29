#ifndef SRC_EVENTS_H_INCLUDED
#define SRC_EVENTS_H_INCLUDED
#include <map>
#include <memory>
#include <functional>


template <typename T> class Subscription;
template <typename T>
class Event {
private:
    friend class Subscription<T>;
    struct Shared {
        int subscriberCount = 0;
        std::map<int, std::function<void(T)>> subscribers;
    };
    std::shared_ptr<Shared> shared = std::shared_ptr<Shared>(new Shared());
public:
    inline Subscription<T> subscribe(std::function<void(T)> subscriber) {
        int id = shared->subscriberCount++;
        shared->subscribers.insert({id, subscriber});
        return Subscription<T>(id, shared); 
    }
    inline void emit(T t) {
        for (auto &p : shared->subscribers) {
            p.second(t);
        }
    }
};

template <typename T>
class Subscription {
    int id;
    std::shared_ptr<typename Event<T>::Shared> shared;
public:
    inline Subscription(int id, std::shared_ptr<typename Event<T>::Shared> shared) : id(id), shared(shared) {}
    inline void unsubscribe() {
        shared->subscribers.erase(id);
    }
    inline ~Subscription() {
        unsubscribe();
    }
};

#endif
