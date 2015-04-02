#ifndef SPATIAL_HASHMAP_H
#define SPATIAL_HASHMAP_H

#include <unordered_map>
#include <QPoint>
#include <math.h>
#include "boost/functional/hash.hpp"

struct BoundingBox{
    QPoint min, max;
};

namespace std {
  template <>
  struct hash<QPoint>
  {
    std::size_t operator()(const QPoint & t) const
    {
      std::size_t val = 0 ;
      boost::hash_combine(val,t.x());
      boost::hash_combine(val,t.y());
      return val;
    }
  };
}

template <class T> class SpatialHashMap : protected std::unordered_map<QPoint, T>{
public:
    SpatialHashMap(int cell_width, int cell_height, int n_horizontal_cells, int n_vertical_cells) :
        m_cell_width(cell_width), m_cell_height(cell_height),
        m_horizontal_cell_count(n_horizontal_cells),
        m_vertical_cell_count(n_vertical_cells) {    }

    int getHorizontalCellCount() const { return m_horizontal_cell_count; }
    int getVerticalCellCount() const { return m_vertical_cell_count; }
    int getCellWidth() const { return m_cell_width; }
    int getCellHeight() const { return m_cell_height; }

    void init_cell(QPoint key)
    {
        std::unordered_map<QPoint, T>::insert(std::pair<QPoint, T>(key, T()));
    }

    std::vector<T*> getCells(QPoint p_center, float p_radius)
    {
        // First calculate the bounding box
        float x_min(p_center.x()-p_radius);
        float x_max(p_center.x()+p_radius);

        float y_min(p_center.y()-p_radius);
        float y_max(p_center.y()+p_radius);

        BoundingBox bb;
        QPoint mins(std::max(0.0f, x_min), std::max(0.0f, y_min));
        QPoint maxs(x_max, y_max);
        bb.min = toHashMapCoordinates(mins);
        bb.max = toHashMapCoordinates(maxs);

        std::vector<T*> ret;
        for(int x (bb.min.x()); x < std::min(m_horizontal_cell_count, bb.max.x()+1); x++)
        {
            for(int y (bb.min.y()); y < std::min(m_vertical_cell_count, bb.max.y()+1); y++)
            {
                // Ensure at least the center of the cell is within reach
                if(sqrt(pow(((x*m_cell_width) + m_cell_width/2) - p_center.x(),2) +
                      pow(((y*m_cell_height) + m_cell_height/2) - p_center.y(),2)) < p_radius)
                {
                    T* cell (get(QPoint(x,y)));
                    if(cell != NULL)
                        ret.push_back(cell);
                }
            }
        }

        // Plant is too small, at least add the single cell in which it resides
        if(ret.size() == 0)
            ret.push_back(getCell(p_center));

        return ret;
    }

    T* getCell(QPoint p_world_space_center, bool init_if_necessary = false)
    {
        QPoint center(toHashMapCoordinates(p_world_space_center));
        if(init_if_necessary && !get(center))
            init_cell(center);

        return get(center);
    }

    T* get(QPoint key)
    {
        auto it (std::unordered_map<QPoint, T>::find(key));
        if(it != std::unordered_map<QPoint, T>::end())
            return &(it->second);

        return NULL;
    }

    const T* get_const(QPoint key) const
    {
        auto it (std::unordered_map<QPoint, T>::find(key));
        if(it != std::unordered_map<QPoint, T>::end())
            return &(it->second);

        return NULL;
    }

    typename std::unordered_map<QPoint, T>::iterator begin()
    {
        return std::unordered_map<QPoint, T>::begin();
    }

    typename std::unordered_map<QPoint, T>::iterator end()
    {
        return std::unordered_map<QPoint, T>::end();
    }

    void clear()
    {
        std::unordered_map<QPoint, T>::clear();
    }

private:
    QPoint toHashMapCoordinates( const QPoint & coord )
    {
        return QPoint(coord.x() / m_cell_width, coord.y() / m_cell_height);
    }

    int m_cell_width, m_cell_height, m_horizontal_cell_count, m_vertical_cell_count;
};

#endif //SPATIAL_HASHMAP_H
