#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <functional>
#include <utility>

namespace Utils {
    template <typename I>
    class Initializer : public I
    {
    public:
        template <typename ...Args>
        explicit Initializer(Args&&...args) : I(std::forward<Args>(args)...)
        {
            I::init_func();
        }
    };
    template <typename T>
    using Init = Initializer<T>;
}


#endif // INITIALIZER_H
