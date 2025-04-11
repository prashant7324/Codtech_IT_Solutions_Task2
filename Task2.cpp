#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>
#include <algorithm>

const int CHUNK_SIZE = 16; // Small chunk size for demo

std::mutex io_mutex;

struct Block {
    std::string data;
    std::string result;
    int index;
};

// Fake "compression" by reversing string (for demo purposes)
void compressBlock(Block& block) {
    block.result = block.data;
    std::reverse(block.result.begin(), block.result.end());
}

// Fake "decompression" by reversing string back
void decompressBlock(Block& block) {
    block.result = block.data;
    std::reverse(block.result.begin(), block.result.end());
}

void process(const std::string& input, std::vector<Block>& blocks, bool compressMode) {
    int index = 0;
    for (size_t i = 0; i < input.size(); i += CHUNK_SIZE) {
        Block block;
        block.data = input.substr(i, CHUNK_SIZE);
        block.index = index++;
        blocks.push_back(block);
    }

    std::vector<std::thread> threads;
    for (auto& block : blocks) {
        if (compressMode)
            threads.emplace_back(compressBlock, std::ref(block));
        else
            threads.emplace_back(decompressBlock, std::ref(block));
    }

    for (auto& t : threads) t.join();
}

int main() {
    std::string mode;
    std::cout << "Enter mode (compress/decompress): ";
    std::cin >> mode;
    std::cin.ignore(); // Clear newline from input buffer

    std::string input;
    std::cout << "Enter input text (single line):\n";
    std::getline(std::cin, input);

    std::vector<Block> blocks;
    process(input, blocks, mode == "compress");

    std::sort(blocks.begin(), blocks.end(), [](const Block& a, const Block& b) {
        return a.index < b.index;
    });

    std::cout << "\nOutput:\n";
    for (const auto& block : blocks) {
        std::cout << block.result;
    }
    std::cout << "\n";

    return 0;
}
