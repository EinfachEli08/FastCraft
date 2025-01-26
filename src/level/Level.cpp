#include "Level.h"
#include "Fastcraft.h"


#include <algorithm>
#include <cmath>
#include <zlib.h>
#include <functional>
#include <fstream>
#include <vector>
#include "Level/tile/Tile.h"
#include "Level/LevelGen.h"

Level::Level(Fastcraft* fc, int width, int height, int depth) : width(0), height(0), depth(0), blocks(0), lightDepths(), coords(1048576), random(rd())
{
    int scWidth;
    int scHeight;
    this->fastCraft = fc;
    scWidth = fc->width * 240 / fc->height;
    scHeight = fc->height * 240 / fc->height;
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0D, (double)scWidth, (double)scHeight, 0.0D, 100.0D, 300.0D);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0F, 0.0F, -200.0F);
    this->fastCraft->showLoadingScreen("Loading level", "Reading..");
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->blocks.resize(width * height * depth);
    this->lightDepths.resize(width * height * depth);
    bool loaded = this->load();

    if (!loaded)
    {
        this->generateMap();
    }

    this->calcLightDepths(0, 0, width, height);
}

void Level::generateMap()
{
    this->fastCraft->showLoadingScreen("Generating level", "Raising..");
    LevelGen* gen = new LevelGen(this->width, this->height, this->depth);
    LevelGen* genSav = gen;
    int gWidth = gen->width;
    int gHeight = gen->height;
    int gDepth = gen->depth;

    std::vector<int> noise1 = NoiseMap(gen->random, 1, true).read(gWidth, gHeight);
    std::vector<int> noise2 = NoiseMap(gen->random, 0, true).read(gWidth, gHeight);
    std::vector<int> noise3 = NoiseMap(gen->random, 2, false).read(gWidth, gHeight);
    std::vector<int> noise4 = NoiseMap(gen->random, 4, false).read(gWidth, gHeight);
    std::vector<int> noise5 = NoiseMap(gen->random, 5, true).read(gWidth, gHeight);
    std::vector<unsigned int> map(gWidth * gHeight * gDepth, 0);
    int var12 = gDepth / 2;

    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    int var17;
    int var19;
    int var20;
    int var21;
    for(int x = 0; x < gWidth; ++x) {
        for(int z = 0; z < gDepth; ++z) {
            for(int y = 0; y < gHeight; ++y) {
                int var16 = noise1[x + y * genSav->width];

                var17 = noise2[x + y * genSav->width];

                int var18 = noise3[x + y * genSav->width];

                var19 = noise4[x + y * genSav->width];

                if(var18 < 128) {
                    var17 = var16;
                }

                var20 = var16;
                if(var17 > var16) {
                    var20 = var17;
                }

                var20 = (var20 - 128) / 8;
                var20 = var20 + var12 - 1;
                var21 = ((noise5[x + y * genSav->width] - 128) / 6 + var12 + var20) / 2;
                if(var19 < 92) {
                    var20 = var20 / 2 << 1;
                } else if(var19 < 160) {
                    var20 = var20 / 4 << 2;
                }

                if(var20 < var12 - 2) {
                    var20 = (var20 - var12) / 2 + var12;
                }

                if(var21 > var20 - 2) {
                    var21 = var20 - 2;
                }

                int var22 = (z * genSav->height + y) * genSav->width + x;
                int var23 = 0;
                if(z == var20 && z >= gDepth / 2) {
                    var23 = Tile::grass->id;
                }

                if(z < var20) {
                    var23 = Tile::dirt->id;
                }

                if(z <= var21) {
                    var23 = Tile::rock->id;
                }

                map[var22] = var23;
            }
        }
    }

    this->blocks = map;
    this->fastCraft->showLoadingScreen("Generating level", "Carving..");

    std::vector<unsigned int> var27 = this->blocks;
    genSav = gen;
    gHeight = gen->width;
    gDepth = gen->height;
    int var29 = gen->depth;
    int var30 = gHeight * gDepth * var29 / 256 / 64;

    int var25;
    for(int var31 = 0; var31 < var30; ++var31) {
        float var33 = dist(genSav->random) * (float)gHeight;
        float var35 = dist(genSav->random) * (float)var29;
        float var37 = dist(genSav->random) * (float)gDepth;
        var12 = (int)(dist(genSav->random) + dist(genSav->random) * 150.0F);
        float var39 = (float)(dist(genSav->random) * Math::PI * 2.0f);
        float var40 = 0.0F;
        float var41 = (float)(dist(genSav->random) * Math::PI * 2.0f);
        float var42 = 0.0F;

        for(var17 = 0; var17 < var12; ++var17) {
            var33 = (float)((double)var33 + std::sin((double)var39) * std::cos((double)var41));
            var37 = (float)((double)var37 + std::cos((double)var39) * std::cos((double)var41));
            var35 = (float)((double)var35 + std::sin((double)var41));
            var39 += var40 * 0.2F;
            var40 *= 0.9F;
            var40 += dist(genSav->random) - dist(genSav->random);
            var41 += var42 * 0.5F;
            var41 *= 0.5F;
            var42 *= 0.9F;
            var42 += dist(genSav->random) - dist(genSav->random);
            float var43 = (float)(std::sin((double)var17 * Math::PI / (double)var12) * 2.5D + 1.0D);

            for(var19 = (int)(var33 - var43); var19 <= (int)(var33 + var43); ++var19) {
                for(var20 = (int)(var35 - var43); var20 <= (int)(var35 + var43); ++var20) {
                    for(var21 = (int)(var37 - var43); var21 <= (int)(var37 + var43); ++var21) {
                        float var44 = (float)var19 - var33;
                        float var45 = (float)var20 - var35;
                        float var24 = (float)var21 - var37;
                        var24 = var44 * var44 + var45 * var45 * 2.0F + var24 * var24;
                        if(var24 < var43 * var43 && var19 >= 1 && var20 >= 1 && var21 >= 1 && var19 < genSav->width - 1 && var20 < genSav->depth - 1 && var21 < genSav->height - 1) {
                            var25 = (var20 * genSav->height + var21) * genSav->width + var19;
                            if(var27[var25] == Tile::rock->id) {
                                var27[var25] = 0;
                            }
                        }
                    }
                }
            }
        }
    }

    this->blocks = var27;
    this->fastCraft->showLoadingScreen("Generating level", "Watering..");
    //TODO: IMPL once there are liquids

    /*
     this.minecraft.showLoadingScreen("Generating level", "Watering..");
		long var26 = System.nanoTime();
		long var28 = 0L;
		var25 = Tile.calmWater.id;

		for(var30 = 0; var30 < this.width; ++var30) {
			var28 += this.floodFill(var30, this.depth / 2 - 1, 0, 0, var25);
			var28 += this.floodFill(var30, this.depth / 2 - 1, this.height - 1, 0, var25);
		}

		for(var30 = 0; var30 < this.height; ++var30) {
			var28 += this.floodFill(0, this.depth / 2 - 1, var30, 0, var25);
			var28 += this.floodFill(this.width - 1, this.depth / 2 - 1, var30, 0, var25);
		}

		long var32 = System.nanoTime();
     */
    this->fastCraft->showLoadingScreen("Generating level", "Melting..");
        /*var25 = 0;

		for(var29 = 0; var29 < 400; ++var29) {
			int var34 = this.random.nextInt(this.width);
			int var36 = this.random.nextInt(this.depth / 2);
			int var38 = this.random.nextInt(this.height);
			if(this.getTile(var34, var36, var38) == 0) {
				++var25;
				var28 += this.floodFill(var34, var36, var38, 0, Tile.calmLava.id);
			}
		}

		System.out.println("LavaCount: " + var25);
		System.out.println("Flood filled " + var28 + " tiles in " + (double)(var32 - var26) / 1000000.0D + " ms");
		this.calculateLightDepths(0, 0, this.width, this.height);

		for(var29 = 0; var29 < this.levelListeners.size(); ++var29) {
			((LevelRenderer)this.levelListeners.get(var29)).resetChunks();
		}
     */
    for(var29 = 0; var29 < this->levelListeners.size(); ++var29) {
        (this->levelListeners.at(var29))->resetChunks();
    }

}

bool Level::load()
{
    try
    {
        // Read the file
        std::ifstream file("level.dat", std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Unable to open file for reading.");
        }

        std::vector<char> compressedData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Decompress
        uLongf decompressedSize = this->blocks.size() * sizeof(int);
        if (uncompress(reinterpret_cast<Bytef *>(this->blocks.data()), &decompressedSize,
                       reinterpret_cast<const Bytef *>(compressedData.data()), compressedData.size()) != Z_OK)
        {
            throw std::runtime_error("Failed to decompress block data.");
        }

        if (decompressedSize != this->blocks.size() * sizeof(int))
        {
            throw std::runtime_error("Decompressed data size mismatch.");
        }

        // Recalculate lighting for the entire level
        calcLightDepths(0, 0, this->width, this->depth);

        for(int var3 = 0; var3 < this->levelListeners.size(); ++var3) {
            (this->levelListeners.at(var3))->resetChunks();
        }

        std::cout << "Level loaded successfully." << std::endl;
        return true;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error loading level: " << ex.what() << std::endl;
        return false;
    }
}

void Level::save()
{
    try
    {
        // Compress the entire block data
        uLongf compressedSize = compressBound(this->blocks.size() * sizeof(int));
        std::vector<char> compressedData(compressedSize);

        if (compress(reinterpret_cast<Bytef *>(compressedData.data()), &compressedSize,
                     reinterpret_cast<const Bytef *>(this->blocks.data()), this->blocks.size() * sizeof(int)) != Z_OK)
        {
            throw std::runtime_error("Failed to compress block data.");
        }
        compressedData.resize(compressedSize); // Adjust size to actual compressed data

        // Save to file
        std::ofstream file("level.dat", std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Unable to open file for writing.");
        }

        file.write(compressedData.data(), compressedData.size());
        file.close();
        std::cout << "Level saved successfully." << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error saving level: " << ex.what() << std::endl;
    }
}

void Level::calcLightDepths(int x, int y, int width, int height)
{
    for(int var5 = x; var5 < x + width; ++var5) {
        for(int var6 = y; var6 < y + height; ++var6) {
            int var7 = this->lightDepths[var5 + var6 * this->width];

            int var8;
            for(var8 = this->depth - 1; var8 > 0; --var8) {
                Tile* var14 = Tile::tiles[this->getTile(var5, var8, var6)];
                if(var14 == nullptr ? false : var14->blocksLight()) {
                    break;
                }
            }

            this->lightDepths[var5 + var6 * this->width] = var8 + 1;
            if(var7 != var8) {
                int var9 = var7 < var8 ? var7 : var8;
                var7 = var7 > var8 ? var7 : var8;

                for(var8 = 0; var8 < this->levelListeners.size(); ++var8) {
                    LevelRenderer *var10 = this->levelListeners.at(var8);
                    var10->setDirty(var5 - 1, var9 - 1, var6 - 1, var5 + 1, var7 + 1, var6 + 1);
                }
            }
        }
    }

}

bool Level::setTile(int x, int y, int z, int tileId)
{
    if(x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height) {
        if(tileId == this->blocks[(y * this->height + z) * this->width + x]) {
            return false;
        } else {
            this->blocks[(y * this->height + z) * this->width + x] = tileId;
            this->updateNeighbourAt(x - 1, y, z, tileId);
            this->updateNeighbourAt(x + 1, y, z, tileId);
            this->updateNeighbourAt(x, y - 1, z, tileId);
            this->updateNeighbourAt(x, y + 1, z, tileId);
            this->updateNeighbourAt(x, y, z - 1, tileId);
            this->updateNeighbourAt(x, y, z + 1, tileId);
            this->calculateLightDepths(x, z, 1, 1);

            for(tileId = 0; tileId < this->levelListeners.size(); ++tileId) {
                LevelRenderer* var5 = this->levelListeners.at(tileId);
                var5->setDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
            }

            return true;
        }
    } else {
        return false;
    }
}

void Level::calculateLightDepths(int x, int y, int width, int height) {
    for(int var5 = x; var5 < x + width; ++var5) {
        for(int var6 = y; var6 < y + height; ++var6) {
            int var7 = this->lightDepths[var5 + var6 * this->width];

            int var8;
            for(var8 = this->depth - 1; var8 > 0; --var8) {
                Tile* var14 = Tile::tiles[this->getTile(var5, var8, var6)];
                if(var14 == nullptr ? false : var14->blocksLight()) {
                    break;
                }
            }

            this->lightDepths[var5 + var6 * this->width] = var8 + 1;
            if(var7 != var8) {
                int var9 = var7 < var8 ? var7 : var8;
                var7 = var7 > var8 ? var7 : var8;

                for(var8 = 0; var8 < this->levelListeners.size(); ++var8) {
                    LevelRenderer* var10 = this->levelListeners.at(var8);
                    var10->setDirty(var5 - 1, var9 - 1, var6 - 1, var5 + 1, var7 + 1, var6 + 1);
                }
            }
        }
    }

}

bool Level::setTileNoUpdate(int x, int y, int z, int tileId){
    if(x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height) {
        if (tileId == this->blocks[(y * this->height + z) * this->width + x]) {
            return false;
        }else{
            this->blocks[(y * this->height + z) * this->width + x] = tileId;
            return true;
        }
    }
    else
    {
        return false;
    }
}

void Level::updateNeighbourAt(int x, int y, int z, int tileId){
    if(x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height) {
        Tile* tile = Tile::tiles[this->blocks[(y*this->height + z)*this->width + x]];
        if(tile != nullptr){
            tile->neighborChanged(this, x, y, z, tileId);
        }

    }
}

bool Level::isLit(int x, int y, int z)
{
    return x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height ? y >= this->lightDepths[x + z * this->width] : true;
}

int Level::getTile(int x, int y, int z)
{
    return x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height ? this->blocks[(y * this->height + z) * this->width + x] : 0;
}

bool Level::containsLiquid(AABB* aabb, int type){
    int x0 = (int)std::floor((double)aabb->x0);
    int x1 = (int)std::floor((double)aabb->x1 + 1.0F);
    int y0 = (int)std::floor((double)aabb->y0);
    int y1 = (int)std::floor((double)aabb->y1 + 1.0F);
    int z0 = (int)std::floor((double)aabb->z0);
    int z1 = (int)std::floor((double)aabb->z1 + 1.0F);

    if(x0 < 0) {
        x0 = 0;
    }

    if(y0 < 0) {
        y0 = 0;
    }

    if(z0 < 0) {
        z0 = 0;
    }

    if(x1 > this->width) {
        x1 = this->width;
    }

    if(y1 > this->depth) {
        y1 = this->depth;
    }

    if(z1 > this->height) {
        z1 = this->height;
    }

    for(x0 = x0; x0 < x1; ++x0) {
        for(int var8 = y0; var8 < y1; ++var8) {
            for(int var9 = z0; var9 < z1; ++var9) {
                Tile* tile = Tile::tiles[this->getTile(x0, var8, var9)];
                if(tile != nullptr && tile->getLiquidType() == type) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Level::isLightBlocker(int x, int y, int z)
{
    Tile *tile = Tile::tiles[getTile(x, y, z)];
    return tile == nullptr ? false : tile->blocksLight();
}

std::vector<AABB> Level::getCubes(const AABB &aabb)
{
    std::vector<AABB> cubes;
    int minX = aabb.x0;
    int maxX = aabb.x1 + 1.0f;
    int minY = aabb.y0;
    int maxY = aabb.y1 + 1.0f;
    int minZ = aabb.z0;
    int maxZ = aabb.z1 + 1.0f;

    minX = std::max(minX, 0);
    minY = std::max(minY, 0);
    minZ = std::max(minZ, 0);
    maxX = std::min(maxX, this->width);
    maxY = std::min(maxY, this->depth);
    maxZ = std::min(maxZ, this->height);

    for (int x = minX; x < maxX; ++x)
    {
        for (int y = minY; y < maxY; ++y)
        {
            for (int z = minZ; z < maxZ; ++z)
            {
                Tile *tile = Tile::tiles[this->getTile(x, y, z)];
                if (tile != nullptr)
                {
                    AABB *aabb = tile->getBoundingBox(x, y, z);
                    if (aabb != nullptr)
                    {
                        cubes.emplace_back(*aabb);
                    }
                }
            }
        }
    }

    return cubes;
}

bool Level::isSolidTile(int x, int y, int z)
{
    Tile *tile = Tile::tiles[this->getTile(x, y, z)];
    return tile == nullptr ? false : tile->isSolid();
}

void Level::tick()
{
    this->unprocessed += this->width * this->height * this->depth;
    int var1 = this->unprocessed / 400;
    this->unprocessed -= var1 * 400;

    std::uniform_int_distribution<int> widthDist(0, this->width - 1);
    std::uniform_int_distribution<int> depthDist(0, this->depth - 1);
    std::uniform_int_distribution<int> heightDist(0, this->height - 1);

    for (int var2 = 0; var2 < var1; ++var2)
    {
        int var3 = widthDist(this->random);
        int var4 = depthDist(this->random);
        int var5 = heightDist(this->random);

        Tile *var6 = Tile::tiles[this->getTile(var3, var4, var5)];
        if (var6 != nullptr)
        {

            std::default_random_engine defaultRandom(this->random());
            var6->tick(this, var3, var4, var5, defaultRandom);
        }
    }
}

long Level::floodFill(int var1, int var2, int var3, int var4, int var5) {
    int var18 = var5;
    std::vector<std::vector<int>> var19;
    int var6 = 0;
    int var7 = this->height - 1;
    int var8 = this->width - 1;
    int var20 = var6 + 1;
    this->coords[0] = ((var2 << 8) + var3 << 8) + var1;
    long var11 = 0L;
    var1 = this->width * this->height;

    while (var20 > 0) {
        --var20;
        var2 = this->coords[var20];
        if (var20 == 0 && !var19.empty()) {
            std::cout << "IT HAPPENED!" << std::endl;
            this->coords = var19.back();
            var19.pop_back();
            var20 = this->coords.size();
        }

        var3 = var2 >> 8 & var7;
        int var9 = var2 >> 16;
        int var10 = var2 & var8;

        int var13;
        for (var13 = var10; var10 > 0 && this->blocks[var2 - 1] == 0; --var2) {
            --var10;
        }

        while (var13 < this->width && this->blocks[var2 + var13 - var10] == 0) {
            ++var13;
        }

        int var14 = var2 >> 8 & var7;
        int var15 = var2 >> 16;
        if (var14 != var3 || var15 != var9) {
            std::cout << "hoooly fuck" << std::endl;
        }

        bool var21 = false;
        bool var22 = false;
        bool var16 = false;
        var11 += (var13 - var10);

        for (var10 = var10; var10 < var13; ++var10) {
            this->blocks[var2] = var18;
            bool var17;
            if (var3 > 0) {
                var17 = this->blocks[var2 - this->width] == 0;
                if (var17 && !var21) {
                    if (var20 == this->coords.size()) {
                        var19.push_back(this->coords);
                        this->coords = std::vector<int>(1048576);
                        var20 = 0;
                    }

                    this->coords[var20++] = var2 - this->width;
                }

                var21 = var17;
            }

            if (var3 < this->height - 1) {
                var17 = this->blocks[var2 + this->width] == 0;
                if (var17 && !var22) {
                    if (var20 == this->coords.size()) {
                        var19.push_back(this->coords);
                        this->coords = std::vector<int>(1048576);
                        var20 = 0;
                    }

                    this->coords[var20++] = var2 + this->width;
                }

                var22 = var17;
            }

            if (var9 > 0) {
                var17 = this->blocks[var2 - var1] == 0;
                if (var17 && !var16) {
                    if (var20 == this->coords.size()) {
                        var19.push_back(this->coords);
                        this->coords = std::vector<int>(1048576);
                        var20 = 0;
                    }

                    this->coords[var20++] = var2 - var1;
                }

                var16 = var17;
            }

            ++var2;
        }
    }
    return var11;
}