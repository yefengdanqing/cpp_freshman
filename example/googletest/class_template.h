#pragma once

template <typename K, typename V, template<typename, typename> typename C>
class CommonFileDict : public FileDict {
public:
CommonFileDict(const std::string& file_name) {
    _dict_file = file_name;
}
private:

};