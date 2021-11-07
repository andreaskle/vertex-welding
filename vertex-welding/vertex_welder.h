#ifndef VERTEX_WELDER_H_
#define VERTEX_WELDER_H_
#include <vector>
#include <cstdint>
#include <limits>
#include <tuple>
#include "vertex.h"

struct WeldVertex {
    Vertex vertex;
    std::uint32_t index;
};

class VertexWelder {
   public:
    /// Welds a mesh and returns the list of unique vertices and the updated index list
    std::pair<std::vector<Vertex>, std::vector<std::uint32_t>> Weld(const std::vector<Vertex>& vertices,
                                                                    const std::vector<std::uint32_t>& indices);

   private:
    void AddToBucket(const Vertex& vertex, std::uint32_t index);

    std::uint32_t FindVertex(const Vertex& vertex);
    std::tuple<std::int64_t, std::int64_t, std::int64_t> Index(const Vertex& vertex) const;

    std::uint64_t Hash(const std::tuple<std::int64_t, std::int64_t, std::int64_t>& index) const;

    // Prime numbers from Teschner et al:  "Optimized Spatial Hashing for Collision Detection of Deformable Objects"
    std::int64_t kPrimeX = 73856093;
    std::int64_t kPrimeY = 19349663;
    std::int64_t kPrimeZ = 83492791;

    std::int64_t kNumBuckets = 16;
    float kCellSize = 0.5f;
    float kWeldTolerance = 0.1f;

    std::vector<std::vector<WeldVertex>> buckets;
};
#endif