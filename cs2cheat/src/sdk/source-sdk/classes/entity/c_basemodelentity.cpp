#include "c_basemodelentity.hpp"

Vector C_BaseModelEntity::GetEyePosition() {
    return m_pGameSceneNode()->m_vecOrigin() + this->m_vecViewOffset();
}
