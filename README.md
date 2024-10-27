# AdBidder: Auction-Based Advertising Platform

AdBidder is an auction-based advertising platform that enables advertisers to bid for ad slots in real-time using a variety of bidding strategies. The platform is designed for high concurrency and scalability, using a lock-free ring buffer for click history, and supports a Generalized Second Price (GSP) auction mechanism to determine the winner based on bid amount and quality score.

## Features
- **Customizable Bidding Strategies**: Advertisers can choose from various strategies (truthful, aggressive, conservative).
- **Ad Slots**: Configurable with reserve prices, base click-through rates (CTR), and adjustable quality scores.
- **Auction Engine**: Performs real-time auctions, CTR predictions, and budget tracking.
- **Generalized Second Price Auction**: Utilizes quality scoring and click-based learning updates.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Project Structure](#project-structure)
3. [Installation](#installation)
4. [Usage](#usage)
5. [Testing](#testing)
6. [Contribution Guidelines](#contribution-guidelines)
7. [Future Improvements](#future-improvements)
8. [License](#license)
9. [Acknowledgments](#acknowledgments)

## Prerequisites
- **CMake**: Version 3.10 or higher for building the project.
- **Google Test**: For running tests (automatically installed if using the CMake FetchContent method).
- **C++17 Compatible Compiler**: Such as GCC, Clang, or MSVC.

## Project Structure
```
project-root/
├── src/
│   ├── advertiser.hpp          # Advertiser class definition
│   ├── adslot.hpp              # AdSlot class definition
│   ├── auction_engine.hpp      # AuctionEngine class definition
│   ├── lock_free_ring_buffer.hpp # Lock-free ring buffer definition
│   └── main.cpp                # Optional: main application entry point
├── tests/
│   ├── advertiser_test.cpp     # Unit tests for Advertiser class
│   ├── adslot_test.cpp         # Unit tests for AdSlot class
│   ├── auction_engine_test.cpp # Unit tests for AuctionEngine class
│   └── test_main.cpp           # Entry point for running all tests
├── CMakeLists.txt              # Build script for CMake
└── README.md                   # Project documentation
```

## Installation
### Step 1: Clone the Repository
```bash
git clone https://github.com/yourusername/AdBidder.git
cd AdBidder
```

### Step 2: Build the Project
Create a build directory:
```bash
mkdir build
cd build
```
Run CMake to configure the build:
```bash
cmake ..
```
Compile the project:
```bash
cmake --build .
```

### Step 3: Install Google Test (if not using FetchContent)
If you’re not using the FetchContent method for Google Test, ensure it is installed and properly linked in the CMakeLists.txt file.

## Usage
To run a basic auction example, modify or use the provided `main.cpp` in the `src` directory.

### Example Code
```cpp
#include "advertiser.hpp"
#include "adslot.hpp"
#include "auction_engine.hpp"

int main() {
    AuctionEngine engine(0.01); // Set learning rate

    // Create Advertisers
    auto advertiser1 = std::make_shared<Advertiser>("adv1", 100.0, {{"top", 10.0}}, BiddingStrategy::TRUTHFUL);
    auto advertiser2 = std::make_shared<Advertiser>("adv2", 100.0, {{"top", 15.0}}, BiddingStrategy::AGGRESSIVE);

    // Create Ad Slot
    auto slot = std::make_shared<AdSlot>("top", 5.0, 0.2);

    // Add advertisers and slots to the engine
    engine.add_advertiser(advertiser1);
    engine.add_advertiser(advertiser2);
    engine.add_slot(slot);

    // Run auction round
    auto results = engine.run_auction_round();

    // Output results
    for (const auto& result : results) {
        std::cout << "Winner: " << result.winner_id << ", Clearing Price: " << result.clearing_price << "\n";
    }
    return 0;
}
```

### Compile and Run
After setting up `main.cpp`, compile and run:
```bash
cmake --build .
./main_exec
```

## Testing
Testing is done with Google Test. The project includes test files in the `tests/` directory to cover all major components.

### Running Tests
After building the project, you can run tests with:
```bash
./run_tests
```

### Test Coverage
- **Advertiser Tests** (`advertiser_test.cpp`): Budget deduction, bid calculation, and affordability tests.
- **AdSlot Tests** (`adslot_test.cpp`): Quality score management and ad slot attribute tests.
- **AuctionEngine Tests** (`auction_engine_test.cpp`): Auction rounds, GSP auction mechanics, and CTR prediction tests.

## Contribution Guidelines
We welcome contributions! Please follow these guidelines:
1. Fork the repository and clone it locally.
2. Create a new branch for your feature or bug fix.
3. Write clear and concise commits. Follow best practices for commit messages.
4. Ensure tests pass before submitting a pull request.
5. Document your code thoroughly if adding new features or modifying existing ones.

## Future Improvements
- **Additional Bidding Strategies**: Introduce strategies such as random bidding, highest-bid, or learning-based approaches.
- **Advanced CTR Prediction Models**: Experiment with machine learning-based CTR predictions.
- **Logging and Monitoring**: Implement detailed logging for tracking auction outcomes and analyzing advertiser performance.
- **Web Interface**: Develop a web interface with API endpoints for advertiser interaction.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Acknowledgments
This project uses:
- [Google Test](https://github.com/google/googletest) for unit testing.
- [CMake](https://cmake.org/) for build configuration and dependency management.

Happy coding!
