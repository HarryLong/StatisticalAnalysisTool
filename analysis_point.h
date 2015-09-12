#ifndef ANALYSIS_POINT_H
#define ANALYSIS_POINT_H

#include <QPoint>
#include <boost/functional/hash.hpp>

class AnalysisPoint{
public:
    AnalysisPoint(int category_id, QPoint center, int radius = 1);
    virtual ~AnalysisPoint();
    bool operator==(const AnalysisPoint & other) const;
    bool operator!=(const AnalysisPoint & other) const;
    virtual int getRadius() const;
    virtual QPoint getCenter() const;
    virtual int getCategoryId() const;

    void setCenter(QPoint center);
    void setRadius(int radius);

private:
    int r;
    QPoint center;
    int category_id;
};

namespace std {
  template <>
  struct hash<AnalysisPoint>
  {
    std::size_t operator()(const AnalysisPoint & p) const
    {
      std::size_t val = 0 ;
      boost::hash_combine(val,p.getRadius());
      boost::hash_combine(val,p.getCategoryId());
      boost::hash_combine(val,p.getCenter().x());
      boost::hash_combine(val,p.getCenter().y());
      return val;
    }
  };
}

#endif // ANALYSIS_POINT_H
