#include "BonesWriter.h"

namespace 
{
template<class T>
void Write(std::ostream& stream, const T& data)
{
    stream.write(reinterpret_cast<const char*>(&data), sizeof(T));
}

template<class T>
void Write(std::ostream& stream, const T* data, size_t size)
{
    stream.write(reinterpret_cast<const char*>(data), size);
}
} // anonymous namespace

namespace nc::convert
{
void Write(std::ostream& stream, const nc::asset::BodySpaceNode* parentNode)
{
    if (!parentNode)
    {
        return;
    }
    ::Write(stream, parentNode->boneName);
    Write(stream, parentNode->localSpace);
    ::Write(stream, parentNode->children.size());

    for (auto& child : parentNode->children)
    {
        Write(stream, &child);
    }
}

void Write(std::ostream& stream, const std::unordered_map<std::string, uint32_t>& boneNamesToIds)
{
    for (const auto& pair: boneNamesToIds)
    {
        ::Write(stream, pair.first.size());
        ::Write(stream, pair.first.data(), pair.first.size() * sizeof(char));
        ::Write(stream, pair.second);
    }
}

void Write(std::ostream& stream, const DirectX::XMMATRIX& matrix)
{
    DirectX::XMFLOAT4X4 view;
    XMStoreFloat4x4(&view, matrix);

    float a1 = view._11;
    float a2 = view._12;
    float a3 = view._13;
    float a4 = view._14;

    float b1 = view._21;
    float b2 = view._22;
    float b3 = view._23;
    float b4 = view._24;

    float c1 = view._31;
    float c2 = view._32;
    float c3 = view._33;
    float c4 = view._34;

    float d1 = view._41;
    float d2 = view._42;
    float d3 = view._43;
    float d4 = view._44;

    ::Write(stream,  a1); ::Write(stream,  a2); ::Write(stream,  a3); ::Write(stream,  a4);
    ::Write(stream,  b1); ::Write(stream,  b2); ::Write(stream,  b3); ::Write(stream,  b4);
    ::Write(stream,  c1); ::Write(stream,  c2); ::Write(stream,  c3); ::Write(stream,  c4);
    ::Write(stream,  d1); ::Write(stream,  d2); ::Write(stream,  d3); ::Write(stream,  d4);
}
}
