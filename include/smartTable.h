#ifndef SMARTTABLE_H
#define SMARTTABLE_H

typedef struct pos
{
    int x=-1;
    int y=-1;
} pos;

enum directions
{
    up,
    down,
    left,
    right
};

bool operator== (SDL_Color color1,SDL_Color color2)
{
    if (color1.a != color2.a) return false;
    if (color1.b != color2.b) return false;
    if (color1.r != color2.r) return false;
    if (color1.g != color2.g) return false;
    return true;
}

const int verticalTableSize=64;
const int horizontalTableSize=64;

class smartTable
{
private:
    Cell** cells;
    int xCells;
    int yCells;
    int xPos;
    int yPos;

    pos highlightedCell;

public:

    smartTable ()
    {
        xCells=horizontalTableSize;
        yCells=verticalTableSize;
        cells = new Cell*[verticalTableSize];
        for (int i=0; i<verticalTableSize; i++)
        {
            cells[i] = new Cell[horizontalTableSize];
        }
    }
//    //not tested
//    smartTable (int numberOfHorizontalCells, int numberOfVerticalCells, SDL_Rect rectPair, SDL_Color lineColor, SDL_Color bColor, int posx=0, int posy=0)
//    {
//        cells= calloc(sizeof(Cell*),numberOfVerticalCells);
//        for (int i=0; i<numberOfVerticalCells; i++)
//        {
//            cells[i].resize(numberOfHorizontalCells);
//            for (int k=0; k<numberOfHorizontalCells; k++)
//            {
//                cells[i][k].setPos(rectPair.h*i, rectPair.w*k);
//                cells[i][k].rect.h=rectPair.h;
//                cells[i][k].rect.w=rectPair.w;
//                cells[i][k].outlineColor=lineColor;
//                cells[i][k].fillColor=bColor;
//            }
//        }
//        x=posx;
//        y=posy;
//    }

    smartTable (int numberOfHorizontalCells, int numberOfVerticalCells, Cell* sampleCell, int x=0, int y=0)
    {
        xPos=x;
        yPos=y;
        xCells=numberOfHorizontalCells;
        yCells=numberOfVerticalCells;
        cells = new Cell*[numberOfVerticalCells];
        for (int i=0; i<numberOfVerticalCells; i++)
        {
            cells[i] = new Cell[numberOfHorizontalCells];
            for (int k=0; k<numberOfHorizontalCells; k++)
            {
                cells[i][k].rect.x=(sampleCell->rect.w)*i+xPos;
                cells[i][k].rect.y=(sampleCell->rect.h)*k+yPos;
                cells[i][k].rect.h=sampleCell->rect.h;
                cells[i][k].rect.w=sampleCell->rect.w;
                cells[i][k].outlineColor=sampleCell->outlineColor;
                cells[i][k].fillColor=sampleCell->fillColor;
                cells[i][k].texture=sampleCell->texture;
            }
        }
    }
    ~smartTable ()
    {
        for (int i=0; i<yCells; i++)
        {
            delete[] cells[i];
        }
        delete[] cells;
    }


//TODO
    smartTable (const smartTable &to)
    {

        std::cout<<"asdaSD";
//        cells = new Cell*[numberOfVerticalCells];
//        for (int i=0; i<numberOfVerticalCells; i++)
//        {
//            cells[i] = new Cell[numberOfHorizontalCells];
//        }
//        for (int i=0; i<verticalTableSize; i++)
//        {
//            for (int k=0; k<horizontalTableSize; k++)
//            {
//                to[i][k]=cells[i][k];
//            }
//        }
    }


    void operator= (const smartTable& to)
    {
        for (int i=0; i<verticalTableSize; i++)
        {
            for (int k=0; k<horizontalTableSize; k++)
            {
                cells[i][k]=to.cells[i][k];
            }
        }
        this->highlightedCell=to.highlightedCell;
        xCells=to.xCells;
        yCells=to.yCells;
        xPos=to.xPos;
        yPos=to.yPos;
//        return *this;
    }


    void draw(SDL_Renderer* renderer)
    {
        SDL_Rect outlines;
        outlines.w=yCells*cells[0][0].rect.w+2;
        outlines.h=xCells*cells[0][0].rect.h+2;
        outlines.x=cells[0][0].rect.x-1;
        outlines.y=cells[0][0].rect.y-1;

        SDL_SetRenderDrawColor(renderer, cells[0][0].outlineColor.r, cells[0][0].outlineColor.g, cells[0][0].outlineColor.b, cells[0][0].outlineColor.a);
        SDL_RenderDrawRect( renderer, &outlines );

        for (int i=0; i<yCells; i++)
        {
            for (int k=0; k<xCells; k++)
            {
                cells[i][k].draw(renderer);
            }
        }
    }

    int  colorCell(SDL_Color outline, SDL_Color fill, int y, int x)
    {
        if (x>-1 && x<xCells && y>-1 && y<yCells)
        {
            cells[y][x].color(outline,fill);
            return 1;
        }
        return 0;
    }
    SDL_Color getCellColor (pos cellPos)
    {
        return cells[cellPos.y][cellPos.x].getFillColor();
    }
    void deHighLight ()
    {
        if (highlightedCell.x>-1 && highlightedCell.y>-1) cells[highlightedCell.y][highlightedCell.x].highlight();
        highlightedCell.x=-1;
        highlightedCell.y=-1;
    }

    int highlightCell (int x, int y)
    {
        if (highlightedCell.x>-1 && highlightedCell.y>-1) cells[highlightedCell.y][highlightedCell.x].highlight();
        if (x>-1 && x<xCells && y>-1 && y<yCells)
        {

            if(cells[y][x].highlight())
            {
                highlightedCell.x=x;
                highlightedCell.y=y;
            }
            else
            {
                highlightedCell.x=-1;
                highlightedCell.y=-1;
            }
            return 1;
        }
        else
        {
            return 0;
        }
    }

    pos getHighlightedCell ()
    {
        return highlightedCell;
    }

    int moveHighlightedCell(directions direction)
    {
        if (highlightedCell.x>-1)
        {
            switch (direction)
            {
            case up:
                if (highlightedCell.x>0) highlightCell(highlightedCell.x-1,highlightedCell.y);
                break;
            case down:
                if (highlightedCell.x<verticalTableSize-1) highlightCell(highlightedCell.x+1,highlightedCell.y);
                break;
            case left:
                if (highlightedCell.y>0) highlightCell(highlightedCell.x,highlightedCell.y-1);
                break;
            case right:
                if (highlightedCell.y<horizontalTableSize-1) highlightCell(highlightedCell.x,highlightedCell.y+1);
                break;
            }
        }

    }


    int checkNeighbors (pos toCheck)
    {
        int nNumb=0;
        for (int l=-1; l<2; l+=2)
        {
            if(cells[toCheck.y+l][toCheck.x+l].getFillColor()==defaultLiveColor) nNumb++;   // jobbra fel, balra le
            if(cells[toCheck.y][toCheck.x+l].getFillColor()==defaultLiveColor) nNumb++;     // jobbra balra
            if(cells[toCheck.y+l][toCheck.x].getFillColor()==defaultLiveColor) nNumb++;     // fel le
            if(cells[toCheck.y-l][toCheck.x+l].getFillColor()==defaultLiveColor) nNumb++;   // balra fel jobbra le
        }
        return nNumb;

    }


    void executeRound ()
    {
        smartTable* temp=new smartTable;

        *temp=*this;



        pos currentPos;
        for (int i=1; i<verticalTableSize-1; i++)
        {
            for (int k=1; k<horizontalTableSize-1; k++)
            {
                currentPos.y=i;
                currentPos.x=k;
                if (getCellColor(currentPos)==defaultLiveColor)
                {
//                    if ((checkNeighbors(currentPos)==2) || (checkNeighbors(currentPos)==3)) temp->colorCell(defaultLineColor,defaultLiveColor,i,k);
                    if ((checkNeighbors(currentPos))<2 || (checkNeighbors(currentPos))>3) temp->colorCell(defaultLineColor,defaultDeadColor,i,k);
                }
                else
                {
//                    if(getCellColor(currentPos)==defaultDeadColor)
//                    {
                    if(checkNeighbors(currentPos)==3) temp->colorCell(defaultLineColor,defaultLiveColor,i,k);
//                    }
                }

            }
        }
        (*this)=*temp;

        delete temp;
    }
};

#endif // SMARTTABLE_H
