#ifndef __spantree_h__
#define __spantree_h__

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <utility>
#include <map>

class Spantree {
public:

    typedef std::pair<double, double> Point;
    typedef std::vector<Point> Points;
    typedef std::map<int, Point> PointIds;
    typedef std::pair< std::map<int, Point> , std::vector< std::pair<int, int> > > Graph;

private:
    typedef CGAL::Exact_predicates_inexact_constructions_kernel             K;
    typedef CGAL::Triangulation_vertex_base_with_info_2<unsigned int, K>    Vb;
    typedef CGAL::Triangulation_data_structure_2<Vb>                        Tds;
    typedef CGAL::Delaunay_triangulation_2<K, Tds>                          Triangulation;

    typedef K::Point_2          Point_2;
    typedef K::Iso_rectangle_2  Iso_rectangle_2;
    typedef K::Segment_2        Segment_2;
    typedef K::Ray_2            Ray_2;
    typedef K::Line_2           Line_2;



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

    std::map<int, Triangulation::Vertex_handle> indexToVertex;
    std::map<int, Point> indexToPoint;
    int indexMax = 0;
    Triangulation triangulation;

    double dis(Point_2 a, Point_2 b);

public:
    void insert(Points p);
    void clear();
    Graph getDelaunay();
    Graph getVoronoi();
    Graph getMSTree();
    const PointIds& getPointIds();
};

#endif
