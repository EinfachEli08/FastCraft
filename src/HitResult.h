class HitResult
{
public:
    int type;
    int x;
    int y;
    int z;
    int f;

    // Constructor
    HitResult(int type, int x, int y, int z, int f)
        : type(type), x(x), y(y), z(z), f(f) {}
};
