#include <cmath>
#include <iostream>
#include <string>

#include "TutorialConfig.h"
#include "MathFunctions.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        // report version
        std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
                << Tutorial_VERSION_MINOR << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }

    // convert input to double
    const double inputValue = std::stod(argv[1]);

    // calculate square root
    const double outputValue = mathfunctions::sqrt(inputValue);
    std::cout << "The square root of " << inputValue
              << " is " << outputValue
              << std::endl;
    return 0;
}