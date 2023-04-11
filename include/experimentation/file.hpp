#include <iostream>
#include <filesystem>

template <typename T>
class file
{
protected:
    virtual ~file();

    const std::filesystem::path _path;
    T _data;

    bool _is_loaded = false;
    
public:
    file(const file&) = delete;
    file(file&&) = delete;

    file();
    explicit file(const std::filesystem::path& path);

    virtual void unload() = 0;
    virtual void load() = 0;

    [[nodiscard]] virtual bool is_loaded() const final { return _is_loaded; }
    [[nodiscard]] virtual const T& get_data() const final;

    [[nodiscard]] virtual const std::filesystem::path& get_path() const final { return _path; }
};

template <typename T>
file<T>::file(const std::filesystem::path& path) : _path(path)
{
}

template <typename T>
file<T>::~file()
{
    std::cout << "destruct file class of: " << _path << std::endl;
}

template <typename T>
const T& file<T>::get_data() const
{
    if (!_is_loaded) {
        std::cout << "File: " << _path << " is not loaded, loading..." << std::endl;
        const_cast<file<T>*>(this)->load();
    }
    return _data;
}