#include "LeeThinning.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

Volume3D makeShape(const std::string& shape, int side, int thickness) {
    if (shape == "tube") return makeTube(side, thickness);
    if (shape == "branch") return makeBranch(side, thickness);
    if (shape == "loop") return makeLoop(side, thickness);
    if (shape == "vascular_tree") return makeVascularTree(side, thickness);
    if (shape == "cube") return makeSolidCube(side, std::max(3, side / 2));
    throw std::invalid_argument(
        "Unknown shape. Use tube, branch, loop, vascular_tree, or cube."
    );
}

ScanMode parseMode(const std::string& value) {
    if (value == "full") return ScanMode::FullVolume;
    if (value == "active") return ScanMode::ActiveSurface;
    throw std::invalid_argument("Mode must be full or active.");
}

void printSummary(const std::string& shape,
                  const Volume3D& input,
                  const RunResult& r) {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "\n=== Lee 3-D Thinning Results ===\n";
    std::cout << "Shape: " << shape << '\n';
    std::cout << "Size: "
              << input.depth() << "x" << input.height() << "x" << input.width() << '\n';
    std::cout << "Foreground: " << input.foregroundCount() << '\n';
    std::cout << "Iterations: " << r.iterations.size() << '\n';
    std::cout << "Runtime: " << r.runtime_ms << " ms\n";
    std::cout << "Components: "
              << r.before.components26 << " -> " << r.after.components26 << '\n';
    std::cout << "Euler: "
              << r.before.euler << " -> " << r.after.euler << '\n';
    std::cout << "Endpoints: " << r.after.endpoints << '\n';
}

void runSingle(const std::string& shape,
               int side,
               int thickness,
               ScanMode mode) {
    Volume3D input = makeShape(shape, side, thickness);
    LeeThinning algorithm(mode);
    RunResult r = algorithm.run(input, 1000, false);

    printSummary(shape, input, r);

    const int z = side / 2;
    LeeThinning::saveSlicePGM(input, z, "original_slice.pgm");
    LeeThinning::saveSlicePGM(r.skeleton, z, "skeleton_slice.pgm");

    std::ofstream csv("single_result.csv");
    writeCSVHeader(csv);
    writeCSVRow(csv, shape, input, r, r.runtime_ms);

    std::cout << "\nSaved: original_slice.pgm, skeleton_slice.pgm, single_result.csv\n";
}

void runExperiment(const std::string& csvPath,
                   int repetitions,
                   ScanMode mode) {
    const std::vector<int> sizes = {21, 25, 29, 33, 37};
    const std::vector<std::string> shapes = {
        "tube",
        "branch",
        "loop",
        "vascular_tree"
    };

    std::ofstream out(csvPath);
    if (!out) throw std::runtime_error("Cannot create CSV file.");
    writeCSVHeader(out);

    for (const std::string& shape : shapes) {
        for (int side : sizes) {
            int thickness = std::max(3, side / 8);
            if (thickness % 2 == 0) ++thickness;

            double runtime_sum = 0.0;
            RunResult representative;
            Volume3D input;

            for (int rep = 1; rep <= repetitions; ++rep) {
                input = makeShape(shape, side, thickness);
                LeeThinning algorithm(mode);
                RunResult current = algorithm.run(input, 1000, false);

                runtime_sum += current.runtime_ms;
                if (rep == 1) representative = std::move(current);
            }

            const double average_runtime = runtime_sum / repetitions;
            writeCSVRow(out, shape, input, representative, average_runtime);

            std::cout << shape
                      << " | " << side << "x" << side << "x" << side
                      << " | foreground=" << input.foregroundCount()
                      << " | iterations=" << representative.iterations.size()
                      << " | runtime(avg)=" << std::fixed << std::setprecision(3)
                      << average_runtime << " ms"
                      << " | components=" << representative.before.components26
                      << "->" << representative.after.components26
                      << " | euler=" << representative.before.euler
                      << "->" << representative.after.euler
                      << " | endpoints=" << representative.after.endpoints
                      << '\n';
        }
    }

    std::cout << "\nExperiment complete. Results saved to " << csvPath << '\n';
}

void usage(const char* exe) {
    std::cout << "Usage:\n"
              << "  " << exe
              << " --single <tube|branch|loop|vascular_tree|cube> <side> <thickness> <full|active>\n"
              << "  " << exe
              << " --experiment <results.csv> [repetitions] [full|active]\n\n"
              << "Examples:\n"
              << "  " << exe << " --single loop 33 5 full\n"
              << "  " << exe << " --single vascular_tree 33 5 active\n"
              << "  " << exe << " --experiment results.csv 5 full\n";
}

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            usage(argv[0]);
            return 0;
        }

        const std::string command = argv[1];

        if (command == "--single") {
            if (argc != 6) {
                usage(argv[0]);
                return 1;
            }

            const std::string shape = argv[2];
            const int side = std::stoi(argv[3]);
            const int thickness = std::stoi(argv[4]);
            const ScanMode mode = parseMode(argv[5]);

            runSingle(shape, side, thickness, mode);
            return 0;
        }

        if (command == "--experiment") {
            if (argc < 3 || argc > 5) {
                usage(argv[0]);
                return 1;
            }

            const std::string path = argv[2];
            const int repetitions = argc >= 4 ? std::stoi(argv[3]) : 5;
            const ScanMode mode = argc == 5 ? parseMode(argv[4]) : ScanMode::FullVolume;

            if (repetitions <= 0) {
                throw std::invalid_argument("Repetitions must be greater than zero.");
            }

            runExperiment(path, repetitions, mode);
            return 0;
        }

        usage(argv[0]);
        return 1;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
