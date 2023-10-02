#include "pch.hpp"

#include <input/ccsgoinput.hpp>

#include <signatures/signatures.hpp>

CCSGOInput* CCSGOInput::Get() {
    static auto inst = signatures::GetCSGOInput.GetPtrAs<CCSGOInput*>();
    return inst;
}
