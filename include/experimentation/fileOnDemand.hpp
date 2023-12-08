#include <iostream>
#include <filesystem>
#include <optional>

template <typename T>
class fileOnDemand
{
protected:
    virtual ~fileOnDemand();

    std::filesystem::path _path;
    std::optional<T> _data;
    
public:
    fileOnDemand(const fileOnDemand&) = delete;
    fileOnDemand& operator=(const fileOnDemand&) = delete;

    explicit fileOnDemand(fileOnDemand&&);
    explicit fileOnDemand(const std::filesystem::path&);
    explicit fileOnDemand(const std::filesystem::path&&);

    virtual void unload() = 0;
    virtual void load() = 0;

    [[nodiscard]] virtual constexpr bool isLoaded() const noexcept final;
    [[nodiscard]] virtual constexpr T& data() final;

    [[nodiscard]] virtual constexpr std::filesystem::path& path() final;
};

template <typename T>
fileOnDemand<T>::fileOnDemand(const std::filesystem::path& path) : _path(path)
{
}

template <typename T>
fileOnDemand<T>::fileOnDemand(const std::filesystem::path&& path) : _path(std::move(path))
{
}

template <typename T>
fileOnDemand<T>::fileOnDemand(fileOnDemand&& other) : _path(std::move(other._path)), _data(std::move(other._data))
{
}

template <typename T>
fileOnDemand<T>::~fileOnDemand()
{
    std::cout << "destruct fileOnDemand class of: " << _path << std::endl;
}

template <typename T>
constexpr T& fileOnDemand<T>::data() 
{
    if (!_data.has_value()) {
        std::cout << "File: " << _path << " is not loaded, loading..." << std::endl;
        const_cast<fileOnDemand<T>*>(this)->load();
    }
    return _data.value();
}

template <typename T>
constexpr bool fileOnDemand<T>::isLoaded() const noexcept
{
    return _data.has_value();
}

template <typename T>
constexpr std::filesystem::path& fileOnDemand<T>::path()
{
    return _path;
}
