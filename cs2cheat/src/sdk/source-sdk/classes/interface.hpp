#pragma once

typedef void *(*InstantiateInterfaceFn)();

// Used internally to register classes.
struct InterfaceReg {
    InstantiateInterfaceFn m_CreateFn;
    const char *m_pName;
    InterfaceReg *m_pNext;  // For the global list.
};
