#pragma once
// это файл из sampo/scheduler/schemas/identifiable - файл Стасяна (нуу почти)

#include <string>

class Identifiable {
public:
    std::string id;

    std::string get_id() {
        return id;
    }
};
