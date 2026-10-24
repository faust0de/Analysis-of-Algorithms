#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <ostream>

struct Pos {
    int z{};
    int y{};
    int x{};

    bool operator==(const Pos& other) const {
        return z == other.z && y == other.y && x == other.x;
    }
    bool operator<(const Pos& other) const {
        if (z != other.z) return z < other.z;
        if (y != other.y) return y < other.y;
        return x < other.x;
    }
};

struct PosHash {
    std::size_t operator()(const Pos& p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.z);
        std::size_t h2 = std::hash<int>{}(p.y);
        std::size_t h3 = std::hash<int>{}(p.x);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

class Volume3D {
public:
    Volume3D() = default;
    Volume3D(int depth, int height, int width, bool value = false);

    int depth() const { return d_; }
    int height() const { return h_; }
    int width() const { return w_; }
    std::size_t size() const { return data_.size(); }

    bool inside(const Pos& p) const;
    bool get(const Pos& p) const;
    void set(const Pos& p, bool value);
    std::size_t foregroundCount() const;
    std::vector<Pos> foregroundPositions() const;

private:
    int d_{0};
    int h_{0};
    int w_{0};
    std::vector<std::uint8_t> data_;

    std::size_t index(const Pos& p) const;
};

struct Direction {
    std::string name;
    Pos delta;
};

struct Counters {
    std::uint64_t volume_voxel_scans = 0;
    std::uint64_t active_surface_visits = 0;
    std::uint64_t border_tests = 0;
    std::uint64_t candidate_voxels = 0;

    std::uint64_t c1_checks = 0;
    std::uint64_t c1_pass = 0;
    std::uint64_t c2_checks = 0;
    std::uint64_t c2_pass = 0;
    std::uint64_t c3_checks = 0;
    std::uint64_t c3_pass = 0;
    std::uint64_t c4_checks = 0;
    std::uint64_t c4_pass = 0;

    std::uint64_t component_neighbor_tests = 0;
    std::uint64_t deleted_voxels = 0;
};

struct DirectionStats {
    std::string direction;
    std::uint64_t candidates = 0;
    std::uint64_t deleted = 0;
};

struct IterationStats {
    int iteration = 0;
    std::uint64_t deleted = 0;
    std::uint64_t remaining = 0;
    std::vector<DirectionStats> directions;
};

struct TopologySummary {
    int components26 = 0;
    long long euler = 0;
    std::uint64_t endpoints = 0;
    std::uint64_t regular_voxels = 0;
    std::uint64_t branch_voxels = 0;
};

struct RunResult {
    Volume3D skeleton;
    std::vector<IterationStats> iterations;
    Counters counters;
    double runtime_ms = 0.0;
    TopologySummary before;
    TopologySummary after;
};

enum class ScanMode {
    FullVolume,
    ActiveSurface
};

class LeeThinning {
public:
    explicit LeeThinning(ScanMode mode = ScanMode::FullVolume);

    RunResult run(const Volume3D& input, int max_iterations = 1000, bool verbose = false);

    static const std::vector<Pos>& offsets6();
    static const std::vector<Pos>& offsets18();
    static const std::vector<Pos>& offsets26();
    static const std::vector<Direction>& directions();

    static TopologySummary summarizeTopology(const Volume3D& volume);
    static long long globalEulerCharacteristic(const Volume3D& volume);
    static int globalComponents26(const Volume3D& volume);
    static void saveSlicePGM(const Volume3D& volume, int z, const std::string& path);

private:
    ScanMode mode_;
    Counters counters_;

    bool isDirectionalBorder(const Volume3D& v, const Pos& p, const Pos& direction);
    std::vector<Pos> neighbors(const Volume3D& v, const Pos& p, const std::vector<Pos>& offsets, bool foreground_only) const;
    std::vector<Pos> localForeground(const Volume3D& v, const Pos& p) const;

    long long eulerCharacteristicLocal(const std::vector<Pos>& local_voxels) const;
    bool conditionC1Euler(const Volume3D& v, const Pos& p);
    int countLocalComponents26(const Volume3D& v, const Pos& p);
    bool conditionC2Connectivity(const Volume3D& v, const Pos& p);
    bool conditionC3Recheck(const Volume3D& v, const Pos& p);
    bool isArcEndpoint(const Volume3D& v, const Pos& p) const;
    bool conditionC4Endpoint(const Volume3D& v, const Pos& p);

    std::vector<Pos> findCandidatesFullScan(Volume3D& v, const Pos& direction);
    std::vector<Pos> findCandidatesActiveSurface(Volume3D& v, const Pos& direction, const std::vector<Pos>& active);
    DirectionStats processDirectionFullScan(Volume3D& v, const Direction& dir);
    DirectionStats processDirectionActiveSurface(Volume3D& v, const Direction& dir, std::vector<Pos>& active);
    std::vector<Pos> initialActiveSurface(const Volume3D& v) const;
};

Volume3D makeTube(int side, int thickness);
Volume3D makeBranch(int side, int thickness);
Volume3D makeSolidCube(int side, int object_side);
Volume3D makeLoop(int side, int thickness);
Volume3D makeVascularTree(int side, int thickness);

void writeCSVHeader(std::ostream& out);
void writeCSVRow(std::ostream& out,
                 const std::string& shape,
                 const Volume3D& input,
                 const RunResult& result,
                 double runtime_ms);
