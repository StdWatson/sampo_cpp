#pragma once
// ��� ���� �� sampo/scheduler/schemas/identifiable - ���� ������� (��� �����)

#include <string>

class Identifiable {
public:
    std::string id;

    std::string get_id() {
        return id;
    }
};
