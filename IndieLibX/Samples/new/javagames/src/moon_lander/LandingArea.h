/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

/**
 * Landing area where rocket will land.
 * 
 * @author www.gametutorial.net
 */

 struct LandingArea {
    
    /**
     * X coordinate of the landing area.
     */
     int x;
    /**
     * Y coordinate of the landing area.
     */
     int y;
    
    /**
     * Image of landing area.
     */
     Image3 landingAreaImg;
    
    /**
     * Width of landing area.
     */
     int landingAreaImgWidth;
    
    
     LandingArea()
    {
        Initialize();
        LoadContent();
    }
    
    
     void Initialize()
    {   
        // X coordinate of the landing area is at 46% frame width.
        x = (int)(Framework.frameWidth * 0.46);
        // Y coordinate of the landing area is at 86% frame height.
        y = (int)(Framework.frameHeight * 0.88);
    }
    
     void LoadContent()
    {
        try
        {
            URL landingAreaImgUrl = IRenderer::GetRendererInstance()->addImageLibTexture("/moon_lander/landing_area.png");
            landingAreaImg = ImageIO.read(landingAreaImgUrl);
            landingAreaImgWidth = landingAreaImg.getWidth();
        }
        catch (IOException ex) {
            Logger.getLogger(LandingArea.struct.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    
     void Draw(Graphics2D g2d)
    {
        g2d.drawImage(landingAreaImg, x, y, null);
    }
    
}
#endif

