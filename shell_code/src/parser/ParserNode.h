#include <vector>
#include <string>
#include <memory>
#include "./ParserNodeType.h"

class ParserNode
{
public:
    ParserNode(std::vector<std::string> args) : m_ParserNodeType(ParserNodeType::Command), m_Args(args) {}
    ParserNode(ParserNodeType type, std::unique_ptr<ParserNode> left, std::unique_ptr<ParserNode> right): m_ParserNodeType(type), m_LeftNode(std::move(left)), m_RightNode(std::move(right)) {}
    

    ParserNodeType m_ParserNodeType;
    std::vector<std::string> m_Args;
    std::unique_ptr<ParserNode> m_LeftNode = nullptr;
    std::unique_ptr<ParserNode> m_RightNode = nullptr;
};