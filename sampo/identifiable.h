#pragma once
// ��� ���� �� sampo/scheduler/schemas/identifiable - ���� ������� (��� �����)

#include <string>

class Identifiable {
public:
    std::string id;
    std::string name;
public:
    explicit Identifiable(std::string id, std::string name) : id(id), name(name) {}
    std::string get_id() {
        return id;
    }
};
