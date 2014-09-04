#include "spantree.h"
#include <algorithm>
#include <set>
#include <boost/pending/disjoint_sets.hpp>
#include <boost/cstdlib.hpp>

double Spantree::dis(Spantree::Point_2 a, Spantree::Point_2 b) {
    return (a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y());
}

void Spantree::insert(Points p) {

    std::vector< std::pair<Point_2, int> > points;
    for (auto point : p) {
        points.push_back(std::make_pair(Point_2(point.first, point.second), indexMax));
        indexToPoint[indexMax] = point;
        indexMax++;
    }
    triangulation.insert(points.begin(), points.end());


    for (auto itr = triangulation.finite_vertices_begin(); itr != triangulation.finite_vertices_end(); itr++) {
        indexToVertex[itr->info()] = itr;
    }
}

void Spantree::clear() {
    triangulation.clear();
    indexToPoint.clear();
    indexToVertex.clear();
    indexMax = 0;
}

Spantree::Graph Spantree::getDelaunay() {
    Graph ret;
    for (auto itr = triangulation.finite_edges_begin(); itr != triangulation.finite_edges_end(); itr++) {
        auto v1 = itr->first->vertex(triangulation.cw(itr->second));
        auto v2 = itr->first->vertex(triangulation.ccw(itr->second));
        ret.second.push_back(std::make_pair(v1->info(), v2->info()));
    }
    ret.first = indexToPoint;
    return ret;
}

Spantree::Graph Spantree::getVoronoi() {
    Graph ret;
    if (indexToPoint.size() < 2) return ret;
    double minx = 1e100, miny = 1e100, maxx = -1e100, maxy = -1e100;
    for (auto entry : indexToPoint) {
        minx = std::min(minx, entry.second.first);
        miny = std::min(miny, entry.second.second);
        maxx = std::max(maxx, entry.second.first);
        maxy = std::max(maxy, entry.second.second);
    }
    minx -= 10; miny -= 10; maxx += 10; maxy += 10;
    Iso_rectangle_2 bbox(minx, miny, maxx, maxy);
    Cropped_voronoi_from_delaunay vor(bbox);

    std::cout << "1" << std::endl;

    triangulation.draw_dual(vor);

    std::cout << "2" << std::endl;

    int cnt = 0;
    for (auto seg : vor.m_cropped_vd) {
        auto v1 = seg.source();
        auto v2 = seg.target();
        ret.first[cnt] = std::make_pair(v1.x(), v1.y());
        ret.first[cnt + 1] = std::make_pair(v2.x(), v2.y());
        ret.second.push_back(std::make_pair(cnt, cnt + 1));
        cnt += 2;
    }
    ret.first[cnt++] = std::make_pair(minx, miny);
    ret.first[cnt++] = std::make_pair(minx, maxy);
    ret.first[cnt++] = std::make_pair(maxx, maxy);
    ret.first[cnt++] = std::make_pair(maxx, miny);
    ret.second.push_back(std::make_pair(cnt - 4, cnt - 3));
    ret.second.push_back(std::make_pair(cnt - 3, cnt - 2));
    ret.second.push_back(std::make_pair(cnt - 2, cnt - 1));
    ret.second.push_back(std::make_pair(cnt - 1, cnt - 4));
    return ret;
}

Spantree::Graph Spantree::getMSTree() {
    using namespace boost;
    std::vector< std::pair< double, std::pair<int, int> > > edge;
    for (auto itr = triangulation.finite_edges_begin(); itr != triangulation.finite_edges_end(); itr++) {
        auto v1 = itr->first->vertex(triangulation.cw(itr->second));
        auto v2 = itr->first->vertex(triangulation.ccw(itr->second));
        edge.push_back(std::make_pair(dis(v1->point(), v2->point()), std::make_pair(v1->info(), v2->info())));
    }
    sort(edge.begin(), edge.end());

    disjoint_sets_with_storage<identity_property_map, identity_property_map, find_with_full_path_compression> dset(indexToPoint.size());
    for (auto entry : indexToPoint)
        dset.make_set(entry.first);

    Graph ret;
    for (auto itr = edge.begin(); itr != edge.end(); itr++) {
        int v1 = dset.find_set(itr->second.first);
        int v2 = dset.find_set(itr->second.second);
        if (v1 != v2) {
            dset.link(v1, v2);
            ret.second.push_back(itr->second);
        }
    }
    ret.first = indexToPoint;
    return ret;
}

const Spantree::PointIds &Spantree::getPointIds() {
    return indexToPoint;
}
