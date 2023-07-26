#pragma once

#include "ncasset/Assets.h"

#include <ostream>

namespace nc::convert
{
void SerializeBodySpaceOffsetTree(std::ostream& stream, BodySpaceNode* rootNode);
}
