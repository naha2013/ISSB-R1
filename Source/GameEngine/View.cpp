
#include "View.h"
#include "Application.h"
#include "Output/Console.h"
#include <math.h>
#include <cmath>

namespace GameEngine
{
	float View::x = 0;
	float View::y = 0;
	
	float View::Zoom = 1.0;

	int View::oldWidth = 480;
	int View::oldHeight = 320;
	
	int View::windowWidth = 480;
	int View::windowHeight = 320;
	
	int View::scaleWidth = 480;
	int View::scaleHeight = 320;
	
	float View::multScale = 1;
	float View::letterBoxW = 0;
	float View::letterBoxH = 0;

	void View::setSize(int w, int h)
	{
		windowWidth = w;
		windowHeight = h;
		if(Application::window != NULL)
		{
			SDL_SetWindowSize(Application::window, w, h);
		}
	}
	
	void View::setScaleSize(int w, int h)
	{
		scaleWidth=w;
		scaleHeight=h;
	}
	
	int View::getWidth()
	{
		return windowWidth;
	}
	
	int View::getHeight()
	{
		return windowHeight;
	}
	
	int View::getScalingWidth()
	{
		return scaleWidth;
	}
	
	int View::getScalingHeight()
	{
		return scaleHeight;
	}
	
	void View::Update(Graphics2D&g)
	{
		multScale = 1;
		if(Application::scalescreen)
		{
			float ratX = (float)windowWidth/(float)scaleWidth;
			float ratY = (float)windowHeight/(float)scaleHeight;
			if(ratX<ratY)
			{
				multScale = ratX;
			}
			else
			{
				multScale = ratY;
			}
		}
		g.scale(Zoom*multScale,Zoom*multScale);
		float difX;
		float difY;
		if(Application::scalescreen)
		{
			difX = (float)((windowWidth - (windowWidth*Zoom))+(windowWidth - (scaleWidth*multScale)))/(float)(2*Zoom*multScale);
			difY = (float)((windowHeight - (windowHeight*Zoom))+(windowHeight - (scaleHeight*multScale)))/(float)(2*Zoom*multScale);
			letterBoxW = (float)std::abs((windowWidth - (scaleWidth*multScale))/2);
			letterBoxH = (float)std::abs((windowHeight - (scaleHeight*multScale))/2);
		}
		else
		{
			difX = (float)(windowWidth - (windowWidth*Zoom))/(float)(2*Zoom);
			difY = (float)(windowHeight - (windowHeight*Zoom))/(float)(2*Zoom);
			letterBoxW = (float)std::abs((windowWidth - (scaleWidth*multScale))/2);
			letterBoxH = (float)std::abs((windowHeight - (scaleHeight*multScale))/2);
		}
		g.translate(difX,difY);
	}
	
	void View::Draw(Graphics2D&g)
	{
		g.setScale(1,1);
		g.setTranslation(0,0);
		if(Application::scalescreen)
		{
			g.setColor(Color::BLACK);
			g.setFont(g.defaultFont);
			if(letterBoxW>0)
			{
				g.fillRect(0,0,letterBoxW,(float)windowHeight);
				g.fillRect((((float)windowWidth)-letterBoxW),0,letterBoxW,(float)windowHeight);
			}
			if(letterBoxH>0)
			{
				g.fillRect(0,0,(float)windowWidth,letterBoxH);
				g.fillRect(0,((float)windowHeight-letterBoxH),(float)windowWidth,letterBoxH);
			}
			
			if(Application::showfps)
			{
				g.setColor(Color::WHITE);
				g.drawString((String)Application::realFPS + (String)" fps", (letterBoxW+(40*multScale)), (letterBoxH+(80*multScale)));
			}
		}
		else
		{
			if(Application::showfps)
			{
				g.setColor(Color::BLACK);
				g.drawString((String)Application::realFPS + (String)" fps", 60, 50);
			}
		}
	}
}
