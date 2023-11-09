
#include <iostream>
#include <functional>

using func = std::function<void(void)>;
using namespace std::placeholders;

template<auto Func>
std::pair<std::string, func> func_rename(){
    // Thanks to Alipha (again) from C++ Basics for this.
    std::string func_name = __PRETTY_FUNCTION__;
    std::string name = func_name.substr(func_name.find('=') + 2 );
    name.pop_back();

    auto testing{Func};
    // auto testing{func(std::bind(Func))};
    return std::make_pair(name, testing);
}