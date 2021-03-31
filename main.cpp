#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include<stdlib.h>
#include<time.h>
#include <windows.h>
#include <mmsystem.h>
using namespace std;

bool generat=0;
int oX,oY,L;
int failed=1;
int culori[]={0,1,2,3,4,5,6,7,8},StyleBila=1,CulBila=13,CulTabla=0,CulJoc=0;

struct bila {
int x,y,color,piese;
};

struct directie {
bool sus,jos,st,dr;
};

struct undo {
int x,y;
};
undo valoriUndo;
directie dir[8];
bila v[8];
int poz[9][9],cul[9][9],piese,NrSpatiiLibere=64;
char spatiilibere[100];
bool stanga,dreapta,down,up;
void userInput();
void start();
void updateTabla(int culoare);
void undo(int mod);
void UpdateNumar(int optiune);
void menu();
void desenTabla()
{

    setcolor(CulJoc);
    setfillstyle(SOLID_FILL, CulJoc);
    floodfill(1,1,CulJoc);
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    setbkcolor(WHITE);
    outtextxy(oX+10*L,oY-70,"Spatii libere:");
    outtextxy(oX+9*L,oY-30,"Pune bilele:");
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    circle(oX+11*L+110,oY-63,L/3+2);
    floodfill(oX+11*L+105,oY-63,WHITE);
    itoa(NrSpatiiLibere,spatiilibere,10);
    setcolor(BLACK);
    outtextxy(oX+11*L+100,oY-70,spatiilibere);
    setcolor(CulTabla);
    setfillstyle(SOLID_FILL,CulTabla);
    rectangle(oX,oY,oX+8*L,oY+8*L);
    floodfill(oX+1,oY+1,CulTabla);
    setcolor(WHITE);
    setbkcolor(BLACK);
    settextstyle(1, HORIZ_DIR,2);
    outtextxy(40,750,"*daca culorile nu au fost alese diferit, acestea");
    outtextxy(40,775," pot sa va incurce la rezolvarea jocului!");
    setcolor(WHITE);
    setbkcolor(WHITE);
    settextstyle(0, HORIZ_DIR,0);
    int i,j;
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
            rectangle(oX+j*L,oY+i*L,oX+j*L+L,oY+i*L+L);
    for(i=1;i<=8;i++)
    {
        v[i].x=0;
        v[i].y=0;
        for(j=1;j<=8;j++)
            {poz[i][j]=0;cul[i][j]=0;}
    }
    for(i=0;i<8;i++)
        v[i].piese=0;
}
void bileRandom()
{
    int i,a,b;
     srand(time(NULL));
    for(i=0;i<8;i++)
    {
        a=rand()%8+1;
        b=rand()%8+1;
        v[i].x=a;
        v[i].y=b;
        poz[a][b]++;
        if(poz[a][b]>1)
        {
            i--;
            poz[a][b]=1;
        }
        else
        {
            cul[a][b]=culori[i+1];
        }
    }
    setbkcolor(CulTabla);
    for(i=0;i<8;i++)
    {
        setcolor(CulBila);
        setfillstyle(StyleBila,CulBila);
        circle(oX+v[i].x*L-L/2,oY+v[i].y*L-L/2,L/3);
        floodfill(oX+v[i].x*L-L/2,oY+v[i].y*L-L/2,CulBila);
    }
    setbkcolor(CulJoc);
    setcolor(CulJoc);
    outtextxy(oX+9*L,oY-30,"Pune bilele:");
    setcolor(CulBila);
    setbkcolor(CulTabla);
}
void butoane(int val)
{
    int X;
    X=oX+9*L;
    setcolor(WHITE);
    setbkcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    rectangle(10,25,170,55);
    floodfill(11,26,WHITE);
    setcolor(BLACK);
    outtextxy(17,31,"Meniu principal");
    if(val==1)
        {

            setcolor(WHITE);
            setfillstyle(SOLID_FILL,WHITE);
            rectangle(X,oY,X+100,oY+50);
            rectangle(X,oY+100,X+100,oY+150);
            floodfill(X+1,oY+1,WHITE);
            floodfill(X+1,oY+101,WHITE);
            setbkcolor(WHITE);
            outtextxy(X+20,oY+18,"Reset");
            setcolor(BLACK);
            outtextxy(X+20,oY+18,"Random");
            outtextxy(X+10,oY+118,"Specific");
        }
    if(val==0)
        {
            setcolor(CulJoc);
            setfillstyle(SOLID_FILL,CulJoc);
            rectangle(X,oY+100,X+100,oY+150);
            floodfill(X+1,oY+101,CulJoc);
            setcolor(WHITE);
            setfillstyle(SOLID_FILL,WHITE);
            rectangle(X,oY,X+100,oY+50);
            floodfill(X+1,oY+1,WHITE);
            setbkcolor(WHITE);
            outtextxy(X+20,oY+18,"Random");
            setbkcolor(WHITE);
            setcolor(BLACK);
            outtextxy(X+20,oY+18,"Reset");


        }
}
void directii(int x,int y)
{
    failed=1;
    if(oX+(x-1)*L-L>=oX and poz[x-1][y]!=1)
    {
        line(oX+(x-1)*L-L/2+5,oY+y*L-L/2,oX+(x-1)*L+1,oY+y*L-L/2);
        stanga=1;
        poz[x-1][y]=2;
        failed=0;
    }
    if(oX+(x+1)*L-L<oX+8*L and poz[x+1][y]!=1)
    {
        line(oX+x*L-1,oY+y*L-L/2,oX+x*L+L/2-5,oY+y*L-L/2);
        dreapta=1;
        poz[x+1][y]=2;
        failed=0;
    }
    if(oY+(y-1)*L-L>=oY and poz[x][y-1]!=1)
    {
        line(oX+x*L-L/2,oY+(y-1)*L-L/2+5,oX+x*L-L/2,oY+(y-1)*L+1);
        up=1;
        poz[x][y-1]=2;
        failed=0;
    }
    if(oY+y*L<oY+8*L and poz[x][y+1]!=1)
    {
        line(oX+x*L-L/2,oY+y*L+L/2-5,oX+x*L-L/2,oY+y*L-1);
        down=1;
        poz[x][y+1]=2;
        failed=0;
    }
    if(failed==1)
        failed=2;
}
void completeaza (int X,int Y,int culoare,int nrBila)
{
    int x,y;
    stanga=0;dreapta=0;up=0;down=0;
            if(ismouseclick(WM_LBUTTONDOWN) and piese<8 and v[nrBila].piese<7)
            {
                        clearmouseclick(WM_LBUTTONDOWN);
                        x=mousex();
                        y=mousey();
                        if(!(x>=oX+9*L and x<=oX+9*L+100 and y>=oY+100 and y<=oY+150))
                        {
                            if(x>=10 and x<=170 and y>=25 and y<=55)
                            {
                                cleardevice();
                                setcolor(WHITE);
                                setbkcolor(WHITE);
                                menu();
                            }
                            else
                            {
                                x=(x-oX)/L+1;
                                y=(y-oY)/L+1;
                                if(poz[x][y]!=2)
                                {
                                            if(mousex()>=oX+9*L and mousex()<=oX+9*L+100 and mousey()>=oY and mousey()<=oY+50)
                                            {
                                                setcolor(BLACK);
                                                outtextxy(oX+9*L+5,oY+18,"Alta bila");
                                                setcolor(WHITE);
                                                setbkcolor(WHITE);
                                                outtextxy(oX+9*L+5,oY+18,"Alta bila");
                                                setcolor(BLACK);
                                                outtextxy(oX+9*L+20,oY+18,"Reset");
                                                return;
                                            }
                                            completeaza (X,Y,culoare,nrBila);
                                }
                                else
                                {
                                            setcolor(culoare);
                                            setfillstyle(SOLID_FILL,culoare);
                                            if(poz[x][y]==2)
                                                {
                                                    if((poz[x-1][y]==1 and cul[x-1][y]==culoare) or (poz[x+1][y]==1 and cul[x+1][y]==culoare) or (poz[x][y-1]==1 and cul[x][y-1]==culoare) or (poz[x][y+1]==1 and cul[x][y+1]==culoare))
                                                    {
                                                        undo(0);
                                                        setcolor(culoare);
                                                        setfillstyle(SOLID_FILL,culoare);
                                                        rectangle(oX+x*L-L+1,oY+y*L-L+1,oX+x*L-1,oY+y*L-1);
                                                        floodfill(oX+x*L-L+2,oY+y*L-L+2,culoare);
                                                        poz[x][y]=1;
                                                        cul[x][y]=culoare;
                                                        directii(x,y);
                                                        valoriUndo.x=x;
                                                        valoriUndo.y=y;
                                                        piese++;
                                                        v[nrBila].piese++;
                                                        X=x;
                                                        Y=y;
                                                        UpdateNumar(1);
                                                    }

                                                }
                                            completeaza(X,Y,culoare,nrBila);

                                }
                            }
                        }
                        else
                        {
                            UpdateNumar(0);
                            if(piese==1)
                            {
                                undo(1);
                                setbkcolor(WHITE);
                            setcolor(WHITE);
                            outtextxy(oX+9*L+5,oY+18,"Alta bila");
                            setcolor(BLACK);
                            setbkcolor(WHITE);
                            outtextxy(oX+9*L+20,oY+18,"Reset");
                            setbkcolor(CulJoc);
                            setcolor(CulJoc);
                            setfillstyle(SOLID_FILL,CulJoc);
                            rectangle(oX+9*L-1,oY+99,oX+9*L+101,oY+151);
                            floodfill(oX+9*L+1,oY+101,CulJoc);
                            return;
                            }
                            else
                            {
                                undo(2);
                                piese--;
                                v[nrBila].piese--;
                                completeaza(valoriUndo.x,valoriUndo.y,culoare,nrBila);
                            }
                        }
            }
            else
                {
                    if(piese<8 and v[nrBila].piese<7)
                    {
                        delay(2);
                        completeaza (X,Y,culoare,nrBila);
                    }
                    else
                    {
                        setcolor(BLACK);
                        outtextxy(oX+9*L+5,oY+18,"Alta bila");
                        setcolor(BLACK);
                        setbkcolor(WHITE);
                        outtextxy(oX+9*L+20,oY+18,"Reset");
                        updateTabla(culoare);
                        return;
                    }
                }

}
void UpdateNumar(int optiune)
{
    if(optiune==1)
        NrSpatiiLibere--;
    if(optiune==0)
        NrSpatiiLibere++;
    if(optiune==2)
        NrSpatiiLibere=64;
    setcolor(BLACK);
    setbkcolor(WHITE);
    itoa(NrSpatiiLibere,spatiilibere,10);
    if(NrSpatiiLibere<10)
    {
        outtextxy(oX+11*L+100,oY-70,"  ");
        outtextxy(oX+11*L+105,oY-70,spatiilibere);
    }
    else
    outtextxy(oX+11*L+100,oY-70,spatiilibere);
    setbkcolor(BLACK);

}
void updateTabla(int culoare)
{
    int i,j;
    cleardevice();
    setcolor(CulJoc);
    setfillstyle(SOLID_FILL, CulJoc);
    floodfill(1,1,CulJoc);
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    setbkcolor(WHITE);
    outtextxy(oX+10*L,oY-70,"Spatii libere:");
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    circle(oX+11*L+110,oY-63,L/3+2);
    floodfill(oX+11*L+110,oY-63,WHITE);
    UpdateNumar(3);
    setcolor(CulTabla);
    setfillstyle(SOLID_FILL,CulTabla);
    rectangle(oX,oY,oX+8*L,oY+8*L);
    floodfill(oX+1,oY+1,CulTabla);
    butoane(0);
    setcolor(WHITE);
    setbkcolor(BLACK);
    settextstyle(1, HORIZ_DIR,2);
    outtextxy(40,750,"*daca culorile nu au fost alese diferit, acestea");
    outtextxy(40,775," pot sa va incurce la rezolvarea jocului!");
    setcolor(WHITE);
    setbkcolor(WHITE);
    settextstyle(0, HORIZ_DIR,0);
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
            {
                setcolor(WHITE);
                rectangle(oX+j*L,oY+i*L,oX+j*L+L,oY+i*L+L);
            }

    for(i=1;i<9;i++)
    {
        for(j=1;j<9;j++)
        {
            if(poz[i][j]>0)
            {
                if(poz[i][j]==2)
                {
                    poz[i][j]=0;
                    setcolor(CulTabla);
                    setfillstyle(SOLID_FILL,CulTabla);
                    rectangle(oX+(i-1)*L+1,oY+(j-1)*L+1,oX+i*L-1,oY+j*L-1);
                    floodfill(oX+(i-1)*L+2,oY+(j-1)*L+2,CulTabla);
                }
                else
                {   if(cul[i-1][j]==cul[i][j] or cul[i+1][j]==cul[i][j] or cul[i][j-1]==cul[i][j] or cul[i][j+1]==cul[i][j])
                {
                    setcolor(cul[i][j]);
                    setfillstyle(SOLID_FILL,cul[i][j]);
                    rectangle(oX+(i-1)*L+1,oY+(j-1)*L+1,oX+i*L-1,oY+j*L-1);
                    floodfill(oX+(i-1)*L+2,oY+(j-1)*L+2,cul[i][j]);
                }
                }
            }
        }
    }
    setcolor(CulBila);
    setbkcolor(CulTabla);
    setfillstyle(StyleBila,CulBila);
    for(i=0;i<8;i++)
    {
        setcolor(CulBila);
        setbkcolor(CulTabla);
        setfillstyle(StyleBila,CulBila);
        circle(oX+v[i].x*L-L/2,oY+v[i].y*L-L/2,L/3);
        floodfill(oX+v[i].x*L-L/2,oY+v[i].y*L-L/2,CulBila);
    }
    int out=0;
    for(i=1;i<9;i++)
    {
        for(j=1;j<9;j++)
        {
            if(poz[i][j]!=1)
                {out=1;break;}
        }
        if(out) break;
        if(i==8 and out==0)
            {
                delay(1000);
                cleardevice();
                setcolor(WHITE);
                setfillstyle(SOLID_FILL,WHITE);
                floodfill(1,1,WHITE);
                setcolor(BLACK);
                setbkcolor(WHITE);
                settextstyle(1,HORIZ_DIR,7);
                outtextxy(200,300,"Felicitari!");
                settextstyle(0,HORIZ_DIR,0);
                setbkcolor(BLACK);
                butoane(2);
                delay(500);
                PlaySound(TEXT("win.wav"), NULL, SND_ASYNC);
            }
    }
}
void userInput()
{
    bool ok=0;
    int x,y,X;
    butoane(0);
    X=oX+9*L;
    while(!ok)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                x=mousex();
                y=mousey();
                if(x>=X and x<=X+100 and y>=oY and y<=oY+50)
                {
                    cleardevice();
                    desenTabla();
                    butoane(1);
                    UpdateNumar(2);
                    generat=0;
                    ok=1;
                }
                else if(x>=10 and x<=170 and y>=25 and y<=55)
                {
                    cleardevice();
                    setcolor(WHITE);
                    setbkcolor(WHITE);
                    menu();
                    ok=1;
                }
                else if(x>=oX+9*L and x<=oX+9*L+100 and y>=oY+100 and y<=oY+150)
                {
                    undo(1);
                    setbkcolor(CulJoc);
                    setcolor(CulJoc);
                    setfillstyle(SOLID_FILL,CulJoc);
                    rectangle(oX+9*L-1,oY+99,oX+9*L+101,oY+151);
                    floodfill(oX+9*L+1,oY+101,CulJoc);
                }
                else if(x>=oX and x<=oX+8*L and y>=oY and y<=oY+8*L)
                {
                    for(int i=0;i<8;i++)
                        if(x>=oX+v[i].x*L-L and x<=oX+v[i].x*L and y>=oY+v[i].y*L-L and y<=oY+v[i].y*L and v[i].piese<7)
                        {
                            srand(time(NULL));
                            setcolor(cul[v[i].x][v[i].y]);
                            setfillstyle(SOLID_FILL, cul[v[i].x][v[i].y]);
                            rectangle(oX+v[i].x*L-L+1, oY+v[i].y*L-L+1, oX+v[i].x*L-1, oY+v[i].y*L-1);
                            floodfill(oX+v[i].x*L-L+2,oY+v[i].y*L-L+2,cul[v[i].x][v[i].y]);
                            int mutari=0;
                            if(oX+v[i].x*L-L/2-L>oX and poz[v[i].x-1][v[i].y]!=1)
                            {
                                line(oX+v[i].x*L-L+1,oY+v[i].y*L-L/2,oX+v[i].x*L-L-L/2+5,oY+v[i].y*L-L/2);
                                mutari=1;
                                dir[0].st=1;
                                poz[v[i].x-1][v[i].y]=2;
                            }
                            if(oX+v[i].x*L+L/2<oX+8*L and poz[v[i].x+1][v[i].y]!=1)
                            {
                                line(oX+v[i].x*L-1,oY+v[i].y*L-L/2,oX+v[i].x*L+L/2-5,oY+v[i].y*L-L/2);
                                mutari=1;
                                dir[0].dr=1;
                                poz[v[i].x+1][v[i].y]=2;
                            }
                            if(oY+v[i].y*L-L/2-L>oY and poz[v[i].x][v[i].y-1]!=1)
                            {
                                line(oX+v[i].x*L-L/2,oY+v[i].y*L-L+1,oX+v[i].x*L-L/2,oY+v[i].y*L-L/2-L+5);
                                mutari=1;
                                dir[0].sus=1;
                                poz[v[i].x][v[i].y-1]=2;
                            }
                            if(oY+v[i].y*L+L/2<oY+8*L and poz[v[i].x][v[i].y+1]!=1)
                            {
                                line(oX+v[i].x*L-L/2,oY+v[i].y*L-1,oX+v[i].x*L-L/2,oY+v[i].y*L+L/2-5);
                                mutari=1;
                                dir[0].jos=1;
                                poz[v[i].x][v[i].y+1]=2;
                            }
                            setcolor(CulBila);
                            setbkcolor(CulTabla);
                            setfillstyle(StyleBila, CulBila);
                            circle(oX+v[i].x*L-L/2,oY+v[i].y*L-L/2,L/3);
                            floodfill(oX+v[i].x*L-L/2+1,oY+v[i].y*L-L/2+1,CulBila);
                            setcolor( cul[v[i].x][v[i].y]);
                            setfillstyle(SOLID_FILL,  cul[v[i].x][v[i].y]);
                            if(mutari!=1 and v[i].piese==0)
                            {
                                cleardevice();
                                setcolor(WHITE);
                                settextstyle(1,HORIZ_DIR,5);
                                outtextxy(200,300,"GAME OVER");
                                settextstyle(0,HORIZ_DIR,0);
                                butoane(0);
                            }
                            else
                            {

                                piese=1;
                                setcolor(BLACK);
                                outtextxy(oX+9*L+20,oY+18,"Reset");
                                setcolor(BLACK);
                                setbkcolor(WHITE);
                                outtextxy(oX+9*L+5,oY+18,"Alta bila");
                                undo(0);
                                valoriUndo.x=v[i].x;
                                valoriUndo.y=v[i].y;
                                UpdateNumar(1);
                                completeaza(v[i].x,v[i].y, cul[v[i].x][v[i].y],i);
                                break;

                            }
                        }
                }
            }
    }
}
void bileSpecific()
{
    int a,b,bile=0,x,y,numar=8;
    char ramas[100];
    setbkcolor(CulJoc);
    setcolor(CulJoc);
    outtextxy(oX+9*L,oY-30,"Pune bilele:");
    setcolor(CulBila);
    setbkcolor(CulTabla);
    setcolor(CulTabla);
    setbkcolor(CulTabla);
    outtextxy(oX+10*L,oY-70,"Spatii libere:");
    setcolor(BLACK);
    setbkcolor(WHITE);
    outtextxy(oX+10*L,oY-70,"Bile ramase:");
    setcolor(WHITE);
    outtextxy(oX+11*L+100,oY-70,spatiilibere);
    setcolor(CulTabla);
    setfillstyle(SOLID_FILL,CulTabla);
    circle(oX+11*L+110,oY-63,L/3+10);
    floodfill(oX+11*L+105,oY-63,CulTabla);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    circle(oX+11*L+110,oY-63,L/3+2);
    floodfill(oX+11*L+105,oY-63,WHITE);
    itoa(numar,ramas,10);
    setcolor(BLACK);
    setbkcolor(WHITE);
    outtextxy(oX+11*L+105,oY-70,ramas);
    setbkcolor(CulTabla);
    while(bile!=8)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            if(x>=oX and x<=8*oX and y>=oY and y<=8*oY)
                {
                    x=(x-oX)/L+1;
                    y=(y-oY)/L+1;
                    if(poz[x][y]==0)
                    {
                        v[bile].x=x;
                        v[bile].y=y;
                        poz[x][y]++;
                        cul[x][y]=culori[bile+1];
                        setbkcolor(CulTabla);
                        setcolor(CulBila);
                        setfillstyle(StyleBila,CulBila);
                        circle(oX+x*L-L/2,oY+y*L-L/2,L/3);
                        floodfill(oX+x*L-L/2,oY+y*L-L/2,CulBila);
                        numar--;
                        itoa(numar,ramas,10);
                        setcolor(BLACK);
                        setbkcolor(WHITE);
                        outtextxy(oX+11*L+105,oY-70,ramas);
                        bile++;
                    }
                }
        }
    }
    delay(100);
}
void undo(int mod)
{
    if(mod==0) //deseneaza buton
    {
        int X;
        X=oX+9*L;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL,WHITE);
        rectangle(X,oY+100,X+100,oY+150);
        floodfill(X+1,oY+101,WHITE);
        setbkcolor(WHITE);
        setcolor(BLACK);
        settextstyle(0,HORIZ_DIR,0);
        outtextxy(X+30,oY+117,"Undo");
        setcolor(BLACK);
    }
    else
    {
            int x,y;
            x=valoriUndo.x;
            y=valoriUndo.y;
            setcolor(BLACK);
            setfillstyle(SOLID_FILL,BLACK);
            if(oX+(x-1)*L-L>=oX and poz[x-1][y]!=1)
            {
                line(oX+(x-1)*L-L/2+5,oY+y*L-L/2,oX+(x-1)*L+1,oY+y*L-L/2);
            }
            if(oX+(x+1)*L-L<oX+8*L and poz[x+1][y]!=1)
            {
                line(oX+x*L-1,oY+y*L-L/2,oX+x*L+L/2-5,oY+y*L-L/2);
            }
            if(oY+(y-1)*L-L>=oY and poz[x][y-1]!=1)
            {
                line(oX+x*L-L/2,oY+(y-1)*L-L/2+5,oX+x*L-L/2,oY+(y-1)*L+1);
            }
            if(oY+y*L<oY+8*L and poz[x][y+1]!=1)
            {
                line(oX+x*L-L/2,oY+y*L+L/2-5,oX+x*L-L/2,oY+y*L-1);
            }
            setcolor(WHITE);
            setfillstyle(SOLID_FILL,WHITE);
            rectangle(oX+x*L-L,oY+y*L-L,oX+x*L,oY+y*L);
            floodfill(oX+x*L-L,oY+y*L-L,WHITE);
            setcolor(BLACK);
            setfillstyle(SOLID_FILL,BLACK);
            rectangle(oX+x*L-L+1,oY+y*L-L+1,oX+x*L-1,oY+y*L-1);
            floodfill(oX+x*L-L+2,oY+y*L-L+2,BLACK);
            setcolor(WHITE);
            setfillstyle(SOLID_FILL,WHITE);
            rectangle(oX+9*L-1,oY+99,oX+9*L+101,oY+151);
            floodfill(oX+9*L,oY+100,WHITE);
        if(mod==2) //undo spatiu
        {
            poz[x][y]=2;
            setcolor(cul[x][y]);
            if(cul[x-1][y]==cul[x][y])
            {
                line(oX+(x-1)*L-5,oY+y*L-L/2,oX+(x-1)*L+L/2-5,oY+y*L-L/2);
            }
            if(cul[x+1][y]==cul[x][y])
            {
                line(oX+x*L-L/2+5,oY+y*L-L/2,oX+x*L,oY+y*L-L/2);
            }
            if(cul[x][y-1]==cul[x][y])
            {
                line(oX+x*L-L/2,oY+(y-1)*L-5,oX+x*L-L/2,oY+(y-1)*L+L/2-5);
            }
            if(cul[x][y+1]==cul[x][y])
            {
                line(oX+x*L-L/2,oY+y*L-L/2+5,oX+x*L-L/2,oY+y*L+1);
            }
            cul[x][y]=0;
        }
        if(mod==1) //undo bila
        {
            setcolor(CulBila);
            setfillstyle(StyleBila,CulBila);
            circle(oX+x*L-L/2,oY+y*L-L/2,L/3);
            floodfill(oX+x*L-L/2,oY+y*L-L/2,CulBila);
        }
    }
}
void puneBile()
{
    int x,y,X;
    X=oX+9*L;
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x=mousex();
        y=mousey();
        if(x>=X and x<=X+100 and y>=oY and y<=oY+50)
        {
             cleardevice();
             desenTabla();
             bileRandom();
             generat=1;
             UpdateNumar(2);
        }
        else if(x>=X and x<=X+100 and y>=oY+100 and y<=oY+150)
        {
             cleardevice();
             desenTabla();
             bileSpecific();
             generat=1;
             UpdateNumar(2);
        }
        else if(x>=10 and x<=170 and y>=25 and y<=55)
                {
                    cleardevice();
                    menu();
                }
    }
}
void start()
{
    cleardevice();
    settextstyle(0,HORIZ_DIR,0);
    desenTabla();
    butoane(1);
    do
    {
        puneBile();
    }while(!generat);
    setcolor(CulJoc);
    setfillstyle(SOLID_FILL, CulJoc);
    userInput();
    int k=0;
    for(int i=0;i<8;i++)
        if(v[i].piese==7)
            k++;
    if(k!=8)
    start();
}
void SchimbaCulori()
{
    cleardevice();
    srand(time(NULL));
    CulBila=rand()%14+1;
    setfillstyle(SOLID_FILL,WHITE);
    floodfill(1,1,WHITE);
    setcolor(WHITE);
    setbkcolor(BLACK);
    settextstyle(3, HORIZ_DIR,6);
    outtextxy(70,50," Alege culorile tale preferate: ");
    setcolor(BLACK);
    settextstyle(0, HORIZ_DIR,3);
    setbkcolor(WHITE);
    outtextxy(40,200,"Bile-");
    settextstyle(0, HORIZ_DIR,0);
    outtextxy(200,240,"style");
    outtextxy(290,240,"culoare");
    settextstyle(0, HORIZ_DIR,3);
    setfillstyle(SOLID_FILL,BLACK);
    rectangle(200,180,250,230);
    floodfill(201,181,BLACK);
    setcolor(CulBila);
    setfillstyle(SOLID_FILL,CulBila);
    circle(225,205,L/3+5);
    floodfill(226,206,CulBila);
    setcolor(CulBila);
    setfillstyle(SOLID_FILL,CulBila);
    rectangle(300,180,350,230);
    floodfill(301,181,CulBila);
    setcolor(BLACK);
    outtextxy(40,300,"Culori careu-");
    setcolor(culori[1]);
    setfillstyle(SOLID_FILL,culori[1]);
    rectangle(40,350,90,400);
    floodfill(41,351,culori[1]);
    setcolor(culori[2]);
    setfillstyle(SOLID_FILL,culori[2]);
    rectangle(134,350,184,400);
    floodfill(135,351,culori[2]);
    setcolor(culori[3]);
    setfillstyle(SOLID_FILL,culori[3]);
    rectangle(228,350,278,400);
    floodfill(229,351,culori[3]);
    setcolor(culori[4]);
    setfillstyle(SOLID_FILL,culori[4]);
    rectangle(322,350,372,400);
    floodfill(323,351,culori[4]);
    setcolor(culori[5]);
    setfillstyle(SOLID_FILL,culori[5]);
    rectangle(416,350,466,400);
    floodfill(417,351,culori[5]);
    setcolor(culori[6]);
    setfillstyle(SOLID_FILL,culori[6]);
    rectangle(510,350,560,400);
    floodfill(511,351,culori[6]);
    setcolor(culori[7]);
    setfillstyle(SOLID_FILL,culori[7]);
    rectangle(604,350,654,400);
    floodfill(605,351,culori[7]);
    setcolor(culori[8]);
    setfillstyle(SOLID_FILL,culori[8]);
    rectangle(698,350,748,400);
    floodfill(699,351,culori[8]);
    setcolor(BLACK);
    outtextxy(55,410,"1");
    outtextxy(149,410,"2");
    outtextxy(243,410,"3");
    outtextxy(337,410,"4");
    outtextxy(431,410,"5");
    outtextxy(525,410,"6");
    outtextxy(619,410,"7");
    outtextxy(713,410,"8");
    outtextxy(40,470,"Culoare background tabla-");
    setcolor(BLACK);
    setfillstyle(SOLID_FILL,BLACK);
    rectangle(670,450,720,500);
    floodfill(671,451,BLACK);
    outtextxy(40,530,"Culoare background joc-");
    //setcolor(WHITE);
    setfillstyle(SOLID_FILL,BLACK);
    rectangle(610,510,660,560);
    floodfill(611,511,BLACK);

    setcolor(RED);
    setfillstyle(SOLID_FILL,BLACK);
    settextstyle(1, HORIZ_DIR,2);
    outtextxy(40,750,"*dati click pe patratel pentru a schimba culoarea");
    outtextxy(40,775,"**este recomandat sa folositi culori diferite");
    setcolor(BLACK);
    setfillstyle(SOLID_FILL,BLACK);
    rectangle(580,680,765,730);
    floodfill(581,681,BLACK);
    setcolor(WHITE);
    setbkcolor(BLACK);
    outtextxy(590,697,"Incepe jocul");
    while(1)
    {
        delay(5);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            int x,y;
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            if(x>=200 and y>=180 and x<=250 and y<=230) //stylefill bila
                {
                    if(StyleBila<10)
                        StyleBila++;
                    else
                        StyleBila=1;
                    setfillstyle(StyleBila,CulBila);
                    circle(225,205,L/3+5);
                    floodfill(226,206,WHITE);
                }
            if(x>=300 and y>=180 and x<=350 and y<=230) //culoare bila
                {
                    if(CulBila<14)
                        CulBila++;
                    else
                        CulBila=1;
                    setfillstyle(SOLID_FILL,CulBila);
                    rectangle(300,180,350,230);
                    floodfill(301,181,WHITE);
                    setfillstyle(StyleBila,CulBila);
                    circle(225,205,L/3+5);
                    floodfill(226,206,WHITE);
                }
            if(x>=40 and y>=350 and x<=90 and y<=400) //careu1
                {
                    if(culori[1]<14)
                        culori[1]++;
                    else
                        culori[1]=1;
                    setfillstyle(SOLID_FILL,culori[1]);
                    rectangle(40,350,90,400);
                    floodfill(41,351,WHITE);
                }
            if(x>=134 and y>=350 and x<=184 and y<=400) //careu2
                {
                    if(culori[2]<14)
                        culori[2]++;
                    else
                        culori[2]=1;
                    setfillstyle(SOLID_FILL,culori[2]);
                    rectangle(134,350,184,400);
                    floodfill(135,351,WHITE);
                }
            if(x>=228 and y>=350 and x<=278 and y<=400) //careu3
                {
                    if(culori[3]<14)
                        culori[3]++;
                    else
                        culori[3]=1;
                    setfillstyle(SOLID_FILL,culori[3]);
                    rectangle(228,350,278,400);
                    floodfill(229,351,WHITE);
                }
            if(x>=322 and y>=350 and x<=372 and y<=400) //careu4
                {
                    if(culori[4]<14)
                        culori[4]++;
                    else
                        culori[4]=1;
                    setfillstyle(SOLID_FILL,culori[4]);
                    rectangle(322,350,372,400);
                    floodfill(323,351,WHITE);
                }
            if(x>=416 and y>=350 and x<=466 and y<=400) //careu5
                {
                    if(culori[5]<14)
                        culori[5]++;
                    else
                        culori[5]=1;
                    setfillstyle(SOLID_FILL,culori[5]);
                    rectangle(416,350,466,400);
                    floodfill(417,351,WHITE);
                }
            if(x>=510 and y>=350 and x<=560 and y<=400) //careu6
                {
                    if(culori[6]<14)
                        culori[6]++;
                    else
                        culori[6]=1;
                    setfillstyle(SOLID_FILL,culori[6]);
                    rectangle(510,350,560,400);
                    floodfill(511,351,WHITE);
                }
            if(x>=604 and y>=350 and x<=654 and y<=400) //careu7
                {
                    if(culori[7]<14)
                        culori[7]++;
                    else
                        culori[7]=1;
                    setfillstyle(SOLID_FILL,culori[7]);
                    rectangle(604,350,654,400);
                    floodfill(605,351,WHITE);
                }
            if(x>=698 and y>=350 and x<=748 and y<=400) //careu8
                {
                    if(culori[8]<14)
                        culori[8]++;
                    else
                        culori[8]=1;
                    setfillstyle(SOLID_FILL,culori[8]);
                    rectangle(698,350,748,400);
                    floodfill(699,351,WHITE);
                }
            if(x>=670 and y>=450 and x<=720 and y<=500) //bk tabla
                {
                    if(CulTabla<13)
                        CulTabla++;
                    else
                        CulTabla=0;
                    setfillstyle(SOLID_FILL,CulTabla);
                    rectangle(670,450,720,500);
                    floodfill(671,451,WHITE);
                }
            if(x>=610 and y>=510 and x<=660 and y<=560) //bk joc
                {
                    if(CulJoc<15)
                        CulJoc++;
                    else
                        CulJoc=0;
                    setcolor(CulJoc);
                    setfillstyle(SOLID_FILL,CulJoc);
                    rectangle(610,510,660,560);
                    floodfill(611,511,CulJoc);
                }
            if(x>=580 and y>=680 and x<=765 and y<=730) //start
                {
                    cleardevice();
                    start();
                    break;
                }
        }
    }
}
void PaginaInfo()
{
    cleardevice();
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    floodfill(1,1,WHITE);
    setcolor(BLACK);
    setbkcolor(WHITE);
    settextstyle(1,HORIZ_DIR,3);
    outtextxy(50,200,"  Impartirea careului presupune o tabla de 8x8");
    outtextxy(30,225,"spatii in care sunt puse 8 bile pe cate un spatiu.");
    outtextxy(30,250,"Pentru a rezolva jocul, trebuie impartita tabla in");
    outtextxy(30,275,"8 parti,iar fiecare parte sa contina cate o bila si");
    outtextxy(30,300,"trebuie sa contina exact 8 spatii. In plus un spa-");
    outtextxy(30,325,"tiu poate fi conectat doar cu spatiul de sus, jos,");
    outtextxy(30,350,"stanga sau dreapta.");
    setfillstyle(SOLID_FILL,BLACK);
    rectangle(350,400,450,450);
    floodfill(351,401,BLACK);
    setcolor(WHITE);
    setbkcolor(BLACK);
    settextstyle(0,HORIZ_DIR,0);
    outtextxy(368,418,"Inapoi");
    while(1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            int x,y;
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            if(x>=350 and x<=450 and y>=400 and y<=450)
                {
                    cleardevice();
                    menu();
                    break;
                }
        }
    }
}
void menu()
{
    PlaySound(TEXT("8bit.wav"), NULL, SND_ASYNC);
    int x,y;
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    floodfill(1,1,WHITE);
    setbkcolor(BLACK);
    setcolor(BLACK);
    setfillstyle(SOLID_FILL,BLACK);
    rectangle(200,110,600,185);
    rectangle(200,285,600,360);
    rectangle(200,460,600,535);
    rectangle(200,635,600,710);
    floodfill(201,111,BLACK);
    floodfill(201,286,BLACK);
    floodfill(201,461,BLACK);
    floodfill(201,636,BLACK);
    setcolor(WHITE);
    settextstyle(0, HORIZ_DIR,3);
    outtextxy(230,136,"Alege culorile");
    outtextxy(240,310,"  Start joc  ");
    outtextxy(235,485," Cum se joaca ");
    outtextxy(230,660,"     Exit     ");
    while(1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            if(x>=200 and x<=600 and y>=110 and y<=185)
                {
                    SchimbaCulori();
                }
            if(x>=200 and x<=600 and y>=285 and y<=360)
                {
                    start();
                    exit(0);
                }
            if(x>=200 and x<=600 and y>=460 and y<=535)
                {
                    PaginaInfo();
                }
            if(x>=200 and x<=600 and y>=635 and y<=710)
                {
                    exit(0);
                }
        }
    }
}
int main()
{
    int x,y;
    oX = 100;
    oY = 100;
    L=50;
    initwindow(800,800);
    readimagefile("background.jpg",0,0,800,800);
    while(1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            if(x>=466 and x<=778 and y>=523 and y<=781)
                {
                    cleardevice();
                    break;
                }
        }
    }
    menu();
    getch();
    closegraph();
    return 0;
}
