#ifndef CELL_H
#define CELL_H

const SDL_Color defaultBackgroundColor= {0xFF, 0xFF, 0xFF, 0xFF};
const SDL_Color defaultLineColor= {0xFF, 0xFF, 0xFF, 0xFF};
const SDL_Color defaultHighlightColor= {0xFF, 0x22, 0x00, 0xFF};
const SDL_Color defaultLiveColor= {0x00, 0x00, 0x00, 0xFF};
const SDL_Color defaultDeadColor= {0xFF, 0xFF, 0xFF, 0xFF};



class Cell
{
    friend class smartTable;
private:
    SDL_Rect rect;
    SDL_Color outlineColor;
    SDL_Color fillColor;
    SDL_Texture* texture;
    bool highlighted=false;
public:
    Cell(SDL_Color oColor, SDL_Color fColor, int width, int height,int x=0, int y=0)
    {
        rect.h=height;
        rect.w=width;
        rect.x=x;
        rect.y=y;
        outlineColor=oColor;
        fillColor=fColor;
        texture=NULL;
        highlighted=false;
    }
    Cell(SDL_Texture *texturePair, int width, int height,int x=0, int y=0)
    {
        rect.h=height;
        rect.w=width;
        rect.x=x;
        rect.y=y;
        texture=texturePair;
        outlineColor=defaultLineColor;
        fillColor=defaultBackgroundColor;
    }
    Cell(SDL_Rect cell, SDL_Color oColor, SDL_Color fColor)
    {
        rect=cell;
        outlineColor=oColor;
        fillColor=fColor;
    }
    Cell(SDL_Rect cell, SDL_Texture* texturePair)
    {
        rect=cell;
        texture=texturePair;
        outlineColor=defaultLineColor;
        fillColor=defaultBackgroundColor;
    }
    Cell()
    {
        rect.h=80;
        rect.w=40;
        outlineColor=defaultLineColor;
        fillColor=defaultBackgroundColor;
        texture = NULL;
    }

    Cell operator= (const Cell &to)
    {
        rect.h=to.rect.h;
        rect.w=to.rect.w;
        rect.x=to.rect.x;
        rect.y=to.rect.y;

        fillColor=to.fillColor;
        outlineColor=to.outlineColor;
        highlighted=to.highlighted;

        texture=to.texture;
    }
    void setPos (int posx, int posy)
    {
        rect.x=posx;
        rect.y=posy;
    }
    int getxPos ()
    {
        return rect.x;
    }
    int getyPos ()
    {
        return rect.y;
    }
    void draw (SDL_Renderer *renderer)
    {

        if(texture!=NULL)
        {
            //Render texture to screen
            SDL_RenderCopy( renderer, texture, NULL, NULL );

        }
        else
        {
            SDL_SetRenderDrawColor( renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a );
            SDL_RenderFillRect( renderer, &rect );
            SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
            SDL_RenderDrawRect( renderer, &rect );
        }
        if (highlighted)
        {
            SDL_Rect highlightRect (rect);
            highlightRect.h-=2;
            highlightRect.w-=2;
            highlightRect.x++;
            highlightRect.y++;
            SDL_SetRenderDrawColor(renderer, defaultHighlightColor.r, defaultHighlightColor.g, defaultHighlightColor.b, defaultHighlightColor.a);
            SDL_RenderDrawRect( renderer, &highlightRect);
        }
    }
    void color (SDL_Color outLine, SDL_Color fill)
    {
        this->outlineColor=outLine;
        this->fillColor=fill;
    }
    SDL_Color getFillColor ()
    {
        return this->fillColor;
    }
    bool highlight ()
    {
        if (highlighted)
        {
            highlighted=false;
        }
        else
        {
            highlighted=true;
        }
        return highlighted;
    }
};

#endif // CELL_H
