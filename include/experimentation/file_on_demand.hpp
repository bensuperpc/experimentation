#include <iostream>
#include <filesystem>
#include <optional>

template <typename T>
class file_on_demand
{
protected:
    virtual ~file_on_demand();

    std::filesystem::path _path;
    std::optional<T> _data;
    
public:
    file_on_demand(const file_on_demand&) = delete;
    file_on_demand& operator=(const file_on_demand&) = delete;

    explicit file_on_demand(file_on_demand&&);
    explicit file_on_demand(const std::filesystem::path&);
    explicit file_on_demand(const std::filesystem::path&&);

    virtual void unload() = 0;
    virtual void load() = 0;

    [[nodiscard]] virtual constexpr bool is_loaded() const noexcept final;
    [[nodiscard]] virtual constexpr T& data() final;

    [[nodiscard]] virtual constexpr std::filesystem::path& path() final;
};

template <typename T>
file_on_demand<T>::file_on_demand(const std::filesystem::path& path) : _path(path)
{
}

template <typename T>
file_on_demand<T>::file_on_demand(const std::filesystem::path&& path) : _path(std::move(path))
{
}

template <typename T>
file_on_demand<T>::file_on_demand(file_on_demand&& other) : _path(std::move(other._path)), _data(std::move(other._data))
{
}

template <typename T>
file_on_demand<T>::~file_on_demand()
{
    std::cout << "destruct file_on_demand class of: " << _path << std::endl;
}

template <typename T>
constexpr T& file_on_demand<T>::data() 
{
    if (!_data.has_value()) {
        std::cout << "File: " << _path << " is not loaded, loading..." << std::endl;
        const_cast<file_on_demand<T>*>(this)->load();
    }
    return _data.value();
}

template <typename T>
constexpr bool file_on_demand<T>::is_loaded() const noexcept
{
    return _data.has_value();
}

template <typename T>
constexpr std::filesystem::path& file_on_demand<T>::path()
{
    return _path;
}
