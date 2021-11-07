#include "vertex_welder.h"

std::pair<std::vector<Vertex>, std::vector<std::uint32_t>> VertexWelder::Weld(
    const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices) {
    buckets.clear();
    buckets.resize(kNumBuckets);

    std::vector<Vertex> unique_vertices;
    std::vector<std::uint32_t> welded_indices;

    for (const auto& vertex_index : indices) {
        auto vertex = vertices[vertex_index];
        auto idx = FindVertex(vertex);
        if (idx == std::numeric_limits<std::uint32_t>::max()) {
            unique_vertices.push_back(vertex);
            welded_indices.push_back(unique_vertices.size() - 1);
            AddToBucket(vertex, welded_indices.back());
        } else {
            welded_indices.push_back(idx);
        }
    }
    return {unique_vertices, welded_indices};
}
void VertexWelder::AddToBucket(const Vertex& vertex, std::uint32_t index) {
    auto idx = Index(vertex);
    auto bucket_idx = Hash(idx);
    buckets[bucket_idx].emplace_back(WeldVertex{vertex, index});
}

float Distance(const Vertex& left, const Vertex& right) {
    return glm::dot(left.pos - right.pos, left.pos - right.pos);
}

std::uint32_t VertexWelder::FindVertex(const Vertex& vertex) {
    std::int64_t left = static_cast<std::int64_t>((vertex.pos.x - kWeldTolerance) / kCellSize);
    std::int64_t right = static_cast<std::int64_t>((vertex.pos.x + kWeldTolerance) / kCellSize);
    std::int64_t top = static_cast<std::int64_t>((vertex.pos.y - kWeldTolerance) / kCellSize);
    std::int64_t bottom = static_cast<std::int64_t>((vertex.pos.y + kWeldTolerance) / kCellSize);
    std::int64_t near = static_cast<std::int64_t>((vertex.pos.z - kWeldTolerance) / kCellSize);
    std::int64_t far = static_cast<std::int64_t>((vertex.pos.z + kWeldTolerance) / kCellSize);
    for (auto i = left; i <= right; ++i) {
        for (auto j = top; j <= bottom; ++j) {
            for (auto k = near; k <= far; ++k) {
                auto bucket_to_check = Hash(std::make_tuple(i, j, k));
                for (const auto& v : buckets[bucket_to_check]) {
                    if (Distance(v.vertex, vertex) < kWeldTolerance * kWeldTolerance) {
                        return v.index;
                    }
                }
            }
        }
    }
    return std::numeric_limits<std::uint32_t>::max();
}
std::tuple<std::int64_t, std::int64_t, std::int64_t> VertexWelder::Index(const Vertex& vertex) const {
    std::int64_t x = static_cast<std::int64_t>(vertex.pos.x / kCellSize);
    std::int64_t y = static_cast<std::int64_t>(vertex.pos.y / kCellSize);
    std::int64_t z = static_cast<std::int64_t>(vertex.pos.z / kCellSize);
    return std::make_tuple(x, y, z);
}

// Hash function is based on the paper of Teschner et al.: "Optimized Spatial Hashing for Collision Detection of
// Deformable Objects"
std::uint64_t VertexWelder::Hash(const std::tuple<std::int64_t, std::int64_t, std::int64_t>& index) const {
    std::uint64_t idx =
        (std::get<0>(index) * kPrimeX) ^ (std::get<1>(index) * kPrimeY) ^ (std::get<2>(index) * kPrimeZ);
    return idx % kNumBuckets;
}