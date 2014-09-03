#ifndef __spantree_h__
#define __spantree_h__

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <utility>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::Point_2          Point_2;
typedef K::Iso_rectangle_2  Iso_rectangle_2;
typedef K::Segment_2        Segment_2;
typedef K::Ray_2            Ray_2;
typedef K::Line_2           Line_2;

class spantree {
public:
    struct Cropped_voronoi_from_delaunay{
        std::list<Segment_2> m_cropped_vd;
        Iso_rectangle_2 m_bbox;

        Cropped_voronoi_from_delaunay(const Iso_rectangle_2& bbox):m_bbox(bbox){}

        template <class RSL>
        void crop_and_extract_segment(const RSL& rsl){
            CGAL::Object obj = CGAL::intersection(rsl,m_bbox);
            const Segment_2* s = CGAL::object_cast<Segment_2>(&obj);
            if (s) m_cropped_vd.push_back(*s);
        }

        void operator << (const Ray_2& ray)    { crop_and_extract_segment(ray); }
        void operator << (const Line_2& line)  { crop_and_extract_segment(line); }
        void operator << (const Segment_2& seg){ crop_and_extract_segment(seg); }
    };

    Triangulation dt2;

    void insert(std::vector< std::pair<double, double> > p) {
        std::vector<Point_2> points;
        for (auto point : p) {
            points.push_back(Point_2(point.first, point.second));
        }
        dt2.insert(points.begin(), points.end());
    }
    std::vector< std::pair< std::pair<double, double>, std::pair<double, double> > > get_tree() {
        std::vector< std::pair< std::pair<double, double>, std::pair<double, double> > > rt;
        for (auto itr = dt2.finite_edges_begin(); itr != dt2.finite_edges_end(); itr++) {
            auto v1 = itr->first->vertex(dt2.cw(itr->second));
            auto v2 = itr->first->vertex(dt2.ccw(itr->second));
            rt.push_back(std::make_pair(std::make_pair(v1->point().x(), v1->point().y()),
                                        std::make_pair(v2->point().x(), v2->point().y())));
        }
        return rt;
    }
};

#endif
