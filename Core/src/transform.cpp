#include "transform.hpp"

#include "serialization/serializer.hpp"

void XnorCore::Transform::Serialize() const
{
    float t1 = 0.4f;
    double t2 = 45.0525;
    std::string caca = "caca";
    Reflectable::Serialize();
    Serializer::BeginXmlElement("Transform","Transform");

    Serializer::FetchAttribute("t1",t1);
    Serializer::FetchAttribute("t2",t2);
    Serializer::FetchAttribute("caca",caca);
    Serializer::EndXmlElement();

}

void XnorCore::Transform::Deserialize()
{
    Reflectable::Deserialize();
}
