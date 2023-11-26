#include <iostream>
#include <string>
#include <random>
#include <algorithm>

std::string generate_password(const std::string::size_type length) {
    const std::string upper_alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lower_alpha = "abcdefghijklmnopqrstuvwxyz";
    const std::string digits = "0123456789";
    const std::string special = "!@#$%^&*()_+";

    std::string alphanum = upper_alpha + lower_alpha + digits + special;

    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_int_distribution<std::string::size_type> distribution(0, alphanum.size() - 1);

    std::string password = "";
    /*
        for (std::string::size_type i = 0; i < length; i++) {
        password += alphanum[distribution(rng)];
    }
    */
    std::generate_n(password.begin(), length, [&]() { return alphanum[distribution(rng)]; });

    return password;
}

int main() {
    std::string password = generate_password(32);
    std::cout << "Activation Key: " << password << std::endl;

    return 0;
}