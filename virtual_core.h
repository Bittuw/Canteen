#ifndef VIRTUAL_CORE_H
#define VIRTUAL_CORE_H

#ifndef _WIN32
#error "Need only Windows platform"
#endif

namespace Core {
    class VCore {
    public:
        virtual void start() = 0 {}
        virtual void stop() = 0 {}
        virtual ~VCore() {}
    };
}

#endif // VIRTUAL_CORE_H
