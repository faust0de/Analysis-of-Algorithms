#include "LeeThinning.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <stdexcept>
#include <tuple>
#include <unordered_set>

namespace {

std::vector<Pos> makeOffsets(int neighborhood) {
    std::vector<Pos> out;
    for (int dz = -1; dz <= 1; ++dz) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dz == 0 && dy == 0 && dx == 0) continue;
                const int manhattan = std::abs(dz) + std::abs(dy) + std::abs(dx);
                if (neighborhood == 6 && manhattan == 1) out.push_back({dz, dy, dx});
                else if (neighborhood == 18 && manhattan <= 2) out.push_back({dz, dy, dx});
                else if (neighborhood == 26) out.push_back({dz, dy, dx});
            }
        }
    }
    return out;
}

Pos add(const Pos& a, const Pos& b) {
    return {a.z + b.z, a.y + b.y, a.x + b.x};
}

struct Vertex {
    int z, y, x;
    bool operator<(const Vertex& o) const {
        return std::tie(z,y,x) < std::tie(o.z,o.y,o.x);
    }
};

struct Edge {
    Vertex a, b;
    bool operator<(const Edge& o) const {
        return std::tie(a.z,a.y,a.x,b.z,b.y,b.x) <
               std::tie(o.a.z,o.a.y,o.a.x,o.b.z,o.b.y,o.b.x);
    }
};

struct Face {
    int axis, plane, a, b;
    bool operator<(const Face& o) const {
        return std::tie(axis,plane,a,b) < std::tie(o.axis,o.plane,o.a,o.b);
    }
};

Edge normalizedEdge(Vertex a, Vertex b) {
    if (b < a) std::swap(a,b);
    return {a,b};
}

long long eulerOfVoxels(const std::vector<Pos>& voxels) {
    std::set<Vertex> vertices;
    std::set<Edge> edges;
    std::set<Face> faces;

    for (const Pos& p : voxels) {
        const int z = p.z, y = p.y, x = p.x;

        for (int dz = 0; dz <= 1; ++dz)
            for (int dy = 0; dy <= 1; ++dy)
                for (int dx = 0; dx <= 1; ++dx)
                    vertices.insert({z+dz, y+dy, x+dx});

        for (int dz = 0; dz <= 1; ++dz)
            for (int dy = 0; dy <= 1; ++dy)
                edges.insert(normalizedEdge({z+dz,y+dy,x}, {z+dz,y+dy,x+1}));

        for (int dz = 0; dz <= 1; ++dz)
            for (int dx = 0; dx <= 1; ++dx)
                edges.insert(normalizedEdge({z+dz,y,x+dx}, {z+dz,y+1,x+dx}));

        for (int dy = 0; dy <= 1; ++dy)
            for (int dx = 0; dx <= 1; ++dx)
                edges.insert(normalizedEdge({z,y+dy,x+dx}, {z+1,y+dy,x+dx}));

        faces.insert({0, z,     y, x});
        faces.insert({0, z + 1, y, x});
        faces.insert({1, y,     z, x});
        faces.insert({1, y + 1, z, x});
        faces.insert({2, x,     z, y});
        faces.insert({2, x + 1, z, y});
    }

    return static_cast<long long>(vertices.size())
         - static_cast<long long>(edges.size())
         + static_cast<long long>(faces.size())
         - static_cast<long long>(voxels.size());
}

void thickenPoint(Volume3D& v, const Pos& center, int thickness) {
    const int r0 = -(thickness / 2);
    const int r1 = r0 + thickness - 1;
    for (int dz = r0; dz <= r1; ++dz)
        for (int dy = r0; dy <= r1; ++dy)
            for (int dx = r0; dx <= r1; ++dx) {
                Pos q{center.z + dz, center.y + dy, center.x + dx};
                if (v.inside(q)) v.set(q, true);
            }
}

} // namespace

Volume3D::Volume3D(int depth, int height, int width, bool value)
    : d_(depth), h_(height), w_(width), data_(static_cast<std::size_t>(depth) * height * width, value ? 1 : 0) {
    if (depth <= 0 || height <= 0 || width <= 0) {
        throw std::invalid_argument("Volume dimensions must be positive.");
    }
}

std::size_t Volume3D::index(const Pos& p) const {
    return (static_cast<std::size_t>(p.z) * h_ + p.y) * w_ + p.x;
}

bool Volume3D::inside(const Pos& p) const {
    return p.z >= 0 && p.z < d_ && p.y >= 0 && p.y < h_ && p.x >= 0 && p.x < w_;
}

bool Volume3D::get(const Pos& p) const {
    if (!inside(p)) return false; // outside is background
    return data_[index(p)] != 0;
}

void Volume3D::set(const Pos& p, bool value) {
    if (!inside(p)) throw std::out_of_range("Voxel outside volume.");
    data_[index(p)] = value ? 1 : 0;
}

std::size_t Volume3D::foregroundCount() const {
    return static_cast<std::size_t>(std::count(data_.begin(), data_.end(), static_cast<std::uint8_t>(1)));
}

std::vector<Pos> Volume3D::foregroundPositions() const {
    std::vector<Pos> out;
    out.reserve(foregroundCount());
    for (int z = 0; z < d_; ++z)
        for (int y = 0; y < h_; ++y)
            for (int x = 0; x < w_; ++x)
                if (get({z,y,x})) out.push_back({z,y,x});
    return out;
}

LeeThinning::LeeThinning(ScanMode mode) : mode_(mode) {}

const std::vector<Pos>& LeeThinning::offsets6() {
    static const std::vector<Pos> v = makeOffsets(6);
    return v;
}
const std::vector<Pos>& LeeThinning::offsets18() {
    static const std::vector<Pos> v = makeOffsets(18);
    return v;
}
const std::vector<Pos>& LeeThinning::offsets26() {
    static const std::vector<Pos> v = makeOffsets(26);
    return v;
}
const std::vector<Direction>& LeeThinning::directions() {
    static const std::vector<Direction> dirs = {
        {"U", { 1, 0, 0}},
        {"B", {-1, 0, 0}},
        {"N", { 0,-1, 0}},
        {"S", { 0, 1, 0}},
        {"W", { 0, 0,-1}},
        {"E", { 0, 0, 1}}
    };
    return dirs;
}

std::vector<Pos> LeeThinning::neighbors(const Volume3D& v, const Pos& p,
                                        const std::vector<Pos>& offsets,
                                        bool foreground_only) const {
    std::vector<Pos> out;
    out.reserve(offsets.size());
    for (const Pos& d : offsets) {
        const Pos q = add(p,d);
        if (!v.inside(q)) continue;
        if (foreground_only && !v.get(q)) continue;
        out.push_back(q);
    }
    return out;
}

bool LeeThinning::isDirectionalBorder(const Volume3D& v, const Pos& p, const Pos& direction) {
    ++counters_.border_tests;
    if (!v.get(p)) return false;
    return !v.get(add(p, direction));
}

std::vector<Pos> LeeThinning::localForeground(const Volume3D& v, const Pos& p) const {
    std::vector<Pos> local;
    for (int dz = -1; dz <= 1; ++dz)
        for (int dy = -1; dy <= 1; ++dy)
            for (int dx = -1; dx <= 1; ++dx) {
                Pos q{p.z+dz,p.y+dy,p.x+dx};
                if (v.inside(q) && v.get(q)) local.push_back({dz+1,dy+1,dx+1});
            }
    return local;
}

long long LeeThinning::eulerCharacteristicLocal(const std::vector<Pos>& local_voxels) const {
    return eulerOfVoxels(local_voxels);
}

bool LeeThinning::conditionC1Euler(const Volume3D& v, const Pos& p) {
    ++counters_.c1_checks;
    auto before = localForeground(v,p);
    const auto e_before = eulerCharacteristicLocal(before);

    std::vector<Pos> after;
    after.reserve(before.size());
    for (const Pos& q : before) {
        if (!(q == Pos{1,1,1})) after.push_back(q);
    }
    const auto e_after = eulerCharacteristicLocal(after);
    const bool ok = e_before == e_after;
    if (ok) ++counters_.c1_pass;
    return ok;
}

int LeeThinning::countLocalComponents26(const Volume3D& v, const Pos& p) {
    std::unordered_set<Pos, PosHash> remaining;
    for (const Pos& q : neighbors(v,p,offsets26(),true)) remaining.insert(q);
    if (remaining.empty()) return 0;

    int components = 0;
    std::vector<Pos> stack;

    while (!remaining.empty()) {
        ++components;
        Pos start = *remaining.begin();
        remaining.erase(start);
        stack.clear();
        stack.push_back(start);

        while (!stack.empty()) {
            Pos cur = stack.back();
            stack.pop_back();
            for (const Pos& d : offsets26()) {
                ++counters_.component_neighbor_tests;
                Pos q = add(cur,d);
                auto it = remaining.find(q);
                if (it != remaining.end()) {
                    remaining.erase(it);
                    stack.push_back(q);
                }
            }
        }
    }
    return components;
}

bool LeeThinning::conditionC2Connectivity(const Volume3D& v, const Pos& p) {
    ++counters_.c2_checks;
    const bool ok = countLocalComponents26(v,p) == 1;
    if (ok) ++counters_.c2_pass;
    return ok;
}

bool LeeThinning::conditionC3Recheck(const Volume3D& v, const Pos& p) {
    ++counters_.c3_checks;
    const bool ok = countLocalComponents26(v,p) == 1;
    if (ok) ++counters_.c3_pass;
    return ok;
}

bool LeeThinning::isArcEndpoint(const Volume3D& v, const Pos& p) const {
    int count = 0;
    for (const Pos& d : offsets26()) {
        if (v.get(add(p,d))) {
            ++count;
            if (count > 1) return false;
        }
    }
    return true; // 0 or 1 foreground neighbor
}

bool LeeThinning::conditionC4Endpoint(const Volume3D& v, const Pos& p) {
    ++counters_.c4_checks;
    const bool ok = !isArcEndpoint(v,p);
    if (ok) ++counters_.c4_pass;
    return ok;
}

std::vector<Pos> LeeThinning::findCandidatesFullScan(Volume3D& v, const Pos& direction) {
    std::vector<Pos> candidates;
    for (int z = 0; z < v.depth(); ++z)
        for (int y = 0; y < v.height(); ++y)
            for (int x = 0; x < v.width(); ++x) {
                ++counters_.volume_voxel_scans;
                Pos p{z,y,x};
                if (!v.get(p)) continue;
                if (!isDirectionalBorder(v,p,direction)) continue;
                ++counters_.candidate_voxels;
                if (!conditionC1Euler(v,p)) continue;
                if (!conditionC2Connectivity(v,p)) continue;
                candidates.push_back(p);
            }
    return candidates;
}

std::vector<Pos> LeeThinning::findCandidatesActiveSurface(Volume3D& v, const Pos& direction,
                                                          const std::vector<Pos>& active) {
    std::vector<Pos> candidates;
    for (const Pos& p : active) {
        ++counters_.active_surface_visits;
        if (!v.get(p)) continue;
        if (!isDirectionalBorder(v,p,direction)) continue;
        ++counters_.candidate_voxels;
        if (!conditionC1Euler(v,p)) continue;
        if (!conditionC2Connectivity(v,p)) continue;
        candidates.push_back(p);
    }
    return candidates;
}

DirectionStats LeeThinning::processDirectionFullScan(Volume3D& v, const Direction& dir) {
    auto candidates = findCandidatesFullScan(v, dir.delta);
    std::uint64_t deleted = 0;
    for (const Pos& p : candidates) {
        if (!v.get(p)) continue;
        if (!conditionC3Recheck(v,p)) continue;
        if (!conditionC4Endpoint(v,p)) continue;
        v.set(p,false);
        ++deleted;
        ++counters_.deleted_voxels;
    }
    return {dir.name, static_cast<std::uint64_t>(candidates.size()), deleted};
}

std::vector<Pos> LeeThinning::initialActiveSurface(const Volume3D& v) const {
    std::vector<Pos> active;
    for (const Pos& p : v.foregroundPositions()) {
        bool border = false;
        for (const Pos& d : offsets6()) {
            if (!v.get(add(p,d))) { border = true; break; }
        }
        if (border) active.push_back(p);
    }
    return active;
}

DirectionStats LeeThinning::processDirectionActiveSurface(Volume3D& v, const Direction& dir,
                                                           std::vector<Pos>& active) {
    auto candidates = findCandidatesActiveSurface(v, dir.delta, active);
    std::unordered_set<Pos, PosHash> considered(candidates.begin(), candidates.end());
    std::unordered_set<Pos, PosHash> next;

    // Retain active voxels not considered in this directional subiteration.
    for (const Pos& p : active) {
        if (v.get(p) && considered.find(p) == considered.end()) next.insert(p);
    }

    std::uint64_t deleted = 0;
    for (const Pos& p : candidates) {
        if (!v.get(p)) continue;
        if (!conditionC3Recheck(v,p)) continue;
        if (!conditionC4Endpoint(v,p)) continue;

        v.set(p,false);
        ++deleted;
        ++counters_.deleted_voxels;

        // Neighbor deletion can make nearby foreground voxels relevant again.
        for (const Pos& d : offsets26()) {
            Pos q = add(p,d);
            if (v.inside(q) && v.get(q)) next.insert(q);
        }
    }

    active.assign(next.begin(), next.end());
    std::sort(active.begin(), active.end());
    return {dir.name, static_cast<std::uint64_t>(candidates.size()), deleted};
}

RunResult LeeThinning::run(const Volume3D& input, int max_iterations, bool verbose) {
    counters_ = {};
    RunResult result;
    result.skeleton = input;
    result.before = summarizeTopology(input);

    std::vector<Pos> active;
    if (mode_ == ScanMode::ActiveSurface) active = initialActiveSurface(result.skeleton);

    auto t0 = std::chrono::steady_clock::now();

    for (int iteration = 1; iteration <= max_iterations; ++iteration) {
        IterationStats its;
        its.iteration = iteration;

        for (const Direction& dir : directions()) {
            DirectionStats ds = (mode_ == ScanMode::FullVolume)
                ? processDirectionFullScan(result.skeleton, dir)
                : processDirectionActiveSurface(result.skeleton, dir, active);
            its.deleted += ds.deleted;
            its.directions.push_back(ds);
        }

        its.remaining = result.skeleton.foregroundCount();
        result.iterations.push_back(its);

        if (verbose) {
            std::cout << "Iteration " << iteration
                      << " | deleted=" << its.deleted
                      << " | remaining=" << its.remaining << '\n';
            for (const auto& d : its.directions) {
                std::cout << "  " << d.direction
                          << " candidates=" << d.candidates
                          << " deleted=" << d.deleted << '\n';
            }
        }

        if (its.deleted == 0) break;
        if (iteration == max_iterations) {
            throw std::runtime_error("Maximum number of iterations reached.");
        }
    }

    auto t1 = std::chrono::steady_clock::now();
    result.runtime_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    result.counters = counters_;
    result.after = summarizeTopology(result.skeleton);
    return result;
}

int LeeThinning::globalComponents26(const Volume3D& volume) {
    std::unordered_set<Pos,PosHash> remaining;
    for (const Pos& p : volume.foregroundPositions()) remaining.insert(p);
    int components = 0;
    std::vector<Pos> stack;
    while (!remaining.empty()) {
        ++components;
        Pos start = *remaining.begin();
        remaining.erase(start);
        stack = {start};
        while (!stack.empty()) {
            Pos cur = stack.back(); stack.pop_back();
            for (const Pos& d : offsets26()) {
                Pos q = add(cur,d);
                auto it = remaining.find(q);
                if (it != remaining.end()) {
                    remaining.erase(it);
                    stack.push_back(q);
                }
            }
        }
    }
    return components;
}

long long LeeThinning::globalEulerCharacteristic(const Volume3D& volume) {
    return eulerOfVoxels(volume.foregroundPositions());
}

TopologySummary LeeThinning::summarizeTopology(const Volume3D& volume) {
    TopologySummary s;
    s.components26 = globalComponents26(volume);
    s.euler = globalEulerCharacteristic(volume);

    for (const Pos& p : volume.foregroundPositions()) {
        int degree = 0;
        for (const Pos& d : offsets26()) if (volume.get(add(p,d))) ++degree;
        if (degree < 2) ++s.endpoints;
        else if (degree == 2) ++s.regular_voxels;
        else ++s.branch_voxels;
    }
    return s;
}

void LeeThinning::saveSlicePGM(const Volume3D& volume, int z, const std::string& path) {
    if (z < 0 || z >= volume.depth()) throw std::out_of_range("Invalid z slice.");
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open PGM output.");
    out << "P5\n" << volume.width() << ' ' << volume.height() << "\n255\n";
    for (int y = 0; y < volume.height(); ++y)
        for (int x = 0; x < volume.width(); ++x) {
            unsigned char v = volume.get({z,y,x}) ? 255 : 0;
            out.write(reinterpret_cast<const char*>(&v),1);
        }
}

Volume3D makeTube(int side, int thickness) {
    Volume3D v(side,side,side,false);
    const int c = side/2;
    const int margin = std::max(2, side/8);
    for (int x = margin; x < side-margin; ++x) thickenPoint(v,{c,c,x},thickness);
    return v;
}

Volume3D makeBranch(int side, int thickness) {
    Volume3D v(side,side,side,false);
    const int c = side/2;
    const int margin = std::max(2, side/8);

    // Main vessel along X.
    for (int x = margin; x <= c; ++x) thickenPoint(v,{c,c,x},thickness);
    // Three branches from the junction: +X, -Y, +Y (a T/Y-like topology in 3D).
    for (int x = c; x < side-margin; ++x) thickenPoint(v,{c,c,x},thickness);
    for (int y = margin; y <= c; ++y) thickenPoint(v,{c,y,c},thickness);
    for (int y = c; y < side-margin; ++y) thickenPoint(v,{c,y,c},thickness);
    return v;
}

Volume3D makeSolidCube(int side, int object_side) {
    Volume3D v(side,side,side,false);
    object_side = std::min(object_side, side-2);
    int start = (side-object_side)/2;
    int end = start + object_side;
    for (int z=start; z<end; ++z)
        for (int y=start; y<end; ++y)
            for (int x=start; x<end; ++x)
                v.set({z,y,x},true);
    return v;
}

Volume3D makeLoop(int side, int thickness) {
    Volume3D v(side, side, side, false);
    const int z = side / 2;
    const int a = std::max(3, side / 4);
    const int b = side - a - 1;

    // Square closed loop in the central XY plane, thickened in 3-D.
    for (int x = a; x <= b; ++x) {
        thickenPoint(v, {z, a, x}, thickness);
        thickenPoint(v, {z, b, x}, thickness);
    }
    for (int y = a; y <= b; ++y) {
        thickenPoint(v, {z, y, a}, thickness);
        thickenPoint(v, {z, y, b}, thickness);
    }

    return v;
}

Volume3D makeVascularTree(int side, int thickness) {
    Volume3D v(side, side, side, false);
    const int c = side / 2;
    const int margin = std::max(3, side / 8);

    // Main trunk along X.
    for (int x = margin; x < side - margin; ++x)
        thickenPoint(v, {c, c, x}, thickness);

    // First bifurcation in Y.
    for (int y = c; y < side - margin; ++y)
        thickenPoint(v, {c, y, c}, thickness);
    for (int y = margin; y <= c; ++y)
        thickenPoint(v, {c, y, c}, thickness);

    // Secondary branches in Z, producing a more complex 3-D tree.
    const int x2 = std::min(side - margin - 1, c + std::max(2, side / 6));
    const int y2 = std::min(side - margin - 1, c + std::max(2, side / 6));

    for (int z = c; z < side - margin; ++z)
        thickenPoint(v, {z, c, x2}, thickness);
    for (int z = margin; z <= c; ++z)
        thickenPoint(v, {z, y2, c}, thickness);

    return v;
}

void writeCSVHeader(std::ostream& out) {
    out << "Shape,Size,Foreground,Iterations,Runtime_ms,Components,Euler,Endpoints\n";
}

void writeCSVRow(std::ostream& out,
                 const std::string& shape,
                 const Volume3D& input,
                 const RunResult& r,
                 double runtime_ms) {
    out << shape << ','
        << input.depth() << 'x' << input.height() << 'x' << input.width() << ','
        << input.foregroundCount() << ','
        << r.iterations.size() << ','
        << runtime_ms << ','
        << r.before.components26 << "->" << r.after.components26 << ','
        << r.before.euler << "->" << r.after.euler << ','
        << r.after.endpoints << '\n';
}
