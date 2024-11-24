#ifndef LEVELLISTENER_HPP
#define LEVELLISTENER_HPP

class LevelListener
{
public:
    virtual ~LevelListener() = default;

    // Method called when a tile has changed at a specific position
    virtual void tileChanged(int x, int y, int z) = 0;

    virtual void lightColumnChanged(int var1, int var2, int var3, int var4) = 0;

    // Method called when all tiles have changed
    virtual void allChanged() = 0;
};

#endif // LEVELLISTENER_HPP
