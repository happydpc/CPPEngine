#ifndef PORTAL_H
#define PORTAL_H

class Portal
{
  public:
    Portal();
   ~Portal();

    bool setup(const HeightMap &hMap,
               const D3DXFROMWINEVECTOR2   &offset,
               const Image3     *random,
			         const Image3     *coverage);

    TerrainCell &getTerrainCell();
    GrassCell   &getGrassCell();
    void         setVisiblePixelsCount(unsigned int visible);

    const BoundsDescriptor &getAABB()               const;
    const unsigned int      getVisiblePixelsCount() const;
    const unsigned int      getOcclusionID()        const;
    const bool              render(int token);

    const bool              startOcclusionQuery();
    const bool              endOcclusionQuery();

    enum RenderTokens
    {
      AABB       = 0x1,
      GRASS      = 0x2,
      TERRAIN    = 0x4,
      OCCLUSIONS = 0x8
    };

  private:
    BoundsDescriptor aabb;
    unsigned int     visiblePixelsCount,
                     occlusionID;
    TerrainCell      tCell;
    GrassCell        gCell;
};  

#endif
