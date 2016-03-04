class SizeProperties{
public:
    SizeProperties() : min(-1), max(-1), avg(.0f), standard_dev(.0f) {}
    ~SizeProperties() {}
    inline bool operator ==(const SizeProperties & other) const
    {
        return (min == other.min && max == other.max && avg == other.avg && standard_dev == other.standard_dev);
    }

    int min;
    int max;
    float avg;
    float standard_dev;
};
